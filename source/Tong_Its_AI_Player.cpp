#include "Tong_Its_AI_Player.h"
#include "Tong_Its.h"
#include "Tong_Its_Game.h"
#include "Tong_Its_Player.h"
#include "Tong_Its_Playing_Card.h"
#include <iostream>


/****************************/
/* TONG ITS AI PLAYER BEGIN */
/****************************/

Tong_Its_AI_Player::Tong_Its_AI_Player(string playerName, int stratNum) : Tong_Its_Player(playerName)
{
    // cout << "TONG ITS AI PLAYER CTOR" << endl;  // DEBUGGING
	// TIP
    // Formerly initialized in the list
    // name = playerName;
    numOfChips = 100;
    playersHand = make_shared<vector<shared_ptr<PCard>>>();
    // Normal initialization
    numOfCards = 0;
    sortBySuit = true;
    calledTongits = false;
    calledDraw = false;
    challengedDraw = false;
    burned = false;
    finalScore = 0;
    open = false;

    // TIAP
    // strategy
    switch (stratNum)
    {
        case (1):
            strategy = stratNum;
            strategyDesc = string("Entirely random discards.  This AI will *never* win because it will never expose a meld");
            break;
        case (2):
            strategy = stratNum;
            strategyDesc = string("Entirely random discards.  This AI will expose random melds if it has them but contains no strategy.");
            break;
        default:
            throw invalid_argument("Tong_Its_AI_Player ctor does not support this strategy");
    }
}

/***** PUBLIC METHODS *****/

void Tong_Its_AI_Player::ai_interface(Tong_Its_Game* theGame_ptr)
{
    // INPUT VALIDATION
    if (theGame_ptr == nullptr)
    {
        throw invalid_argument("Tong_Its_AI_Player::ai_interface() received a nullptr");
    }

    switch (strategy)
    {
        case (1):
            ai_random_interface(theGame_ptr);
            break;
        case (2):
            ai_almost_random_interface(theGame_ptr);
            break;
        default:
            throw invalid_argument("Tong_Its_AI_Player::ai_interface() has detected an unsupported strategy");
    }

    // /* DEBUGGING */
    // for (auto meldVec_ptr : playersExposedMelds)
    // {        
    //     cout << "PRINTING " << get_name() << "'s EXPOSED MELD (inside ai_interface()):" << endl;  // DEBUGGING
    //     print_a_meld(*meldVec_ptr, 0);
    // }

    return;
}

/***** PRIVATE METHODS *****/

void Tong_Its_AI_Player::ai_random_interface(Tong_Its_Game* theGame_ptr)
{
    // LOCAL VARIABLES
    shared_ptr<PCard> tmpCard = nullptr;  // Temp playing card holder
    int tmpInt = 0;  // Random number holder
    string tmpLogEntry = "";  // Reusable string to build log entries

    // INPUT VALIDATION
    if (theGame_ptr == nullptr)
    {
        throw invalid_argument("Tong_Its_AI_Player::ai_interface() received a nullptr");
    }

    // Draw
    if (hand_size() < 13)
    {
        tmpCard = theGame_ptr->card_is_drawn();

        if (tmpCard == nullptr)
        {
            throw runtime_error("Tong_Its_AI_Player::ai_interface() received a nullptr");
        }
        else
        {
            // Log it
            tmpLogEntry = get_name() + " drew a card from the draw pile.";
            theGame_ptr->log_an_entry(tmpLogEntry);
            tmpLogEntry = "";
        }

        receive_a_card(tmpCard);
        tmpCard = nullptr;
    }
    // Discard
    // cout << get_name() << endl;  // DEBUGGING
    // cout << "Hand size: " << hand_size() << endl;  // DEBUGGING
    tmpInt = random_num(1, hand_size());
    // cout << "Random number: " << tmpInt << endl;  // DEBUGGING
    tmpCard = play_a_card(tmpInt);
    // cout << "Card number: " << tmpCard << endl;  // DEBUGGING
    theGame_ptr->receive_a_discard(tmpCard);

    // Log it
    tmpLogEntry = get_name() + " discarded a " + tmpCard->rank + tmpCard->suit;
    theGame_ptr->log_an_entry(tmpLogEntry);
    tmpLogEntry = "";

    // DONE
    return;
}

void Tong_Its_AI_Player::ai_almost_random_interface(Tong_Its_Game* theGame_ptr)
{
    // LOCAL VARIABLES
    shared_ptr<PCard> tmpCard = nullptr;  // Temp playing card holder
    int tmpInt = 0;  // Random number holder
    string tmpLogEntry = "";  // Reusable string to build log entries

    // INPUT VALIDATION
    if (theGame_ptr == nullptr)
    {
        throw invalid_argument("Tong_Its_AI_Player::ai_interface() received a nullptr");
    }

    // 1. Draw
    if (hand_size() < 13)
    {
        tmpCard = theGame_ptr->card_is_drawn();

        if (tmpCard == nullptr)
        {
            throw runtime_error("Tong_Its_AI_Player::ai_interface() received a nullptr");
        }
        else
        {
            // Log it
            tmpLogEntry = get_name() + " drew a card from the draw pile.";
            theGame_ptr->log_an_entry(tmpLogEntry);
            tmpLogEntry = "";
        }

        receive_a_card(tmpCard);
        tmpCard = nullptr;
    }

    // 2. Expose a Meld
    // 2.1. Update melds
    update_potential_melds(true, theGame_ptr->players);
    // 2.2. Have any potential melds?
    tmpInt = count_potential_melds(theGame_ptr->players);
    if (tmpInt > 0)
    {
        // theGame_ptr->log_an_entry(string(get_name() + " HAS " + to_string(tmpInt) + " POTENTIAL MELDS!"));  // DEBUGGING
        tmpInt = random_num(1, tmpInt);
        // theGame_ptr->log_an_entry(string(get_name() + " IS EXPOSING MELD # " + to_string(tmpInt) + "!"));  // DEBUGGING
        if (!expose_a_meld(tmpInt, theGame_ptr->players, true))
        {
            tmpLogEntry = get_name() + " attempted to expose a meld but it failed!";
            theGame_ptr->log_an_entry(tmpLogEntry);
            tmpLogEntry = "";
        }
    }

    // 3. Discard
    // cout << get_name() << endl;  // DEBUGGING
    // cout << "Hand size: " << hand_size() << endl;  // DEBUGGING
    tmpInt = random_num(1, hand_size());
    // cout << "Random number: " << tmpInt << endl;  // DEBUGGING
    tmpCard = play_a_card(tmpInt);
    // cout << "Card number: " << tmpCard << endl;  // DEBUGGING
    theGame_ptr->receive_a_discard(tmpCard);

    // Log it
    tmpLogEntry = get_name() + " discarded a " + tmpCard->rank + tmpCard->suit;
    theGame_ptr->log_an_entry(tmpLogEntry);
    tmpLogEntry = "";

    // /* DEBUGGING */
    // for (auto meldVec_ptr : playersExposedMelds)
    // {        
    //     cout << "PRINTING " << get_name() << "'s EXPOSED MELD (inside ai_almost_random_interface()):" << endl;  // DEBUGGING
    //     print_a_meld(*meldVec_ptr, 0);
    // }

    // DONE
    return;
}

/**************************/
/* TONG ITS AI PLAYER END */
/**************************/
