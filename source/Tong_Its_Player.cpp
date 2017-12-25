#include "Clear_Screen.h"       // clear_screen
#include "Tong_Its.h"
#include "Tong_Its_Game.h"
#include "Tong_Its_Player.h"
#include "Tong_Its_Playing_Card.h"
#include <algorithm>            // random_shuffle && sort 
#include <iostream>
#include <memory>
#include <string>
#include <vector>


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
    challengedDraw = false;
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


/*
    Purpose - Count the players's total number of aces for the purposes of scoring
    Input - None
    Output - The total number of aces
    Note - This method counts both aces in hand and aces in exposed melds but does not count 'sapaw' aces
 */
int Tong_Its_Player::count_aces(void)
{
    int retVal = 0;

    for (auto playingCard : (*playersHand))
    {
        if (playingCard->rankValue == 1 && !(playingCard->sapaw))
        {
            retVal++;
        }
    }
    for (auto oneExposedMeld : playersExposedMelds)
    {
        for (auto playingCard : (*oneExposedMeld))
        {
            if (playingCard->rankValue == 1 && !(playingCard->sapaw))
            {
                retVal++;
            }
        }
    }

    return retVal;
}


void Tong_Its_Player::win_chips(int wonChips)
{
    if (wonChips > 0)
    {
        numOfChips += wonChips;
    }
    else
    {
        throw invalid_argument("Tong_Its_Player::win_chips() was passed an invalid value for wonChips");
    }

    return;
}


/*
    Purpose - The player lost some chips
    Input - Number of chips lost
    Output - Last remaining chips or lostChips, whichever is lowest
    Note - This method will not allow the player's numOfChips to go negative
 */
int Tong_Its_Player::lose_chips(int lostChips)
{
    // LOCAL VARIABLES
    int retVal = 0;

    // PAY OUT
    // Not enough to cover
    if (lostChips > numOfChips)
    {
        numOfChips = 0;
        retVal = numOfChips;
    }
    // Enough to cover
    else
    {
        numOfChips -= lostChips;
        retVal = lostChips;
    }

    // DONE
    return retVal;
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



void Tong_Its_Player::challenge(void)
{
    challengedDraw = true;
    return;
}


bool Tong_Its_Player::challenged_a_draw(void)
{
    return challengedDraw;
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


void Tong_Its_Player::reset(Tong_Its_Game* theGame_ptr)
{
    // 1. Reset member variables
    numOfCards = 0;
    sortBySuit = true;
    calledTongits = false;
    calledDraw = false;
    challengedDraw = false;
    burned = false;
    finalScore = 0;
    // 2. Reset member containers
    // 2.1. Player's Hand
    for (int i = 0; i < (*playersHand).size(); i++)
    {
        theGame_ptr->receive_a_discard(play_a_card(1));
    }
    // 2.2. Player's Melds
    playersMelds.clear();
    // 2.3. Player's Exposed Melds
    playersExposedMelds.clear();
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
