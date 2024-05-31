#include <stdbool.h>
#include <stdlib.h>
#include "board-ht.h"
#include "lib/bitvector.h"
#include "lib/hdict.h"
#include "lib/contracts.h"
#include "lib/xalloc.h"

hdict_key get_ht(hdict_entry e)
{
    REQUIRES(e != NULL);

    struct board_data *res = (struct board_data*) e;
    return (void*)(&(res->board));
}

bool ht_equal(hdict_key k1, hdict_key k2)
{
    bitvector *res1 = (bitvector*) k1;
    bitvector *res2 = (bitvector*) k2;
    return bitvector_equal(*res1, *res2);
}

// size_t pow(size_t base, size_t power)
// {
//     size_t pro = 1;
//     for(size_t a = 0; a < power; a++) pro *= base;
//     return pro;
// }

size_t hash_ht(hdict_key k)
{
    size_t sum = 0;
    bitvector* B = (bitvector*) k;

    for(size_t i = 0; i < BITVECTOR_LIMIT; i++)
    {
        sum *= 31;
        sum += bitvector_get(*B, i);
    }
    return sum;
}

void ht_free(hdict_entry e)
{
    bitvector_free(((struct board_data*) e)->board);
    free((struct board_data*) e);
}

hdict_t ht_new(size_t capacity)
{
    REQUIRES(capacity > 0);
    hdict_t H = hdict_new(capacity, &get_ht, &ht_equal, &hash_ht, &ht_free);

    ENSURES(H != NULL);
    return H;
}

struct board_data *ht_lookup(hdict_t H, bitvector B)
{
    REQUIRES(H != NULL);

    bitvector *ptr = xcalloc(1, sizeof(bitvector));
    *ptr = B;
    struct board_data *res = (struct board_data*) hdict_lookup(H, (void*) ptr);
    free(ptr);
    return res;
}

void ht_insert(hdict_t H, struct board_data *DAT)
{
    REQUIRES(H != NULL && DAT != NULL);
    REQUIRES(ht_lookup(H, DAT->board) == NULL);

    hdict_insert(H, (void*) DAT);
}