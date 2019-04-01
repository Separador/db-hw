#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define MATR_SIZE 16384
#define RECUR_LIMIT 4

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
    printf("Cache oblivious time: %g sec\n", t);
    return 0;    
}
