#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define MATR_SIZE 16384

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
