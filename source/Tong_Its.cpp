// #include "../include/Tong_Its.h"
// #include "include/Tong_Its.h"
#include "Clear_Screen.h"       // clear_screen
#include "Tong_Its.h"
#include <algorithm>            // random_shuffle && sort 
#include <climits>              // INT_MAX
// #include <clocale>           // Set locale
#include <cstdlib>              // srand
#include <ctime>                // time 
// #include <fcntl.h>           // Set mode 16 bit
#include <iostream>
// #include <io.h>              // Set mode 16 bit
#include <locale>               // toupper, locale
#include <memory>
#include <regex>
#include <string>
#include <ctime>                // time
#include <vector>

// #define CLEAR_SCREEN 30      // Number of newlines to print to clear the screen 
#define USER_EXIT 999           // User interface indication to quit the program
#define NUM_CARDS_PER_ROW 52    // Indicates how many cards print_a_row() will print per row

// #define CONTRAST 10  // 0 for normal, 1 for contrast

// #if CONTRAST == 1
// #define SPADE ((char16_t) '\u2664')    // ♤
// #define HEART ((char16_t) '\u2665')    // ♥
// #define DIAMOND ((char16_t) '\u2666')  // ♦
// #define CLUB ((char16_t) '\u2667')     // ♧
// #elif CONTRAST == 69
// #define SPADE L'♤'
// #define HEART L'♥'
// #define DIAMOND L'♦'
// #define CLUB L'♧'
// #elif CONTRAST == 10
#if defined(_WIN32) || defined(__MSDOS__)
   #define SPADE   "\x06"
   #define CLUB    "\x05"
   #define HEART   "\x03"
   #define DIAMOND "\x04"
#else
   #define SPADE   "\xE2\x99\xA0"
   #define CLUB    "\xE2\x99\xA3"
   #define HEART   "\xE2\x99\xA5"
   #define DIAMOND "\xE2\x99\xA6"
#endif
// #else
// #define SPADE ((char16_t) '\u2660')    // ♠
// #define HEART ((char16_t) '\u2661')    // ♡
// #define DIAMOND ((char16_t) '\u2662')  // ♢
// #define CLUB ((char16_t) '\u2663')     // ♣
// #endif // Conditional suit values



using namespace std;
const auto spadeString = string(SPADE);
const auto clubString = string(CLUB);
const auto heartString = string(HEART);
const auto diamondString = string(DIAMOND);

/**********************/
/* PLAYING CARD BEGIN */
/**********************/
Playing_Card::Playing_Card(string pcRank, string pcSuit)
// Playing_Card::Playing_Card(string pcRank, char16_t pcSuit)
{
    locale loc;

    if (pcSuit == spadeString || pcSuit == heartString || pcSuit == diamondString || pcSuit == clubString || pcSuit == " ")
    // if (pcSuit == SPADE || pcSuit == HEART || pcSuit == DIAMOND || pcSuit == CLUB)
    {
        // const wchar_t heart[] = L"\u2665";  // DEBUGGING
        // setlocale(LC_CTYPE,"");  // TESTING
        // setlocale(LC_ALL, "");
        // wcout << heart << L'\n';
        suit = pcSuit;
        // wcout << suit << endl;  // DEBUGGING
        // cout << suit << endl;  // DEBUGGING
        // wcout << L"こんにちは世界\n";
        // wcout << L"♣\n";
        // wcout << L"\u2660\n";
        // cout << "\5";
        // cout << SPADE;
        // wcout << SPADE;
        // cout << "\xE2\x99\xA6";
        // wcout << L"\xE2\x99\xA6";
        if (pcSuit == spadeString)
        {
            suitValue = 1;
        }
        else if (pcSuit == heartString)
        {
            suitValue = 2;
        }
        else if (pcSuit == diamondString)
        {
            suitValue = 3;
        }
        else if (pcSuit == clubString)
        {
            suitValue = 4;
        }
        else // " "
        {
            suitValue = 0;
        }
    }
    else
    {
        // Raise exception
        throw invalid_argument("Playing_Card ctor - Invalid Playing Card suit");
    }

    if (pcRank == "A")
    {
        rankValue = 1;
        pointValue = rankValue;
    }
    else if (pcRank == "2")
    {
        rankValue = 2;
        pointValue = rankValue;
    }
    else if (pcRank == "3")
    {
        rankValue = 3;
        pointValue = rankValue;
    }
    else if (pcRank == "4")
    {
        rankValue = 4;
        pointValue = rankValue;
    }
    else if (pcRank == "5")
    {
        rankValue = 5;
        pointValue = rankValue;
    }
    else if (pcRank == "6")
    {
        rankValue = 6;
        pointValue = rankValue;
    }
    else if (pcRank == "7")
    {
        rankValue = 7;
        pointValue = rankValue;
    }
    else if (pcRank == "8")
    {
        rankValue = 8;
        pointValue = rankValue;
    }
    else if (pcRank == "9")
    {
        rankValue = 9;
        pointValue = rankValue;
    }
    else if (pcRank == "10")
    {
        rankValue = 10;
        pointValue = rankValue;
    }
    else if (pcRank == "J" || pcRank == "j")
    {
        rankValue = 11;
        pointValue = 10;
        pcRank = toupper(pcRank[0], loc);
    }
    else if (pcRank == "Q" || pcRank == "q")
    {
        rankValue = 12;
        pointValue = 10;
        pcRank = toupper(pcRank[0], loc);
    }
    else if (pcRank == "K" || pcRank == "k")
    {
        rankValue = 13;
        pointValue = 10;
        pcRank = toupper(pcRank[0], loc);
    }
    else if (pcRank == " ")
    {
        rankValue = 0;
        pointValue = 0;
    }
    else
    {
        // Raise exception
        throw invalid_argument("Playing_Card ctor - Invalid Playing Card rank");
    }

    numMelds = 0;
    rank = pcRank;
}

/********************/
/* PLAYING CARD END */
/********************/


/*************************/
/* TONG ITS PLAYER BEGIN */
/*************************/
Tong_Its_Player::Tong_Its_Player(string playerName) : 
name(playerName), numOfChips(100), playersHand(make_shared<vector<shared_ptr<PCard>>>())
{
    // cout << "Player Name: " << playerName << endl;  // DEBUGGING
    // name = playerName;
    // numOfChips = 100;
    // playersHand = make_shared<vector<shared_ptr<PCard>>>();
    numOfCards = 0;
    sortBySuit = true;
    calledTongits = false;
    calledDraw = false;
    burned = false;
    finalScore = 0;
}


void Tong_Its_Player::TEST_the_hand(void)
{
    cout << "Testing the deck" << endl;
    if (playersHand)
    {
        cout << "Deck size: " << (*playersHand).size() << endl;
    }
    else
    {
        cout << "Deck is empty!" << endl;
    }        

    // setlocale(LC_ALL, "");
    for (int i = 0; i < (*playersHand).size(); ++i)
    {
        cout << "\nCard # " << i + 1 << endl;
        cout << "Rank: " << (*playersHand)[i]->rank << endl;
        cout << "Suit: " << (*playersHand)[i]->suit << endl;
        cout << "Rank Value: " << (*playersHand)[i]->rankValue << endl;
    }

    return;
}


void Tong_Its_Player::toggle_sort(void)
{
    if (sortBySuit == true)
    {
        sortBySuit = false;
    }
    else
    {
        sortBySuit = true;
    }

    sort_players_hand();

    return;
}


bool Tong_Its_Player::sorting_by_suit(void)
{
    return sortBySuit;
}


string Tong_Its_Player::get_name(void)
{
    return name;
}


int Tong_Its_Player::count_chips(void)
{
    return numOfChips;
}


int Tong_Its_Player::count_cards(void)
{
    return (*playersHand).size();
}


int Tong_Its_Player::count_exposed_melds(void)
{
    return playersExposedMelds.size();
}


int Tong_Its_Player::count_potential_melds(void)
{
    // Refresh player's melds
    update_potential_melds(true);

    // Count the melds
    return playersMelds.size();
}


/*
    Purpose - Glue together all of the 'heavy lifting' methods to expose one meld
    Input - Meld number to expose (see: Index number into the potentialMeld vector)
    Output - True for sucess, False for failure
 */
bool Tong_Its_Player::expose_a_meld(int meldNum)
{
    // LOCAL VARIABLES
    bool retVal = false;
    shared_ptr<vector<shared_ptr<PCard>>> pMeldsVector_ptr = nullptr;  // playersMelds
    shared_ptr<vector<shared_ptr<PCard>>> pExpMeldsVector_ptr = nullptr;  // playersExposedMelds
    shared_ptr<PCard> tmpPCard_ptr = nullptr;
    int tempCardNum = 0;
    vector<shared_ptr<PCard>> tmpMeldSet;

    // INPUT VALIDATION
    if (meldNum < 1)
    {
        throw invalid_argument("Tong_Its_Player::expose_a_meld() received invalid meld number");
    }
    else if (meldNum > playersMelds.size())
    {
        throw invalid_argument("Tong_Its_Player::expose_a_meld() received non-existent meld number");
    }

    // EXPOSE THAT MELD
    // 1. Find the meld
    cout << "You chose meld number " << meldNum << endl;  // DEBUGGING
    cout << "There are " << playersMelds.size() << " melds to choose from" << endl;  // DEBUGGING
    for (auto meld : playersMelds)
    {
        print_a_meld((*meld), 0);
    }
    pMeldsVector_ptr = playersMelds.at(meldNum - 1);
    if (pMeldsVector_ptr == nullptr)
    {
        throw runtime_error("Tong_Its_Player::expose_a_meld() received a nullptr");
    }

    for (auto meldCard : (*pMeldsVector_ptr))
    {
        // 2. Remove the meld from player's hand
        // 2.1. Get the card number
        tempCardNum = get_card_number(meldCard);
        // 2.2. Remove that card
        try
        {
            tmpPCard_ptr = play_a_card(tempCardNum);
        }
        catch (const std::invalid_argument& err)
        {
            cerr << "Invalid argument: " << err.what() << endl;
            // continue;
        }
        // 2.3. Verify return value
        if (tmpPCard_ptr == nullptr)
        {
            throw runtime_error("Tong_Its_Player::expose_a_meld() received a nullptr");    
        }
        else if (meldCard != tmpPCard_ptr)
        {
            throw runtime_error("Tong_Its_Player::expose_a_meld() received the wrong card");
        }

        // 2.4. Add the card to the tmpMeldSet vector
        tmpMeldSet.push_back(tmpPCard_ptr);
    }

    // 3. Add the meld to the player's exposed melds
    // 3.1. Validate tmpMeldSet
    if (tmpMeldSet.size() < 3)
    {
        throw runtime_error("Tong_Its_Player::expose_a_meld() tmpMeldSet is improperly sized");
    }

    // 3.2. Make a new shared_pointer to a vector of PCard shared pointers
    pExpMeldsVector_ptr = make_shared<vector<shared_ptr<PCard>>>();
    if (pExpMeldsVector_ptr == nullptr)
    {
        throw runtime_error("Tong_Its_Player::expose_a_meld() made a nullptr?!");    
    }

    // 3.2. Add a new shared_point to a vector of PCard shared pointers to playersExposedMelds
    playersExposedMelds.push_back(pExpMeldsVector_ptr);

    // 3.3. Copy the PCards to the player's exposed melds
    for (auto meldCard : tmpMeldSet)
    {
        meldCard->numMelds = 1;  // Set meld number
        (*pExpMeldsVector_ptr).push_back(meldCard);
    }

    // 3.4. Validate the copy
    if (tmpMeldSet.size() == (*pExpMeldsVector_ptr).size())
    {
        retVal = true;
    }

    // 4. Recalculate the player's melds
    update_potential_melds(false);

    // 5. Did the player get Tongits?
    if (hand_size() == 0)
    {
        call_tongits();
    }

    // DONE
    return retVal;
}


void Tong_Its_Player::call_tongits(void)
{
    calledTongits = true;
    return;
}


void Tong_Its_Player::call_draw(void)
{
    calledDraw = true;
    return;
}


bool Tong_Its_Player::called_tongits(void)
{
    return calledTongits;
}


bool Tong_Its_Player::called_draw(void)
{
    return calledDraw;
}


int Tong_Its_Player::hand_size(void)
{
    numOfCards = (*playersHand).size();
    return numOfCards;
}


int Tong_Its_Player::current_card_points(void)
{
    // LOCAL VARIABLES
    int retVal = 0;

    // UPDATE HAND
    update_potential_melds(false);

    // CALCULATE POINTS
    if (!called_tongits())
    {
        for (auto card : (*playersHand))
        {
            if (card->numMelds == 0)
            {
                retVal += card->pointValue;
            }
            else if (card->numMelds > 1)
            {
                cout << "Tong_Its_Player::current_card_points() - Find some way to account for overlapping potential melds!" << endl;
            }
        }
    }

    // DONE
    return retVal;
}


/*
    Purpose - Change the player's state to 'burned' (see: They lost)
 */
void Tong_Its_Player::got_burned(void)
{
    burned = true;
    return;
}


bool Tong_Its_Player::is_burned(void)
{
    return burned;
}


void Tong_Its_Player::calc_final_score(void)
{
    finalScore = current_card_points();
}


int Tong_Its_Player::get_final_score(void)
{
    if (finalScore == 0)
    {
        calc_final_score();
    }
    return finalScore;
}


void Tong_Its_Player::receive_a_card(shared_ptr<PCard> drawnCard)
{
    if (drawnCard)
    {
        (*playersHand).push_back(drawnCard);
        sort_players_hand();
        ++numOfCards;
        // cout << "Card: Rank " << drawnCard->rank << " Suit " << drawnCard->suit << endl;  // DEBUGGING
        // cout << "Number of cards in hand: " << numOfCards << endl;  // DEBUGGING
    }
    else
    {
        throw invalid_argument("NULL card pointer");
        // throw exception("NULL card pointer");
    }

    return;
}


/*
    Purpose - Translate a card into a card number
    Input - shared pointer to the card in question
    Output - Card number if found in player's hand, 0 otherwise
 */
int Tong_Its_Player::get_card_number(shared_ptr<PCard> findThisCard)
{
    int retVal = 0;
    int currCardNum = 1;

    for (auto playersCard : (*playersHand))
    {
        if (playersCard == findThisCard)
        {
            retVal = currCardNum;
            break;
        }
        else
        {
            currCardNum++;
        }
    }

    return retVal;
}


shared_ptr<PCard> Tong_Its_Player::play_a_card(int cardNumber)
{
    // LOCAL VARIABLES
    shared_ptr<PCard> retVal = nullptr;
    // cout << "Number of cards in hand: " << numOfCards << endl;  // DEBUGGING
    
    // INPUT VALIDATION
    if (cardNumber < 1)
    {
        throw invalid_argument("Tong_Its_Player::play_a_card() received invalid card number");
    }
    else if (cardNumber > hand_size())
    {
        throw invalid_argument("Tong_Its_Player::play_a_card() received non-existent card number");
    }
    else
    {
        // auto cardPos = (*playersHand).begin() + cardNumber - 1;
        // retVal = (*playersHand).at(cardNumber - 1);
        // // (*playersHand).erase(playersHand.begin() + cardNumber - 1);
        // (*playersHand).erase(cardPos);
        try
        {
            retVal = play_any_card(cardNumber, playersHand);
        }
        catch (...)
        {
            cout << "Tong_Its_Player::play_a_card() received an exception from play_any_card()" << endl;
            throw;
        }

        if (retVal == nullptr)
        {
            throw runtime_error("Tong_Its_Player::play_any_card() returned a nullptr");
        }
        else
        {
            --numOfCards;    
        }        
    }

    return retVal;
}


shared_ptr<PCard> Tong_Its_Player::play_any_card(int cardNumber, shared_ptr<vector<shared_ptr<PCard>>> deckToPlayFrom)
{
    // LOCAL VARIABLES
    auto retVal = make_shared<PCard>(" ", " ");  // Return value for this function
    int deckSize = 0;  // Number of cards in the deck to play from

    // INPUT VALIDATION
    if (deckToPlayFrom == nullptr)
    {
        throw invalid_argument("Tong_Its_Player::play_any_card() received a nullptr");
    }
    else
    {
        deckSize = (*deckToPlayFrom).size();
    }

    if (cardNumber < 1)
    {
        throw invalid_argument("Tong_Its_Player::play_any_card() received invalid card number");
    }
    else if (cardNumber > deckSize)
    {
        throw invalid_argument("Tong_Its_Player::play_any_card() received non-existent card number");        
    }
    else
    {
        auto cardPos = (*deckToPlayFrom).begin() + cardNumber - 1;
        retVal = (*deckToPlayFrom).at(cardNumber - 1);
        (*deckToPlayFrom).erase(cardPos);
    }

    // DONE
    return retVal;
}

void Tong_Its_Player::print_players_hand(void)
{
    int totalCards = (*playersHand).size();
    int currentCardNum = 0;
    // Number of full rows
    int numOfFullRows = (totalCards - totalCards % NUM_CARDS_PER_ROW) / NUM_CARDS_PER_ROW;
    // cout << "Number of full rows: " << numOfFullRows << endl;  // DEBUGGING
    int numOfPrintedRows = numOfFullRows;
    int numOfLeftovers = totalCards % NUM_CARDS_PER_ROW;
    // cout << "Number of leftovers: " << numOfLeftovers << endl;  // DEBUGGING
    // Spare row
    if (numOfLeftovers != 0)
    {
        ++numOfPrintedRows;
    }

    // cout << "Number of rows: " << numOfPrintedRows << endl;  // DEBUGGING

    for (int i = 1; i <= numOfPrintedRows; ++i)
    {
        print_a_row(i);
    }
    // cout << "print_players_hand() not yet implemented!" << endl;

    return;
}


/*
    Purpose - Update the vector of potential melds for a player
    Input - None
    Output - Number of melds in playersMelds
 */
void Tong_Its_Player::update_potential_melds(bool playOne)
{
    // Clear existing melds
    playersMelds.clear();

    if (sortBySuit == true)
    {
        show_all_runs(playOne, 1);
        // cout << "1. Current meld number: " << currMeldNum << endl;  // DEBUGGING

        show_all_sets(playOne, 1);
        // cout << "1. Current meld number: " << currMeldNum << endl;  // DEBUGGING
    }
    else
    {
        show_all_sets(playOne, 1);
        // cout << "2. Current meld number: " << currMeldNum << endl;  // DEBUGGING
        show_all_runs(playOne, 1);   
        // cout << "2. Current meld number: " << currMeldNum << endl;  // DEBUGGING
    }

    return;   
}


/*
    Purpose - Print all the melds in the order based on the TIP::sortBySuit bool
    Input - numbers melds if true
    Output - Number of melds that were found
    Notes - This function will ensure the original sorting state is maintained
 */
int Tong_Its_Player::show_all_melds(bool playOne)
{
    int currMeldNum = 1;
    bool originalSortingState = sortBySuit;
    playersMelds.clear();

    update_potential_melds(playOne);
    // if (sortBySuit == true)
    // {
    //     show_all_runs(playOne, currMeldNum);
    //     // cout << "1. Current meld number: " << currMeldNum << endl;  // DEBUGGING

    //     show_all_sets(playOne, currMeldNum + 1);
    //     // cout << "1. Current meld number: " << currMeldNum << endl;  // DEBUGGING
    // }
    // else
    // {
    //     show_all_sets(playOne, currMeldNum);
    //     // cout << "2. Current meld number: " << currMeldNum << endl;  // DEBUGGING
    //     show_all_runs(playOne, currMeldNum + 1);   
    //     // cout << "2. Current meld number: " << currMeldNum << endl;  // DEBUGGING
    // }

    // cout << "Resetting sort" << endl;  // DEBUGGING
    if (originalSortingState != sortBySuit)
    {
        toggle_sort();
    }

    if (playersMelds.size() == 0)
    {
        cout << "\nYou have no melds.\n" << endl;
    }
    else
    {
        cout << "\nYour Melds:" << endl;  // Clear some space
    }
    for (auto meldToPrint : playersMelds)
    {
        // cout << "Printing a new meld" << endl;  // DEBUGGING
        // cout << "Found this meld at position " << SUPER FANCY ITERATOR COUNTER GOES HERE << endl;  // DEBUGGING
        print_a_meld(*meldToPrint, currMeldNum++);
    }

    return currMeldNum;
}


/*
    Purpose - Print all the runs found in the player's hand
    Input
        playOne - numbers melds if true
        startingNum - If playOne is true, this is the number of the last meld
    Output - Number of the last meld found, startingNum if no runs were found
 */
int Tong_Its_Player::show_all_runs(bool playOne, int startingNum)
{
    int retVal = 0;
    // vector<string> cardRanks = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
    vector<string> cardSuits = {spadeString, clubString, heartString, diamondString};
    // auto allSpades = make_shared<vector<shared_ptr<PCard>>>();
    // auto allClubs = make_shared<vector<shared_ptr<PCard>>>();
    // auto allHearts = make_shared<vector<shared_ptr<PCard>>>();
    // auto allDiamonds = make_shared<vector<shared_ptr<PCard>>>();
    // auto currentSet = make_shared<vector<shared_ptr<PCard>>>();
    // bool thisIsOne = false;

    // INPUT VALIDATION
    if (startingNum > 0)
    {
        retVal = startingNum;
    }
    else
    {
        throw invalid_argument("show_all_runs() - Invalid starting number");
    }

    // CHECK SORTING
    if (sortBySuit == false)
    {
        toggle_sort();
    }

    // FIND RUNS
    for (auto suitToSort : cardSuits)
    {
        find_a_suit_run(suitToSort);
    }

    // // COPY AND SEPARATE
    // for (shared_ptr<PCard> cardToEvaluate : (*playersHand))
    // {
    //     if (cardToEvaluate->suit == spadeString)
    //     {
    //         (*allSpades).push_back(cardToEvaluate);
    //     }
    //     else if (cardToEvaluate->suit == clubString)
    //     {
    //         (*allClubs).push_back(cardToEvaluate);
    //     }
    //     else if (cardToEvaluate->suit == heartString)
    //     {
    //         (*allHearts).push_back(cardToEvaluate);
    //     }
    //     else if (cardToEvaluate->suit == diamondString)
    //     {
    //         (*allDiamonds).push_back(cardToEvaluate);
    //     }
    //     else
    //     {
    //         // cout << "Invalid rank: " << cardToEvaluate->rank << endl;  // DEBUGGING
    //         throw invalid_argument("Tong_Its_Player::show_all_runs() - Invalid rank on a playing card");
    //     }
    // }
    // // cout << "Done separating" << endl;  // DEBUGGING

    // // EVALUATE SEPARATE VECTORS FOR RUNS
    // // Spades
    // while ((*allSpades).size() >= 3)
    // {
    //     int lastCard = (*allSpades).size() - 1;
    //     if ((*allSpades).at(lastCard)->rankValue == (*allSpades).at(lastCard - 1)->rankValue + 1 && \
    //         (*allSpades).at(lastCard)->rankValue == (*allSpades).at(lastCard - 2)->rankValue + 2)
    //     {
    //         (*currentSet).push_back((*allSpades).at(lastCard - 2));
    //         (*currentSet).push_back((*allSpades).at(lastCard - 1));
    //         (*currentSet).push_back((*allSpades).at(lastCard));

    //         while((*allSpades).size() > 0)
    //         {
    //             (*allSpades).pop_back();
    //         }
    //     }
    //     else
    //     {
    //         (*allSpades).pop_back();
    //     }

    //     if ((*currentSet).size() >= 3)
    //     {
    //         print_a_meld((*currentSet));
    //         ++retVal;

    //         while ((*currentSet).size() > 0)
    //         {
    //             (*currentSet).pop_back();
    //         }
    //     }
    // }
    // // cout << "Done evaluating spades" << endl;  // DEBUGGING

    // // Clubs
    // while ((*allClubs).size() >= 3)
    // {
    //     int lastCard = (*allClubs).size() - 1;
    //     if ((*allClubs).at(lastCard)->rankValue == (*allClubs).at(lastCard - 1)->rankValue + 1 && \
    //         (*allClubs).at(lastCard)->rankValue == (*allClubs).at(lastCard - 2)->rankValue + 2)
    //     {
    //         (*currentSet).push_back((*allClubs).at(lastCard - 2));
    //         (*currentSet).push_back((*allClubs).at(lastCard - 1));
    //         (*currentSet).push_back((*allClubs).at(lastCard));

    //         while((*allClubs).size() > 0)
    //         {
    //             (*allClubs).pop_back();
    //         }
    //     }
    //     else
    //     {
    //         (*allClubs).pop_back();
    //     }

    //     if ((*currentSet).size() >= 3)
    //     {
    //         print_a_meld((*currentSet));
    //         ++retVal;

    //         while ((*currentSet).size() > 0)
    //         {
    //             (*currentSet).pop_back();
    //         }
    //     }
    // }
    // // cout << "Done evaluating clubs" << endl;  // DEBUGGING

    // // Hearts
    // while ((*allHearts).size() >= 3)
    // {
    //     int lastCard = (*allHearts).size() - 1;
    //     if ((*allHearts).at(lastCard)->rankValue == (*allHearts).at(lastCard - 1)->rankValue + 1 && \
    //         (*allHearts).at(lastCard)->rankValue == (*allHearts).at(lastCard - 2)->rankValue + 2)
    //     {
    //         (*currentSet).push_back((*allHearts).at(lastCard - 2));
    //         (*currentSet).push_back((*allHearts).at(lastCard - 1));
    //         (*currentSet).push_back((*allHearts).at(lastCard));

    //         while((*allHearts).size() > 0)
    //         {
    //             (*allHearts).pop_back();
    //         }
    //     }
    //     else
    //     {
    //         (*allHearts).pop_back();
    //     }

    //     if ((*currentSet).size() >= 3)
    //     {
    //         print_a_meld((*currentSet));
    //         ++retVal;

    //         while ((*currentSet).size() > 0)
    //         {
    //             (*currentSet).pop_back();
    //         }
    //     }
    // }
    // // cout << "Done evaluating hearts" << endl;  // DEBUGGING

    // // Diamonds
    // while ((*allDiamonds).size() >= 3)
    // {
    //     int lastCard = (*allDiamonds).size() - 1;
    //     if ((*allDiamonds).at(lastCard)->rankValue == (*allDiamonds).at(lastCard - 1)->rankValue + 1 && \
    //         (*allDiamonds).at(lastCard)->rankValue == (*allDiamonds).at(lastCard - 2)->rankValue + 2)
    //     {
    //         (*currentSet).push_back((*allDiamonds).at(lastCard - 2));
    //         (*currentSet).push_back((*allDiamonds).at(lastCard - 1));
    //         (*currentSet).push_back((*allDiamonds).at(lastCard));

    //         while((*allDiamonds).size() > 0)
    //         {
    //             (*allDiamonds).pop_back();
    //         }
    //     }
    //     else
    //     {
    //         (*allDiamonds).pop_back();
    //     }

    //     if ((*currentSet).size() >= 3)
    //     {
    //         print_a_meld((*currentSet));
    //         ++retVal;

    //         while ((*currentSet).size() > 0)
    //         {
    //             (*currentSet).pop_back();
    //         }
    //     }
    // }
    // // cout << "Done evaluating diamonds" << endl;  // DEBUGGING

    return retVal;
}


/*
    Purpose - Print all the sets found in the player's hand
    Input
        playOne - numbers melds if true
        startingNum - If playOne is true, this is the number of the last meld
    Output - Number of the last meld found, startingNum if no sets were found
 */
int Tong_Its_Player::show_all_sets(bool playOne, int startingNum)
{
    auto tempMeld = make_shared<vector<shared_ptr<PCard>>>();
    vector<string> cardRanks = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
    int tempCount = 0;  // Count for each rank
    int retVal = 0;

    // INPUT VALIDATION
    if (startingNum > 0)
    {
        retVal = startingNum;
    }
    else
    {
        throw invalid_argument("show_all_sets() - Invalid starting number");
    }

    // CHECK SORTING
    if (sortBySuit == true)
    {
        toggle_sort();
    }

    // COUNT RANKS
    for (auto currRank : cardRanks)
    {
        tempCount = 0;  // Reset temp variable

        for (auto currCard : (*playersHand))
        {
            if (currCard->rank == currRank)
            {
                (*tempMeld).push_back(currCard);                
                ++tempCount;
            }
        }

        // Three, or more, of a kind
        if (tempCount >= 3)
        {
            // print_a_meld(*currentSet);
            // playersMelds.push_back(tempMeld);

            playersMelds.push_back(make_shared<vector<shared_ptr<PCard>>>());
            for (shared_ptr<PCard> meldCard : (*tempMeld))
            {
                playersMelds.at(playersMelds.size() - 1)->push_back(meldCard);
            }
            (*tempMeld).clear();
        }

        // Clear the temp vector
        // cout << "Current Set size for " << currRank << " is: " << (*currentSet).size() << endl;  // DEBUGGING
        while((*tempMeld).size() > 0)
        {
            (*tempMeld).pop_back();
        }
        // cout << "Done with rank: " << currRank << endl;  // DEBUGGING
    }

    // cout << "Returning" << endl;  // DEBUGGING
    return retVal;
}


/*
    Purpose - Print a meld
    Input
        oneMeld - A vector of shared_pointers to PCard objects
        meldNum - Meld number, if any, to print with the meld
    Output - None
    Note - If meldNum is less than 1, no number will be printed with the meld
 */
void Tong_Its_Player::print_a_meld(vector<shared_ptr<PCard>> oneMeld, int meldNum)
{
    string leftBorder = "";
    string leftNumber = "";
    if (meldNum > 0)
    {
        leftBorder = "  ";
        // leftNumber = string(meldNum) + " ";
        switch (meldNum)
        {
            case(0):
                leftNumber = "0 ";
                break;
            case(1):
                leftNumber = "1 ";
                break;
            case(2):
                leftNumber = "2 ";
                break;
            case(3):
                leftNumber = "3 ";
                break;
            case(4):
                leftNumber = "4 ";
                break;
            case(5):
                leftNumber = "5 ";
                break;
            case(6):
                leftNumber = "6 ";
                break;
            case(7):
                leftNumber = "7 ";
                break;
            case(8):
                leftNumber = "8 ";
                break;
            case(9):
                leftNumber = "9 ";
                break;
            default:
                leftNumber = "# ";
                break;
        };
    }

    // Print the card pointers
    // Card Row 1
    cout << leftNumber;
    for (shared_ptr<PCard> cardToPrint : oneMeld)
    {
        cout << BORDER_UPPER_LEFT << BORDER_HORIZONTAL << BORDER_HORIZONTAL << BORDER_HORIZONTAL << BORDER_HORIZONTAL << BORDER_UPPER_RIGHT;        
    }
    cout << endl;
    // Row 2
    cout << leftBorder;
    for (shared_ptr<PCard> cardToPrint : oneMeld)
    {
        if (cardToPrint->rank == "10")
        {
            cout << BORDER_VERTICAL << cardToPrint->rank << BORDER_SPACE << BORDER_SPACE << BORDER_VERTICAL;
        }
        else
        {
            cout << BORDER_VERTICAL << cardToPrint->rank << BORDER_SPACE << BORDER_SPACE << BORDER_SPACE << BORDER_VERTICAL;
        }        
    }
    cout << endl;
    // Row 3
    cout << leftBorder;
    for (shared_ptr<PCard> cardToPrint : oneMeld)
    {
        cout << BORDER_VERTICAL << BORDER_SPACE << cardToPrint->suit << BORDER_SPACE << BORDER_SPACE << BORDER_VERTICAL;
    }
    cout << endl;
    // Row 4
    cout << leftBorder;
    for (shared_ptr<PCard> cardToPrint : oneMeld)
    {
        if (cardToPrint->rank == "10")
        {
            cout << BORDER_VERTICAL << BORDER_SPACE << BORDER_SPACE << cardToPrint->rank << BORDER_VERTICAL;
        }
        else
        {
            cout << BORDER_VERTICAL << BORDER_SPACE << BORDER_SPACE << BORDER_SPACE << cardToPrint->rank << BORDER_VERTICAL;
        }        
    }
    cout << endl;
    // Row 5
    cout << leftBorder;
    for (shared_ptr<PCard> cardToPrint : oneMeld)
    {
        cout << BORDER_LOWER_LEFT << BORDER_HORIZONTAL << BORDER_HORIZONTAL << BORDER_HORIZONTAL << BORDER_HORIZONTAL << BORDER_LOWER_RIGHT;        
    }    
    cout << endl;

    return;
}


/*
    Purpose - Print a player's exposed melds
    Input - None
    Output - None
 */
void Tong_Its_Player::print_exposed_melds(void)
{
    for (auto oneExposedMeld : playersExposedMelds)
    {
        if (oneExposedMeld != nullptr)
        {
            print_a_meld((*oneExposedMeld), 0);
        }
        else
        {
            throw runtime_error("How did a nullptr get into this player's exposed melds?!");
        }
    }

    return;
}


void Tong_Its_Player::sort_players_hand(void)
{
    // if (sortBySuit == true)
    // {
    //     sort((*playersHand).begin(), (*playersHand).end(), [ ]( const auto& left, const auto& right )
    //         {
    //             if (left->suitValue < right->suitValue)
    //             {
    //                 return true;
    //             }
    //             else if (left->suitValue == right->suitValue)
    //             {
    //                 return left->rankValue < right->rankValue;
    //             }
    //         });
    // }
    // else
    // {
    //     sort((*playersHand).begin(), (*playersHand).end(), [ ]( const auto& left, const auto& right )
    //         {
    //             if (left->rankValue < right->rankValue)
    //             {
    //                 return true;
    //             }
    //             else if (left->rankValue == right->rankValue)
    //             {
    //                 return left->suitValue < right->suitValue;
    //             }
    //         });
    // }

    sort_cards(playersHand, sortBySuit);

    return;
}


/*
    Purpose - Find all runs in the player's hand for a given suit
    Input
        sortThisSuit - string representation of the suit to match for
    Output - Number of the last meld found, startingNum if no sets were found
 */
int Tong_Its_Player::find_a_suit_run(string sortThisSuit)
{
    // INPUT VALIDATION
    // IMPLEMENT THIS LATER!  CHECK AGAINST VALID SUITS?

    // LOCAL VARIABLES
    auto suitMatches = make_shared<vector<shared_ptr<PCard>>>();
    auto tempMeld = make_shared<vector<shared_ptr<PCard>>>();

    // COPY AND SEPARATE
    for (shared_ptr<PCard> cardToEvaluate : (*playersHand))
    {
        if (cardToEvaluate->suit == sortThisSuit)
        {
            (*suitMatches).push_back(cardToEvaluate);
        }
    }    

    // EVALUATE LOCAL VECTOR FOR RUNS
    while ((*suitMatches).size() >= 3)
    {
        int lastCard = (*suitMatches).size() - 1;
        if ((*suitMatches).at(lastCard)->rankValue == (*suitMatches).at(lastCard - 1)->rankValue + 1 && \
            (*suitMatches).at(lastCard)->rankValue == (*suitMatches).at(lastCard - 2)->rankValue + 2)
        {
            // Found 3
            // cout << "Found a suit run of three" << endl;  // DEBUGGING
            (*tempMeld).push_back((*suitMatches).at(lastCard - 2));
            (*tempMeld).push_back((*suitMatches).at(lastCard - 1));
            (*tempMeld).push_back((*suitMatches).at(lastCard));

            // Clear 3
            (*suitMatches).pop_back();
            (*suitMatches).pop_back();
            (*suitMatches).pop_back();

            // Check the next one
            while((*suitMatches).size() > 0)
            {
                lastCard = (*suitMatches).size() - 1;
                if ((*suitMatches).at(lastCard)->rankValue + 1 == (*tempMeld).at(0)->rankValue)
                {
                    // cout << "Found another card to add to the suit run" << endl;  // DEBUGGING
                    (*tempMeld).push_back((*suitMatches).at(lastCard));
                    (*suitMatches).pop_back();
                    sort_cards(tempMeld, true);
                }
                else
                {
                    // playersMelds.push_back(tempMeld);
                    // (*tempMeld).clear();
                    // break;
                    // cout << "Found a suit run" << endl;  // DEBUGGING
                    playersMelds.push_back(make_shared<vector<shared_ptr<PCard>>>());
                    for (shared_ptr<PCard> meldCard : (*tempMeld))
                    {
                        playersMelds.at(playersMelds.size() - 1)->push_back(meldCard);
                    }
                    (*tempMeld).clear();
                    break;
                }
            }

            if ((*tempMeld).size() > 0)
            {
                playersMelds.push_back(make_shared<vector<shared_ptr<PCard>>>());
                for (shared_ptr<PCard> meldCard : (*tempMeld))
                {
                    playersMelds.at(playersMelds.size() - 1)->push_back(meldCard);
                }
                (*tempMeld).clear();
            }
        }
        else
        {
            (*suitMatches).pop_back();
        }
    }


}


void Tong_Its_Player::print_a_row(int rowToPrint)
{
    int numCardsInHand = (*playersHand).size();
    int numCardsToPrint = 0;
    vector<shared_ptr<PCard>> cardsToPrint = vector<shared_ptr<PCard>>();

    // Input Validation
    if (rowToPrint < 1)
    {
        throw invalid_argument("print_a_row() - Invalid row number to print");
    }
    // else if ((((rowToPrint - 1) * NUM_CARDS_PER_ROW) + 1) > numCardsInHand)
    // {
    //     throw range_error("print_a_row() - Row does not exist in player's hand");
    // }

    // Determine Number Of Cards To Print
    if(NUM_CARDS_PER_ROW < 1)
    {
        throw range_error("Tong_Its_Player::print_a_row() - Invalid number of cards per row to print");
    }

    numCardsToPrint = numCardsInHand - ((rowToPrint - 1) * NUM_CARDS_PER_ROW);
    if (numCardsToPrint > NUM_CARDS_PER_ROW)
    {
        numCardsToPrint = NUM_CARDS_PER_ROW;
    }
    else if (numCardsInHand < NUM_CARDS_PER_ROW)
    {
        numCardsToPrint = numCardsInHand;
    }

    // Get the card pointers
    // cout << "Size of hand: " << (*playersHand).size() << endl;  // DEBUGGING
    // cout << "Card 1: Rank " << (*playersHand).at(0)->rank << endl;  // DEBUGGING
    // cout << "Card 2: Rank " << (*playersHand).at(1)->rank << endl;  // DEBUGGING
    // TEST_the_hand();  // DEBUGGING
    if (numCardsToPrint < numCardsInHand)
    {
        for (int i = ((rowToPrint - 1) * NUM_CARDS_PER_ROW); i < (numCardsToPrint + ((rowToPrint - 1) * NUM_CARDS_PER_ROW)); ++i)
        {
            // cout << "Card # " << i << endl;  // DEBUGGING 
            cardsToPrint.push_back((*playersHand).at(i));
        }
    }
    else if (numCardsToPrint == numCardsInHand)
    {
        cardsToPrint = (*playersHand);
    }

    // DO NOT DELETE... UNDERSTANDING REQUIRES AN IDE
    // for (auto it = begin(*playersHand) + ((rowToPrint - 1) * 4); it < begin(*playersHand) + (rowToPrint * 4); ++it)
    // {
    //     cout << "Rank: " << (*it)->rank << endl;  // DEBUGGING
    //     cout << "Suit: " << (*it)->suit << endl;  // DEBUGGING
    // }

    // Print the card pointers
    // Card Row 1
    for (shared_ptr<PCard> cardToPrint : cardsToPrint)
    {
        cout << BORDER_UPPER_LEFT << BORDER_HORIZONTAL << BORDER_HORIZONTAL << BORDER_HORIZONTAL << BORDER_HORIZONTAL << BORDER_UPPER_RIGHT;        
    }
    cout << endl;
    // Row 2
    for (shared_ptr<PCard> cardToPrint : cardsToPrint)
    {
        if (cardToPrint->rank == "10")
        {
            cout << BORDER_VERTICAL << cardToPrint->rank << BORDER_SPACE << BORDER_SPACE << BORDER_VERTICAL;
        }
        else
        {
            cout << BORDER_VERTICAL << cardToPrint->rank << BORDER_SPACE << BORDER_SPACE << BORDER_SPACE << BORDER_VERTICAL;
        }        
    }
    cout << endl;
    // Row 3
    for (shared_ptr<PCard> cardToPrint : cardsToPrint)
    {
        cout << BORDER_VERTICAL << BORDER_SPACE << cardToPrint->suit << BORDER_SPACE << BORDER_SPACE << BORDER_VERTICAL;
    }
    cout << endl;
    // Row 4
    for (shared_ptr<PCard> cardToPrint : cardsToPrint)
    {
        if (cardToPrint->rank == "10")
        {
            cout << BORDER_VERTICAL << BORDER_SPACE << BORDER_SPACE << cardToPrint->rank << BORDER_VERTICAL;
        }
        else
        {
            cout << BORDER_VERTICAL << BORDER_SPACE << BORDER_SPACE << BORDER_SPACE << cardToPrint->rank << BORDER_VERTICAL;
        }        
    }
    cout << endl;
    // Row 5
    for (shared_ptr<PCard> cardToPrint : cardsToPrint)
    {
        cout << BORDER_LOWER_LEFT << BORDER_HORIZONTAL << BORDER_HORIZONTAL << BORDER_HORIZONTAL << BORDER_HORIZONTAL << BORDER_LOWER_RIGHT;        
    }    
    cout << endl;

    return;
}


void Tong_Its_Player::sort_cards(shared_ptr<vector<shared_ptr<PCard>>> cardsToSort, bool sortByTheSuits)
{
    if (sortByTheSuits == true)
    {
        sort((*cardsToSort).begin(), (*cardsToSort).end(), [ ]( const auto& left, const auto& right )
            {
                if (left->suitValue < right->suitValue)
                {
                    return true;
                }
                else if (left->suitValue == right->suitValue)
                {
                    return left->rankValue < right->rankValue;
                }
            });
    }
    else
    {
        sort((*cardsToSort).begin(), (*cardsToSort).end(), [ ]( const auto& left, const auto& right )
            {
                if (left->rankValue < right->rankValue)
                {
                    return true;
                }
                else if (left->rankValue == right->rankValue)
                {
                    return left->suitValue < right->suitValue;
                }
            });
    }

    return;
}


int Tong_Its_Player::random_num(int start, int stop)
{
    // Local Variables
    int low = start;
    int high = stop;
    int retVal = 0;

    // Input Validation
    if (low > high)
    {
        low = low ^ high;
        high = low ^ high;
        low = low ^ high;
    }
    
    if (start == stop)
    {
        retVal = start;
    }
    else
    {
        retVal = (rand() % (high - low + 1)) + (high - low);
    }

    return retVal;
}


/*
    Purpose - Determine if this card is in a potential meld
    Input - findThisCard - PCard to find in playersPotentialMelds
    Output - true if in a potential meld, false otherwise
    Note - Also updates how many melds it is in
 */
bool Tong_Its_Player::card_in_a_meld(shared_ptr<PCard> findThisCard)
{
    // LOCAL VARIABLES
    bool retVal = false;

    // RESET MELD COUNTER
    findThisCard->numMelds = 0;

    // FIND IT
    for (auto meldSet : playersMelds)
    {
        if (meldSet == nullptr)
        {
            throw runtime_error("Tong_Its_Player::card_in_a_meld() found a nullptr");
        }

        for (auto meldCard : (*meldSet))
        {
            if (meldCard == findThisCard)
            {
                retVal = true;
                findThisCard->numMelds++;
            }
        }
    }

    // DONE
    return retVal;
}
/***********************/
/* TONG ITS PLAYER END */
/***********************/


/***********************/
/* TONG ITS GAME BEGIN */
/***********************/
// Tong_Its_Game::Tong_Its_Game(shared_ptr<string> humanPlayerName) : 
Tong_Its_Game::Tong_Its_Game(const shared_ptr<string>& humanPlayerName) : 
players({*humanPlayerName, string("Mike"), string("Eren")})
// player1(*humanPlayerName), player2(string("Mike")), player3(string("Eren"))
// player1(*humanPlayerName), player2(string("Player2")), player3(string("Player3"))
// player1("Human"), player2("Player2"), player3("Player3")
// player1(*humanPlayerName), player2(*humanPlayerName), player3(*humanPlayerName)

{
    // for (auto player : players)
    // {
    //     cout << "Player: " << player.get_name() << endl;  // DEBUGGING
    // }
    // 0. Seed the random number generator
    srand(unsigned(time(NULL)));

    // 1. Build the deck of cards
    // cout << "Function call to build_a_deck()" << endl;  // DEBUGGING
    drawPile = build_a_deck();
    // cout << "build_a_deck() made " << drawPile << endl;  // DEBUGGING

    // 2. Shuffle the deck of cards
    // cout << "Function call to shuffle_a_deck()" << endl;  // DEBUGGING
    shuffle_a_deck(drawPile);  // COMMENTED FOR TESTING

    // 3. Create the discard pile
    discardPile = make_shared<vector<shared_ptr<PCard>>>();

    // 4. Set the current dealer to Player 1
    // currentDealer = 1;

    // 5. Set current player
    currentPlayer = 1;  // NOTE: Uncessary when currentDealer is (re)implemented?

    // 6. Deal hands to each player
    // cout << "Function call to deal_player_hands()" << endl;  // DEBUGGING
    deal_player_hands(players[0]);
    // print_a_card((*drawPile).at(0));  // Accomplished in user_interface() now


    // cout << "Ctor - Current player: " << currentPlayer << endl;  // DEBUGGING
}


void Tong_Its_Game::start_the_game(void)
{
    // LOCAL VARIABLES
    int gameOver = 0;
    int winningPlayerNumber = 0;

    while(gameOver != USER_EXIT)
    {
        // 1. Whose turn is it?
        if (currentPlayer == 1)
        {
            gameOver = user_interface();
        }
        else if (currentPlayer == 2 || currentPlayer == 3)
        {
            // AI Turn
            // Implement this
        }
        else
        {
            throw runtime_error("start_the_game() - currentPlayer appears to be broken");
        }

        // 2. Has anyone won?
        if (is_the_game_over())
        {
            // Scoring
            winningPlayerNumber = score_the_game();

            // Resolve chips
            // IMPLEMENT THIS LATER

            // Continue playing?
            // If yes, reset game and player variables, increment the dealer then call start_the_game() again
            // If no...
            cout << "'Game over man. Game over!' -Pvt. Hudson" << endl;  // DEBUGGING
            break;
        }
        else
        {
            // 3. Iterate to the next player
            currentPlayer = next_player();
        }
    }

    return;
}


void Tong_Its_Game::TEST_the_deck(shared_ptr<vector<shared_ptr<PCard>>> deckToTest)
{
    cout << "Testing the deck" << endl;
    if (deckToTest)
    {
        cout << "Deck size: " << (*deckToTest).size() << endl;
    }
    else
    {
        cout << "Deck is is non-existent!" << endl;
    }        

    // setlocale(LC_ALL, "");
    for (int i = 0; i < (*deckToTest).size(); ++i)
    {
        cout << "\nCard # " << i + 1 << endl;
        cout << "Rank: " << (*deckToTest)[i]->rank << endl;
        cout << "Suit: " << (*deckToTest)[i]->suit << endl;
        cout << "Value: " << (*deckToTest)[i]->rankValue << endl;
    }

    return;
}


/*
    Purpose - Tong_Its_Player discards a card
    Input - Shared pointer to the card to be discarded
    Output - None
 */
void Tong_Its_Game::receive_a_discard(shared_ptr<PCard> discardedCard)
{
    if (discardedCard)
    {
        (*discardPile).push_back(discardedCard);
    }

    return;
}


/*
    Purpose - Tong_Its_Player draws a card
    Input - None (card drawn always comes from the back)
    Output - Shared pointer to PCard removed from the drawPile
 */
shared_ptr<PCard> Tong_Its_Game::card_is_drawn(void)
{
    int cardPos = (*drawPile).size() - 1;
    auto retVal = (*drawPile).at(cardPos);
    if (retVal)
    {
        (*drawPile).pop_back();
    }
    return retVal;
}


/*
    Purpose - Tong_Its_Player takes the top discard from the discardPile
    Input - None (card taken always comes from the back)
    Output - Shared pointer to PCard removed from the discardPile
 */
shared_ptr<PCard> Tong_Its_Game::discard_is_taken(void)
{
    int cardPos = (*discardPile).size() - 1;
    auto retVal = (*discardPile).at(cardPos);
    if (retVal)
    {
        (*discardPile).pop_back();
    }
    return retVal;
}


bool Tong_Its_Game::is_the_game_over(void)
{
    // LOCAL VARIABLES
    bool retVal = false;
    // int run = 0;  // DEBUGGING

    // CHECK GAME STATUS
    // 1. Draw pile exhausted
    if ((*drawPile).size() == 0)
    {
        cout << "GAME OVER:  Ran out of cards!" << endl;  // DEBUGGING
        retVal = true;
    }
    else
    {
        // 2. Did a player end the game?
        for (auto player : players)
        {
            // ++run;  // DEBUGGING
            // 2.1. A player called Tongits
            if (player.called_tongits())
            {
                cout << "GAME OVER:  " << player.get_name() << " called Tongits!" << endl;  // DEBUGGING
                retVal = true;
                break;
            }
            // 2.2. A player called Draw
            if (player.called_draw())
            {
                // cout << "Run #" << run << endl;  // DEBUGGING
                cout << "GAME OVER:  " << player.get_name() << " called Draw!" << endl;  // DEBUGGING
                retVal = true;
                break;
            }
            // 2.3. A player is out of cards (see: TONGITS!)
            if (player.hand_size() == 0)
            {
                player.call_tongits();
                cout << "GAME OVER:  " << player.get_name() << " got Tongits!" << endl;  // DEBUGGING
                retVal = true;
                break;
            }
        }
    }

    // DONE
    return retVal;
}


shared_ptr<vector<shared_ptr<PCard>>> Tong_Its_Game::build_a_deck(void)
{
    auto retVal = make_shared<vector<shared_ptr<PCard>>>();
    shared_ptr<PCard> tempCard;
    vector<string> cardRanks = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
    vector<string> cardSuits = {spadeString, clubString, heartString, diamondString};

    // Build the deck
    try
    {
        // cout << "Building deck." << endl;  // DEBUGGING
        for (string suit : cardSuits)
        {
            for (string rank : cardRanks)
            {
                // cout << rank << endl;  // DEBUGGING
                tempCard = make_shared<PCard>(PCard(rank, suit));
                (*retVal).emplace_back(tempCard);
            }
        }        
    }
    catch (...)
    {
        cout << "EXCEPTION!";  // DEBUGGING
    }

    return retVal;
}


/*
    Input - Shared pointer to a vector of PCard shared pointers (see: Pointer to a stack of cards)
    Output - None
    Purpose - Shuffle the cards
 */
void Tong_Its_Game::shuffle_a_deck(shared_ptr<vector<shared_ptr<PCard>>> deckOfCards)
{
    // srand(unsigned(time(NULL)));  // This will be performed in the game constructor

    random_shuffle((*deckOfCards).begin(), (*deckOfCards).end());

    return;
}


/*
    Note - 13 cards to the dealer, 12 cards to the other players
 */
void Tong_Its_Game::deal_player_hands(Tong_Its_Player currentDealer)
{
    // auto tempDeck = make_shared<vector<shared_ptr<PCard>>>();
    // auto tempCard = make_shared<PCard>(" ", " ");

    // One to the dealer   
    auto tempCard = card_is_drawn();
    // cout << "Card: Rank " << tempCard->rank << " Suit " << tempCard->suit << endl;  // DEBUGGING

    // cout << "deal_player_hands() - Current player: " << currentPlayer << endl;  // DEBUGGING

    if (currentPlayer == 1)
    {
        players[0].receive_a_card(tempCard);
    }
    else if (currentPlayer == 2)
    {
        players[1].receive_a_card(tempCard);        
    }
    else if (currentPlayer == 3)
    {
        players[2].receive_a_card(tempCard);
    }
    else
    {
        throw runtime_error("deal_player_hands() - Tong_Its_Game.currentPlayer has become corrupted");
    }

    // 12 to everyone
    for (int i = 0; i < 12; ++i)
    {
        tempCard = card_is_drawn();
        players[0].receive_a_card(tempCard);

        tempCard = card_is_drawn();
        players[1].receive_a_card(tempCard);

        tempCard = card_is_drawn();
        players[2].receive_a_card(tempCard);
    }

    return;
}


void Tong_Its_Game::print_a_card(shared_ptr<PCard> cardToPrint)
{
    if (cardToPrint)
    {
        // Row 1
        cout << BORDER_UPPER_LEFT << BORDER_HORIZONTAL << BORDER_HORIZONTAL << BORDER_HORIZONTAL << BORDER_HORIZONTAL << BORDER_UPPER_RIGHT << endl;
        // Row 2
        if (cardToPrint->rank == "10")
        {
            cout << BORDER_VERTICAL << cardToPrint->rank << BORDER_SPACE << BORDER_SPACE << BORDER_VERTICAL << endl;
        }
        else
        {
            cout << BORDER_VERTICAL << cardToPrint->rank << BORDER_SPACE << BORDER_SPACE << BORDER_SPACE << BORDER_VERTICAL << endl;            
        }
        // Row 3
        cout << BORDER_VERTICAL << BORDER_SPACE << cardToPrint->suit << BORDER_SPACE << BORDER_SPACE << BORDER_VERTICAL << endl;
        // Row 4
        if (cardToPrint->rank == "10")
        {
            cout << BORDER_VERTICAL << BORDER_SPACE << BORDER_SPACE << cardToPrint->rank << BORDER_VERTICAL << endl;
        }
        else
        {
            cout << BORDER_VERTICAL << BORDER_SPACE << BORDER_SPACE << BORDER_SPACE << cardToPrint->rank << BORDER_VERTICAL << endl;
        }
        // Row 5
        cout << BORDER_LOWER_LEFT << BORDER_HORIZONTAL << BORDER_HORIZONTAL << BORDER_HORIZONTAL << BORDER_HORIZONTAL << BORDER_LOWER_RIGHT << endl;
    }
    else
    {
        throw invalid_argument("NULL card pointer");
    }

    return;
}


int Tong_Its_Game::user_interface(void)
{
    auto tempCard = make_shared<PCard>(" ", " ");
    string dynamicChoice1opt1 = string("Draw a card");
    string dynamicChoice1opt2 = string("Discard a card");
    string dynamicChoice1 = dynamicChoice1opt1;

    string dynamicChoice2 = string("Expose a meld");

    string dynamicChoice3 = string("Show all melds");

    string dynamicChoice4 = string("Reprint game state");

    string dynamicChoice5opt1 = string("Toggle hand sort (to sort-by-rank)");
    string dynamicChoice5opt2 = string("Toggle hand sort (to sort-by-suit)");
    string dynamicChoice5 = dynamicChoice5opt1;

    // string dynamicChoice6 = string("Show sets in hand");

    string dynamicChoice6 = string("TONGITS!");
    string dynamicChoice7 = string("Call Draw");
    string dynamicChoice9 = string("Exit");
    int menuChoice = 0;
    int subMenuChoice = 0;
    bool isTurnOver = false;
    bool eligibleToCallDraw = true;

    game_state();

    while(menuChoice != USER_EXIT && isTurnOver == false && is_the_game_over() == false)
    {
        // Reset temp variables
        menuChoice = 0;
        subMenuChoice = 0;

        // Print interface
        if (players[0].count_cards() == 13)
        {
            dynamicChoice1 = dynamicChoice1opt2;
        }
        cout << "1. " << dynamicChoice1 << endl;
        cout << "2. " << dynamicChoice2 << endl;
        cout << "3. " << dynamicChoice3 << endl;
        cout << "4. " << dynamicChoice4 << endl;
        cout << "5. " << dynamicChoice5 << endl;
        if (players[0].count_cards() <= 1)
        {
            cout << "6. " << dynamicChoice6 << endl;  // TONGITS
        }
        if (eligibleToCallDraw)
        {
            cout << "7. " << dynamicChoice7 << endl;  // Draw
        }
        cout << USER_EXIT << ". " << dynamicChoice9 << endl;

        // Take user input
        menuChoice = input_number();
        // cin >> menuChoice;

        switch (menuChoice)
        {
            // 1. DRAW/DISCARD
            case 1:
                // Draw a card
                if (dynamicChoice1 == dynamicChoice1opt1)
                {
                    // Prompt
                    cout << "Draw a card from 1. Draw pile or 2. Discard pile?" << endl;

                    // Input
                    subMenuChoice = input_number();
                    // cin >> subMenuChoice;

                    // Input Validation
                    if (subMenuChoice < 1 || subMenuChoice > 2)
                    {
                        // Try again
                        cout << "Invalid card number.\n" << "Please choose again." << endl;
                        break;
                    }
                    else if (subMenuChoice == 1)
                    {
                        players[0].receive_a_card(card_is_drawn());
                        eligibleToCallDraw = false;
                    }
                    else if (subMenuChoice == 2)
                    {
                        players[0].receive_a_card(discard_is_taken());
                        eligibleToCallDraw = false;
                    }
                    else
                    {
                        throw range_error("user_interface() - Menu item #1 sub-selection is invalid");
                    }
                    
                    // Toggle menu choice
                    dynamicChoice1 = dynamicChoice1opt2;

                    // Reprint the state of the game
                    game_state();
                }
                // Discard a card
                else if (dynamicChoice1 == dynamicChoice1opt2)
                {
                    // Prompt
                    cout << "Enter the number of the card you would like to discard: " << endl;
                    
                    // Input
                    subMenuChoice = input_number();
                    // cin >> subMenuChoice;

                    // Input Validation
                    if (subMenuChoice < 1 || subMenuChoice > players[0].count_cards())
                    {
                        // Try again
                        cout << "Invalid card number.\n" << "Please choose again." << endl;
                        break; 
                    }
                    else
                    {
                        // Remove the card
                        // Player removes the card from his hand
                        try
                        {
                            tempCard = players[0].play_a_card(subMenuChoice);
                        }
                        catch (const std::invalid_argument& err)
                        {
                            cerr << "Invalid argument: " << err.what() << endl;
                            cout << "Please choose again." << endl;
                            continue;
                        }
                        // Removed card is added to the discard pile
                        receive_a_discard(tempCard);
                        eligibleToCallDraw = false;
                        // Turn is over
                        isTurnOver = true;
                        if (players[0].count_cards() == 0)
                        {
                            players[0].call_tongits();
                        }
                    }
                }
                else
                {
                    throw "Menu option 1 was corrupted!";
                }
                break;
            // 2. EXPOSE A MELD
            case 2:
                if (players[0].count_potential_melds() > 0)
                {
                    // Print
                    players[0].show_all_melds(true);

                    // Prompt
                    cout << "Enter the number of the meld you would like to expose: " << endl;
                    
                    // Input
                    subMenuChoice = input_number();

                    // Input Validation
                    if (subMenuChoice < 1 || subMenuChoice > players[0].count_potential_melds())
                    {
                        // Try again
                        cout << "Invalid meld number.\n" << "Please choose again." << endl;
                        break;
                    }
                    else
                    {
                        if (!(players[0].expose_a_meld(subMenuChoice)))
                        {
                            cout << "There was a problem exposing your meld.\n" << endl;
                        }
                        else
                        {
                            game_state();
                        }
                    }
                }
                else
                {
                    cout << "\nYou have no melds.\n" << endl;                    
                }
                break;
            // 3. SHOW ALL MELDS
            case 3:
                players[0].show_all_melds(false);
                break;
            // 4. REPRINT GAME STATE
            case 4:
                clear_the_screen();
                // for (int i = 0; i < CLEAR_SCREEN; ++i)
                // {
                //     cout << "\n";
                // }
                game_state();
                break;
            // 5. TOGGLE HAND SORTING
            case 5:
                if (players[0].sorting_by_suit() == true)
                {
                    players[0].toggle_sort();
                    dynamicChoice5 = dynamicChoice5opt2;
                }
                else
                {
                    players[0].toggle_sort();
                    dynamicChoice5 = dynamicChoice5opt1;    
                }
                game_state();
                break;
            // 6. TONGITS
            case 6:
                if (players[0].count_cards() <= 1)
                {
                    players[0].call_tongits();
                }
                else
                {
                    // Try again
                    cout << "You are not permitted to call Tongits at this time.\n" << "Please choose again." << endl;
                }
                break;
            // 7. DRAW
            case 7:
                if (eligibleToCallDraw)
                {
                    // IMPLEMENT THIS LATER
                    // 1. Call draw
                    players[0].call_draw();
                    // 2. Other players can challenge or fold (lose if ineligible to challenge)
                    // 3. Score and compare the players that challenged
                }
                else
                {
                    // Try again
                    cout << "You are not permitted to call Draw at this time.\n" << "Please choose again." << endl;
                }
                break;
            case USER_EXIT:
                cout << "Exiting game" << endl;
                break;
            default:
                cout << "Invalid selection\n" << "Please choose again" << endl;
                break;
        }
    }

    return menuChoice;
}


void Tong_Its_Game::game_state(void)
{
    // Clear the screen
    clear_the_screen();
    // Blank card
    shared_ptr<PCard> blankCard = make_shared<PCard>(PCard(" ", " "));
    // Deck size
    int deckSize = (*discardPile).size();
    // Print player status
    cout << "Player 1: " << players[0].get_name() << " has " << players[0].count_cards() << " cards and " << players[0].count_chips() << " chips." << endl;
    cout << "Player 2: " << players[1].get_name() << " has " << players[1].count_cards() << " cards and " << players[1].count_chips() << " chips." << endl;
    cout << "Player 3: " << players[2].get_name() << " has " << players[2].count_cards() << " cards and " << players[2].count_chips() << " chips." << endl;
    cout << "\n";

    // Print discard pile
    cout << "DISCARD PILE" << endl;
    if (deckSize > 0)
    {
        print_a_card((*discardPile).at(deckSize - 1));
    }
    else  // Game just began
    {
        print_a_card(blankCard);
    }
    cout << "\n";

    // Print exposed melds
    if (players[0].count_exposed_melds() > 0)
    {
        cout << "Player 1: " << players[0].get_name() << "'s Exposed Melds" << endl;
        players[0].print_exposed_melds();
    }
    if (players[1].count_exposed_melds() > 0)
    {
        cout << "Player 2: " << players[1].get_name() << "'s Exposed Melds" << endl;
    }
    if (players[2].count_exposed_melds() > 0)
    {
        cout << "Player 3: " << players[2].get_name() << "'s Exposed Melds" << endl;
    }
    if (players[0].count_exposed_melds() > 0 || players[1].count_exposed_melds() > 0 || players[2].count_exposed_melds() > 0)
    {
        cout << endl;
    }

    // Print player's hand
    cout << "YOUR HAND" << endl;
    players[0].print_players_hand();

    return;
}


int Tong_Its_Game::next_player(void)
{
    int retVal = currentPlayer;

    if (retVal < 1 || retVal > players.size())
    {
        throw runtime_error("next_player() - Tong_Its_Game.currentPlayer has become corrupted"); 
    }    
    else if (retVal == players.size())
    {
        retVal = 1;
    }
    else
    {
        ++retVal;
    }

    return retVal;
}


/*
    Purpose - Score the game and determine the winner
    Input - None
    Output - Player number of the winner
    Note - Prints the winning statement
 */
int Tong_Its_Game::score_the_game(void)
{
    // LOCAL VARIABLES
    string currentWinner = "";
    int winningScore = INT_MAX;
    int winningPlayerNumber = 0;
    int playerNumber = 0;
    bool someoneCalledDraw = false;

    // PROCESS SCORES
    // Did anyone call Tongits or Draw?
    for (auto player : players)
    {
        playerNumber++;
        if (player.called_tongits())
        {
            cout << "Player " << playerNumber << ": " << player.get_name() << " called Tongits." << endl;
            break;
        }
        else if (player.called_draw())
        {
            cout << "Player " << playerNumber << ": " << player.get_name() << " called Draw." << endl;
            someoneCalledDraw = true;
            break;
        }
    }
    playerNumber = 0;

    // Did anyone get burned?
    for (auto player : players)
    {
        playerNumber++;
        if (player.count_exposed_melds() == 0)
        {
            player.got_burned();
            cout << "Player " << playerNumber << ": " << player.get_name() << " has " << player.count_exposed_melds() << " melds and got burned!" << endl;            
        }
    }
    playerNumber = 0;

    // Calculate scores for those that didn't get burned
    for (auto player: players)
    {
        playerNumber++;
        if (!player.is_burned())
        {
            if (player.get_final_score() < winningScore)
            {
                winningScore = player.get_final_score();
                currentWinner = player.get_name();
                winningPlayerNumber = playerNumber;
            }
        }
    }
    playerNumber = 0;

    // Validate the end-of-game state
    if (players[winningPlayerNumber - 1].called_tongits() == false && someoneCalledDraw == false && (*drawPile).size() > 0)
    {
        throw runtime_error("Tong_Its_Game::score_the_game() has been called but the game doesn't appear to be over yet!"); 
    }
    else
    {
        // Print the winner
        cout << "Player " << winningPlayerNumber << ": " << currentWinner << " won with a score of " << winningScore << "!  CONGRATULATIONS!" << endl;
    }

    // DONE
    return winningPlayerNumber;
}
/*********************/
/* TONG ITS GAME END */
/*********************/


int input_number(void)
{
    string str;
    bool instructions = false;
    regex regex_pattern("^[1-9]\\d*$");

    do
    {
        if (instructions)
        {
            cout << "Input a positive number: ";
        }
        cin >> str;
        instructions = true;
    }
    while(!regex_match(str,regex_pattern));

    return stoi(str);
}

/*
*************
* INTERFACE *
*************

1. Draw/Discard a card
2. Reprint hand
3. Toggle hand sort (to _____)
4. Show sets in hand
5. 
 */


/*
****************
* CARD DESIGNS *
****************
#define SPADE L'♤'
#define HEART L'♥'
#define DIAMOND L'♦'
#define CLUB L'♧'
#define BORDER_UPPER_LEFT "╔"
#define BORDER_UPPER_RIGHT "╗"
#define BORDER_VERTICAL "║"
#define BORDER_LOWER_RIGHT "╝"
#define BORDER_LOWER_LEFT "╚"
#define BORDER_HORIZONTAL "═"
╔═══╗
║ A ║
║ ♤ ║
╚═══╝ 

╔═══╗
║ K ║
║ ♥ ║
╚═══╝ 

╔═══╗
║ 2 ║
║ ♧ ║
╚═══╝ 

╔═════╗
║ 2   ║
║  ♧  ║
║   2 ║
╚═════╝ 

╔═══╗
║2  ║
║ ♧ ║
║  2║
╚═══╝ 

 */
