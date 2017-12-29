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
    open = false;
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


int Tong_Its_Player::count_potential_melds(vector<Tong_Its_Player>& players)
{
    // Refresh player's melds
    update_potential_melds(true, players);

    // Count the melds
    return playersMelds.size();
}


int Tong_Its_Player::count_special_melds(vector<Tong_Its_Player>& players)
{
    int retVal = 0;

    // Refresh player's melds
    update_potential_melds(true, players);

    // Count the special cards
    for (auto playingCard : (*playersHand))
    {
        if (playingCard->special)
        {
            retVal++;
        }
    }
    for (auto oneExposedMeld : playersExposedMelds)
    {
        for (auto playingCard : (*oneExposedMeld))
        {
            if (playingCard->special)
            {
                retVal++;
            }
        }
    }

    // Translate special count to meld count
    if (retVal % 4 != 0)
    {
        throw runtime_error("Tong_Its_Player::count_special_melds() appears to have found a rogue 'special' card");
    }
    else
    {
        retVal = retVal / 4;
    }

    return retVal;
}


/*
    Purpose - Glue together all of the 'heavy lifting' methods to expose one meld
    Input - Meld number to expose (see: Index number into the potentialMeld vector)
    Output - True for sucess, False for failure
 */
bool Tong_Its_Player::expose_a_meld(int meldNum, vector<Tong_Its_Player>& players)
{
    // LOCAL VARIABLES
    bool retVal = false;
    shared_ptr<vector<shared_ptr<PCard>>> pMeldsVector_ptr = nullptr;  // playersMelds
    shared_ptr<vector<shared_ptr<PCard>>> pExpMeldsVector_ptr = nullptr;  // playersExposedMelds
    shared_ptr<PCard> tmpPCard_ptr = nullptr;
    int tempCardNum = 0;
    vector<shared_ptr<PCard>> tmpMeldSet;
    bool runMeld = false;  // Used to define the type of meld the PCards are in
    bool setMeld = false;  // Used to define the type of meld the PCards are in

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

    if ((*pMeldsVector_ptr)[0]->sapaw)
    {
        retVal = expose_a_sapaw_meld(pMeldsVector_ptr, players);
    }
    else
    {
        retVal = expose_a_normal_meld(pMeldsVector_ptr);    
    }
    
    // for (auto meldCard : (*pMeldsVector_ptr))
    // {
    //     // 2. Remove the meld from player's hand
    //     // 2.1. Get the card number
    //     tempCardNum = get_card_number(meldCard);
    //     // 2.2. Remove that card
    //     try
    //     {
    //         tmpPCard_ptr = play_a_card(tempCardNum);
    //     }
    //     catch (const std::invalid_argument& err)
    //     {
    //         cerr << "Invalid argument: " << err.what() << endl;
    //         // continue;
    //     }
    //     // 2.3. Verify return value
    //     if (tmpPCard_ptr == nullptr)
    //     {
    //         throw runtime_error("Tong_Its_Player::expose_a_meld() received a nullptr");    
    //     }
    //     else if (meldCard != tmpPCard_ptr)
    //     {
    //         throw runtime_error("Tong_Its_Player::expose_a_meld() received the wrong card");
    //     }

    //     // 2.4. Add the card to the tmpMeldSet vector
    //     tmpMeldSet.push_back(tmpPCard_ptr);
    // }

    // // 3. Add the meld to the player's exposed melds
    // // 3.1. Make a new shared_pointer to a vector of PCard shared pointers
    // pExpMeldsVector_ptr = make_shared<vector<shared_ptr<PCard>>>();
    // if (pExpMeldsVector_ptr == nullptr)
    // {
    //     throw runtime_error("Tong_Its_Player::expose_a_meld() made a nullptr?!");    
    // }

    // // 3.2. Add a new shared_point to a vector of PCard shared pointers to playersExposedMelds
    // playersExposedMelds.push_back(pExpMeldsVector_ptr);

    // // 3.3. Copy the PCards to the player's exposed melds
    // for (auto meldCard : tmpMeldSet)
    // {
    //     meldCard->numMelds = 1;  // Set meld number
    //     meldCard->inRun = runMeld;
    //     meldCard->inSet = setMeld;
    //     (*pExpMeldsVector_ptr).push_back(meldCard);
    // }

    // // 3.4. Validate the meld
    // if (set_meld_type(pExpMeldsVector_ptr))
    // {
    //     // 3.5. Validate the copy
    //     if (tmpMeldSet.size() == (*pExpMeldsVector_ptr).size())
    //     {
    //         retVal = true;
    //         open = true;
    //     }   
    // }

    // 4. Recalculate the player's melds
    update_potential_melds(false, players);

    // 5. Did the player get Tongits?
    if (hand_size() == 0)
    {
        call_tongits();
    }

    // DONE
    return retVal;
}


/*
    Purpose - Expose a meld from the players hand into the players exposed melds
    Input
        pMeldsVector_ptr - pointer to a vector of PCard pointers that represents the meld to expose
    Output - True for sucess, False for failure
    Note - Extricated from expose_a_meld() to allow for normal/sapaw meld branching
 */
bool Tong_Its_Player::expose_a_normal_meld(shared_ptr<vector<shared_ptr<PCard>>> pMeldsVector_ptr)
{
    // LOCAL VARIABLES
    bool retVal = false;
    int tempCardNum = 0;
    shared_ptr<PCard> tmpPCard_ptr = nullptr;
    vector<shared_ptr<PCard>> tmpMeldSet;
    shared_ptr<vector<shared_ptr<PCard>>> pExpMeldsVector_ptr = nullptr;  // playersExposedMelds
    bool runMeld = false;  // Used to define the type of meld the PCards are in
    bool setMeld = false;  // Used to define the type of meld the PCards are in

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
    // 3.1. Make a new shared_pointer to a vector of PCard shared pointers
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
        meldCard->inRun = runMeld;
        meldCard->inSet = setMeld;
        (*pExpMeldsVector_ptr).push_back(meldCard);
    }

    // 3.4. Validate the meld
    if (set_meld_type(pExpMeldsVector_ptr))
    {
        // 3.5. Validate the copy
        if (tmpMeldSet.size() == (*pExpMeldsVector_ptr).size())
        {
            retVal = true;
            open = true;
        }   
    }

    // DONE
    return retVal;
}


/*
    Purpose - Expose a meld from the players hand into a players exposed melds as a 'sapaw'
    Input
        pMeldsVector_ptr - pointer to a vector of PCard pointers that represents the meld to expose
        players - vector of Tong Its Players to place the pMeldsVector into
    Output - True for sucess, False for failure
    Note - Extricated from expose_a_meld() to allow for normal/sapaw meld branching
 */
bool Tong_Its_Player::expose_a_sapaw_meld(shared_ptr<vector<shared_ptr<PCard>>> pMeldsVector_ptr, vector<Tong_Its_Player>& players)
{
    // LOCAL VARIABLES
    bool retVal = false;
    int tempCardNum = 0;
    shared_ptr<PCard> tmpPCard_ptr = nullptr;
    vector<shared_ptr<PCard>> tmpMeldSet;
    shared_ptr<vector<shared_ptr<PCard>>> pExpMeldsVector_ptr = nullptr;  // playersExposedMelds
    bool runMeld = false;  // Used to define the type of meld the PCards are in
    bool setMeld = false;  // Used to define the type of meld the PCards are in

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
    // 3.1. Make a new shared_pointer to a vector of PCard shared pointers
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
        meldCard->inRun = runMeld;
        meldCard->inSet = setMeld;
        (*pExpMeldsVector_ptr).push_back(meldCard);
    }

    // 3.4. Validate the meld
    if (set_meld_type(pExpMeldsVector_ptr))
    {
        // 3.5. Validate the copy
        if (tmpMeldSet.size() == (*pExpMeldsVector_ptr).size())
        {
            retVal = true;
            open = true;
        }   
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


int Tong_Its_Player::current_card_points(vector<Tong_Its_Player>& players)
{
    // LOCAL VARIABLES
    int retVal = 0;

    // UPDATE HAND
    update_potential_melds(false, players);

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


void Tong_Its_Player::calc_final_score(vector<Tong_Its_Player>& players)
{
    finalScore = current_card_points(players);
}


int Tong_Its_Player::get_final_score(vector<Tong_Its_Player>& players)
{
    if (finalScore == 0)
    {
        calc_final_score(players);
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
    open = false;
    // 2. Reset member containers
    // 2.1. Player's Expsosed Melds
    // 2.1.1. Move them back to the player's hand
    for (auto exposedMeld : playersExposedMelds)
    {
        while ((*exposedMeld).size() > 0)
        {
            receive_a_card(play_any_card(1, exposedMeld));
        }
    }
    // 2.1.2. Remove the empty vectors
    while (!playersExposedMelds.empty())
    {
        playersExposedMelds.pop_back();
    }
    // 2.2. Player's Hand
    cout << "Pre-Reset:\t" << get_name() << " has " << count_cards() << " in his hand and " << count_exposed_melds() << " exposed melds." << endl;  // DEBUGGING
    while ((*playersHand).size() > 0)
    // for (int i = 0; i < (*playersHand).size(); i++)
    {
        theGame_ptr->receive_a_discard(play_a_card(1));
    }
    cout << "Pre-Reset:\t" << get_name() << " has " << count_cards() << " in his hand and " << count_exposed_melds() << " exposed melds." << endl;  // DEBUGGING
    // 2.3. Player's Melds (this is just a copy)
    playersMelds.clear();
}


bool Tong_Its_Player::already_open(void)
{
    return open;
}


bool Tong_Its_Player::challenge_a_draw(Tong_Its_Player& drawPlayer, vector<Tong_Its_Player>& players)
{
    // LOCAL VARIABLES
    bool retVal = false;
    int userChoice = 0;

    // PROCESS REQUEST
    // 1. Determine eligibility to challenge
    if (already_open())
    {
        // 2. State the situation
        cout << drawPlayer.get_name() << " has called Draw." << endl;
        cout << drawPlayer.get_name() << " has " << drawPlayer.count_cards() << " still in hand." << endl;
        cout << drawPlayer.get_name() << " has " << drawPlayer.count_exposed_melds() << "." << endl;
        drawPlayer.print_exposed_melds();

        // 3. Print current hand score
        cout << "You currently have " << count_cards() << " valued at " << current_card_points(players) << "." << endl;
        cout << "Do you want to challenge?  Enter 1 to challenge, any other positive number to fold." << endl;

        // 4. Take input from the user
        userChoice = input_number();

        // 5. Updated challenged draw accordingly
        if (userChoice == 1)
        {
            retVal = true;
        }
    }

    // DONE
    return retVal;
}


/*
    Purpose - Set the meld type for a vector of PCard pointers
    Input - Pointer to a vectore of PCard pointers
    Output - True on success, false on failure
    Note - This function calls validate_meld() before returning to verify the meld's integrity
 */
bool Tong_Its_Player::set_meld_type(shared_ptr<vector<shared_ptr<PCard>>> meldVec_ptr)
{
    // LOCAL VARIABLES
    bool retVal = false;
    string firstRank = "";
    string firstSuit = "";
    bool foundRun = true;
    bool foundSet = true;

    // INPUT VALIDATION
    if (meldVec_ptr == nullptr)
    {
        throw invalid_argument("Tong_Its_Player::set_meld_type() was passed a nullptr");
    }
    else if ((*meldVec_ptr).size() >= 3)
    {
        // DETERMINE MELD TYPE
        // Get the first card's details
        firstRank = (*meldVec_ptr)[0]->rank;
        firstSuit = (*meldVec_ptr)[0]->suit;
        // Compare the meld to the first card
        for (int i = 1; i < (*meldVec_ptr).size(); i++)
        {
            // Check for a run
            if ((*meldVec_ptr)[i]->suit != firstSuit)
            {
                foundRun = false;
            }

            // Check for a set
            if ((*meldVec_ptr)[i]->rank != firstRank)
            {
                foundSet = false;
            }

            // Premature failure
            if ((!foundRun && !foundSet) || (foundRun && foundSet))
            {
                break;
            }
        }
    }

    // Check results
    if ((foundRun || foundSet) && !(foundRun && foundSet))
    {
        // Set the PCard members accordingly
        for (auto meldCard : (*meldVec_ptr))
        {
            meldCard->inRun = foundRun;
            meldCard->inSet = foundSet;
        }

        // VALIDATE MELD
        retVal = validate_meld(meldVec_ptr);
    }

    // DONE
    return retVal;
}


bool Tong_Its_Player::validate_meld(shared_ptr<vector<shared_ptr<PCard>>> meldVec_ptr)
{
    // LOCAL VARIABLES
    bool retVal = true;
    string startRank = "";
    string startSuit = "";
    bool startInRun = false;
    bool startInSet = false;

    // INPUT VALIDATION
    if (meldVec_ptr == nullptr)
    {
        throw invalid_argument("Tong_Its_Player::validate_meld was passed a nullptr");
    }

    // PERFORM VALIDATION
    // 1. Validate individual PCards
    for (auto meldCard_ptr : (*meldVec_ptr))
    {
        retVal = meldCard_ptr->validate_playing_card();
        if (!retVal)
        {
            break;
        }
    }

    // 2. Validate the meld as a whole
    // 2.1. Verify meld meets minimum size
    if (retVal)
    {
        if ((*meldVec_ptr).size() < 3)
        {
            retVal = false;
        }
        // Set starting condition
        else
        {
            startRank = (*meldVec_ptr)[0]->rank;
            startSuit = (*meldVec_ptr)[0]->suit;
            startInRun = (*meldVec_ptr)[0]->inRun;
            startInSet = (*meldVec_ptr)[0]->inSet;
        }
    }
    // 2.2. Verify inRun and inSet are all the same
    if (retVal)
    {
        for (int i = 1; i < (*meldVec_ptr).size(); i++)
        {
            // Check inRun
            if ((*meldVec_ptr)[i]->inRun != startInRun)
            {
                retVal = false;
                break;
            }
            // Check inSet
            if ((*meldVec_ptr)[i]->inSet != startInSet)
            {
                retVal = false;
                break;
            }
        }
    }
    // 2.3. Verify inRun or inSet are valid
    if (retVal)
    {
        // 2.3.1. Run
        if (startInRun)
        {
            retVal = is_this_a_run(meldVec_ptr);
            // for (int i = 1; i < (*meldVec_ptr).size(); i++)
            // {
            //     // 2.3.1.1. Verify ordered
            //     if ((*meldVec_ptr)[i - 1]->rankValue + 1 != (*meldVec_ptr)[i]->rankValue)
            //     {
            //         retVal = false;
            //         break;
            //     }
            //     // 2.3.1.2. Verify all same suit
            //     else if ((*meldVec_ptr)[i]->suit != startSuit)
            //     {
            //         retVal = false;
            //         break;
            //     }
            // }
        }
        // 2.3.2. Set
        else if (startInSet)
        {
            retVal = is_this_a_set(meldVec_ptr);
            // // 2.3.2.1. Size is 3 or 4
            // if ((*meldVec_ptr).size() > 4)
            // {
            //     retVal = false;
            // }
            // else
            // {
            //     for (int i = 1; i < (*meldVec_ptr).size(); i++)
            //     { 
            //         // 2.3.2.2. All ranks match
            //         if ((*meldVec_ptr)[i]->rank != startRank)
            //         {
            //             retVal = false;
            //             break;
            //         }
            //         // 2.3.2.3. No suits match
            //         else if ((*meldVec_ptr)[i]->suit == startSuit)
            //         {
            //             retVal = false;
            //             break;
            //         }
            //     }
            // }
        }
    }

    // DONE
    return retVal;
}


/*
    Purpose - Evaluate a vector of PCard pointers as a run
    Input
        meldVec_ptr - pointer to a vector of PCard pointers
    Output - True if meldVec_ptr holds a meld, false otherwise
    Note
        This method was extricated from validate_a_meld() to aid 'sapaw' validation
 */
bool Tong_Its_Player::is_this_a_run(shared_ptr<vector<shared_ptr<PCard>>> meldVec_ptr)
{
    // LOCAL VARIABLES
    bool retVal = true;

    // INPUT VALIDATION
    if ((*meldVec_ptr).size() < 3)
    {
        retVal = false;
    }
    else
    {
        // VALIDATE RUN
        for (int i = 1; i < (*meldVec_ptr).size(); i++)
        {
            // Verify ordered
            if ((*meldVec_ptr)[i - 1]->rankValue + 1 != (*meldVec_ptr)[i]->rankValue)
            {
                retVal = false;
                break;
            }
            // Verify all same suit
            else if ((*meldVec_ptr)[i - 1]->suit != (*meldVec_ptr)[i]->suit)
            {
                retVal = false;
                break;
            }
        }
    }

    // DONE
    return retVal;
}


bool Tong_Its_Player::is_this_a_set(shared_ptr<vector<shared_ptr<PCard>>> meldVec_ptr)
{
    // LOCAL VARIABLES
    bool retVal = true;

    // INPUT VALIDATION
    if ((*meldVec_ptr).size() < 3 || (*meldVec_ptr).size() > 4)
    {
        retVal = false;
    }
    else
    {
        // VALIDATE RUN
        for (int i = 1; i < (*meldVec_ptr).size(); i++)
        { 
            // All ranks must match
            if ((*meldVec_ptr)[i - 1]->rank != (*meldVec_ptr)[i]->rank)
            {
                retVal = false;
                break;
            }
            // No suits can match
            else if ((*meldVec_ptr)[i - 1]->suit == (*meldVec_ptr)[i]->suit)
            {
                retVal = false;
                break;
            }
        }
    }

    // DONE
    return retVal;
}


vector<shared_ptr<vector<shared_ptr<PCard>>>> Tong_Its_Player::get_exposed_melds(void)
{
    return playersExposedMelds;
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
    print_playing_cards(true, playersHand);

    return;
}


void Tong_Its_Player::print_playing_cards(bool printNums, shared_ptr<vector<shared_ptr<PCard>>> printTheseCards)
{
    // INPUT VALIDATION
    if (printTheseCards == nullptr)
    {
        throw invalid_argument("print_a_row() - Received a nullptr");
    }

    int totalCards = (*printTheseCards).size();
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
        print_a_row(i, printNums, printTheseCards);
    }

    return;
}


/*
    Purpose - Update the vector of potential melds for a player
    Input - None
    Output - Number of melds in playersMelds
 */
void Tong_Its_Player::update_potential_melds(bool playOne, vector<Tong_Its_Player>& players)
{
    // Reset existing melds
    for (auto meldVec_ptr : playersMelds)
    {
        if (meldVec_ptr != nullptr)
        {
            for (auto meldCard_ptr : (*meldVec_ptr))
            {
                meldCard_ptr->sapaw = false;
                meldCard_ptr->special = false;
                meldCard_ptr->numMelds = 0;
                meldCard_ptr->inSet = false;
                meldCard_ptr->inRun = false;
            }
        }
    }

    // Clear existing melds
    playersMelds.clear();

    if (sortBySuit == true)
    {
        show_all_runs(playOne);
        show_all_sets(playOne);
    }
    else
    {
        show_all_sets(playOne);
        show_all_runs(playOne);   
    }

    update_potential_sapaw(playOne, players);

    return;
}


/*
    Purpose - Print all the melds in the order based on the TIP::sortBySuit bool
    Input - numbers melds if true
    Output - Number of melds that were found
    Notes - This function will ensure the original sorting state is maintained
 */
int Tong_Its_Player::show_all_melds(bool playOne, vector<Tong_Its_Player>& players)
{
    int currMeldNum = 1;
    bool originalSortingState = sortBySuit;
    playersMelds.clear();

    update_potential_melds(playOne, players);

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
    Output - None
 */
void Tong_Its_Player::show_all_runs(bool playOne)
{
    vector<string> cardSuits = {spadeString, clubString, heartString, diamondString};
    bool originalSortingState = sortBySuit;

    // INPUT VALIDATION
    // Existing validation factored out

    // CHECK SORTING
    if (sortBySuit == false)
    {
        toggle_sort();
    }

    // FIND RUNS
    // Find runs in player's hand
    for (auto suitToSort : cardSuits)
    {
        find_a_suit_run(suitToSort);
    }

    // RESET SORTING
    if (originalSortingState != sortBySuit)
    {
        toggle_sort();
    }

    return;
}


/*
    Purpose - Print all the sets found in the player's hand
    Input
        playOne - numbers melds if true
    Output - None
 */
void Tong_Its_Player::show_all_sets(bool playOne)
{
    auto tempMeld = make_shared<vector<shared_ptr<PCard>>>();
    vector<string> cardRanks = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
    int tempCount = 0;  // Count for each rank
    bool specialMeld = false;
    bool originalSortingState = sortBySuit;

    // INPUT VALIDATION
    // Existing validation factored out

    // CHECK SORTING
    if (sortBySuit == true)
    {
        toggle_sort();
    }

    // RESET ANY SPECIAL MELDS
    for (auto currCard : (*playersHand))
    {
        currCard->special = false;
    }

    // COUNT RANKS
    for (auto currRank : cardRanks)
    {
        tempCount = 0;  // Reset temp variable
        specialMeld = false;  // Reset temp variable

        for (auto currCard : (*playersHand))
        {
            if (currCard->rank == currRank)
            {
                (*tempMeld).push_back(currCard);                
                ++tempCount;
            }
        }

        // Determine if meld is special
        // 1. There must be four
        if (tempCount == 4)
        {
            specialMeld = true;
            for (auto meldCard : (*tempMeld))
            {
                // 2. There can't be sapaw
                if (meldCard->sapaw)
                {
                    specialMeld = false;
                    break;
                }
            }
            // 3. It's special
            if (specialMeld)
            {
                for (auto meldCard : (*tempMeld))
                {
                    // 4. Make it special
                    meldCard->special = true;
                }
            }            
        }

        // Three, or more, of a kind
        if (tempCount >= 3)
        {
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

    // RESET SORTING
    if (originalSortingState != sortBySuit)
    {
        toggle_sort();
    }

    // cout << "Returning" << endl;  // DEBUGGING
    return;
}


/*
    Purpose - Wrapper method to update playersMelds with possible 'sapaw' plays
    Input
        playOne - numbers melds if true
        players - reference to the Tong_Its_Game vector of players
    Output - None
    Note
        playersMelds will be updated directly
        This method will check each player's exposed melds when looking for sapaw
 */
void Tong_Its_Player::update_potential_sapaw(bool playOne, vector<Tong_Its_Player>& players)
{
    if (sortBySuit == true)
    {
        show_all_sapaw_runs(playOne, players);
        // show_all_sapaw_sets(playOne, players);
    }
    else
    {
        // show_all_sapaw_sets(playOne, players);
        show_all_sapaw_runs(playOne, players);   
    }

    return;
}


/*
    Purpose - Find all of the sapaw for exposed runs among the players
    Input
        playOne - numbers melds if true
        players - reference to the Tong_Its_Game vector of players        
    Output - None
    Note
        playersMelds will be updated directly
        This method will check each player's exposed melds when looking for sapaw
 */
void Tong_Its_Player::show_all_sapaw_runs(bool playOne, vector<Tong_Its_Player>& players)
{
    // cout << "ENTERING SHOW_ALL_SAPAW_RUNS" << endl;  // DEBUGGING
    vector<string> cardSuits = {spadeString, clubString, heartString, diamondString};
    bool originalSortingState = sortBySuit;
    auto tmpExposedMelds = make_shared<vector<shared_ptr<vector<shared_ptr<PCard>>>>>();
    auto tmpMeldVector = make_shared<vector<shared_ptr<PCard>>>();

    // INPUT VALIDATION
    // Existing validation factored out

    // CHECK SORTING
    if (sortBySuit == false)
    {
        toggle_sort();
    }

    // FIND RUNS
    // Iterate through each player
    for (auto player : players)
    {
        // Get the player's exposed melds
        (*tmpExposedMelds) = player.get_exposed_melds();
        // cout << player.get_name() << " has " << player.count_exposed_melds() << " exposed melds." << endl;  // DEBUGGING

        // Iterate through each meld vector pointer
        for (auto meldVec_ptr : (*tmpExposedMelds))
        {
            // If that meld vector has at least one card, let's check it
            if ((*meldVec_ptr).size() > 0)
            {
                // Set
                if ((*meldVec_ptr)[0]->inSet)
                {
                    continue;  // We're not looking for sets here
                }
                // Run
                else
                {
                    // Iterate through each of the player's cards and check it against this meld
                    for (auto playerCard : (*playersHand))
                    {
                        // cout << "LOOKING FOR A SAPAW RUN WITH " << playerCard->rank << playerCard->suit << endl;  // DEBUGGING
                        // Add the playing card to a temp meld
                        (*tmpMeldVector).push_back(playerCard);

                        // Add the exposed meld to the temp meld
                        for (auto meldCard_ptr : (*meldVec_ptr))
                        {
                            (*tmpMeldVector).push_back(meldCard_ptr);
                        }

                        // Sort the temp meld
                        sort_cards(tmpMeldVector, true);

                        // Check if it's a meld
                        if(is_this_a_run(tmpMeldVector))
                        {
                            // cout << "FOUND A SAPAW RUN!" << endl;  // DEBUGGING
                            playerCard->sapaw = true;
                            playersMelds.push_back(make_shared<vector<shared_ptr<PCard>>>());
                            playersMelds.at(playersMelds.size() - 1)->push_back(playerCard);
                            // (*meldVec_ptr).push_back(playerCard);
                            // sort_cards(meldVec_ptr, true);
                            (*tmpMeldVector).clear();
                        }
                    } 
                }
            }
        }
    }

    // RESET SORTING
    if (originalSortingState != sortBySuit)
    {
        toggle_sort();
    }

    return;
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
    bool specialMeld = false;

    // Printing numbers?
    if (meldNum > 0)
    {
        leftBorder = "  ";
        // leftNumber = string(meldNum) + " ";
        switch (meldNum)
        {
            // case(0):
            //     leftNumber = "0 ";
            //     break;
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

    // Is this a special meld?
    if (oneMeld.size() == 4)
    {
        // cout << "The size of this meld is " << oneMeld.size() << endl;  // DEBUGGING
        specialMeld = true;

        for (int i = 1; i < oneMeld.size(); i++)
        {
            if (oneMeld[i]->rank != oneMeld[0]->rank)
            {
                specialMeld = false;
            }
        }
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
        if (specialMeld)
        {
            cout << BORDER_VERTICAL << BORDER_SPACE << specialString << BORDER_SPACE << BORDER_SPACE << BORDER_VERTICAL;
        }
        else
        {
            cout << BORDER_VERTICAL << BORDER_SPACE << cardToPrint->suit << BORDER_SPACE << BORDER_SPACE << BORDER_VERTICAL;
        }
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
    // LOCAL VARIABLES
    // shared_ptr<vector<shared_ptr<PCard>>>
    auto tempHandOfMelds = make_shared<vector<shared_ptr<PCard>>>();

    // BUILD TEMP HAND OF MELDS
    for (auto oneExposedMeld : playersExposedMelds)
    {
        if (oneExposedMeld != nullptr)
        {
            // print_a_meld((*oneExposedMeld), 0);
            for (auto meldCard : (*oneExposedMeld))
            {
                (*tempHandOfMelds).push_back(meldCard);    
            }
            // Add a spacer
            (*tempHandOfMelds).push_back(make_shared<PCard>(" ", " "));
        }
        else
        {
            throw runtime_error("How did a nullptr get into this player's exposed melds?!");
        }
    }

    // Pop last empty card
    (*tempHandOfMelds).pop_back();

    // PRINT TEMP HAND OF MELDS
    print_playing_cards(false, tempHandOfMelds);    

    return;
}


void Tong_Its_Player::sort_players_hand(void)
{
    sort_cards(playersHand, sortBySuit);

    return;
}


/*
    Purpose - Find all runs in the player's hand for a given suit
    Input
        sortThisSuit - string representation of the suit to match for
    Output - None
 */
void Tong_Its_Player::find_a_suit_run(string sortThisSuit)
{   
    // LOCAL VARIABLES
    auto suitMatches = make_shared<vector<shared_ptr<PCard>>>();
    auto tempMeld = make_shared<vector<shared_ptr<PCard>>>();
    vector<string> validSuits = {spadeString, heartString, diamondString, clubString};
    vector<string>::iterator it;

    // INPUT VALIDATION
    it = find(validSuits.begin(), validSuits.end(), sortThisSuit);
    if (it == validSuits.end())
    {
        throw invalid_argument("Tong_Its_Player::find_a_suit_run() was passed an invalid suit!");
    }   

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

    return;
}


void Tong_Its_Player::print_a_row(int rowToPrint, bool printNums, shared_ptr<vector<shared_ptr<PCard>>> printTheseCards)
{
    int numCardsInHand = (*printTheseCards).size();
    int numCardsToPrint = 0;
    int startingCardNum = 0;
    vector<shared_ptr<PCard>> cardsToPrint = vector<shared_ptr<PCard>>();
    string emptyCard = "      ";

    // Input Validation
    if (rowToPrint < 1)
    {
        throw invalid_argument("print_a_row() - Invalid row number to print");
    }
    else if (printTheseCards == nullptr)
    {
        throw invalid_argument("print_a_row() - Received a nullptr");
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
    // cout << "Size of hand: " << (*printTheseCards).size() << endl;  // DEBUGGING
    // cout << "Card 1: Rank " << (*printTheseCards).at(0)->rank << endl;  // DEBUGGING
    // cout << "Card 2: Rank " << (*printTheseCards).at(1)->rank << endl;  // DEBUGGING
    // TEST_the_hand();  // DEBUGGING
    if (numCardsToPrint < numCardsInHand)
    {
        for (int i = ((rowToPrint - 1) * NUM_CARDS_PER_ROW); i < (numCardsToPrint + ((rowToPrint - 1) * NUM_CARDS_PER_ROW)); ++i)
        {
            // cout << "Card # " << i << endl;  // DEBUGGING 
            cardsToPrint.push_back((*printTheseCards).at(i));
        }
    }
    else if (numCardsToPrint == numCardsInHand)
    {
        cardsToPrint = (*printTheseCards);
    }

    // Determine starting card number to print
    startingCardNum = ((rowToPrint - 1) * NUM_CARDS_PER_ROW) + 1;

    // DO NOT DELETE... UNDERSTANDING REQUIRES AN IDE
    // for (auto it = begin(*printTheseCards) + ((rowToPrint - 1) * 4); it < begin(*printTheseCards) + (rowToPrint * 4); ++it)
    // {
    //     cout << "Rank: " << (*it)->rank << endl;  // DEBUGGING
    //     cout << "Suit: " << (*it)->suit << endl;  // DEBUGGING
    // }

    // Print the card pointers
    // Row 0 - Card numbers
    if (printNums)
    {
        for (int i = startingCardNum; i < (startingCardNum + numCardsToPrint); i++)
        {
            if (i > 9)
            {
                cout << BORDER_SPACE << BORDER_SPACE << i << BORDER_SPACE << BORDER_SPACE;
            }
            else
            {
                cout << BORDER_SPACE << BORDER_SPACE << i << BORDER_SPACE << BORDER_SPACE << BORDER_SPACE;
            }
        }
        cout << endl;
    }
    // Row 1
    for (shared_ptr<PCard> cardToPrint : cardsToPrint)
    {
        if (cardToPrint->rank != " " && cardToPrint->suit != " ")
        {
            cout << BORDER_UPPER_LEFT << BORDER_HORIZONTAL << BORDER_HORIZONTAL << BORDER_HORIZONTAL << BORDER_HORIZONTAL << BORDER_UPPER_RIGHT;        
        }
        else
        {
            cout << emptyCard;
        }
    }
    cout << endl;
    // Row 2
    for (shared_ptr<PCard> cardToPrint : cardsToPrint)
    {
        if (cardToPrint->rank == " " && cardToPrint->suit == " ")
        {
            cout << emptyCard;
        }
        else if (cardToPrint->rank == "10")
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
        if (cardToPrint->rank == " " && cardToPrint->suit == " ")
        {
            cout << emptyCard;
        }
        else
        {
            cout << BORDER_VERTICAL << BORDER_SPACE << cardToPrint->suit << BORDER_SPACE << BORDER_SPACE << BORDER_VERTICAL;
        }
    }
    cout << endl;
    // Row 4
    for (shared_ptr<PCard> cardToPrint : cardsToPrint)
    {
        if (cardToPrint->rank == " " && cardToPrint->suit == " ")
        {
            cout << emptyCard;
        }
        else if (cardToPrint->rank == "10")
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
        if (cardToPrint->rank == " " && cardToPrint->suit == " ")
        {
            cout << emptyCard;
        }
        else
        {
            cout << BORDER_LOWER_LEFT << BORDER_HORIZONTAL << BORDER_HORIZONTAL << BORDER_HORIZONTAL << BORDER_HORIZONTAL << BORDER_LOWER_RIGHT;        
        }
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
