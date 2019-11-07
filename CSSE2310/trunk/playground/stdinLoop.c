#include <stdio.h>
#include <stdlib.h>


char** read_line(FILE* file) { 
	char** move = malloc(sizeof(char*) * 3);
	for (int i = 0; i < 3; i++) {
		move[i] = malloc(sizeof(char) * 32);
	}
	 
	for (int i = 0; i < 3; i++) {
		int position = 0;
		int next = 0;

		while (1) {
			next = fgetc(file);
			if (next == '\n') {
				if (i == 2) {
					return move;
				} else { //error
				}
			}

			if (next == ' ') {
				move[i][position] = '\0';
				break;
			} else {
				move[i][position++] = (char)next;
			}
		}
	}
	return move;

} 



void game(void) {
	while (1) {
		
		

	}

}

int main(int argc, char** argv) {
	char** strings = read_line(stdin);
	printf(strings[0]);
	printf(strings[1]);
	printf(strings[2]);
	printf("\n");
	return 0; 
}
