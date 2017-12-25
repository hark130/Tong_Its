#ifndef __TONG_ITS_GAME__
#define __TONG_ITS_GAME__

#include <memory>
#include <string>
#include "Tong_Its_Playing_Card.h"
#include "Tong_Its_Player.h"
#include <vector>

using namespace std;

// class Tong_Its_Player;  // Forward declared

class Tong_Its_Game 
{
public:
    // Tong_Its_Game(shared_ptr<string> humanPlayerName);
    Tong_Its_Game(const shared_ptr<string>& humanPlayerName);
    // ~Tong_Its_Game();  // NOT USED
    // Tong_Its_Player player1;
    // Tong_Its_Player player2;
    // Tong_Its_Player player3;
    void start_the_game(void);
    void TEST_the_deck(shared_ptr<vector<shared_ptr<PCard>>> deckToTest);  // DEBUGGING
    void receive_a_discard(shared_ptr<PCard> discardedCard);  // Tong_Its_Player discards a card
    shared_ptr<PCard> card_is_drawn(void);  // Tong_Its_Player draws a card from the drawPile
    shared_ptr<PCard> discard_is_taken(void);  // Tong_Its_Player takes the top discard from the discardPile
    // int currentDealer;  // Also the last winner
    int currentPlayer;  // AKA the dealer
    bool is_the_game_over(void);
    vector<Tong_Its_Player> players;
private:
    // Current draw pile of unseen cards
    shared_ptr<vector<shared_ptr<PCard>>> drawPile;  // NOTE: Draw from the back()
    // Current discard pile (only the top should be accessible)
    shared_ptr<vector<shared_ptr<PCard>>> discardPile;  // NOTE: Discard to the back()
    // Builds a vector of unique pointers to Card objects
    shared_ptr<vector<shared_ptr<PCard>>> build_a_deck(void);
    // Randomizes the order of a vector of card pointers
    void shuffle_a_deck(shared_ptr<vector<shared_ptr<PCard>>> deckOfCards);
    // Used to play, discard, deal, etc
    // void move_one_card(int cardNumber, shared_ptr<vector<shared_ptr<PCard>>> source, \
    //                    shared_ptr<vector<shared_ptr<PCard>>> destination);

    void deal_player_hands(Tong_Its_Player currentDealer);
    void print_a_card(shared_ptr<PCard> cardToPrint);
    int user_interface(void);
    void game_state(void);
    int next_player(void);
    int score_the_game(void);
    // int calc_chip_loss(const Tong_Its_Player& winner, const Tong_Its_Player& loser);
    int calc_chip_loss(Tong_Its_Player& winner, Tong_Its_Player& loser);
    void reset_game(int winnerNum);
    };

#endif // __TONG_ITS_GAME__
