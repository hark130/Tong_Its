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
        default:
            throw invalid_argument("Tong_Its_AI_Player::ai_interface() has detected an unsupported strategy");
    }
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

/**************************/
/* TONG ITS AI PLAYER END */
/**************************/
