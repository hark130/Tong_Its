#include "Clear_Screen.h"       // clear_screen
#include "Tong_Its.h"
#include "Tong_Its_Game.h"
#include "Tong_Its_Playing_Card.h"
#include "Tong_Its_Player.h"
#include <algorithm>            // random_shuffle && sort 
#include <climits>              // INT_MAX
#include <cstdlib>              // srand
#include <ctime>                // time 
#include <iostream>
#include <memory>
#include <string>
#include <vector>


/***********************/
/* TONG ITS GAME BEGIN */
/***********************/

// Tong_Its_Game::Tong_Its_Game(shared_ptr<string> humanPlayerName) : 
Tong_Its_Game::Tong_Its_Game(const shared_ptr<string>& humanPlayerName) : 
players({*humanPlayerName, string("Mike"), string("Eren")})
{
    // 0. Seed the random number generator
    srand(unsigned(time(NULL)));

    // 1. Build the deck of cards
    // cout << "Function call to build_a_deck()" << endl;  // DEBUGGING
    drawPile = build_a_deck();
    // cout << "build_a_deck() made " << drawPile << endl;  // DEBUGGING

    // 2. Shuffle the deck of cards
    // cout << "Function call to shuffle_a_deck()" << endl;  // DEBUGGING
    // shuffle_a_deck(drawPile);  // COMMENTED FOR TESTING

    // 3. Create the discard pile
    discardPile = make_shared<vector<shared_ptr<PCard>>>();

    // 4. Set the current dealer to Player 1
    // currentDealer = 1;

    // 5. Set current player
    currentPlayer = 1;  // NOTE: Uncessary when currentDealer is (re)implemented?

    // 6. Deal hands to each player
    // cout << "Function call to deal_player_hands()" << endl;  // DEBUGGING
    deal_player_hands(players[0]);
    // cout << "Ctor - Current player: " << currentPlayer << endl;  // DEBUGGING
}


void Tong_Its_Game::start_the_game(void)
{
    // LOCAL VARIABLES
    int gameOver = 0;               // Return value from user_interface()
    int stopPlaying = 1;            // User choice to stop playing or continue
    int winningPlayerNumber = 0;
    int tmpLoss = 0;                // Temp variable which stores a player's chip loss
    int playerNumber = 0;           // Temp variable tracking player number

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
            // IMPLEMENT THIS LATER!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        }
        else
        {
            throw runtime_error("start_the_game() - currentPlayer appears to be broken");
        }

        // 2. Has anyone won?
        if (is_the_game_over())
        {
            // cout << "Draw pile size:\t" << (*drawPile).size() << endl;  // DEBUGGING
            // 2.1. Scoring
            winningPlayerNumber = score_the_game();

            // 2.2. Resolve chips
            for (int i = 0; i < players.size(); i++)
            {
                if (i == winningPlayerNumber - 1)
                {
                    continue;
                }
                // 2.2.1. Calculate loss
                tmpLoss = calc_chip_loss(players[winningPlayerNumber - 1], players[i]);
                // 2.2.2. Take away loser's chips
                tmpLoss = players[i].lose_chips(tmpLoss);
                // 2.2.3. Give those chips to the winner
                players[winningPlayerNumber - 1].win_chips(tmpLoss);
            }

            // 2.3. Print results
            for (auto player : players)
            {
                playerNumber++;
                cout << "Player " << playerNumber << ": " << player.get_name() << " has " << player.count_chips() << " chips." << endl;
            }

            // 2.4. Continue playing?
            cout << "Would you like to continue playing?  Enter '1'.  Any other positive number will exit the game." << endl;
            stopPlaying = input_number();
            if (stopPlaying == 1)
            {
                // 2.4.1. Reset game (which resets players)
                // 2.4.1.1. Reset local variables
                gameOver = 0;
                stopPlaying = 1;
                // winningPlayerNumber = 0;
                tmpLoss = 0;
                playerNumber = 0;
                // 2.4.1.2. Reset players
                for (int i = 0; i < players.size(); i++)
                // for (auto player : players)
                {
                    cout << "Pre-Reset:\t" << players[i].get_name() << " has " << players[i].count_cards() << " in his hand and " << players[i].count_exposed_melds() << " exposed melds." << endl;  // DEBUGGING
                    // player.reset(this);
                    players[i].reset(this);
                    cout << "Post-Reset:\t" << players[i].get_name() << " has " << players[i].count_cards() << " in his hand and " << players[i].count_exposed_melds() << " exposed melds." << endl;  // DEBUGGING
                }
                // 2.4.1.3. Reset game state
                reset_game(winningPlayerNumber);
                // 2.4.1.4. Reset final local variable
                winningPlayerNumber = 0;
            }
            else
            {
                cout << "'Game over man. Game over!' -Pvt. Hudson, Aliens" << endl;  // DEBUGGING
                break;
            }
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
    // cout << "Draw pile size:\t" << (*drawPile).size() << endl;  // DEBUGGING
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
                if (tempCard->validate_playing_card())
                {
                    (*retVal).emplace_back(tempCard);
                }
                else
                {
                    throw runtime_error("Tong_Its_Game::build_a_deck() encountered an invalid playing card!");
                }
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

    string dynamicChoice6 = string("TONGITS!");
    string dynamicChoice7 = string("Call Draw");
    string dynamicChoice9 = string("Exit");
    int menuChoice = 0;
    int subMenuChoice = 0;
    bool isTurnOver = false;
    bool eligibleToCallDraw = true;
    bool originalSapawState = players[0].na_sapaw_ako();  // Holds the current sapaw state in case a method fails
    bool playerSapawHimself = false;

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
        if (eligibleToCallDraw && players[0].already_open() && !(players[0].na_sapaw_ako()))
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

                    // Input Validation
                    if (subMenuChoice < 1 || subMenuChoice > 2)
                    {
                        // Try again
                        cout << "Invalid card number.\n" << "Please choose again." << endl;
                        break;
                    }
                    else if (subMenuChoice == 1)
                    {
                        tempCard = card_is_drawn();
                        players[0].receive_a_card(tempCard);
                    }
                    else if (subMenuChoice == 2)
                    {
                        cout << "DRAWING FROM DISCARD" << endl;  // DEBUGGING
                        tempCard = discard_is_taken();
                        players[0].receive_a_card(tempCard);
                        // Validate that tempCard makes a meld
                        if (players[0].card_can_meld(tempCard, players))
                        {
                            cout << "IT CAN MELD" << endl;  // DEBUGGING
                            // Force player to expose melds until the drawn discard is not longer in their hand
                            while((players[0].get_card_number(tempCard)) != 0)
                            {
                                cout << "IT'S STILL IN YOUR HAND!" << endl;  // DEBUGGING
                                // Clear the sapaw state
                                players[0].wala_nang_sapaw();
                                
                                // Print
                                players[0].show_all_melds(true, players);

                                // Prompt
                                cout << "You must now play the drawn discard." << endl;
                                cout << "Enter the number of the meld you would like to expose: " << endl;
                                
                                // Input
                                subMenuChoice = input_number();

                                // Input Validation
                                if (subMenuChoice < 1 || subMenuChoice > players[0].count_potential_melds(players))
                                {
                                    // Try again
                                    cout << "Invalid meld number.\n" << "Please choose again." << endl;
                                    // Reset sapaw state (which was cleared earlier)
                                    if (originalSapawState)
                                    {
                                        players[0].na_sapaw_ka();
                                    }
                                    break;
                                }
                                else
                                {
                                    if (!(players[0].expose_a_meld(subMenuChoice, players)))
                                    {
                                        cout << "There was a problem exposing your meld.\n" << endl;
                                        // Reset sapaw state (which was cleared earlier)
                                        if (originalSapawState)
                                        {
                                            players[0].na_sapaw_ka();
                                        }
                                    }
                                    else
                                    {
                                        // No longer eligible to call Draw
                                        eligibleToCallDraw = false;
                                        playerSapawHimself = players[0].na_sapaw_ako();
                                        // game_state();
                                    }
                                }
                            }
                            // Reset temp variable
                            subMenuChoice = 2;
                            game_state();
                        }
                        else
                        {
                            // Chastise the user
                            cout << "Invalid draw.\n" << "This card can not meld." << endl;
                            
                            // Take the card from the player's hand
                            if (tempCard != players[0].play_a_card(players[0].get_card_number(tempCard)))
                            {
                                throw runtime_error("user_interface() - Unable to recover invalid discard draw");
                            }

                            // Place the card back in the discards
                            receive_a_discard(tempCard);

                            // Continue the game
                            continue;
                        }
                    }
                    else
                    {
                        throw range_error("user_interface() - Menu item #1 sub-selection is invalid");
                    }
                    
                    // Toggle menu choice
                    dynamicChoice1 = dynamicChoice1opt2;

                    // No longer eligible to call Draw
                    eligibleToCallDraw = false;

                    // Reprint the state of the game
                    game_state();

                    // Print the card you just drew
                    if (subMenuChoice == 1)
                    {                        
                        // print_a_card(tempCard);
                        cout << "You just drew a " << tempCard->rank << tempCard->suit << "\n" << endl;
                    }
                    tempCard = nullptr;
                }
                // Discard a card
                else if (dynamicChoice1 == dynamicChoice1opt2)
                {
                    // Prompt
                    cout << "Enter the number of the card you would like to discard: " << endl;
                    
                    // Input
                    subMenuChoice = input_number();

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
                if (players[0].count_potential_melds(players) > 0)
                {                        
                    // Clear the sapaw state
                    players[0].wala_nang_sapaw();
                    
                    // Print
                    players[0].show_all_melds(true, players);

                    // Prompt
                    cout << "Enter the number of the meld you would like to expose: " << endl;
                    
                    // Input
                    subMenuChoice = input_number();

                    // Input Validation
                    if (subMenuChoice < 1 || subMenuChoice > players[0].count_potential_melds(players))
                    {
                        // Try again
                        cout << "Invalid meld number.\n" << "Please choose again." << endl;
                        // Reset sapaw state (which was cleared earlier)
                        if (originalSapawState)
                        {
                            players[0].na_sapaw_ka();
                        }
                        break;
                    }
                    else
                    {
                        if (!(players[0].expose_a_meld(subMenuChoice, players)))
                        {
                            cout << "There was a problem exposing your meld.\n" << endl;
                            // Reset sapaw state (which was cleared earlier)
                            if (originalSapawState)
                            {
                                players[0].na_sapaw_ka();
                            }
                        }
                        else
                        {
                            // No longer eligible to call Draw
                            eligibleToCallDraw = false;
                            playerSapawHimself = players[0].na_sapaw_ako();
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
                players[0].show_all_melds(false, players);
                break;
            // 4. REPRINT GAME STATE
            case 4:
                clear_the_screen();
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
                if (eligibleToCallDraw && players[0].already_open() && !(players[0].na_sapaw_ako()))
                {
                    // 1. Call draw
                    players[0].call_draw();
                    // IMPLEMENT THIS LATER!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
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

    // Ensure sapaw state was cleared if player has not sapaw themselves
    if (!playerSapawHimself)
    {
        players[0].wala_nang_sapaw();
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
    // cout << "Draw pile size:\t" << (*drawPile).size() << endl;  // DEBUGGING
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
    // NOTE: Different iteration here because, and I'm guessing here, that
    //  my usual auto stuff : stuffs was calling a copy constructor for stuff.
    //  I'm guessing that because modifications to stuff weren't being 'saved'
    //  here.  Changing to a standard for-loop coupled with dereferencing
    //  indices in my vector cleared up the problem.
    for (int i = 0; i < players.size(); i++)
    {
        playerNumber++;
        if (players[i].count_exposed_melds() == 0)
        {
            // cout << "Before logging the burn... Player " << playerNumber << ": " << player.get_name() << " is burned?  " << player.is_burned() << endl;  // DEBUGGING
            players[i].got_burned();
            // cout << "After logging the burn... Player " << playerNumber << ": " << players[i].get_name() << " is burned?  " << players[i].is_burned() << endl;  // DEBUGGING
            cout << "Player " << playerNumber << ": " << players[i].get_name() << " has " << players[i].count_exposed_melds() << " melds and got burned!" << endl;
        }
    }
    playerNumber = 0;

    // Calculate scores for those that didn't get burned
    for (auto player : players)
    {
        // cout << "Before getting final score... Player " << playerNumber << ": " << player.get_name() << " is burned?  " << player.is_burned() << endl;  // DEBUGGING
        playerNumber++;
        if (!player.is_burned())
        {
            if (player.get_final_score(players) < winningScore)
            {
                winningScore = player.get_final_score(players);
                currentWinner = player.get_name();
                winningPlayerNumber = playerNumber;
            }
        }
        // cout << "After getting final score... Player " << playerNumber << ": " << player.get_name() << " is burned?  " << player.is_burned() << endl;  // DEBUGGING
    }
    playerNumber = 0;

    // Print the winner
    cout << "Player " << winningPlayerNumber << ": " << currentWinner << " won with a score of " << winningScore << "!  CONGRATULATIONS!" << endl;

    // DONE
    return winningPlayerNumber;
}


int Tong_Its_Game::calc_chip_loss(Tong_Its_Player& winner, Tong_Its_Player& loser)
{
    // LOCAL VARIABLES
    int retVal = 0;

    // INPUT VALIDATION
    if (winner.get_name() == loser.get_name())
    {
        retVal = 0;
    }
    else
    {
        // CALCULATE
        // Winning by Tongits
        if (winner.called_tongits())
        {
            // cout << "Winner got Tongits... adding 3" << endl;  // DEBUGGING
            retVal += 3;
        }
        // Winning a Draw
        else if (winner.called_draw() && loser.challenged_a_draw())
        {
            // cout << "Winner won Draw... adding 3" << endl;  // DEBUGGING
            retVal += 3;
        }
        // Just winning
        else
        {
            // cout << "Winner merely won... adding 1" << endl;  // DEBUGGING
            retVal += 1;
        }
        // Aces in hand
        if (winner.count_aces() > 0)
        {
            // cout << "Winner had aces... adding " << winner.count_aces() << endl;  // DEBUGGING
            retVal += winner.count_aces();
        }
        // Burned?
        // cout << "Loser " << loser.get_name() << " is burned?  " << loser.is_burned() << endl;  // DEBUGGING
        if (loser.is_burned() == true)
        {
            // cout << "Loser got burned... adding 1" << endl;  // DEBUGGING
            retVal += 1;
        }
        // Secret set
        if (winner.count_special_melds(players) > 0)
        {
            // cout << "Winner had special melds... adding " << (3 * winner.count_special_melds()) << endl;  // DEBUGGING
            retVal += (3 * winner.count_special_melds(players));
        }
    }

    // DONE
    return retVal;
}


void Tong_Its_Game::reset_game(int winnerNum)
{
    // VALIDATION
    if (winnerNum < 1 || winnerNum > players.size())
    {
        throw invalid_argument("Tong_Its_Game::reset_game() was passed an invalid winner number");
    }
    // cout << "Draw pile size:\t\t" << (*drawPile).size() << endl;  // DEBUGGING
    // cout << "Discard pile size:\t" << (*discardPile).size() << endl;  // DEBUGGING

    // RESET
    // 1. Winner becomes the new dealer
    currentPlayer = winnerNum;
    // 2. Move the discards into the draw pile
    for (auto discard : (*discardPile))
    {
        (*drawPile).push_back(discard);
    }
    (*discardPile).clear();
    // Validate the move
    if ((*drawPile).size() != 52 || (*discardPile).size() != 0)
    {
        // cerr << "Draw pile size:\t\t" << (*drawPile).size() << endl;  // DEBUGGING
        // cerr << "Discard pile size:\t" << (*discardPile).size() << endl;  // DEBUGGING
        throw runtime_error("Tong_Its_Game::reset_game() draw pile reset has failed");
    }
    // 3. Shuffle the draw pile
    shuffle_a_deck(drawPile);
    // 4. Reset the state of the playing cards
    for (auto playingCard : (*drawPile))
    {
        playingCard->sapaw = false;
        playingCard->special = false;
        playingCard->numMelds = 0;
        playingCard->inSet = false;
        playingCard->inRun = false;
        if (!(playingCard->validate_playing_card()))
        {
            throw runtime_error("Tong_Its_Game::reset_game() has discovered an invalid playing card");
        }
    }
    // 5. Deal cards to the players
    deal_player_hands(players[winnerNum - 1]);

    // DONE
    return;
}
/*********************/
/* TONG ITS GAME END */
/*********************/
