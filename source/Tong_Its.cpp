// #include "../include/Tong_Its.h"
// #include "include/Tong_Its.h"
#include "Tong_Its.h"
#include <algorithm>    // random_shuffle
// #include <clocale>         // Set locale
// #include <fcntl.h>      // Set mode 16 bit
#include <iostream>
// #include <io.h>         // Set mode 16 bit
#include <locale>
#include <memory>
#include <string>
#include <vector>

#define CONTRAST 10  // 0 for normal, 1 for contrast

#if CONTRAST == 1
#define SPADE ((char16_t) '\u2664')    // ♤
#define HEART ((char16_t) '\u2665')    // ♥
#define DIAMOND ((char16_t) '\u2666')  // ♦
#define CLUB ((char16_t) '\u2667')     // ♧
#elif CONTRAST == 69
#define SPADE L'♤'
#define HEART L'♥'
#define DIAMOND L'♦'
#define CLUB L'♧'
#elif CONTRAST == 10
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
#else
#define SPADE ((char16_t) '\u2660')    // ♠
#define HEART ((char16_t) '\u2661')    // ♡
#define DIAMOND ((char16_t) '\u2662')  // ♢
#define CLUB ((char16_t) '\u2663')     // ♣
#endif // Conditional suit values

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
    if (pcSuit == spadeString || pcSuit == heartString || pcSuit == diamondString || pcSuit == clubString)
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
    }
    else
    {
        // Raise exception
    }

    if (pcRank == "A")
    {
        value = 1;
    }
    else if (pcRank == "2")
    {
        value = 2;
    }
    else if (pcRank == "3")
    {
        value = 3;
    }
    else if (pcRank == "4")
    {
        value = 4;
    }
    else if (pcRank == "5")
    {
        value = 5;
    }
    else if (pcRank == "6")
    {
        value = 6;
    }
    else if (pcRank == "7")
    {
        value = 7;
    }
    else if (pcRank == "8")
    {
        value = 8;
    }
    else if (pcRank == "9")
    {
        value = 9;
    }
    else if (pcRank == "10")
    {
        value = 10;
    }
    else if (pcRank == "J" || pcRank == "j")
    {
        value = 11;
    }
    else if (pcRank == "Q" || pcRank == "q")
    {
        value = 12;
    }
    else if (pcRank == "K" || pcRank == "k")
    {
        value = 13;
    }
    else
    {
        // Raise exception
    }

    rank = pcRank;
}

/***********************/
/* TONG ITS PLAYER END */
/***********************/


/*************************/
/* TONG ITS PLAYER BEGIN */
/*************************/
Tong_Its_Player::Tong_Its_Player(string playerName)
{
    name = playerName;
    numOfChips = 100;
}

string Tong_Its_Player::get_name(void)
{
    return name;
}

int Tong_Its_Player::count_chips(void)
{
    return numOfChips;
}

/***********************/
/* TONG ITS PLAYER END */
/***********************/


/***********************/
/* TONG ITS GAME BEGIN */
/***********************/
Tong_Its_Game::Tong_Its_Game(const shared_ptr<string>& humanPlayerName) : \
player1(*humanPlayerName), player2(string("Player2")), player3(string("Player3")) //, drawPile(build_a_deck()))
{
    // player1 = Tong_Its_Player::Tong_Its_Player(*humanPlayerName);
    // player2 = Tong_Its_Player(string("Player 2"));
    // player3 = Tong_Its_Player(string("Player 3"));
    // cout << "Function call to build_a_deck()" << endl;  // DEBUGGING
    drawPile = build_a_deck();
    // cout << "build_a_deck() made " << drawPile << endl;  // DEBUGGING
    shuffle_a_deck(drawPile);
}


void Tong_Its_Game::TEST_the_deck(void)
{
    // for (shared_ptr<PCard> card : drawPile)
    // {
    //     cout << "Rank: " << card->rank;
    //     cout << "Suit: " << card->suit;
    //     cout << "Value: " << card->value << endl;
    // }

    // cout << "Rank: " << *drawPile.begin()->rank;
    // cout << "Suit: " << *drawPile.begin()->suit;
    // cout << "Value: " << *drawPile.begin()->value << endl;

    cout << "Testing the deck" << endl;
    if (drawPile)
    {
        cout << "Draw pile size: " << (*drawPile).size() << endl;
    }
    else
    {
        cout << "Draw pile is empty!" << endl;
    }        

    // setlocale(LC_ALL, "");
    for (int i = 0; i < (*drawPile).size(); ++i)
    {
        cout << "Card # " << i + 1 << endl;
        cout << "Rank: " << (*drawPile)[i]->rank << endl;
        cout << "Suit: " << (*drawPile)[i]->suit << endl;
        // wcout << L"Suit: " << (*drawPile)[i]->suit << endl;
        cout << "Value: " << (*drawPile)[i]->value << endl;
    }

    return;
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
    random_shuffle((*deckOfCards).begin(), (*deckOfCards).end());

    return;
}
/*********************/
/* TONG ITS GAME END */
/*********************/
