#include "Clear_Screen.h"       // clear_screen
#include "Tong_Its.h"
#include "Tong_Its_Playing_Card.h"
#include <locale>               // toupper, locale
#include <string>


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
    numMelds = 0;
    rank = pcRank;
}

/********************/
/* PLAYING CARD END */
/********************/
