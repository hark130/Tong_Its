// #include "../include/Tong_Its.h"
// #include "include/Tong_Its.h"
#include "Tong_Its.h"
#include <algorithm>    // random_shuffle
// #include <clocale>         // Set locale
#include <cstdlib>      // srand
// #include <fcntl.h>      // Set mode 16 bit
#include <iostream>
// #include <io.h>         // Set mode 16 bit
#include <locale>
#include <memory>
#include <regex>
#include <string>
#include <ctime>         // time
#include <vector>

#define CLEAR_SCREEN 30     // Number of newlines to print to clear the screen 

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
    }
    else
    {
        // Raise exception
        throw invalid_argument("Invalid Playing Card suit");
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
    else if (pcRank == " ")
    {
        value = 0;
    }
    else
    {
        // Raise exception
        throw invalid_argument("Invalid Playing Card rank");
    }

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
    sortBySuit = true;
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
        cout << "Value: " << (*playersHand)[i]->value << endl;
    }

    return;
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
    int retVal = 0;

    retVal = (*playersHand).size();

    return retVal;
}


void Tong_Its_Player::receive_a_card(shared_ptr<PCard> drawnCard)
{
    if (drawnCard)
    {
        (*playersHand).push_back(drawnCard);
        sort_players_hand();
        ++numOfCards;
    }
    else
    {
        throw invalid_argument("NULL card pointer");
        // throw exception("NULL card pointer");
    }

    return;
}


shared_ptr<PCard> Tong_Its_Player::play_a_card(int cardNumber)
{
    auto retVal = make_shared<PCard>(" ", " ");

    // Input Validation
    if (cardNumber < 1)
    {
        throw invalid_argument("Tong_Its_Player::play_a_card() received invalid card number");
    }
    else if (cardNumber > numOfCards)
    {
        throw invalid_argument("Tong_Its_Player::play_a_card() received non-existent card number");
    }
    else
    {
        auto cardPos = (*playersHand).begin() + cardNumber - 1;
        retVal = (*playersHand).at(cardNumber - 1);
        // (*playersHand).erase(playersHand.begin() + cardNumber - 1);
        (*playersHand).erase(cardPos);
    }

    return retVal;
}


void Tong_Its_Player::print_players_hand(void)
{
    cout << "print_players_hand() not yet implemented!" << endl;

    return;
}


void Tong_Its_Player::sort_players_hand(void)
{
    cout << "sort_players_hand() not yet implemented!" << endl;

    return;
}
/***********************/
/* TONG ITS PLAYER END */
/***********************/


/***********************/
/* TONG ITS GAME BEGIN */
/***********************/
// Tong_Its_Game::Tong_Its_Game(shared_ptr<string> humanPlayerName) : 
Tong_Its_Game::Tong_Its_Game(const shared_ptr<string>& humanPlayerName) : 
player1(*humanPlayerName), player2(string("Mike")), player3(string("Eren"))
// player1(*humanPlayerName), player2(string("Player2")), player3(string("Player3"))
// player1("Human"), player2("Player2"), player3("Player3")
// player1(*humanPlayerName), player2(*humanPlayerName), player3(*humanPlayerName)

{
    // 1. Build the deck of cards
    // cout << "Function call to build_a_deck()" << endl;  // DEBUGGING
    drawPile = build_a_deck();
    // cout << "build_a_deck() made " << drawPile << endl;  // DEBUGGING

    // 2. Shuffle the deck of cards
    // cout << "Function call to shuffle_a_deck()" << endl;  // DEBUGGING
    shuffle_a_deck(drawPile);

    // 3. Create the discard pile
    discardPile = make_shared<vector<shared_ptr<PCard>>>();

    // 4. Set the current dealer to Player 1
    // currentDealer = 1;

    // 5. Deal hands to each player
    // cout << "Function call to deal_player_hands()" << endl;  // DEBUGGING
    deal_player_hands(player1);
    // print_a_card((*drawPile).at(0));  // Accomplished in user_interfact() now

    // 6. Set current player
    currentPlayer = 1;  // NOTE: Uncessary when currentDealer is (re)implemented?
}


void Tong_Its_Game::start_the_game(void)
{
    user_interface();

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
        cout << "Value: " << (*deckToTest)[i]->value << endl;
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
    srand(unsigned(time(NULL)));

    random_shuffle((*deckOfCards).begin(), (*deckOfCards).end());

    return;
}


/*
    Input
        cardNumber - 1 through N, human readable card number
        source - shared pointer to a vector of card pointers to take from
        destination - shared pointer to a vector of card pointers to give to
    Output - None
    Purpose - Take a given card number from a deck and place that card in another deck
 */
void Tong_Its_Game::move_one_card(int cardNumber, shared_ptr<vector<shared_ptr<PCard>>> source, \
                                  shared_ptr<vector<shared_ptr<PCard>>> destination)
{
    if (source == nullptr)
    {
        throw runtime_error("Source deck is NULL");
    }
    else if (destination == nullptr)
    {
        throw runtime_error("Destination deck is NULL");
    }
    else if (cardNumber < 1)
    {
        throw range_error("Invalid card number");
    }
    else if (cardNumber > (*source).size())
    {
        throw out_of_range("This card number does not exist in the source deck");
    }

    // 1. Get the source vector
    auto tempVector = (*source);
    // TEST_the_deck(source);  // DEBUGGING

    // 2. Get the card from the source vector
    // auto tempCard = tempVector.at(cardNumber - 1)
    shared_ptr<PCard> tempCard = tempVector.at(cardNumber - 1);
    // cout << "Temp Card Rank: " << tempCard->rank << endl;  // DEBUGGING
    // auto tempCard = (*source).at(cardNumber - 1)

    // 3. Erase the card from source
    // tempVector.erase(tempVector.begin() + cardNumber - 1);
    // (*source).erase((*source).begin() + cardNumber - 1);

    // 4. Insert the card into destination
    // tempVector = (*destination);
    // (*destination).push_back(tempCard);
    // tempVector.emplace_back(tempCard);
    // tempVector.push_back(tempCard);
    (*destination).push_back(tempCard);


    return;
}


/*
    Note - 13 cards to the dealer, 12 cards to the other players
 */
void Tong_Its_Game::deal_player_hands(Tong_Its_Player currentDealer)
{
    auto tempDeck = make_shared<vector<shared_ptr<PCard>>>();
    // auto tempCard = make_shared<PCard>();


    // One to the dealer   
    move_one_card(1, drawPile, tempDeck);
    // cout << (*tempDeck).size();  // DEBUGGING
    auto tempCard = (*tempDeck).at(0);
    // cout << "Dealer!\n";  // DEBUGGING
    currentDealer.receive_a_card(tempCard);
    (*tempDeck).erase((*tempDeck).begin());

    // 12 to everyone
    for (int i = 0; i < 12; ++i)
    {
        move_one_card(1, drawPile, tempDeck);
        tempCard = (*tempDeck).at(0);
        player1.receive_a_card(tempCard);
        // cout << "Player 1";  // DEBUGGING
        (*tempDeck).pop_back();

        move_one_card(1, drawPile, tempDeck);
        tempCard = (*tempDeck).at(0);
        player2.receive_a_card(tempCard);
        // cout << "Player 2";  // DEBUGGING
        (*tempDeck).pop_back();

        move_one_card(1, drawPile, tempDeck);
        tempCard = (*tempDeck).at(0);
        player3.receive_a_card(tempCard);
        // cout << "Player 3";  // DEBUGGING
        (*tempDeck).pop_back();
    }

    return;
}


void Tong_Its_Game::print_a_card(shared_ptr<PCard> cardToPrint)
{
    if (cardToPrint)
    {
        cout << BORDER_UPPER_LEFT << BORDER_HORIZONTAL << BORDER_HORIZONTAL << BORDER_HORIZONTAL << BORDER_HORIZONTAL << BORDER_UPPER_RIGHT << endl;
        cout << BORDER_VERTICAL << cardToPrint->rank << BORDER_SPACE << BORDER_SPACE << BORDER_SPACE << BORDER_VERTICAL << endl;
        cout << BORDER_VERTICAL << BORDER_SPACE << cardToPrint->suit << BORDER_SPACE << BORDER_SPACE << BORDER_VERTICAL << endl;
        cout << BORDER_VERTICAL << BORDER_SPACE << BORDER_SPACE << BORDER_SPACE << cardToPrint->rank << BORDER_VERTICAL << endl;
        cout << BORDER_LOWER_LEFT << BORDER_HORIZONTAL << BORDER_HORIZONTAL << BORDER_HORIZONTAL << BORDER_HORIZONTAL << BORDER_LOWER_RIGHT << endl;
    }
    else
    {
        throw invalid_argument("NULL card pointer");
    }

    return;
}


void Tong_Its_Game::user_interface(void)
{
    auto tempCard = make_shared<PCard>(" ", " ");
    string dynamicChoice1opt1 = string("Draw a card");
    string dynamicChoice1opt2 = string("Discard a card");
    string dynamicChoice1 = dynamicChoice1opt1;
    string dynamicChoice2 = string("Reprint game state");
    string dynamicChoice3 = string("Toggle hand sort ");
    string dynamicChoice4 = string("Show sets in hand");
    string dynamicChoice9 = string("Exit");
    int menuChoice = 0;
    int subMenuChoice = 0;

    game_state();

    while(menuChoice != 999)
    {
        // Reset temp variables
        menuChoice = 0;
        subMenuChoice = 0;

        // Print interface
        if (player1.count_cards() == 13)
        {
            dynamicChoice1 = dynamicChoice1opt2;
        }
        cout << "1. " << dynamicChoice1 << endl;
        cout << "2. " << dynamicChoice2 << endl;
        cout << "3. " << dynamicChoice3 << endl;
        cout << "4. " << dynamicChoice4 << endl;
        cout << "999. " << dynamicChoice9 << endl;

        // Take user input
        menuChoice = input_number();
        // cin >> menuChoice;

        switch (menuChoice)
        {
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
                    
                    // Toggle menu choice
                    dynamicChoice1 = dynamicChoice1opt2;
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
                    if (subMenuChoice < 1 || subMenuChoice > player1.count_cards())
                    {
                        // Try again
                        cout << "Invalid card number.\n" << "Please choose again." << endl;
                        break; 
                    }
                    else
                    {
                        // Remove the card
                        // Player removes the card from his hand
                        tempCard = player1.play_a_card(subMenuChoice);
                        // Removed card is added to the discard pile
                        receive_a_discard(tempCard);
                    }
                }
                else
                {
                    throw "Menu option 1 was corrupted!";
                }
                break;
            case 2:
                for (int i = 0; i < CLEAR_SCREEN; ++i)
                {
                    cout << "\n";
                }
                game_state();
                player1.print_players_hand();
                break;
            case 3:
                // Behavior
                // break;
            case 4:
                // Behavior
                // break;
            case 999:
                cout << "Exiting game" << endl;
                break;
            default:
                cout << "Invalid selection\n" << "Please choose again" << endl;
                break;
        }
    }

    return;
}


void Tong_Its_Game::game_state(void)
{
    // Blank card
    shared_ptr<PCard> blankCard = make_shared<PCard>(PCard(" ", " "));
    // Print player status
    cout << "Player 1: " << player1.get_name() << " has " << player1.count_cards() << " cards and " << player1.count_chips() << " chips." << endl;
    cout << "Player 2: " << player2.get_name() << " has " << player2.count_cards() << " cards and " << player2.count_chips() << " chips." << endl;
    cout << "Player 3: " << player3.get_name() << " has " << player3.count_cards() << " cards and " << player3.count_chips() << " chips." << endl;
    cout << "\n";

    // Print discard pile
    cout << "DISCARD PILE" << endl;
    if ((*discardPile).size() > 0)
    {
        print_a_card((*discardPile).back());
    }
    else  // Game just began
    {
        print_a_card(blankCard);
    }
    cout << "\n";

    return;
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
