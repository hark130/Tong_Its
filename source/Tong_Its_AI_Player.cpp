#include "Tong_Its_AI_Player.h"
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

void Tong_Its_AI_Player::ai_interface(Tong_Its_Game& theGame)
{
    switch (strategy)
    {
        case (1):
            ai_random_interface(theGame);
            break;
        default:
            throw invalid_argument("Tong_Its_AI_Player::ai_interface() has detected an unsupported strategy");
    }
}

/***** PRIVATE METHODS *****/

void Tong_Its_AI_Player::ai_random_interface(Tong_Its_Game& theGame)
{
    // LOCAL VARIABLES
    shared_ptr<PCard> tmpCard = nullptr;  // Temp playing card holder
    int tmpInt = 0;  // Random number holder

    // Draw
    if (count_cards() < 13)
    {
        tmpCard = theGame.card_is_drawn();
        if (tmpCard == nullptr)
        {
            throw runtime_error("Tong_Its_AI_Player::ai_interface() received a nullptr");
        }
        receive_a_card(tmpCard);
        tmpCard = nullptr;
    }
    // Discard
    tmpInt = random_num(1, count_cards());
    tmpCard = play_a_card(tmpInt);
    theGame.receive_a_discard(tmpCard);

    // DONE
    return;
}

/**************************/
/* TONG ITS AI PLAYER END */
/**************************/
