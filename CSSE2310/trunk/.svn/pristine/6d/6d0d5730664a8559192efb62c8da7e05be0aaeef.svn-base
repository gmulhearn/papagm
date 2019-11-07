#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef enum {
    NORMAL_EXIT = 0,
    INCORRECT_NUM_ARGS = 1,
    INVALID_NUM_PLAYERS = 2,
    INVALID_POSITION = 3,
    INVALID_THRESHOLD = 4,
    INVALID_HAND_SIZE = 5,
    INVALID_MSG = 6,
    UNEXPECTED_EOF = 7
} Status;

typedef struct { 
    char rank;
    char suit;
    int used; //1 if used, 0 if unused
} Card;

typedef struct {
    int size;
    Card* cards;
} Hand;

typedef struct {
    int myID;
    Hand hand;
    int wins;
    int totalV;
} Player;

typedef struct {
    char leadSuit;
    Player leader;
    Player winner;
    Card* cardsPlaced;
    int turn;
} Round;

typedef struct {
    int numPlayers;
    Player* players;

    int currentRound;
    int numRounds;
    Round* rounds;
    
    int threshold;
    Player player;
    int givenHandSize;
} Game;

extern Status show_message(Status s);
extern char* check_basic_args(int argv, char** argc);
extern int interpret_msg(char* hubMsg);
extern Hand interpret_hand(char* hubMsg, Game game);
extern char* read_stdin(void);
extern Player init_player(int argv, char** argc, Game* game, char* handMsg);
extern Game init(int argv, char** argc);
extern void print_info(Game game);
extern char* hand_to_string(Hand* playerHand);
extern void play_card(int cardIndex, Game* game);
extern int whomst_turn(char* hubMsg, Game game);
extern void game_loop(Game* game);
extern bool has_suit(Hand hand, char suit);
extern int find_highest_suit(Hand hand, char suit);
extern int find_lowest_suit(Hand hand, char suit);
extern bool is_lead(Game* game);
extern int strat(Game* game);
