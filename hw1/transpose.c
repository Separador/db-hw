/* Program initializes square matrix with side size MATR_SIZE
 * and uses _obvious_ algorithm to transpose it.
 * At the end the transposition is checked for correctness.
 * 
 * INPUT: none
 * OUTPUT: transposition time in secs (check time not included) */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define MATR_SIZE (1 << 14)     /* side size is selected so that
                                 * whole matrix can't be placed in cache */

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


int main()
{
    printf("Using %.3f MB of memory\n",
            (float)MATR_SIZE * MATR_SIZE * sizeof(int)/ 1e6);
    int *matr = calloc(MATR_SIZE * MATR_SIZE, sizeof(int));
    int i = 0, j = 0, temp = 0;
    double t = 0;
    /* init array */
    for (i = 0; i < MATR_SIZE; i++)
        for (j = 0; j < MATR_SIZE; j++)
            matr[i*MATR_SIZE + j] = i*MATR_SIZE + j;
    /* transpose array*/
    t = clock();
    for (i = 0; i < MATR_SIZE; i++)
        for (j = i; j < MATR_SIZE; j++) {
            temp = matr[i*MATR_SIZE + j];
            matr[i*MATR_SIZE + j] = matr[j*MATR_SIZE + i];
            matr[j*MATR_SIZE + i] = temp;
        }
    t = (clock() - t) / (double)CLOCKS_PER_SEC;
    check(matr);
    printf("Time: %g sec\n", t);
    free(matr);
    return 0;    
}
