#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void arrayFunc(int** arr, int x, int y) {
	for (int i = 0; i < x; i++) {
		for (int j = 0; j < y; j++) {			
			arr[i][j] = 0;
		}
	}
}

void printArr(int** arr, int x, int y) {
	for (int i = 0; i < x; i++) {
		for (int j = 0; j < y; j++) {
			printf("%d ", arr[i][j]);
		}
		printf("\n");
	}
}

int main(int argc, char** argv) {
	int h = 5;
	int w = 4;
	
	int** arr = (int **) malloc(sizeof(int *) * w);
	for (int i = 0; i < w; i++) {
		arr[i] = (int *) malloc(sizeof(int) * h);
	}

	arrayFunc(arr, w, h);
	printArr(arr, w, h);
	free(arr);

	return 0;
}


