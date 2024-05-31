#include <stdlib.h>
#include <stdbool.h>
#include "lib/xalloc.h"
#include "board-ht.h"
#include "lib/bitvector.h"
#include "lib/boardutil.h"
#include "lib/queue.h"
#include "lib/hdict.h"
#include "lib/contracts.h"

bitvector press_button(bitvector B, index_t row, index_t col, index_t width, index_t height)
{
    REQUIRES(row < height && col < width);

    B = bitvector_flip(B, get_index(row, col, width, height));
    if(row - 1 >= 0) B = bitvector_flip(B, get_index(row - 1, col, width, height));
    if(row + 1 < height) B = bitvector_flip(B, get_index(row + 1, col, width, height));
    if(col - 1 >= 0) B = bitvector_flip(B, get_index(row, col - 1, width, height));
    if(col + 1 < width) B = bitvector_flip(B, get_index(row, col + 1, width, height));

    return B;
}

int main(int argc, char** argv)
{
    // take in arguments
    if (argc != 2) {  
        fprintf(stderr, "Usage: lightsout <board name>\n");  
        return EXIT_FAILURE;
    }  
    char *board = argv[1];

    // variable delcaration
    hdict_t H = ht_new(100);
    queue_t Q = queue_new();
    index_t *width = xcalloc(1, sizeof(index_t));
    index_t *height = xcalloc(1, sizeof(index_t));
    bitvector* output = xcalloc(1, sizeof(bitvector));

    // read in file board
    bool read = file_read(board, output, width, height);

    if(!read)
    {
        free(width);
        free(height);
        free(output);
        hdict_free(H);
        queue_free(Q, NULL);
        return EXIT_FAILURE;
    }

    if(bitvector_equal(*output, bitvector_new()))
    {
        free(width);
        free(height);
        free(output);
        hdict_free(H);
        queue_free(Q, NULL);
        return EXIT_SUCCESS;
    }

    // creating the initial bitvector to enq
    struct board_data *tmp = xmalloc(sizeof(struct board_data));
    tmp->board = *output;
    tmp->moves = bitvector_new();

    enq(Q, (queue_elem) tmp);
    free(output);

    // go until queue of boards is empty
    while (!queue_empty(Q)) {  
        // Find a board that we haven't looked at yet from the queue  
        struct board_data *B = (struct board_data*) deq(Q);
  
        // Consider all the moves  
        for (index_t row = 0; row < *height; row++) {  
            for (index_t col = 0; col < *width; col++) {  
                index_t i = get_index(row, col, *width, *height);  
                bitvector newboard = press_button(B->board, row, col, *width, *height);

                // number of lights of newboard == 0
                if (bitvector_equal(newboard, bitvector_new())) 
                {
                    // print out all the steps
                    for(index_t idx = 0; idx < *width * *height; idx++)
                    {
                        if((bitvector_get(B->moves, idx))){
                            printf("%d:%d\n", idx / *width, idx % *width);
                        }
                    }
                    printf("%d:%d\n", row, col);

                    // Free all memory
                    free(width);
                    free(height);
                    bitvector_free(tmp->board);
                    bitvector_free(B->board);
                    free(tmp);
                    hdict_free(H);
                    queue_free(Q, NULL);
                    return EXIT_SUCCESS;
                }  

                // hash table H doesn't contain newboard
                if (ht_lookup(H, newboard) == NULL) 
                {  
                    // Allocate memory to hold hashtable element N
                    struct board_data *N = xmalloc(sizeof(struct board_data));

                    // Set the field N->board to newboard, set other fields
                    N->board = newboard;
                    N->moves = bitvector_flip(B->moves, i);

                    // Insert N into the hashtable H
                    ht_insert(H, N);

                    // Enqueue N into the queue Q
                    enq(Q, (queue_elem) N);
                }
            }  
        }  
    }  
    // Free all memory
    free(width);
    free(height);
    bitvector_free(tmp->board);
    free(tmp);
    hdict_free(H);
    queue_free(Q, NULL);
    fprintf(stderr, "No solution was found!\n");
    return EXIT_FAILURE;
}