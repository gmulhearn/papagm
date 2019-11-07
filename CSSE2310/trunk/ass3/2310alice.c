#include "2310sharedPlayer.h"

//determines a card in the players hand to play, based on the alice stragery
//in the spec.
//Returns: integer card Index of the card in the player hand to play.
//returns negative int if fails.
int strat(Game* game) {
    char suits[5];
    strcpy(suits, "SCDH");

    if (is_lead(game)) {
        for (int i = 0; i < 4; i++) {
            if (has_suit(game->player.hand, suits[i])) {
                return find_highest_suit(game->player.hand, suits[i]);
            }
        }
    } else if (has_suit(game->player.hand,
            game->rounds[game->currentRound].leadSuit)) {
        return find_lowest_suit(game->player.hand, 
                game->rounds[game->currentRound].leadSuit);
    } else {
        strcpy(suits, "DHSC");
        for (int i = 0; i < 4; i++) {
            if (has_suit(game->player.hand, suits[i])) {
                return find_highest_suit(game->player.hand, suits[i]);
            }
        }
    }

    return -420;
}

//main function.
int main(int argv, char** argc) {
    
    //fprintf(stderr, "bruh");

    Game game = init(argv, argc);

    game_loop(&game);

    return 0;
}
