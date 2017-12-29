#include "Clear_Screen.h"       // clear_screen
#include "Tong_Its.h"
#include "Tong_Its_Playing_Card.h"
#include <locale>               // toupper, locale
#include <string>               // stoi


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

    sapaw = false;
    special = false;
    numMelds = 0;
    inRun = false;
    inSet = false;
    rank = pcRank;
}


/*
    Purpose - Validate a single playing card
    Input - None
    Output - True if valid, false if invalid
    Note - 
        Minimize calling this method:
            TIG::build a deck, TIG::reset_game, TIP::validate_meld
        This method returns false for 'blank' PCards (still debating otherwise)

 */
bool Playing_Card::validate_playing_card(void)
{
    // LOCAL VARIABLES
    bool retVal = true;
    vector<string> validRanks = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
    vector<string> validSuits = {spadeString, heartString, diamondString, clubString};
    vector<string>::iterator it;

    // VALIDATE PCARD
    // Verify valid rank
    if (retVal)
    {
        it = find(validRanks.begin(), validRanks.end(), rank);
        if (it == validRanks.end())
        {
            retVal = false;
        }
    }

    // Verify valid suit
    if (retVal)
    {
        it = find(validSuits.begin(), validSuits.end(), suit);
        if (it == validSuits.end())
        {
            retVal = false;
        }
    }

    // Check rank against rankValue
    if (retVal)
    {
        // Verify rankValue bounds
        if (rankValue > 13 || rankValue < 1)
        {
            retVal = false;
        }
        // Use rankValue to compare validRanks index to the rank
        else if (validRanks[rankValue - 1] != rank)
        {
            retVal = false;
        }
    }

    // Check suit against suitValue
    if (retVal)
    {
        // Verify suitValue bounds
        if (suitValue > 4 || suitValue < 1)
        {
            retVal = false;
        }
        // Use rankValue to compare validSuits index to the suit
        else if (validSuits[suitValue - 1] != suit)
        {
            retVal = false;
        }
    }

    // Check rank against pointValue
    if (retVal)
    {
        // Verify bounds
        if (pointValue > 10 || pointValue < 1)
        {
            retVal = false;
        }
        //  1 - 9
        else if (rankValue < 10 && rankValue != pointValue)
        {
            retVal = false;
        }
        // 10 - K
        else if ((rank == "10" || rank == "J" || rank == "Q" || rank == "K") && pointValue != 10)
        {
            retVal = false;
        }
    }

    // Verify (((inRun || inSet) || (!inRun && !inSet)) && !(inRun && inSet))
    if (retVal)
    {
        // inRun or inSet, but not both, *must* be true
        if ((!inRun && !inSet) || (inRun && inSet))
        {
            retVal = false;
        }
    }

    // DONE
    return retVal;
}
/********************/
/* PLAYING CARD END */
/********************/
