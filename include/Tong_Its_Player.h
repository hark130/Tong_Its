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
    int count_potential_melds(vector<shared_ptr<Tong_Its_Player>> players);
    int count_special_melds(vector<shared_ptr<Tong_Its_Player>> players);
    void receive_a_card(shared_ptr<PCard> drawnCard);
    int get_card_number(shared_ptr<PCard> findThisCard);
    shared_ptr<PCard> play_a_card(int cardNumber);
    shared_ptr<PCard> play_any_card(int cardNumber, shared_ptr<vector<shared_ptr<PCard>>> deckToPlayFrom);
    void print_players_hand(void);
    void print_playing_cards(bool printNums, shared_ptr<vector<shared_ptr<PCard>>> cardsToPrint);
    int show_all_melds(bool playOne, vector<shared_ptr<Tong_Its_Player>> players);
    void update_potential_melds(bool playOne, vector<shared_ptr<Tong_Its_Player>> players);
    void show_all_runs(bool playOne);
    void show_all_sets(bool playOne);
    void update_potential_sapaw(bool playOne, vector<shared_ptr<Tong_Its_Player>> players);
    void show_all_sapaw_runs(bool playOne, vector<shared_ptr<Tong_Its_Player>> players);
    void show_all_sapaw_sets(bool playOne, vector<shared_ptr<Tong_Its_Player>> players);
    void print_a_meld(vector<shared_ptr<PCard>> oneMeld, int meldNum);
    void print_exposed_melds(void);
    void sort_players_hand(void);
    bool expose_a_meld(int meldNum, vector<shared_ptr<Tong_Its_Player>> players, bool silent);
    bool expose_a_normal_meld(shared_ptr<vector<shared_ptr<PCard>>> pMeldsVector_ptr);
    bool expose_a_sapaw_meld(shared_ptr<vector<shared_ptr<PCard>>> pMeldsVector_ptr, vector<shared_ptr<Tong_Its_Player>> players);
    void call_tongits(void);
    void call_draw(void);
    bool called_tongits(void);
    bool called_draw(void);
    void challenge(void);
    bool challenged_a_draw(void);
    int hand_size(void);
    int current_card_points(vector<shared_ptr<Tong_Its_Player>> players);
    void got_burned(void);
    bool is_burned(void);
    void calc_final_score(vector<shared_ptr<Tong_Its_Player>> players);
    int get_final_score(vector<shared_ptr<Tong_Its_Player>> players);
    void reset(Tong_Its_Game* theGame_ptr);
    bool already_open(void);
    bool challenge_a_draw(Tong_Its_Player& drawPlayer, vector<shared_ptr<Tong_Its_Player>> players);
    bool set_meld_type(shared_ptr<vector<shared_ptr<PCard>>> meldVec_ptr);
    bool validate_meld(shared_ptr<vector<shared_ptr<PCard>>> meldVec_ptr);
    bool is_this_a_run(shared_ptr<vector<shared_ptr<PCard>>> meldVec_ptr);
    bool is_this_a_set(shared_ptr<vector<shared_ptr<PCard>>> meldVec_ptr);
    vector<shared_ptr<vector<shared_ptr<PCard>>>> get_exposed_melds(void);
    bool na_sapaw_ako(void);  // Has this player been 'layed off' on?  'Getter' for sapawAko
    void na_sapaw_ka(void);  // Call this when a player 'lays off'.  Setter' for sapawAko
    void wala_nang_sapaw(void);  // Call this to clear sapawAko
    bool card_can_meld(shared_ptr<PCard> oneCard, vector<shared_ptr<Tong_Its_Player>> players);
protected:
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
    bool sapawAko;  // Set to true if a player lays off (sapaw) on your exposed meld
    shared_ptr<vector<shared_ptr<PCard>>> playersHand;
    vector<shared_ptr<vector<shared_ptr<PCard>>>> playersMelds;
    vector<shared_ptr<vector<shared_ptr<PCard>>>> playersExposedMelds;
    void find_a_suit_run(string sortThisSuit);
    void print_a_row(int rowToPrint, bool printNums, shared_ptr<vector<shared_ptr<PCard>>> printTheseCards);
    void sort_cards(shared_ptr<vector<shared_ptr<PCard>>> cardsToSort, bool sortByTheSuits);
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
