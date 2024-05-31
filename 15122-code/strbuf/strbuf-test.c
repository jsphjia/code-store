 #include <assert.h>
 #include <string.h>
 #include <stdio.h>
 #include <stdlib.h>
 #include "strbuf.h"
 #include "lib/contracts.h"

void tests()
{
    // task 1 C implementation
    strbuf* A = NULL;
    ASSERT(!is_strbuf(A));

    A = strbuf_new(1);
    ASSERT(is_strbuf(A)); // normal 
    free(strbuf_free(A));

    strbuf* B = strbuf_new(10);
    strbuf_addstr(B, "hello");
    ASSERT(is_strbuf(B)); // valid add
    // free(strbuf_free(B));
    B->len = 4;
    ASSERT(!is_strbuf(B)); // invalid len (too short)
    B->len = 5;
    // free(strbuf_free(B));
    B->len = 8;
    ASSERT(!is_strbuf(B)); // invalid len (too long)
    B->len = 5;
    // free(strbuf_free(B));
    // B->len = 5;
    B->buf[4] = '\0';
    ASSERT(!is_strbuf(B)); // nul terminate too early
    B->buf[4] = 'o';
    B->buf[5] = '\0';
    ASSERT(is_strbuf(B));
    free(strbuf_free(B));

    // task 2 + 3 C implementation

    strbuf* C = strbuf_new(1);
    ASSERT(C->limit == 1);
    ASSERT(C->len == 0);
    ASSERT(C->buf[0] == '\0');
    free(strbuf_free(C));

    C = strbuf_new(100);
    ASSERT(C->limit == 100);
    ASSERT(C->len == 0);
    ASSERT(C->buf[0] == '\0');
    free(strbuf_free(C));

    C = strbuf_new(1);
    char *res = strbuf_str(C);
    ASSERT(is_strbuf(C));
    ASSERT(res[0] == '\0');
    free(res);
    free(strbuf_free(C));

    C = strbuf_new(10);
    strbuf_addstr(C, "hello");
    res = strbuf_str(C);
    ASSERT(is_strbuf(C));
    ASSERT(C->len == 5);
    ASSERT(C->limit == 10);
    ASSERT(res[0] == 'h');
    ASSERT(res[1] == 'e');
    ASSERT(res[2] == 'l');
    ASSERT(res[3] == 'l');
    ASSERT(res[4] == 'o');
    ASSERT(res[5] == '\0');
    free(res);
    // free(strbuf_free(C));

    strbuf_add(C, "hi", 2);
    res = strbuf_str(C);
    ASSERT(is_strbuf(C));
    ASSERT(C->len == 7);
    ASSERT(C->limit == 10);
    ASSERT(res[0] == 'h');
    ASSERT(res[1] == 'e');
    ASSERT(res[2] == 'l');
    ASSERT(res[3] == 'l');
    ASSERT(res[4] == 'o');
    ASSERT(res[5] == 'h');
    ASSERT(res[6] == 'i');
    ASSERT(res[7] == '\0');
    free(res);
    // free(strbuf_free(C));

    strbuf_addstr(C, "hi");
    res = strbuf_str(C);
    ASSERT(is_strbuf(C));
    ASSERT(C->len == 9);
    ASSERT(C->limit == 10);
    ASSERT(res[0] == 'h');
    ASSERT(res[1] == 'e');
    ASSERT(res[2] == 'l');
    ASSERT(res[3] == 'l');
    ASSERT(res[4] == 'o');
    ASSERT(res[5] == 'h');
    ASSERT(res[6] == 'i');
    ASSERT(res[7] == 'h');
    ASSERT(res[8] == 'i');
    ASSERT(res[9] == '\0');
    free(res);
    // free(strbuf_free(C));

    strbuf_add(C, "t", 1);
    res = strbuf_str(C);
    ASSERT(is_strbuf(C));
    ASSERT(C->len == 10);
    ASSERT(C->limit > 10);
    ASSERT(res[0] == 'h');
    ASSERT(res[1] == 'e');
    ASSERT(res[2] == 'l');
    ASSERT(res[3] == 'l');
    ASSERT(res[4] == 'o');
    ASSERT(res[5] == 'h');
    ASSERT(res[6] == 'i');
    ASSERT(res[7] == 'h');
    ASSERT(res[8] == 'i');
    ASSERT(res[9] == 't');
    ASSERT(res[10] == '\0');
    free(res);
    free(strbuf_free(C));
}

int main()
{
    tests();
    return 0;
}

