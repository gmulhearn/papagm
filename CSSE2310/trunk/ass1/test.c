#include <stdio.h>
#include <stdlib.h>


int main(int argc, char** argv) {
	
	FILE* test = fopen("tests/", "r");
	if (test == NULL) {
		printf("harry is correct");
	}
	else {
		printf("harry is wrong\n");
	}


	return 0;
}
