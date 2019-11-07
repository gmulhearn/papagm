#include "2310sharedPlayer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//puts an error message to stderr given a certain error Status int
//Returns: the same Status that was inputted
Status show_message(Status s) {
    const char* messages[] = {"",
            "Usage: player players myid threshold handsize\n",
            "Invalid players\n",
            "Invalid position\n",
            "Invalid threshold\n",
            "Invalid hand size\n",
            "Invalid message\n",
            "EOF\n"};

    fputs(messages[s], stderr);
    return s;
}

//checks if the provided string is an integer
//Returns: true if string can be converted to an int, false otherwise
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

//checks if the provided 2 chars are valid to be a suit and rank of a card
//Returns: true if suit and rank characters are valid, false otherwise
bool check_valid_card(char suit, char rank) {
    if (suit != 'S' && suit != 'C' && suit != 'D' && suit != 'H') {
        return false;
    }
    if (!((rank >= '1' && rank <= '9') || (rank >= 'a' && rank <= 'f'))) {
        return false;
    }
    return true;
}

//checks the basic intial arguments (argc) against several constraints
//Returns: the first line of stdin (should be the players hand if hub works).
//Throws error: if any of the arguments or first line of stdin are incorrect.
char* check_basic_args(int argv, char** argc) {
    
    if (argv != 5) {
        exit(show_message(INCORRECT_NUM_ARGS));
    }
    if (!check_if_num(argc[1]) || atoi(argc[1]) < 2) {
        exit(show_message(INVALID_NUM_PLAYERS));
    }
    if (!check_if_num(argc[2]) || atoi(argc[2]) > atoi(argc[1]) - 1) {
        exit(show_message(INVALID_POSITION));
    }
    if (!check_if_num(argc[3]) || atoi(argc[3]) < 2) {
        exit(show_message(INVALID_THRESHOLD));
    }
    if (!check_if_num(argc[4]) || atoi(argc[4]) < 1) {
        exit(show_message(INVALID_HAND_SIZE));
    }
    char* firstMsg = read_stdin();
    if (interpret_msg(firstMsg) != 1) {
        exit(show_message(INVALID_MSG));
    }

    return firstMsg;

    //fprintf(stdout, "@");
    //fflush(stdout);
}

//reads stdin until the first new line character
//Returns: a string of what it read until the new line character
//Throws Error: if stdin contained an EOF character
char* read_stdin(void) {
    char* string = malloc(sizeof(char) * 420); //fix buffer
    char currCharacter = ' ';
    int count = 0;

    while (1) {
        currCharacter = fgetc(stdin);
        if (currCharacter == EOF) {
            exit(show_message(UNEXPECTED_EOF));
        }
        if (currCharacter == '\n') {
            string[count] = '\0';
            return string;
        } else {
            string[count] = currCharacter;
            count++;
        }
    }
}

//interprets the string and determines what type of message it is.
//Returns: 1 if string is a message beginning with HAND
// 2 if message begins with PLAYED
// 3 if message begins with NEWROUND
// 4 if message begins with GAMEOVER
//Throws Error: if none of these four cases.
int interpret_msg(char* hubMsg) {
    const char* hand = "HAND";
    const char* newRound = "NEWROUND";
    const char* played = "PLAYED";
    const char* gameover = "GAMEOVER";

    if (!strncmp(hubMsg, hand, 4)) {
        //interpret_hand(hubMsg);
        return 1;
    } else if (!strncmp(hubMsg, played, 6)) {
        //interpret move
        return 2;
    } else if (!strncmp(hubMsg, newRound, 8)) {
        //interpret round
        return 3;
    } else if (!strncmp(hubMsg, gameover, 8)) {
        //intrpet game over
        return 4;
    } else {
        exit(show_message(INVALID_MSG));
    }
}

//given a string message and the Game Struct, decode the message into a Hand
//Returns: a hand struct with cards and length from the message
//Throws error: if incorrectly formatted message (i.e. not HANDn,SR,SR,..,SR)
Hand interpret_hand(char* hubMsg, Game game) {
    char handSize[69]; //fix
    int counter = 4;

    for (int i = 0; i < 69; i++) {
        if (hubMsg[i + 4] == ',') {
            handSize[i] = '\0';
            break;
        } else if (hubMsg[i + 4] == '\0') {
            exit(show_message(INVALID_MSG));
            break;
        } else {
            handSize[i] = hubMsg[i + 4];
        }
        counter++;
    }
    if (!check_if_num(handSize)) {
        exit(show_message(INVALID_MSG));
    }
    if (atoi(handSize) != game.givenHandSize) {
        exit(show_message(INVALID_MSG));
    }
    //printf("%d%d", atoi(handSize), game.player.hand.size);
    Hand hand;
    Card* cards = malloc(sizeof(Card) * atoi(handSize));
    for (int i = 0; i < atoi(handSize); i++) {
        Card card;
        if (hubMsg[counter++] != ',') {
            exit(show_message(INVALID_MSG));
        } 
        card.suit = hubMsg[counter++];
        if (!check_valid_card(card.suit, '2')) {
            exit(show_message(INVALID_MSG));
        }
        card.rank = hubMsg[counter++];
        if (!check_valid_card(card.suit, card.rank)) {
    
            exit(show_message(INVALID_MSG));
        }
        card.used = 0;
        cards[i] = card;
    }
    if (hubMsg[counter++] != '\0') {
        exit(show_message(INVALID_MSG));
    }
    hand.cards = cards;
    hand.size = atoi(handSize);

    return hand;
}

//Initializes the player given initial arguments, the game struct and handMsg
//Returns: player struct constructed with all given arguments
Player init_player(int argv, char** argc, Game* game, char* handMsg) {
    Player player;
    player.myID = atoi(argc[2]);
    game->numPlayers = atoi(argc[1]);
    game->threshold = atoi(argc[3]);
    player.hand = interpret_hand(handMsg, *game);

    //init players
    game->players = malloc(sizeof(Player) * game->numPlayers);
    for (int i = 0; i < game->numPlayers; i++) {
        Player otherPlayer;
        otherPlayer.myID = i;
        otherPlayer.wins = 0;
        otherPlayer.totalV = 0;
        game->players[i] = otherPlayer;
    }

    return player;
}

//initializes the game information given the arguments
//Returns: game struct constructed with all given arguments
Game init(int argv, char** argc) {
    Game game;

    char* handMsg = check_basic_args(argv, argc);
    game.givenHandSize = atoi(argc[4]);
    game.player = init_player(argv, argc, &game, handMsg);

    game.currentRound = (-1);
    game.numRounds = game.givenHandSize;
    game.rounds = malloc(sizeof(Round) * game.numRounds);

    printf("@");
    fflush(stdout);

    return game;
}

//converts the players in the game into a string displaying their V score
//Returns that string ^ (used for debuggin)
char* players_to_string(Game* game) {
    char* string = malloc(sizeof(char) * 420); //fix
    strcpy(string, "");
    char playerV[20];

    for (int i = 0; i < game->numPlayers; i++) {
        if (game->players[i].myID == game->player.myID) {
            sprintf(playerV, "player V (me): %d\n", game->players[i].totalV);
        } else {
            sprintf(playerV, "player V: %d\n", game->players[i].totalV);
        }
        strcat(string, playerV);
    }

    return string;
}

//converts a hand struct into a string (used for debugging purposes)
//Returns: string format of the provided Hand struct
char* hand_to_string(Hand* playerHand) {

    char* string = malloc(sizeof(char) * 4 * playerHand->size);
    strcpy(string, "");
    char suitRank[4];

    for (int i = 0; i < playerHand->size; i++) {
        sprintf(suitRank, "%d%c%c ", playerHand->cards[i].used,
                playerHand->cards[i].suit, 
                playerHand->cards[i].rank);
        strcat(string, suitRank);
    }

    return string;
}

//converts a given round into a string (used for stderr msg and debugging)
//Returns: string format of the provided Round 
char* round_to_string(Game* game, int roundNum) {
    
    char* string = malloc(sizeof(char) * (4 * game->numPlayers
            + 69)); //fix
    strcpy(string, "Lead player=");
    char playerNum[69];//fix
    char suitRank[4];
    Round round = game->rounds[roundNum];

    sprintf(playerNum, "%d:", round.leader.myID);
    strcat(string, playerNum);

    for (int i = 0; i < game->numPlayers; i++) {
        sprintf(suitRank, " %c.%c", round.cardsPlaced[i].suit, 
                round.cardsPlaced[i].rank);
        strcat(string, suitRank);
    }
    strcat(string, "\n");

    return string;
}

//prints the current game and player info for debugging purposes
void print_info(Game game) {
    printf("\n\nGame: numPlayers - %d, threshold - %d \n",
            game.numPlayers, game.threshold);
    printf("Player: myID - %d \n", game.player.myID);
    printf("Hand: size - %d \n", game.player.hand.size);
    printf("Current Hand - %s \n", hand_to_string(&(game.player.hand)));
    printf("Round %d Cards - %s \n", game.currentRound,
            round_to_string(&game, game.currentRound));
    printf("Scores: \n %s \n\n", players_to_string(&game));
}

//plays a card at a certain index on the players hand (relies of prior checks)
//sends the played card to stdout in format "PLAYSR"
void play_card(int cardIndex, Game* game) {
    game->player.hand.cards[cardIndex].used = 1;

    char* string = malloc(sizeof(char) * 8);
    char suitRank[3];

    strcpy(string, "PLAY");
    sprintf(suitRank, "%c%c", game->player.hand.cards[cardIndex].suit,
            game->player.hand.cards[cardIndex].rank);
    strcat(string, suitRank);
    strcat(string, "\n");

    printf(string);
    fflush(stdout);

    //add to cards Placed
    game->rounds[game->currentRound].
            cardsPlaced[game->rounds[game->currentRound].turn] 
            = game->player.hand.cards[cardIndex];
    game->rounds[game->currentRound].turn++;
}

//given a certain hubMsg, determine whether it is the players turn now
//
int whomst_turn(char* hubMsg, Game game) {
    //int numPlayers = game.numPlayers;
    //int myID = game.player.myID;
    char pNum[420]; //fix
    char terminator;
    int pos, origin;
    if (interpret_msg(hubMsg) == 2) {
        pos = 6;
        terminator = ',';
    } else if (interpret_msg(hubMsg) == 3) {
        pos = 8;
        terminator = '\0';
    }
    origin = pos;

    while (1) {
        if (hubMsg[pos] == '\0' && origin != 8) { 
            exit(show_message(INVALID_MSG)); //indicates invalid PLAYED msg
        }
        if (hubMsg[pos] == terminator) {
            pNum[pos - origin] = '\0';
            break;
        } else {
            pNum[pos - origin] = hubMsg[pos];
            pos++;
        }
    }
    if (!check_if_num(pNum)) {
        exit(show_message(INVALID_MSG));
    } else if (atoi(pNum) >= game.numPlayers) {
        exit(show_message(INVALID_MSG));
    }

    return atoi(pNum);
}

//initalize new round struct; note lead player and update currentRound num
//update previous round struct; i.e. give winner
void game_loop_newround(Game* game, char* hubMsg) {
    Round thisRound;
    game->currentRound++;
    int playerID = whomst_turn(hubMsg, *game);
    
    if (game->currentRound > 0) { 

        game->rounds[game->currentRound - 1].winner = game->players[playerID];
        game->players[playerID].wins++;

        for (int i = 0; i < game->numPlayers; i++) {
            if (game->rounds[game->currentRound - 1].cardsPlaced[i].suit
                    == 'D') {
                game->players[playerID].totalV++;
            }
        }

    }
    
    thisRound.turn = 0;
    thisRound.leader = game->players[playerID];

    thisRound.cardsPlaced = malloc(sizeof(Card) * game->numPlayers);
    for (int i = 0; i < game->numPlayers; i++) {
        thisRound.cardsPlaced[i].rank = '0';
        thisRound.cardsPlaced[i].suit = '0';
    }

    game->rounds[game->currentRound] = thisRound;

}

//if first turn, record lead suit for round, update turn count for round,
//record cards placed in round
void game_loop_played(Game* game, char* hubMsg) {

    if (game->currentRound == -1) {
        exit(show_message(INVALID_MSG)); //newround hasnt been called yet
    } else if (game->rounds[game->currentRound].turn >= game->numPlayers) {
        exit(show_message(INVALID_MSG)); //too many turns
    }

    char next = 'a';
    int position = 0;
    //put position to 1 after comma.
    while (next != ',') {
        next = hubMsg[position++];
    }
    char suit = hubMsg[position];
    char rank = hubMsg[position + 1];

    if (!check_valid_card(suit, rank)) {
        exit(show_message(INVALID_MSG));
    }
    if (game->rounds[game->currentRound].turn == 0) {
        game->rounds[game->currentRound].leadSuit = suit;
    }

    Card playedCard;
    playedCard.suit = suit;
    playedCard.rank = rank;
    
    //dont add my own card bcus its already added after play_card method
    if (whomst_turn(hubMsg, *game) != game->player.myID) {
        game->rounds[game->currentRound].
                cardsPlaced[game->rounds[game->currentRound].turn] 
                = playedCard;
    }

    game->rounds[game->currentRound].turn++;

    if (game->rounds[game->currentRound].turn == game->numPlayers) {
        fprintf(stderr, round_to_string(game, game->currentRound));
    }

}

//the main loop of the game, takes a pointer to game struct.
void game_loop(Game* game) {
    char prevMsg[69]; //fix
    strcpy(prevMsg, "THIS IS A TEMPORARY MSG");
    bool gameover = false;

    while (!gameover) {
        char* hubMsg = read_stdin();
        int msgType = interpret_msg(hubMsg);
        
        //printf("is my turn? - %d\n\n", whomst_turn(hubMsg, *game));
        if (strcmp(hubMsg, prevMsg) != 0) {
            if (msgType == 3) {
                //NEWROUND
                game_loop_newround(game, hubMsg);

                if (whomst_turn(hubMsg, *game) == game->player.myID) {
                    play_card(strat(game), game); //strat in bob and alice
                }
                
            } else if (msgType == 2) {
                //PLAYED 
                game_loop_played(game, hubMsg);

                if ((whomst_turn(hubMsg, *game) + 1) % game->numPlayers
                        == game->player.myID &&
                        game->rounds[game->currentRound].turn 
                        < game->numPlayers) {
                    play_card(strat(game), game);
                }
            } else if (msgType == 4) {
                //gameover 
                gameover = true;
                exit(NORMAL_EXIT);
            } else {
                exit(show_message(INVALID_MSG));
            }
            strcpy(prevMsg, hubMsg); 
        }
        //print_info(*game);
    }
}

//determines if theres an unused card in a given hand of a given suit
//Returns: true if there is, false if there is not.
bool has_suit(Hand hand, char suit) {
    for (int i = 0; i < hand.size; i++) {
        if (hand.cards[i].suit == suit && hand.cards[i].used == 0) {
            return true;
        }
    }

    return false;
}


//finds the highest unused card in a given hand of a given suit (for strat)
//Returns: the index of that card in the hand.
//Note: only used after has_suit returns true for the given suit
int find_highest_suit(Hand hand, char suit) {
    int index = -1;
    char max = '0';
    for (int i = 0; i < hand.size; i++) {
        if (hand.cards[i].suit == suit && hand.cards[i].used == 0) {
            if (((int) hand.cards[i].rank) > ((int) max)) {
                max = hand.cards[i].rank;
                index = i;
            }
        }
    }
    
    return index;
}

//finds the lowest unused card in a given hand of a given suit (for strat)
//Returns: the index of that card in the hand
//Note: only used after has_suit returns true for the given suit
int find_lowest_suit(Hand hand, char suit) {
    int index = -1;
    char min = 'g';
    for (int i = 0; i < hand.size; i++) {
        if (hand.cards[i].suit == suit && hand.cards[i].used == 0) {
            if (((int) hand.cards[i].rank) < ((int) min)) {
                min = hand.cards[i].rank;
                index = i;
            }
        }
    }

    return index;
}

//determines whether the player is the leader of the current round
//Returns: true if they are the leader, false otherwise.
bool is_lead(Game* game) {
    if (game->rounds[game->currentRound].leader.myID == game->player.myID) {
        return true;
    } else {
        return false;
    }
}