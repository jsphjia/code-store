/**************************************************************************/
/*              COPYRIGHT Carnegie Mellon University 2023                 */
/* Do not post this file or any derivative on a public site or repository */
/**************************************************************************/
#include <assert.h>
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

#include "lib/xalloc.h"
#include "lib/stack.h"
#include "lib/contracts.h"
#include "lib/c0v_stack.h"
#include "lib/c0vm.h"
#include "lib/c0vm_c0ffi.h"
#include "lib/c0vm_abort.h"

/* call stack frames */
typedef struct frame_info frame;
struct frame_info {
  c0v_stack_t  S;   /* Operand stack of C0 values */
  ubyte       *P;   /* Function body */
  size_t       pc;  /* Program counter */
  c0_value    *V;   /* The local variables */
};

// HELPER FUNCTIONS
void push_int(c0v_stack_t S, int32_t i){
  c0v_push(S, int2val(i));
}

void push_ptr(c0v_stack_t S, void *i){
  c0v_push(S, ptr2val(i));
}

int32_t pop_int(c0v_stack_t S){
  return val2int(c0v_pop(S));
}

void *pop_ptr(c0v_stack_t S){
  return val2ptr(c0v_pop(S));
}

void stack_elem_print(void *x){
  frame *F = (frame *) x;
  printf("%d\n", (int32_t) F->pc);
}

// MAIN EXECUTE FILE
int execute(struct bc0_file *bc0) {
  REQUIRES(bc0 != NULL);

  /* Variables */
  c0v_stack_t S = c0v_stack_new();    /* Operand stack of C0 values */
  ubyte *P = bc0->function_pool[0].code;   /* Array of bytes that make up the current function */
  size_t pc = 0;     /* Current location within the current byte array P */
  c0_value *V = xcalloc(bc0->function_pool[0].num_vars, sizeof(c0_value));   /* Local variables (you won't need this till Task 2) */

  /* The call stack, a generic stack that should contain pointers to frames */
  /* You won't need this until you implement functions. */
  gstack_t callStack = stack_new();

  while (true) {

#ifdef DEBUG
    /* You can add extra debugging information here */
    fprintf(stderr, "Opcode %x -- Stack size: %zu -- PC: %zu\n",
            P[pc], c0v_stack_size(S), pc);
#endif
    switch (P[pc]) {

    /* Additional stack operation: */

    case POP: {
      pc++;
      c0v_pop(S);
      break;
    }

    case DUP: {
      pc++;
      c0_value v = c0v_pop(S);
      c0v_push(S,v);
      c0v_push(S,v);
      break;
    }

    case SWAP: {
      pc++;
      int32_t v2 = pop_int(S);
      int32_t v1 = pop_int(S);
      push_int(S, v2);
      push_int(S, v1);
      break;
    }

    /* Returning from a function.
     * This currently has a memory leak! You will need to make a slight
     * change for the initial tasks to avoid leaking memory.  You will
     * need to revise it further when you write INVOKESTATIC. */

    case RETURN: {
      c0_value retval = c0v_pop(S);
      c0v_stack_free(S);
      free(V);
      if(!stack_empty(callStack)) {
        frame *F = (frame *) pop(callStack);
        S = F->S;
        pc = F->pc;
        V = F->V;
        P = F->P;
        free(F);
        c0v_push(S, retval);
        break;
      }
      else {
        stack_free(callStack, NULL);
        return val2int(retval);
      }
    }

    /* Arithmetic and Logical operations */

    case IADD: {
      pc++;
      int32_t y = (int32_t) pop_int(S);
      int32_t x = (int32_t) pop_int(S);
      push_int(S, (int32_t)(x + y));
      break;
    }

    case ISUB: {
      pc++;
      int32_t y = pop_int(S);
      int32_t x = pop_int(S);
      push_int(S, x - y);
      break;
    }

    case IMUL: {
      pc++;
      int32_t y = pop_int(S);
      int32_t x = pop_int(S);
      push_int(S, x * y);
      break;
    }

    case IDIV: {
      pc++;
      int32_t y = pop_int(S);
      int32_t x = pop_int(S);
      if(y == 0) c0_arith_error("can't divide by zero!");
      if(y == -1 && x == INT32_MIN) c0_arith_error("invalid math!");
      push_int(S, x / y);
      break;
    }

    case IREM: {
      pc++;
      int32_t y = pop_int(S);
      int32_t x = pop_int(S);
      if(y == 0) c0_arith_error("can't find remainder when dividing by zero!");
      if(y == -1 && x == INT32_MIN) c0_arith_error("invalid math!");
      push_int(S, x % y);
      break;
    }

    case IAND: {
      pc++;
      int32_t y = pop_int(S);
      int32_t x = pop_int(S);
      push_int(S, x & y);
      break;
    }

    case IOR: {
      pc++;
      int32_t y = pop_int(S);
      int32_t x = pop_int(S);
      push_int(S, x | y);
      break;
    }

    case IXOR: {
      pc++;
      int32_t y = pop_int(S);
      int32_t x = pop_int(S);
      push_int(S, x ^ y);
      break;
    }

    case ISHR: {
      pc++;
      int32_t y = pop_int(S);
      int32_t x = pop_int(S);
      if(y < 0 || y >= 32) c0_arith_error("invalid shift!");
      push_int(S, x >> y);
      break;
    }

    case ISHL: {
      pc++;
      int32_t y = pop_int(S);
      int32_t x = pop_int(S);
      if(y < 0 || y >= 32) c0_arith_error("invalid shift!");
      push_int(S, x << y);
      break;
    }

    /* Pushing constants */

    case BIPUSH: {
      pc++;
      int8_t x = (int8_t) (P[pc]);
      pc++;
      push_int(S, x);
      break;
    }

    case ILDC: {
      pc++;
      uint32_t c1 = (uint32_t) P[pc];
      pc++;
      uint32_t c2 = (uint32_t) P[pc];
      pc++;
      int32_t x = (int32_t) (bc0->int_pool[(c1 << 8) | c2]);
      push_int(S, x);
      break;
    }

    case ALDC: {
      pc++;
      uint32_t c1 = (uint32_t) P[pc];
      pc++;
      uint32_t c2 = (uint32_t) P[pc];
      pc++;
      void *x = &(bc0->string_pool[(c1 << 8) | c2]);
      push_ptr(S, x);
      break;
    }

    case ACONST_NULL: {
      pc++;
      push_ptr(S, NULL);
      break;
    }

    /* Operations on local variables */

    case VLOAD: {
      pc++;
      c0v_push(S, V[P[pc]]);
      pc++;
      break;
    }

    case VSTORE: {
      pc++;
      V[P[pc]] = c0v_pop(S);
      pc++;
      break;
    }

    /* Assertions and errors */

    case ATHROW: {
      pc++;
      c0_user_error((char*) (pop_ptr(S)));
      break;
    }

    case ASSERT: {
      pc++;
      void *a = pop_ptr(S);
      if(pop_int(S) == 0) c0_assertion_failure((char*) a);
      break;
    }

    /* Control flow operations */

    case NOP: {
      pc++;
      break;
    }

    case IF_CMPEQ: {
      pc++;
      int32_t o1 = (int32_t) P[pc];
      pc++;
      int32_t o2 = (int32_t) P[pc];
      pc++;
      c0_value v1 = c0v_pop(S);
      c0_value v2 = c0v_pop(S);
      if(val_equal(v1, v2)) pc = pc + (int16_t) ((o1 << 8) | o2) - 3;
      break;
    }

    case IF_CMPNE: {
      pc++;
      int32_t o1 = (int32_t) P[pc];
      pc++;
      int32_t o2 = (int32_t) P[pc];
      pc++;
      c0_value v2 = c0v_pop(S);
      c0_value v1 = c0v_pop(S);
      if(!val_equal(v1, v2)) pc = pc + (int16_t) ((o1 << 8) | o2) - 3;
      break;
    }

    case IF_ICMPLT: {
      pc++;
      int32_t o1 = (int32_t) P[pc];
      pc++;
      int32_t o2 = (int32_t) P[pc];
      pc++;
      int32_t y = pop_int(S);
      int32_t x = pop_int(S);
      if(x < y) pc = pc + (int16_t) ((o1 << 8) | o2) - 3;
      break;
    }

    case IF_ICMPGE: {
      pc++;
      int32_t o1 = (int32_t) P[pc];
      pc++;
      int32_t o2 = (int32_t) P[pc];
      pc++;
      int32_t y = pop_int(S);
      int32_t x = pop_int(S);
      if(x >= y) pc = pc + (int16_t) ((o1 << 8) | o2) - 3;
      break;
    }

    case IF_ICMPGT: {
      pc++;
      int32_t o1 = (int32_t) P[pc];
      pc++;
      int32_t o2 = (int32_t) P[pc];
      pc++;
      int32_t y = pop_int(S);
      int32_t x = pop_int(S);
      if(x > y) pc = pc + (int16_t) ((o1 << 8) | o2) - 3;
      break;
    }

    case IF_ICMPLE: {
      pc++;
      int32_t o1 = (int32_t) P[pc];
      pc++;
      int32_t o2 = (int32_t) P[pc];
      pc++;
      int32_t y = pop_int(S);
      int32_t x = pop_int(S);
      if(x <= y) pc = pc + (int16_t) ((o1 << 8) | o2) - 3;
      break;
    }

    case GOTO: {
      pc++;
      int32_t o1 = (int32_t) P[pc];
      pc++;
      int32_t o2 = (int32_t) P[pc];
      pc++;
      pc = pc + (int16_t) ((o1 << 8) | o2) - 3;
      break;
    }

    /* Function call operations: */

    case INVOKESTATIC: {
      pc++; 
      int32_t c1 = (int32_t) P[pc];
      pc++;
      int32_t c2 = (int32_t) P[pc];
      pc++;
      uint16_t vars = bc0->function_pool[(c1 << 8) | c2].num_vars;
      uint16_t args = bc0->function_pool[(c1 << 8) | c2].num_args;
      frame *F = xcalloc(1, sizeof(frame));
      F->S = S;
      F->P = P;
      F->pc = pc;
      F->V = V;
      push(callStack, (stack_elem) F);
      V = xcalloc(vars, sizeof(c0_value));
      for(uint16_t i = args; i > 0; i--){
        V[i - 1] = c0v_pop(S);
      }
      pc = 0;
      P = bc0->function_pool[(c1 << 8) | c2].code;
      S = c0v_stack_new();
      break;
    }

    case INVOKENATIVE: {
      pc++; 
      int32_t c1 = (int32_t) P[pc];
      pc++;
      int32_t c2 = (int32_t) P[pc];
      pc++;
      uint16_t args = bc0->native_pool[(c1 << 8) | c2].num_args;
      uint16_t findex = bc0->native_pool[(c1 << 8) | c2].function_table_index;
      native_fn *func = native_function_table[findex];
      c0_value *W = xcalloc(args, sizeof(c0_value));
      for(int i = args; i > 0; i--){
        W[i - 1] = c0v_pop(S);
      }
      c0_value res = (*func)(W); 
      c0v_push(S, res);
      free(W);
      break;
    }

    /* Memory allocation and access operations: */

    case NEW: {
      pc++;
      uint32_t s = P[pc];
      pc++;
      void *a = xcalloc(1, s);
      push_ptr(S, a);
      break;
    }

    case IMLOAD: {
      pc++;
      void *a = pop_ptr(S);
      if(a == NULL) c0_memory_error("NULL pointer");
      int32_t x = *(int32_t *)a;
      push_int(S, x);
      break;
    }

    case IMSTORE: {
      pc++;
      int32_t x = pop_int(S);
      void *a = pop_ptr(S);
      if(a == NULL) c0_memory_error("NULL pointer");
      *((int *) a) = x;
      break;
    }

    case AMLOAD: {
      pc++;
      char **a = (char **) pop_ptr(S);
      if(a == NULL) c0_memory_error("NULL pointer");
      void *b = *a;
      push_ptr(S, b);
      break;
    }

    case AMSTORE: {
      pc++;
      void *b = pop_ptr(S);
      void **a = (void **) pop_ptr(S);
      if(a == NULL) c0_memory_error("NULL pointer");
      *(char **)a = (char *)b;
      break;
    }

    case CMLOAD: {
      pc++;
      char *a = (char *) pop_ptr(S);
      if(a == NULL) c0_memory_error("NULL pointer");
      int32_t x = (int32_t) *a;
      push_int(S, x);
      break;
    }

    case CMSTORE: {
      pc++;
      int32_t x = pop_int(S); 
      char *a = (char *) pop_ptr(S);
      if(a == NULL) c0_memory_error("NULL pointer");
      *a = x & 0x7f;
      break;
    }

    case AADDF: {
      pc++;
      char *a = (char *) pop_ptr(S);
      uint8_t f = P[pc];
      pc++;
      if(a == NULL) c0_memory_error("NULL pointer");
      push_ptr(S, (void *)(a+f));
      break;
    }

    /* Array operations: */

    case NEWARRAY: {
      pc++;
      int32_t n = pop_int(S);
      uint32_t s = P[pc];
      pc++;
      if(n < 0) c0_memory_error("negative size");
      c0_array *a = xcalloc(1, sizeof(c0_array));
      if(a == NULL) c0_memory_error("NULL pointer");
      a->elems = xcalloc(n, s);
      a->count = n;
      a->elt_size = s;
      push_ptr(S, (void *)a);
      break;
    }

    case ARRAYLENGTH: {
      pc++;
      c0_array *a = (c0_array *) pop_ptr(S);
      push_int(S, a->count);
      break;
    }

    case AADDS: {
      pc++;
      uint32_t i = pop_int(S);
      c0_array *a = (c0_array *) pop_ptr(S);
      if(a == NULL) c0_memory_error("NULL pointer");
      if(i >= a->count) c0_memory_error("index out of bounds");
      push_ptr(S, (void *)(((char *)a->elems)+(a->elt_size)*i));
      break;
    }

    /* BONUS -- C1 operations */

    case CHECKTAG:

    case HASTAG:

    case ADDTAG:

    case ADDROF_STATIC:

    case ADDROF_NATIVE:

    case INVOKEDYNAMIC:

    default:
      fprintf(stderr, "invalid opcode: 0x%02x\n", P[pc]);
      abort();
    }
  }

  /* cannot get here from infinite loop */
  assert(false);
}
