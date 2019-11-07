#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <limits.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>

//Exit Status enum's, to be used with exit(Status);
typedef enum {
    NORMAL_EXIT = 0,
    LESS_THAN_FOUR_ARGS = 1,
    INVALID_THRESHOLD = 2,
    DECK_READ_ERR = 3,
    NOT_ENOUGH_CARDS = 4,
    PLAYER_ERR = 5,
    UNEXPECTED_EOF = 6,
    INVALID_MSG = 7,
    INVALID_CHOICE = 8,
    RECEIVED_SIGHUP = 9
} Status;

/**
 * Places a message in stderr depending on the Status parameter
 * Returns the same status that was inputed
 */
Status show_message(Status s) {
    const char* messages[] = {"",
            "Usage: 2310hub deck threshold player0 {player1}\n",
            "Invalid threshold\n",
            "Deck error\n",
            "Not enough cards\n",
            "Player error\n",
            "Player EOF\n",
            "Invalid message\n",
            "Invalid card choice\n",
            "Ended due to signal\n"};

    fputs(messages[s], stderr);
    return s;
}

//struct for Card. Stores the rank and suit of the card.
typedef struct {
    char rank;
    char suit;
    int used;
} Card;

//struct for Deck. Stores an array of cards, the length of that array,
//and the amount of cards that have been used (i.e. given) in that deck.
typedef struct {
    unsigned int length;
    unsigned int used;
    Card* cards;
} Deck;

//struct for player Hand. Stores an array of cards which a player possesses,
//and the size of that array.
typedef struct {
    unsigned int size;
    Card* cards;
} Hand;

//struct for Player. Stores the players Hand, their unique ID given to them,
//their name (specified in the input args i.e. ./alice), their current num
//of points in the game, the amount of rounds they have won, and the amount
//of D cards they had placed during rounds they won.
typedef struct {
    Hand hand;
    int myID;
    char* name;
    int points;
    int roundsWon;
    int totalV;
    int sendPipe[2]; //hub has write end
    int receivePipe[2]; //player has write end
} Player;

//struct to store child pids, set up to be used globally
typedef struct {
    int numReady;
    pid_t* pidTags;
    int numKids;
} Pids;

//global pids
Pids pids;

//struct for Round. Stores the lead card suit type, lead player, and the ID
//of the current players turn.
//Post round, a winner and array of cards placed during the round will be
//also stored.
typedef struct {
    char leadSuit;
    int currTurn;
    Player leader;
    Player winner;
    Card* cardsPlaced;
} Round;

//Struct for Game. stores the number of rounds, the current round number,
//an array of rounds in the game, the number of players and array of Player's,
//the threshold and initial arguments.
typedef struct {
    //int numReady;

    int numRounds;
    int currentRound;
    Round* rounds;

    int numPlayers;
    Player* players;

    int threshold;
    int argv;
    char** argc;
} Game;

//return false if inputed string is not an int
//return true if is an int
bool check_if_num(char* string) {
    for (int i = 0; i < strlen(string); i++) {
        if (string[i] - '0' < 0 || string[i] - '0' > 9) {
            return false;
        }
    }
    if (strlen(string) == 0) {
        return false;
    }
    return true;
}

//performs initial check of inputted arguments,
//throws error if any of the arguments are invalid
void check_basic_args(int argv, char** argc) {
    if (argv < 5) {
        exit(show_message(LESS_THAN_FOUR_ARGS));
    }
    if (!check_if_num(argc[2]) || atoi(argc[2]) < 2) {
        exit(show_message(INVALID_THRESHOLD));
    }
}

//checks 2 chars to see if they conform to being a suit and rank of a card
//returns false if either the suit or rank character are invalid.
//returns true if valid for card.
bool check_card_valid(char suit, char rank) {
    if (suit != 'S' && suit != 'C' && suit != 'D' && suit != 'H') {
        return false;
    }
    if (!((rank >= '1' && rank <= '9') || (rank >= 'a' && rank <= 'f'))) {
        return false;
    }
    return true;
}

//attempts to load/generate a Deck from a file name. 
//Returns: deck struct if the file was formatted correctly
//Throws error: if incorrectly formated deck file, or if file doesnt exist.
Deck load_deck_file(char* fileName) {
    FILE* deckFile = fopen(fileName, "r");
    if (!deckFile) {
        exit(show_message(DECK_READ_ERR));
    }
    Deck deck;
    const short buffSize = (short)log10(INT_MAX) + 3;
    char buffer[buffSize];
    if (!fgets(buffer, buffSize - 1, deckFile)) {
        exit(show_message(DECK_READ_ERR));
    }
    char* err;
    unsigned int deckSize = strtoul(buffer, &err, 10);
    if (*err != '\n') {
        exit(show_message(DECK_READ_ERR));
    }

    Card* cards = malloc(sizeof(Card) * deckSize);

    deck.length = deckSize;
    for (int i = 0; i < deckSize; i++) {
        if (!fgets(buffer, buffSize - 1, deckFile)) {
            free(cards);
            exit(show_message(DECK_READ_ERR));
        }
        if (((buffer[2] != '\n') && (buffer[2] != '\0')) ||
                !check_card_valid(buffer[0], buffer[1])) {
            free(cards);
            exit(show_message(DECK_READ_ERR));
        }
        cards[i].suit = buffer[0];
        cards[i].rank = buffer[1];
    }
    deck.used = 0;
    deck.cards = cards;
    fclose(deckFile);

    return deck;
}

//converts a hand into a string that is valid to be sent to players.
//Returns: a string format of the inputted Hand struct.
char* hand_to_string(Hand* playerHand) {

    char num[50];
    sprintf(num, "%d", playerHand->size);
    char* string = malloc(sizeof(char) * 
            (4 + strlen(num) + 3 * (playerHand->size) + 4));

    char commaSuitRank[3];

    strcpy(string, "HAND"); 
    strcat(string, num);
    
    for (int i = 0; i < playerHand->size; i++) {
        sprintf(commaSuitRank, ",%c%c", 
                playerHand->cards[i].suit, playerHand->cards[i].rank);
        strcat(string, commaSuitRank);
    }
    strcat(string, "\n");
    
    return string;
}

//Generates a hand from the deck given a certain handSize
//Returns: a hand for a player containing *handSize* new cards from the deck
Hand init_hand(Deck* deck, int handSize) {
    Hand playerHand;
    playerHand.size = handSize;

    Card* cards = malloc(sizeof(Card) * handSize);

    for (int i = 0; i < handSize; i++) {
        cards[i] = deck->cards[deck->used];
        deck->used++;
    }
    playerHand.cards = cards;

    return playerHand;
}

//helper function for init_players. intializes basics for the player 
//Returns: an initialized player for init_players to use based on the args
Player init_player(Game* game, Deck* deck, int iD, int handSize) {
    Player player;
    Hand playerHand = init_hand(deck, handSize);
    player.hand = playerHand;
    player.myID = iD;
    player.name = game->argc[iD + 3];
    player.points = 0;
    player.roundsWon = 0;
    player.totalV = 0;

    return player;
}

//Initialises players for the game.
//uses the game deck and inputted arguments (argc) from the game struct to
//generate players, set up their pipes and exececute their programs. 
void init_players(Game* game, Deck* deck) {
    int handSize = (deck->length) / game->numPlayers;
    if (handSize == 0) {
        exit(show_message(NOT_ENOUGH_CARDS));
    }
    game->numRounds = handSize;
    if (game->numPlayers < 10) { //temp safety measure
        char** args = malloc(sizeof(char*) * 6);
        for (int i = 0; i < 5; i++) {
            args[i] = malloc(sizeof(char) * 69); //fix
        }
        for (int i = 0; i < game->numPlayers; i++) {
            Player player = init_player(game, deck, i, handSize);   
            if (pipe(player.sendPipe) != 0) {
                exit(show_message(PLAYER_ERR));
            } else if (pipe(player.receivePipe) != 0) {
                exit(show_message(PLAYER_ERR));
            }
            if ((pids.pidTags[i] = fork()) == -1) { //err
                exit(show_message(PLAYER_ERR));
            } else if (pids.pidTags[i] == 0) { //child     
                close(player.sendPipe[1]);
                dup2(player.sendPipe[0], 0); //stdin from read end of send
                close(player.sendPipe[0]);
                close(player.receivePipe[0]);
                dup2(player.receivePipe[1], 1); //stdout to write end of return
                close(player.receivePipe[1]);
                int logFd = open("log.txt", O_CREAT, O_WRONLY);
                dup2(logFd, 2);
                close(2);
                sprintf(args[0], player.name);
                sprintf(args[1], "%d", game->numPlayers);
                sprintf(args[2], "%d", i);
                sprintf(args[3], "%d", game->threshold);
                sprintf(args[4], "%d", handSize);
                args[5] = (char*) NULL;
                execv(args[0], args);
                exit(show_message(PLAYER_ERR));
            } else { //hub
                close(player.sendPipe[0]);
                close(player.receivePipe[1]);
                //send hand to player
                write(player.sendPipe[1], hand_to_string(&(player.hand)), 
                        strlen(hand_to_string(&(player.hand))));
                pids.numReady++;
            }     
            game->players[i] = player;
        }
    }
}

//function that checks for the initial '@' reply from the player via pipe
//Returns: true if just a single '@' character was sent, false otherwise
void check_init_reply(Game* game) {
    for (int i = 0; i < game->numPlayers; i++) {
        char buffer[2];
        int bytes = read(game->players[i].receivePipe[0], 
                buffer, sizeof(buffer));
        if (bytes != 1) {
            exit(show_message(PLAYER_ERR));
        } else if (buffer[0] != '@') {
            exit(show_message(PLAYER_ERR));
        }
    }
}

//Given initial arguments, generates the game struct 
//Returns: initialized Game struct to be used for full game
Game init_game(int argv, char** argc) {
    Game game1;
    game1.numPlayers = argv - 3;
    game1.threshold = atoi(argc[2]); //is ok because already checked arg
    game1.players = malloc(sizeof(Player) * game1.numPlayers);

    pids.pidTags = malloc(sizeof(pid_t) * game1.numPlayers);
    pids.numKids = game1.numPlayers;
    for (int i = 0; i < pids.numKids; i++) {
        pids.pidTags[i] = 0;
    }
    pids.numReady = 0;

    //temporary measure
    game1.argv = argv;
    game1.argc = argc;

    return game1;
}

//Runs initial processes to check and set up the game in accordance 
//with the inputted arugments (argc)
Game init(int argv, char** argc) {
    check_basic_args(argv, argc);
    Deck deck = load_deck_file(argc[1]);
    Game game1 = init_game(argv, argc);
    init_players(&game1, &deck);

    //init rounds (init player set numRounds)
    game1.currentRound = -1;
    game1.rounds = malloc(sizeof(Round) * game1.numRounds);

    for (int i = 0; i < game1.numRounds; i++) {
        game1.rounds[i].currTurn = 0;
        game1.rounds[i].cardsPlaced = malloc(sizeof(Card) * game1.numPlayers);
        for (int j = 0; j < game1.numPlayers; j++) {
            game1.rounds[i].cardsPlaced[j].rank = '0';
            game1.rounds[i].cardsPlaced[j].suit = '0';
        }
    }

    return game1;
}

//writes the given message to ALL players, throws error if unsuccessful
void send_all(Game* game, char* hubMsg) {
    for (int i = 0; i < game->numPlayers; i++) {
        if (write(game->players[i].sendPipe[1], hubMsg, strlen(hubMsg)) < 0) {
            //err
        }
    }
}

//checks whether a specific player can play the card they chose.
//Returns: true if they have the specific card and haven't already used it,
//  false otherwises.
//Note: also marks the specific card as used for future reference
bool valid_play(Game* game, int pNum, Card card) {
    Hand pHand = game->players[pNum].hand;
    for (int i = 0; i < pHand.size; i++) {
        if ((pHand.cards[i].rank == card.rank) 
                && (pHand.cards[i].suit == card.suit)
                && (pHand.cards[i].used == 0)) {
            game->players[pNum].hand.cards[i].used = 1;
            return true;
        }
    }
    return false;
}

//read PLAY message, interprets it and adds it to the rounds played cards
//throws error: if incorrect format
void interpret_msg(Game* game, int pNum) {
    char msg[10];
    int bytes = read(game->players[pNum].receivePipe[0], msg, sizeof(msg));

    if (bytes != 7) {
        exit(show_message(INVALID_MSG));
    }
    msg[7] = '\0';
    if (strncmp(msg, "PLAY", 4)) {
        exit(show_message(INVALID_MSG));
    }

    Card card;
    card.suit = msg[4];
    card.rank = msg[5];

    if (!check_card_valid(card.suit, card.rank)) {
        exit(show_message(INVALID_MSG));
    }
    //check if player has and hasn't already valid
    if (!valid_play(game, pNum, card)) {
        exit(show_message(INVALID_CHOICE));
    }
    game->rounds[game->currentRound].
            cardsPlaced[game->rounds[game->currentRound].currTurn] = card;
    //if the player is the leader, they set the lead suit
    if (game->rounds[game->currentRound].leader.myID == pNum) {
        game->rounds[game->currentRound].leadSuit = card.suit;
    }

    //send played msg to everyone but the player
    char playedMsg[69]; //fix
    sprintf(playedMsg, "PLAYED%d,%c%c\n", pNum, card.suit, card.rank);
    for (int i = 0; i < game->numPlayers; i++) {
        if (i != pNum) {
            if (write(game->players[i].sendPipe[1], 
                    playedMsg, strlen(playedMsg)) < 0) {
                //err
            }
        }
    }
}

//game loop helper function for when there is a new round
//increases game.currentRound, provides lead player (based on previous winner)
//sends out newround message too
void game_loop_newround(Game* game) {
    game->currentRound++;
    int roundNum = game->currentRound;
     
    if (roundNum > 0) {
        game->rounds[roundNum].leader = game->rounds[roundNum - 1].winner;
    } else {
        game->rounds[roundNum].leader = game->players[0];
    }

    char newRoundMsg[69]; //fix
    sprintf(newRoundMsg, "NEWROUND%d\n", game->rounds[roundNum].leader.myID);
    send_all(game, newRoundMsg);
    printf("Lead player=%d\n", game->rounds[roundNum].leader.myID);
    fflush(stdout);

}

//determines who the winner of the round was based on the spec and 
//updates the round winner accordingly
void determine_round_winner(Game* game) {
    int highest = 0;
    int pNum = -1;
    char leadSuit = game->rounds[game->currentRound].leadSuit;
    Card* cards = game->rounds[game->currentRound].cardsPlaced;

    for (int i = 0; i < game->numPlayers; i++) {
        if (cards[i].suit == leadSuit) {
            if ((int) cards[i].rank > highest) {
                highest = (int) cards[i].rank;
                pNum = (i + game->rounds[game->currentRound].leader.myID)
                        % game->numPlayers;
            }
        }
       
    }
    game->rounds[game->currentRound].winner = game->players[pNum];
    game->players[pNum].points++;
    char* string = malloc(sizeof(char) * (4 * game->numPlayers + 6)); //check
    strcpy(string, "Cards=");
    
    char suitRank[5]; //cehck
    for (int i = 0; i < game->numPlayers; i++) {
        if (cards[i].suit == 'D') {
            game->players[pNum].totalV++;
        }
        if (i == 0) {
            sprintf(suitRank, "%c.%c", cards[i].suit, cards[i].rank);
        } else {
            sprintf(suitRank, " %c.%c", cards[i].suit, cards[i].rank);
        }
        strcat(string, suitRank);
    }
    strcat(string, "\n");
    printf(string);
    fflush(stdout);
}

//calculates the final scores for each player and prints them to stdout
void final_scores(Game* game) {
    char* string = malloc(sizeof(char) * 100); //fix
    strcpy(string, "");
    char scoreString[10]; //fix

    for (int i = 0; i < game->numPlayers; i++) {
        int score;
        if (game->players[i].totalV >= game->threshold) {
            score = game->players[i].totalV + game->players[i].points;
        } else {
            score = game->players[i].points - game->players[i].totalV;
        }
        if (i == 0) {
            sprintf(scoreString, "%d:%d", i, score);
        } else {
            sprintf(scoreString, " %d:%d", i, score);
        }
        strcat(string, scoreString);
    }

    strcat(string, "\n");

    printf(string);
    fflush(stdout);
}

//main game loop which runs all the rounds and turns.
void game_loop(Game* game) {
    
    for (int i = 0; i < game->numRounds; i++) {
        //send new round info
        game_loop_newround(game);
        int leader = game->rounds[game->currentRound].leader.myID;

        //read/interpret pipe from each player
        for (int j = 0; j < game->numPlayers; j++) {
            int pNum = (j + leader) % (game->numPlayers);
            interpret_msg(game, pNum);
            game->rounds[game->currentRound].currTurn++;

        }

        //determine winner of round
        determine_round_winner(game);
    }

    send_all(game, "GAMEOVER\n");
    final_scores(game);
}

//function to run when a SIGHUP signal is received
void handler(int s) {
     
    //kill kids (only the ones initialized)
    for (int i = 0; i < pids.numKids; i++) {
        if (pids.pidTags[i] != 0) {
            kill(pids.pidTags[i], SIGKILL);
        }
    }
    
    exit(show_message(RECEIVED_SIGHUP));
}

//main function
int main(int argv, char** argc) {
    //set up signal handler
    struct sigaction sa;
    sa.sa_handler = handler;
    sa.sa_flags = SA_RESTART;
    sigaction(SIGHUP, &sa, 0);
    
    Game game = init(argv, argc);

    check_init_reply(&game);

    game_loop(&game);

    return 0;
}