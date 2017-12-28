#ifndef __TONG_ITS_PLAYER__
#define __TONG_ITS_PLAYER__

#include <memory>
#include <string>
#include "Tong_Its.h"
#include "Tong_Its_Playing_Card.h"
#include <vector>

using namespace std;

class Tong_Its_Game;  // Forward declared

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
    int count_aces(void);
    void win_chips(int wonChips);
    int lose_chips(int lostChips);
    int count_cards(void);
    int count_exposed_melds(void);
    int count_potential_melds(void);
    int count_special_melds(void);
    void receive_a_card(shared_ptr<PCard> drawnCard);
    int get_card_number(shared_ptr<PCard> findThisCard);
    shared_ptr<PCard> play_a_card(int cardNumber);
    shared_ptr<PCard> play_any_card(int cardNumber, shared_ptr<vector<shared_ptr<PCard>>> deckToPlayFrom);
    void print_players_hand(void);
    void print_playing_cards(bool printNums, shared_ptr<vector<shared_ptr<PCard>>> cardsToPrint);
    int show_all_melds(bool playOne);
    int show_all_runs(bool playOne, int startingNum);
    int show_all_sets(bool playOne, int startingNum);
    void print_a_meld(vector<shared_ptr<PCard>> oneMeld, int meldNum);
    void print_exposed_melds(void);
    void sort_players_hand(void);
    void update_potential_melds(bool playOne);
    bool expose_a_meld(int meldNum);
    void call_tongits(void);
    void call_draw(void);
    bool called_tongits(void);
    bool called_draw(void);
    void challenge(void);
    bool challenged_a_draw(void);
    int hand_size(void);
    int current_card_points(void);
    void got_burned(void);
    bool is_burned(void);
    void calc_final_score(void);
    int get_final_score(void);
    void reset(Tong_Its_Game* theGame_ptr);
    bool already_open(void);
    bool challenge_a_draw(Tong_Its_Player& drawPlayer);
private:
    bool sortBySuit;
    string name;
    int numOfChips;
    int numOfCards;
    bool calledTongits;
    bool calledDraw;
    bool challengedDraw;
    bool burned;
    int finalScore;
    bool open;
    shared_ptr<vector<shared_ptr<PCard>>> playersHand;
    vector<shared_ptr<vector<shared_ptr<PCard>>>> playersMelds;
    vector<shared_ptr<vector<shared_ptr<PCard>>>> playersExposedMelds;
    int find_a_suit_run(string sortThisSuit);
    void print_a_row(int rowToPrint, bool printNums, shared_ptr<vector<shared_ptr<PCard>>> printTheseCards);
    void sort_cards(shared_ptr<vector<shared_ptr<PCard>>> cardsToSort, bool sortBySuit);
    int random_num(int start, int stop);
    bool card_in_a_meld(shared_ptr<PCard> findThisCard);
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

#endif // __TONG_ITS_PLAYER__
