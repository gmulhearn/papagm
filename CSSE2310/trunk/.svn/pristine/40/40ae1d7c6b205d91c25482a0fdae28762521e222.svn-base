#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct card {
	char suit;
	int number;
	int null; //1 = null
};

struct player {
	int id;
	struct card* hand;
	char type;
};

void printBoard(int, int, struct card**);
void game(int width, int height, struct card** board, struct player p1, struct player p2, struct card* deck, int* deckLength);


struct card* readDeck(FILE* deckFile, int* numCardPointer) {
	//--------------------------------------
	// Function takes an opened deckFile and pointer to a num card
	// 
	// Returns an array of card *objects* (structs) and changes the
	// dereferenced numCardPointer to the value of the num of cards 
	// in the array
	// -------------------------------------
	
	int next = 0;
	int numCards;
	int position = 0;
	char* charNumCards = malloc(sizeof(char) * 5); //change size later
	
	while (1) {
		next = fgetc(deckFile);
		if (next == '\n') {
			charNumCards[position] = '\0';
			break;
		} else {
			//check if int and if EOF
			charNumCards[position++] = (char) next;
		}
	}
	numCards = atoi(charNumCards);
	struct card* deck = malloc(sizeof(struct card) * (numCards));

	//for array size tracking
	*numCardPointer = numCards;
	if (numCards < 11) {
		printf("not enough cards in deck \n");
		//err
		return deck;
	}
	for (int i = 0; i < numCards; i++) {
		int num;
		char suitChar;
		struct card tempCard;
		
		next = fgetc(deckFile);
		if (((next - '0') <= 9) && ((next - '0') >= 1)) {
			num = next - '0';
		} else { //throw error
			printf("no sir \n");
			break;
		}
		
		next = fgetc(deckFile);
		if ((next - 'A') >= 0 && (next - 'A') <= 25) {
			suitChar = (char) next;
		} else { //throw err
			printf("char error \n");
			break;
		}
		
		next = fgetc(deckFile);
		if (next != '\n') {
			//throw err
		}
		
		tempCard.null = 0;
		tempCard.number = num;
		tempCard.suit = suitChar;
		deck[i] = tempCard;
		
	}
	
	return deck;

}

char** readMove(FILE* file) { 
	//for processing stdin during game
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




void newGame(char** argv) {
	//this function focuses on initializing and setting up the game
	struct card blankCard;
	blankCard.null = 1;
	char* deckFileString = argv[1]; 
	int w = atoi(argv[2]);
	int h = atoi(argv[3]);

	//check compliance	
	if (w > 100 || w < 3 || h > 100 || h < 3) {
		fprintf(stderr, "Incorrect arg types");
		return;
	}
	for (int i = 4; i < 6; i++) {
		if (strcmp("h", argv[i]) != 0 && strcmp("a", argv[i]) != 0) {
			fprintf(stderr, "Incorrect arg types");
			return;
		}
	}
	//initialize new players
	struct player p1;
	p1.id = 1;
	p1.hand = (struct card*) malloc(sizeof(struct card) * 6); 
	struct player p2;
	p2.id = 2;
	p2.hand = (struct card*) malloc(sizeof(struct card) * 6);
	
	//init deck
	FILE* deckFile = fopen(deckFileString, "r");
	int numDeckCards;
	struct card* deck;
	deck = readDeck(deckFile, &numDeckCards);
	fclose(deckFile);

	//init hand
	for (int i = 0; i < 6; i++) {
		int p2Index = i + 6;
		p1.hand[i] = deck[i];
		p2.hand[i] = deck[p2Index];
		deck[i].null = 1;
		deck[p2Index].null = 1;
	}

	//init board
	struct card** board = (struct card**) malloc(sizeof(struct card*) * w);
	for (int i = 0; i < w; i++) {
		board[i] = (struct card*) malloc(sizeof(struct card) * h);
		//set to default blank cards
		for (int j = 0; j < h; j++) {
			board[i][j] = blankCard; 
		}
	}
	//after all is initialized, start game
	
	game(w, h, board, p1, p2, deck, &numDeckCards); 
}

int place(int x, int y, int cardNum, struct player* p, 
		struct card* deck, int numDeckCards, struct card** board) {
	
	struct card pickedCard = p->hand[(cardNum - 1)];
	
	for (int i = cardNum; i < 6; i++) {
		//if (p->hand[i-1].number != p->hand[i].number || 
		//	p->hand[i-1].suit != p->
		p->hand[i - 1] = p->hand[(i)];
	}

	for (int i = 0; i < numDeckCards; i++) {
		if (deck[i].null == 0) {
			p->hand[5] = deck[i];
			deck[i].null = 1;
			board[x][y] = pickedCard;
			return 0;
		} else {
			//p->hand[5].null = 1;
		}
	}
	//if code gets here, there was no cards left in deck
	return 1;
}

void printBoard(int width, int height, struct card** board) {

	for (int h = 0; h < height; h++) {
		for (int w = 0; w < width; w++) {
			//decode the card for printing
			if (board[w][h].null == 1) {
				printf("..");
			}
			else {
				printf("%d%c", 
					board[w][h].number, 
					board[w][h].suit);
			}
		}
		printf("\n");
	}
}

void printDeck(struct card* deck, int numCards) {
	printf("current deck: ");
	for (int i = 0; i < numCards; i++) {
		if (deck[i].null != 1) {
			printf(" %d%c ",deck[i].number,deck[i].suit);
		}
	}
	printf("\n");

}

void printHand(struct player p) {
	for (int i = 0; i < 6; i++) {
		if (p.hand[i].null == 0) {	
			printf(" %d%c ", p.hand[i].number, 
				p.hand[i].suit);
		}
	}
	printf("\n");

}

int checkMove(int width, int height, int x, int y, 
		int pickedCard, struct card** board) {
	//checks if in game move was legal, returns 1 if not
	if (x>=width || y>= height || pickedCard > 6) {
		return 1;
	}

	if (board[x][y].null == 0) {
		return 1;
	}

	return 0;
}

void game(int width, int height, struct card** board, struct player p1, 
		struct player p2, struct card* deck, int* numCards) {
		
	int turn = 0;
	while (1) {
		int playerNum = 1 + (turn % 2);
		printBoard(width, height, board);
		printf("Hand(%d): ", playerNum);
		if (playerNum == 1) {
			printHand(p1);
		} else {
			printHand(p2);
		}
		printf("Move? ");

		char** input = readMove(stdin);
		//error check input
		int x = atoi(input[1]) - 1;
		int y = atoi(input[2]) - 1;
		int pickedCard = atoi(input[0]);
		
		//only process if Legal Move, else try loop again
		if (checkMove(width, height, x, y, pickedCard, board) 
			== 0) {

			if (playerNum == 1) {
				if (place(x, y, pickedCard, 
					&p1, deck, *numCards, board)) {
					//no cards in deck
					//exit status
					break;
				}
						
			} else {
				if (place(x, y, pickedCard,
					&p2, deck, *numCards, board)) {
					//exit status
					
					break;
				}


			}
			turn++;
		}

	}
}

int main(int argc, char** argv) {
	
	if (argc == 6) {
		newGame(argv);
	}

	else if (argc == 4) {
		//code to load game
	}

	else {
		//exit status 1	
		fprintf(stderr,"Usage: bark savefile p1type p2type bark deck width height p1type p2type");
		return 1;
	}	

	return 0;
}
