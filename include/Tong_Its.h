#ifndef __TONG_ITS__
#define __TONG_ITS__

#include <iostream>
#include <memory>
#include <string>
#include <vector>

// #define SKINNY

#ifdef SKINNY
#define BORDER_UPPER_LEFT "┌"
#define BORDER_UPPER_RIGHT "┐"
#define BORDER_VERTICAL "│"
#define BORDER_LOWER_RIGHT "┘"
#define BORDER_LOWER_LEFT "└"
#define BORDER_HORIZONTAL "─"
#define BORDER_SPACE " "
#else
#define BORDER_UPPER_LEFT "╔"
#define BORDER_UPPER_RIGHT "╗"
#define BORDER_VERTICAL "║"
#define BORDER_LOWER_RIGHT "╝"
#define BORDER_LOWER_LEFT "╚"
#define BORDER_HORIZONTAL "═"
#define BORDER_SPACE " "
#endif // SKINNY OR FAT BOXES


using namespace std;

typedef struct Playing_Card
{
    Playing_Card(string pcRank, string pcSuit);
    // Playing_Card(string pcRank, char16_t pcSuit);
    string rank;    // Value as a string
    string suit;  // U+2660–2667
    int rankValue;      // Value as a number
    int suitValue;   // Ranking of the suit {1:4}
} PCard, *PCard_ptr;


class Tong_Its_Player
{
public:
    Tong_Its_Player(string playerName);
    // ~Tong_Its_Player();  // NOT USED
    void TEST_the_hand(void);  // DEBUGGING
    void toggle_sort(void);
    bool sorting_by_suit(void);
    string get_name(void);
    int count_chips(void);
    int count_cards(void);
    void receive_a_card(shared_ptr<PCard> drawnCard);
    shared_ptr<PCard> play_a_card(int cardNumber);
    void print_players_hand(void);
    int show_all_melds(bool playOne);
    int show_all_runs(bool playOne, int startingNum);
    int show_all_sets(bool playOne, int startingNum);
    void print_a_meld(vector<shared_ptr<PCard>> oneMeld);
    void sort_players_hand(void);
private:
    bool sortBySuit;
    string name;
    int numOfChips;
    int numOfCards;
    shared_ptr<vector<shared_ptr<PCard>>> playersHand;
    void print_a_row(int rowToPrint);
    bool sort_by_suit(shared_ptr<PCard> left, shared_ptr<PCard> right);
    bool sort_by_rank(shared_ptr<PCard> left, shared_ptr<PCard> right);
    int random_num(int start, int stop);
    // AI decision algorithm
    //    Random Plays
    //    Overt
    //    Covert
    //      Minimize sets played
    //      Minimize pulls from the discard pile
    //      Both
    //    Overt/Covert mix
    //    Perfect strategy (predictive)
    //    Maximize points   
    //    Cheater 
};


class Tong_Its_Game 
{
public:
    // Tong_Its_Game(shared_ptr<string> humanPlayerName);
    Tong_Its_Game(const shared_ptr<string>& humanPlayerName);
    // ~Tong_Its_Game();  // NOT USED
    Tong_Its_Player player1;
    Tong_Its_Player player2;
    Tong_Its_Player player3;
    void start_the_game(void);
    void TEST_the_deck(shared_ptr<vector<shared_ptr<PCard>>> deckToTest);  // DEBUGGING
    void receive_a_discard(shared_ptr<PCard> discardedCard);  // Tong_Its_Player discards a card
    shared_ptr<PCard> card_is_drawn(void);  // Tong_Its_Player draws a card from the drawPile
    shared_ptr<PCard> discard_is_taken(void);  // Tong_Its_Player takes the top discard from the discardPile
    // int currentDealer;  // Also the last winner
    int currentPlayer;
private:
    // Builds a vector of unique pointers to Card objects
    shared_ptr<vector<shared_ptr<PCard>>> build_a_deck(void);
    // Current draw pile of unseen cards
    shared_ptr<vector<shared_ptr<PCard>>> drawPile;  // NOTE: Draw from the back()
    // Current discard pile (only the top should be accessible)
    shared_ptr<vector<shared_ptr<PCard>>> discardPile;  // NOTE: Discard to the back()
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

    // Calculate points()
    // Winner?()
    };

int input_number(void);

#endif // __TONG_ITS__
