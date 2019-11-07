#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct card
{
  char suit;
  int number;
  int null;			//1 = null
};

struct player
{
  int id;
  struct card *hand;
  char type;
};

void printBoard (int, int, struct card **);
void game (int width, int height, struct card **board, struct player p1,
	   struct player p2, struct card *deck, int *deckLength, int turn);
struct card **readBoard (FILE * gameFile, int width, int height);
struct card *readHand (char *hand, int expectedCards);
void printHand (struct player p);
void printDeck (struct card *deck, int numCards);
struct card *readDeck (FILE * deckFile, int *numCardPointer);
char **readWHNV (FILE * gameFile);

struct card *readDeckLoad (char *deckFileChar, int numPicked,
			   int *numDeckCards);

void
readGame (FILE * gameFile, char **argv)
{
  if (gameFile == NULL)
    {
      fprintf (stderr, "Unable to parse savefile\n");
      exit (4);
    }

  char **whnv = readWHNV (gameFile);
  char **deckAndHands = malloc (sizeof (char *) * 3);
  for (int i = 0; i < 3; i++)
    {
      //buff
      deckAndHands[i] = malloc (sizeof (char) * 100);
    }
  //read 2nd 3rd 4th line
  for (int i = 0; i < 3; i++)
    {
      int position = 0;
      int next = 0;

      while (1)
	{
	  next = fgetc (gameFile);
	  if (next == EOF)
	    {
	      //fprintf(stderr,"Unable to parse savefile\n");
	      //exit(4);
	    }
	  if (next == '\n')
	    {
	      deckAndHands[i][position] = '\0';
	      break;
	    }
	  else
	    {
	      deckAndHands[i][position++] = (char) next;
	    }
	}
    }
  //deck
  int numDeckCards;
  struct card *deck =
    readDeckLoad (deckAndHands[0], atoi (whnv[2]), &numDeckCards);

  //players with hands
  struct player p1;
  p1.id = 1;
  p1.type = argv[2][0];
  struct player p2;
  p2.id = 2;
  p2.type = argv[3][0];
  int turn;
  if (atoi (whnv[3]) == 2)
    {
      turn = 1;
      p1.hand = readHand (deckAndHands[1], 5);
      p2.hand = readHand (deckAndHands[2], 6);
      for (int i = 0; i < numDeckCards; i++)
	{
	  if (deck[i].null == 0)
	    {
	      p1.hand[5] = deck[i];
	      deck[i].null = 1;
	      break;
	    }
	}
    }
  else if (atoi (whnv[3]) == 1)
    {
      turn = 0;
      p1.hand = readHand (deckAndHands[1], 6);
      p2.hand = readHand (deckAndHands[2], 5);
      for (int i = 0; i < numDeckCards; i++)
	{
	  if (deck[i].null == 0)
	    {
	      p2.hand[5] = deck[i];
	      deck[i].null = 1;
	      break;
	    }
	}
    }
  else
    {				//err
      //fprintf(stderr, "Unable to parse savefile\n");
      //exit(4);
    }
  //check
  int width = atoi (whnv[0]);
  int height = atoi (whnv[1]);

  struct card **board;
  board = readBoard (gameFile, width, height);
  game (width, height, board, p1, p2, deck, &numDeckCards, turn);
}

char **
readWHNV (FILE * gameFile)
{
  char **whnv = malloc (sizeof (char *) * 4);
  for (int i = 0; i < 4; i++)
    {
      //buffer, subject to change
      whnv[i] = malloc (sizeof (char) * 32);
    }
  //read first line
  for (int i = 0; i < 4; i++)
    {
      int position = 0;
      int next = 0;

      while (1)
	{
	  next = fgetc (gameFile);
	  if (next == EOF)
	    {
	      //fprintf(stderr, "Unable to parse savfile\n");
	      //exit(4);
	    }
	  if (next == '\n')
	    {
	      if (i == 3)
		{
		  whnv[3][position] = '\0';
		  break;
		}
	      else
		{
		  //fprintf(stderr, "Unable to parse savfile\n");
		  //exit(4);
		}
	    }

	  if (next == ' ')
	    {
	      whnv[i][position] = '\0';
	      break;
	    }
	  //check if int's
	  else
	    {
	      whnv[i][position++] = (char) next;
	    }
	}
    }
  return whnv;
}

struct card *
readDeckLoad (char *deckFileChar, int numPicked, int *numDeckCards)
{

  FILE *deckFile = fopen (deckFileChar, "r");
  if (deckFile == NULL)
    {
      fprintf (stderr, "Unable to parse deckfile\n");
      exit (3);
    }

  struct card *deck;
  deck = readDeck (deckFile, numDeckCards);
  for (int i = 0; i < numPicked; i++)
    {
      deck[i].null = 1;
    }

  fclose (deckFile);

  return deck;

}

struct card *
readHand (char *charHand, int expectedCards)
{
  struct card *hand;
  hand = malloc (sizeof (struct card) * 6);
  if (strlen (charHand) > (expectedCards * 2))
    {
      //fprintf(stderr, "Unable to parse savfile\n");
      //exit(4);
    }
  for (int i = 0; i < expectedCards; i++)
    {
      struct card card;
      int num = (int) charHand[i * 2] - '0';
      char suit = charHand[i * 2 + 1];
      //check err
      card.null = 0;
      card.number = num;
      card.suit = suit;
      hand[i] = card;
    }

  return hand;
}

struct card **
readBoard (FILE * gameFile, int width, int height)
{

  struct card **board = malloc (sizeof (struct card *) * width);

  for (int i = 0; i < width; i++)
    {
      board[i] = malloc (sizeof (struct card) * height);
    }
  for (int i = 0; i < height; i++)
    {
      int position = 0;
      int next = 0;

      while (1)
	{
	  struct card card;
	  next = fgetc (gameFile);
	  if (next == EOF)
	    {
	      //err
	    }
	  if (next == '\n')
	    {
	      break;
	    }
	  if (next == '*')
	    {
	      next = fgetc (gameFile);
	      if (next != '*')
		{
		  //fprintf(stderr, "Unable to parse savfile\n");
		  //exit(4);
		}
	      else
		{
		  card.null = 1;
		  board[position][i] = card;
		}
	    }
	  if (next - '0' > 0 && next - '0' <= 9)
	    {
	      card.null = 0;
	      card.number = next - '0';
	      next = fgetc (gameFile);
	      if (next >= 'A' && next <= 'Z')
		{
		  card.suit = next;
		  board[position][i] = card;
		}
	      else
		{
		  //fprintf(stderr, "Unable to parse savfile\n");
		  //exit(4);
		}
	    }
	  else
	    {
	      //fprintf(stderr, "Unable to parse savfile\n");
	      //exit(4);
	    }
	  position++;
	}
    }
  return board;
}

struct card *
readDeck (FILE * deckFile, int *numCardPointer)
{
  //--------------------------------------
  // Function takes an opened deckFile and pointer to a num card
  //
  // Returns an array of card *objects* (structs) and changes the
  // dereferenced numCardPointer to the value of the num of cards
  // in the array
  // -------------------------------------

  if (deckFile == NULL)
    {
      fprintf (stderr, "Unable to parse deckfile\n");
      exit (3);
    }

  int next = 0;
  int numCards;
  int position = 0;
  char *charNumCards = malloc (sizeof (char) * 5000);	//change size later

  while (1)
    {
      next = fgetc (deckFile);
      if (next == '\n')
	{
	  charNumCards[position] = '\0';
	  break;
	}
      else
	{
	  //check if int and if EOF
	  charNumCards[position++] = (char) next;
	}
    }
  numCards = atoi (charNumCards);
  struct card *deck = malloc (sizeof (struct card) * (numCards));

  //for array size tracking
  *numCardPointer = numCards;
  if (numCards < 11)
    {
      fprintf (stderr, "Short deck\n");
      exit (5);
      return deck;
    }
  for (int i = 0; i < numCards; i++)
    {
      int num;
      char suitChar;
      struct card tempCard;

      next = fgetc (deckFile);
      if (((next - '0') <= 9) && ((next - '0') >= 1))
	{
	  num = next - '0';
	}
      else
	{			//throw error
	  fprintf (stderr, "Unable to parse deckfile\n");
	  exit (3);
	  break;
	}

      next = fgetc (deckFile);
      if ((next - 'A') >= 0 && (next - 'A') <= 25)
	{
	  suitChar = (char) next;
	}
      else
	{			//throw err
	  fprintf (stderr, "Unable to parse deckfile\n");
	  exit (3);
	  break;
	}

      next = fgetc (deckFile);
      if (next != '\n')
	{
	  fprintf (stderr, "Unable to parse deckfile\n");
	  exit (3);
	}

      tempCard.null = 0;
      tempCard.number = num;
      tempCard.suit = suitChar;
      deck[i] = tempCard;

    }

  return deck;

}

char **
readMove (FILE * file)
{
  //for processing stdin during game
  char **move = malloc (sizeof (char *) * 3);
  for (int i = 0; i < 3; i++)
    {
      move[i] = malloc (sizeof (char) * 32);
    }

  for (int i = 0; i < 3; i++)
    {
      int position = 0;
      int next = 0;

      while (1)
	{
	  next = fgetc (file);
	  if (next == EOF)
	    {
	      fprintf (stderr, "End of input\n");
	      exit (7);
	    }
	  if (next == '\n')
	    {
	      if (i == 2)
		{
		  return move;
		}
	      else
		{		//error
		  char **invalid = malloc (sizeof (char *) * 3);
		  invalid[0] = "0";
		  invalid[1] = "0";
		  invalid[2] = "0";
		  return invalid;
		}
	    }

	  if (next == ' ')
	    {
	      move[i][position] = '\0';
	      break;
	    }
	  else
	    {
	      move[i][position++] = (char) next;
	    }
	}
    }
  return move;
}


void
newGame (char **argv)
{
  //this function focuses on initializing and setting up the game
  struct card blankCard;
  blankCard.null = 1;
  char *deckFileString = argv[1];


  //check compliance
  int w = atoi (argv[2]);
  int h = atoi (argv[3]);

  if (w > 100 || w < 3 || h > 100 || h < 3)
    {
      fprintf (stderr, "Incorrect arg types\n");
      exit (2);
    }
  for (int i = 4; i < 6; i++)
    {
      if (strcmp ("h", argv[i]) != 0 && strcmp ("a", argv[i]) != 0)
	{
	  fprintf (stderr, "Incorrect arg types\n");
	  exit (2);
	}
    }
  //initialize new players
  struct player p1;
  p1.id = 1;
  p1.type = argv[4][0];
  p1.hand = (struct card *) malloc (sizeof (struct card) * 6);
  struct player p2;
  p2.id = 2;
  p2.type = argv[5][0];
  p2.hand = (struct card *) malloc (sizeof (struct card) * 6);

  //init deck
  FILE *deckFile = fopen (deckFileString, "r");
  int numDeckCards;
  struct card *deck;
  deck = readDeck (deckFile, &numDeckCards);
  fclose (deckFile);

  //init hand
  for (int i = 0; i < 5; i++)
    {
      int p2Index = i + 5;
      p1.hand[i] = deck[i];
      p2.hand[i] = deck[p2Index];
      deck[i].null = 1;
      deck[p2Index].null = 1;
    }
  //pick up card
  p1.hand[5] = deck[10];
  deck[10].null = 1;
  p2.hand[5] = deck[11];
  deck[11].null = 1;

  //init board
  struct card **board = (struct card **) malloc (sizeof (struct card *) * w);
  for (int i = 0; i < w; i++)
    {
      board[i] = (struct card *) malloc (sizeof (struct card) * h);
      //set to default blank cards
      for (int j = 0; j < h; j++)
	{
	  board[i][j] = blankCard;
	}
    }
  //after all is initialized, start game

  game (w, h, board, p1, p2, deck, &numDeckCards, 0);
}

int
isBoardFull (int width, int height, struct card **board)
{
  int result = 0;
  for (int i = 0; i < width; i++)
    {
      for (int j = 0; j < height; j++)
	{
	  if (board[i][j].null == 1)
	    {
	      result = 1;
	    }
	}
    }
  if (result == 0)
    {
      return 1;
    }
  else
    {
      return 0;
    }
}

int
isDeckEmpty (int numDeckCards, struct card *deck)
{

  for (int i = 0; i < numDeckCards; i++)
    {
      if (deck[i].null == 0)
	{
	  return 0;
	}
    }
  //will only get here if empty deck

  return 1;
}

int
place (int x, int y, int cardNum, struct player *p,
       struct card *deck, int numDeckCards, struct card **board)
{

  struct card pickedCard = p->hand[(cardNum - 1)];

  for (int i = cardNum; i < 6; i++)
    {
      p->hand[i - 1] = p->hand[(i)];
    }

  for (int i = 0; i < numDeckCards; i++)
    {
      if (deck[i].null == 0)
	{
	  p->hand[5] = deck[i];
	  deck[i].null = 1;
	  break;
	}
      else
	{

	}
    }
  //if code gets here, there was no cards left in deck
  //temp
  board[x][y] = pickedCard;
  return 0;
}

void
printBoard (int width, int height, struct card **board)
{

  for (int h = 0; h < height; h++)
    {
      for (int w = 0; w < width; w++)
	{
	  //decode the card for printing
	  if (board[w][h].null == 1)
	    {
	      printf ("..");
	    }
	  else
	    {
	      printf ("%d%c", board[w][h].number, board[w][h].suit);
	    }
	}
      printf ("\n");
    }
}

void
printDeck (struct card *deck, int numCards)
{
  printf ("current deck: ");
  for (int i = 0; i < numCards; i++)
    {
      if (deck[i].null != 1)
	{
	  printf (" %d%c", deck[i].number, deck[i].suit);
	}
    }
  printf ("\n");

}

void
printHand (struct player p)
{
  for (int i = 0; i < 6; i++)
    {
      if (p.hand[i].null == 0)
	{
	  printf (" %d%c", p.hand[i].number, p.hand[i].suit);
	}
    }
  printf ("\n");

}

int
checkAdjacent (int width, int height, int x, int y, struct card **board)
{
  //return 0 if found, 1 if none
  int leftX = x - 1;
  int rightX = x + 1;
  int aboveY = y - 1;
  int belowY = y + 1;

  if (leftX < 0)
    {
      leftX = width - 1;
    }
  if (rightX == width)
    {
      rightX = 0;
    }
  if (aboveY < 0)
    {
      aboveY = height - 1;
    }
  if (belowY == height)
    {
      belowY = 0;
    }

  if (board[leftX][y].null == 0 || board[rightX][y].null == 0 ||
      board[x][aboveY].null == 0 || board[x][belowY].null == 0)
    {
      return 0;
    }
  else
    {
      return 1;
    }
}

int
checkMove (int width, int height, int x, int y,
	   int pickedCard, struct card **board, int turn)
{
  //checks if in game move was legal, returns 1 if not
  if (x >= width || x < 0 || y >= height || y < 0 || pickedCard > 6)
    {
      return 1;
    }

  if (board[x][y].null == 0)
    {
      return 1;
    }

  if (turn > 0 && checkAdjacent (width, height, x, y, board) == 1)
    {
      return 1;
    }

  return 0;

}

void
autoMove (int width, int height, struct card **board, struct player *p,
	  struct card *deck, int *numCards)
{
  int empty = 1;
  for (int i = 0; i < width; i++)
    {
      for (int j = 0; j < height; j++)
	{
	  if (board[i][j].null == 0)
	    {
	      empty = 0;
	    }
	}
    }
  if (empty == 1)
    {
      int x = (int) ((width - 1) / 2);
      int y = (int) ((height - 1) / 2);
      place (x, y, 1, p, deck, *numCards, board);
      printf ("Player %d plays %d%c in column %d row %d\n",
	      p->id, board[x][y].number, board[x][y].suit, (x + 1), (y + 1));

      return;
    }


  if (p->id == 1)
    {
      for (int y = 0; y < height; y++)
	{
	  for (int x = 0; x < width; x++)
	    {
	      if (0 == checkMove (width, height, x, y, 1, board, 1))
		{
		  place (x, y, 1, p, deck, *numCards, board);
		  printf ("Player %d plays %d%c in column %d row %d\n",
			  1, board[x][y].number, board[x][y].suit,
			  (x + 1), (y + 1));
		  return;
		}
	    }

	}
    }
  else if (p->id == 2)
    {
      for (int y = height - 1; y >= 0; y--)
	{
	  for (int x = width - 1; x >= 0; x--)
	    {
	      if (0 == checkMove (width, height, x, y, 1, board, 1))
		{
		  place (x, y, 1, p, deck, *numCards, board);
		  printf ("Player %d plays %d%c in column %d row %d\n",
			  2, board[x][y].number, board[x][y].suit,
			  (x + 1), (y + 1));
		  return;
		}
	    }

	}


    }

}

void
calculateScores (int width, int height, struct card **board)
{
  //uhhhhhhhhhhhhhh
  int p1Score = 0;
  int p2Score = 0;
  printf ("Player 1=%d Player 2=%d\n", p1Score, p2Score);
}

void
game (int width, int height, struct card **board, struct player p1,
      struct player p2, struct card *deck, int *numCards, int turn)
{

  int reMove = 0;
  int count = 0;
  while (1)
    {
      int playerNum = 1 + (turn % 2);
      //printDeck(deck, *numCards);
      if (reMove == 0)
	{
	  printBoard (width, height, board);
	  count += isDeckEmpty (*numCards, deck);
	  //printf("%d", count);
	  if (isBoardFull (width, height, board) || count == 3)
	    {
	      calculateScores (width, height, board);
	      exit (0);
	    }

	  if (playerNum == 1)
	    {
	      if (p1.type == 'a')
		{
		  printf ("Hand:");
		}
	      else
		{
		  printf ("Hand(%d):", playerNum);
		}
	      printHand (p1);
	    }
	  else
	    {
	      if (p2.type == 'a')
		{
		  printf ("Hand:");
		}
	      else
		{
		  printf ("Hand(%d):", playerNum);
		}
	      printHand (p2);
	    }
	}
      reMove = 0;
      //printf("%d", playerNum);
      if (playerNum == 1 && p1.type == 'a')
	{
	  autoMove (width, height, board, &p1, deck, numCards);
	  turn++;
	}
      else if (playerNum == 2 && p2.type == 'a')
	{
	  autoMove (width, height, board, &p2, deck, numCards);
	  turn++;
	}
      else
	{
	  printf ("Move? ");

	  char **input = readMove (stdin);
	  //error check input
	  int x = atoi (input[1]) - 1;
	  int y = atoi (input[2]) - 1;
	  int pickedCard = atoi (input[0]);

	  //only process if Legal Move, else try loop again
	  int checkMo = checkMove (width, height, x, y,
				   pickedCard, board, turn);

	  if (checkMo == 0)
	    {
	      if (playerNum == 1)
		{
		  if (place (x, y, pickedCard, &p1, deck, *numCards, board))
		    {
		      //no cards in deck
		      //exit status
		      //score
		      exit (0);
		    }

		}
	      else
		{
		  if (place (x, y, pickedCard, &p2, deck, *numCards, board))
		    {
		      //exit status
		      //score
		      exit (0);
		    }
		}
	      turn++;
	    }
	  else if (checkMo == 1)
	    {
	      reMove = 1;
	    }
	}
    }
}

int
main (int argc, char **argv)
{

  if (argc == 6)
    {
      newGame (argv);
    }
  else if (argc == 4)
    {
      //code to load game
      char *loadFileChar = argv[1];
      if (loadFileChar[strlen (loadFileChar) - 1] == '/')
	{
	  fprintf (stderr, "Unable to parse deckfile\n");
	  exit (3);
	}
      FILE *loadFile = fopen (loadFileChar, "r");
      char t1 = argv[2][0];
      char t2 = argv[3][0];
      if (strlen (argv[2]) > 1 || strlen (argv[3]) > 1)
	{
	  fprintf (stderr, "Incorrect arg types\n");
	  exit (2);
	}
      if ((t1 != 'h' && t1 != 'a') || (t2 != 'h' && t2 != 'a'))
	{
	  fprintf (stderr, "Incorrect arg types\n");
	  exit (2);
	}
      readGame (loadFile, argv);
    }
  else
    {
      //exit status 1
      fprintf (stderr,
	       "Usage: bark savefile p1type p2type\nbark deck width height p1type p2type\n");
      exit (1);
    }

  return 0;
}