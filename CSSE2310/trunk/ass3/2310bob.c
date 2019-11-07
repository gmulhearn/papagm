#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "2310sharedPlayer.h"

//helper function for strat function.
//Returns: true if statisifies the second bob condition in the spec stragery,
//      false otherwise.
//Note: assumes that the spec means if ANY player has won threshold - 2 D cards
bool strat_decision_two(Game* game) {

    bool someD = false;
    bool someoneElse = false;
    for (int i = 0; i < game->numPlayers; i++) {
        if (game->rounds[game->currentRound].cardsPlaced[i].suit == 'D') {
            someD = true;
        }
        Player p = game->players[i];
        if (p.totalV >= (game->threshold - 2)) {
            someoneElse = true;
        }
    }
    if (!(someD && someoneElse)) {
        return false;
    }
    return true;

}

//function for determining which card to play based on the bob stragery (spec)
//Returns: the int cardIndex for which card to play in the players hand
//returns negative int if fails
int strat(Game* game) {
    
    char suits[5];
    strcpy(suits, "DHSC");
    char leadSuit = game->rounds[game->currentRound].leadSuit;
    Hand hand = game->player.hand;

    if (is_lead(game)) { //1
        for (int i = 0; i < 4; i++) {
            if (has_suit(hand, suits[i])) {
                return find_lowest_suit(hand, suits[i]);
            }
        }
    } else if (strat_decision_two(game)) { //2
        if (has_suit(hand, leadSuit)) {
            return find_highest_suit(hand, leadSuit);
        } else {
            strcpy(suits, "SCHD");
            for (int i = 0; i < 4; i++) {
                if (has_suit(hand, suits[i])) {
                    return find_lowest_suit(hand, suits[i]);
                }
            }
        }
    } else if (has_suit(hand, leadSuit)) { //3
        return find_lowest_suit(hand, leadSuit);
    } else { //4
        strcpy(suits, "SCDH");
        for (int i = 0; i < 4; i++) {
            if (has_suit(hand, suits[i])) {
                return find_highest_suit(hand, suits[i]);
            }
        }
    }

    return -1;
}

//main function
int main(int argv, char** argc) {
    Game game = init(argv, argc);

    game_loop(&game);
    return 0;
}