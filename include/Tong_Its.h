#ifndef __TONG_ITS__
#define __TONG_ITS__

#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace std;

typedef struct Playing_Card
{
    Playing_Card(string pcRank, string pcSuit);
    // Playing_Card(string pcRank, char16_t pcSuit);
    string rank;    // Value as a string
    string suit;  // U+2660–2667
    int value;      // Value as a number
} PCard, *PCard_ptr;


class Tong_Its_Player
{
public:
    Tong_Its_Player(string playerName);
    // ~Tong_Its_Player();
    void TEST_the_hand(void);
    string get_name(void);
    int count_chips(void);
    // int count_cards(void);
    void receive_a_card(shared_ptr<PCard> drawnCard);
private:
    string name;
    int numOfChips;
    shared_ptr<vector<shared_ptr<PCard>>> playersHand;
    // AI decision algorithm
    //    Random Plays
    //    Overt
    //    Covert
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
    // ~Tong_Its_Game();
    Tong_Its_Player player1;
    Tong_Its_Player player2;
    Tong_Its_Player player3;
    void TEST_the_deck(shared_ptr<vector<shared_ptr<PCard>>> deckToTest);  // DEBUGGING
    // int currentDealer;  // Also the last winner
private:
    // Builds a vector of unique pointers to Card objects
    shared_ptr<vector<shared_ptr<PCard>>> build_a_deck(void);
    // Current draw pile of unseen cards
    shared_ptr<vector<shared_ptr<PCard>>> drawPile;
    // Randomizes the order of a vector of card pointers
    void shuffle_a_deck(shared_ptr<vector<shared_ptr<PCard>>> deckOfCards);
    // Used to play, discard, deal, etc
    void move_one_card(int cardNumber, shared_ptr<vector<shared_ptr<PCard>>> source, \
                       shared_ptr<vector<shared_ptr<PCard>>> destination);

    void deal_player_hands(Tong_Its_Player currentDealer);
    // Discard == vector of unique pointers to Card objects
    // currentPlayer

    // Calculate points()
    // Winner?()
    // Next player()
};


#endif // __TONG_ITS__
