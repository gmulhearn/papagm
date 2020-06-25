#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

typedef struct {
	bool turn;
	int pit;
} Player;

typedef struct {
        int* bowls;                                     int turn;
        Player* players;  
	int selected;
	int hand;
	int mode;
} Board;

void nextTurn(Board* board);
void autoMove(Board* board);

bool checkNum(char* string) {

	if (string[1] != '\n' && string[1] != '\0') {
		return false;
	} else if (string[0] > '9' || string[0] < '0') {
		return false;
	} else if (atoi(string) > 7 || atoi(string) == 0) {
		return false;
	} else {
		return true;
	}

}

bool checkValid(Board* board, int move) {
	//check range
	int bowl = move-1 + 7*board->turn;
	if (board->bowls[bowl] == 0) {
		return false;
	} else {
		return true;
	}
}
	
Board* init(void) {

	int mode = 0;

	char buff[2];
	while (1) {
		printf("choose a mode:\n1 - singleplayer\n2 - multiplayer\n:");
		fgets(buff, 2, stdin);
		if (buff[0] == '1') {
			break;
		} else if (buff[0] == '2') {
			mode = 1;
			break;
		} else {
			printf("invalid selection, try again\n");
		}
	}

	Board* board = (Board*)malloc(sizeof(Board)*2); // look into this...
	Player* players = (Player*)malloc(sizeof(Player)*2);

	players[0].pit = 0;
	players[0].turn = true;

	players[1].pit = 0;
	players[1].turn = false;

	board->turn = 0;

	board->mode = mode;

	board->selected = 0;

	board->players = players;

	board->hand = 0;

	board->bowls = (int*) malloc(sizeof(int) * 14);
	for (int i = 0; i < 14; i++) {
		board->bowls[i] = 7;
	}

	return board;
}

void printBoard(Board* board) {
	printf("\nplayer 2 pit: %d\n", board->players[1].pit);

	printf("1    2    3    4    5    6    7\n---------------------------------\n");

	char num[6];

	for (int i = 7; i < 14; i++) {
		//fprintf(stderr, "d%dd",board->bowls[11]);
		if (i == board->selected) {
			sprintf(num, "(%d)  ", board->bowls[i]);
		} else {
			sprintf(num, "%d    ", board->bowls[i]);
		}
		num[5] = '\0';
		printf("%s", num);
		fflush(stdout);

		if (i == 13) {
			printf("\n");
		}
	}

	printf("\n%d              %d             %d\n\n", board->players[0].pit, board->hand, board->players[1].pit);


	for (int i = 6; i >= 0; i--) {  
		if (i == board->selected) {
			sprintf(num, "(%d)  ", board->bowls[i]); 
		} else {
			sprintf(num, "%d    ", board->bowls[i]);
		}

		num[5] = '\0';
                printf("%s", num);
		
		if (i == 0) {                                       printf("\n");
		}
	}

	printf("---------------------------------\n7    6    5    4    3    2    1\n");

	printf("player 1 pit: %d\n\n", board->players[0].pit);

	fprintf(stderr,"%d", board->turn);
	printf("player %d's turn\n\n", board->turn + 1);

	fflush(stdout);
}

void makeMove(Board* board, int move) {

	char buff[80];

	int startBowl = move - 1 + 7 * board->turn;
	int currBowl = startBowl;
	int hand = board->bowls[startBowl];
	board->bowls[startBowl] = 0;

	while (hand > 0) {

		//fprintf(stderr, "\n1d%d\n", board->bowls[11]);

		currBowl = (currBowl + 1) % 14;
		if ((currBowl + 7*board->turn) % 14 == 7) {
			//fprintf(stderr, "\n1d%d\n", board->bowls[11]);
			//pit
			board->players[board->turn].pit++;
			hand--;

			if (hand == 0) {

				while (1) {
				
					printf("Finised in pit, type a new bowl to pick up: ");

					if (board->mode == 1 && board->turn == 1) {
						autoMove(board);
						break;
					} else {
				
						fgets(buff, 80, stdin);
						if (checkNum(buff) && checkValid(board, atoi(buff))) {
							makeMove(board, atoi(buff));
							break;
						} else {
							printf("invalid move or input, try again\n");
						}
					}
				}
			} else {
				board->bowls[currBowl]++;
				hand--;
			}
		} else {
		//	fprintf(stderr, "\n2d%d\n", board->bowls[11]);
			board->bowls[currBowl]++;
		//	fprintf(stderr, "\n3d%d\n", board->bowls[11]);
			hand--;

		}
		if (hand == 0) {
	//		fprintf(stderr, "\n1d%d\n", board->bowls[11]);
			//fprintf(stderr, "debug, here!");
			if (board->bowls[currBowl] > 1) {
				hand = board->bowls[currBowl];
				board->bowls[currBowl] = 0;
			} else if ((currBowl + 7*board->turn) % 14 < 7) { //on players side
				int oppositeBowl = (currBowl + (7 - (currBowl % 7))*2 - 1) % 14;
		
				board->players[board->turn].pit += board->bowls[oppositeBowl];
				printf("Finished on your side, %d shells added to your pot\n", board->bowls[oppositeBowl]);
				board->bowls[oppositeBowl] = 0;
				board->selected = currBowl;
				board->hand = hand;
				break;
			} else {
				printf("Finished turn on oppisition side\n");
				board->selected = currBowl;
				board->hand = hand;
				break;
			}
		}
		sleep(1);
		//fprintf(stderr, "\n4d%d\n", board->bowls[11]);
		board->selected = currBowl;
		board->hand = hand;
		//fprintf(stderr, "\n5d%d\n", board->bowls[11]);
		printBoard(board);
		printf("Turn in progress... (%d shells in hand)\n", hand);
	}
}

void autoMove(Board* board) {
	srand(time(0));

	while (1) { 
		int move = rand() % 7 + 1;
		if (checkValid(board, move)) {
			makeMove(board, move);
			nextTurn(board);
			break;
		}
	}
}
		

void nextTurn(Board* board) {
	int next = board->turn;

	for (int i = 7*next; i<7+7*next; i++) {
		if (board->bowls[i] != 0) {
			next = (board->turn + 1) % 2;
			break;
		}
	}

	board->turn = next;
}

void game(Board* board) {

	char buff[80];

	//get move,
	//check if move num and validity
	//make move
	//if next play can make move, changs turn
	//check if game over

	while(1) {
		printBoard(board);

		if (board->mode == 0 && board->turn == 1) {
			autoMove(board);
		} else {
			printf("type a bowl to pick up: ");
			fgets(buff, 80, stdin);

			if (checkNum(buff) && checkValid(board, atoi(buff))) {
				makeMove(board, atoi(buff));
				nextTurn(board);
			} else {
				printf("invalid move or input, try again\n");
			}
		} 
	}

}

int main(int argc, char** argv) {

	Board* board = init();
	game(board);

	//printf("%d", -7 % 14;



	return 0;
}
