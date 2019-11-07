#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct card {
	int number;
	char suit;
	int null;
};


struct card* readDeck(FILE* deckFile, int* numCardPointer) {
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




int main(int argc, char** argv) {
	FILE* deckFile = fopen("deckFile.txt","r");
	int numCards;
	struct card* deck = readDeck(deckFile, &numCards);
	
	for (int i = 0; i < numCards; i++) {
		printf("%d %c \n",deck[i].number,deck[i].suit);
	}
	return 0;
}


