/* Program initializes square matrix with side size MATR_SIZE
 * and uses _cache-oblivious_ algorithm to transpose it.
 * At the end the transposition is checked for correctness.
 * 
 * INPUT: none
 * OUTPUT: transposition time in secs (check time not included) */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define MATR_SIZE (1 << 14)     /* side size is selected so that
                                 * whole matrix can't be placed in cache */
#define RECUR_LIMIT 4       /* limit is selected so that
                             * matrix is divided until
                             * it completely resides in the lowest cache */

void check(int *matr)
{
    int i = 0, j = 0;
    int *correct = calloc(MATR_SIZE * MATR_SIZE, sizeof(int)); 
    for (j = 0; j < MATR_SIZE; j++) {
        for (i = 0; i < MATR_SIZE; i++) {
            if (matr[i*MATR_SIZE + j] != j*MATR_SIZE + i) {
                printf("[%d, %d]%d =/= %d, ", 
                        i, j, matr[i*MATR_SIZE + j], j*MATR_SIZE + i);
            }
        }
    }
    free(correct);
}

void transposerec(int *matr, int starti, int startj, int size)
{
    int i = 0, j = 0, temp = 0;
    if (size == RECUR_LIMIT) {
        /* simple transpose */
        for (i = starti; i < starti + size; i++)
            for (j = startj; j < startj + size; j++) {
                if (j > i) {
                    temp = matr[i*MATR_SIZE + j];
                    matr[i*MATR_SIZE + j] = matr[j*MATR_SIZE + i];
                    matr[j*MATR_SIZE + i] = temp;
                }
            }
    } else {
        /* recursion */
        int half = size / 2;
        transposerec(matr, starti, startj, half);
        transposerec(matr, starti, startj + half, half);
        transposerec(matr, starti + half, startj, half);
        transposerec(matr, starti + half, startj + half, half);
    }
}

int main()
{
    printf("Using %.3f MB of memory\n",
            (float)MATR_SIZE * MATR_SIZE * sizeof(int)/ 1e6);
    int *matr = calloc(MATR_SIZE * MATR_SIZE, sizeof(int));
    int i = 0, j = 0;
    double t = 0;
    /* init array */
    for (i = 0; i < MATR_SIZE; i++)
        for (j = 0; j < MATR_SIZE; j++)
            matr[i*MATR_SIZE + j] = i*MATR_SIZE + j;
    /* transpose array*/
    t = clock();
    transposerec(matr, 0, 0, MATR_SIZE);
    t = (clock() - t) / (double)CLOCKS_PER_SEC;
    check(matr);
    free(matr);
    printf("Cache oblivious time: %g sec\n", t);
    return 0;    
}
