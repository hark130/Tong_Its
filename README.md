# Tong_Its
Command line replication of the Filipino card game ["Tong Its"](https://www.pagat.com/rummy/tong-its.html) in C++


## Why?
* I haven't programmed anything in a while (I'm supposed to be on vacation)
* I could use some practice in C++
* My wife always beats me and I want to test out some algorithmic strategies


## How?
g++ --std=c++14 -I include source/Clear_Screen.cpp source/Tong_Its.cpp main.cpp -o game.exe -lncurses
./game
NOTE: I wanted std::make_unique so I needed C++ 14

## To Do
* Interface <br />
    - [X] Calls card printer <br />
    - [X] Calls hand printer <br />
* Card printer <br />
    - [X] Print one card
* Hand printer <br />
    - [X] Prints horizontally and vertically <br />
    - [X] Modular horizontal/vertical printing <br />
* Card dealer <br />
    - [X] Deal cards to all the players
* Deck builder <br />
    - [X] Allocates shared pointers to a Card class <br />
    - [X] Builds vector of cards <br />
* Shuffler <br />
    - [X] Calls randomizer <br />
    - [X] Randomizes the order of the vector of unique pointers to the Card objects <br />
* Randomizer <br />
    - [X] Returns a random number from x to y <br />
* Game Play <br />
    - [ ] Melds <br />
        - [X] Show melds <br />
        - [X] Number melds <br />
        - [ ] Expose a meld <br />
            - [X] Remove that meld from player's hand <br />
            - [X] Add that meld to player's exposed melds <br />
            - [X] Recalculate player's melds <br />
            - [X] Printing the game state should show all of the exposed melds <br />
            - [ ] Showing player's melds should also show potential 'add ons' to previously exposed melds <br />
            - [ ] Print a player's melds horizontally to conserve space <br />
            - [ ] Allow the user to build specific melds (e.g. 9999 789 vs 999 *and* 789) <br />
            - [ ] Account for "special" melds somehow (struct with bool special and vector<shared_ptr<PCard>> ?) and don't forget to update T_I_G::calc_chip_loss() when you do <br />
    - [ ] Declaring Draw (public method you_can_not_draw() coupled with a member variable ableToDraw) <br />
        - [ ] You cannot call Draw if someone laid off on any of your melds since your previous turn <br />
        - [ ] You cannot call Draw if you laid off on your own melds in your previous turn <br />
    - [ ] {re}Implement "currentDealer" functionality <br />
    - [X] Account for runs longer than 3 <br />
    - [ ] Only allowed to take from the discard if you can make a meld with it <br />
    - [ ] Default draw location (1. Draw pile 2. Discard pile [Draw pile]) <br />
    - [ ] "You just drew a ..." assistant <br />
* End Of Play <br />
    - [ ] Determine if the game is over
        - [X] Draw pile empty
        - [X] Player called Tongits (bool calledTongits)
        - [X] Player called Draw (bool calledDraw)
            - [ ] Other players can fold or challenge
* Scoring <br />
    - [X] Did anyone get 'burned'?
    - [X] Did anyone call Tongits?
    - [X] Did anyone call Draw?
* Chips <br />
    - [ ] Winner gets the following from each loser:
        - [X] 1 chip for the winner (3 if by Tong Its)
        - [X] 1 chip per ace in the winner's melds (not sapaw)
        - [X] 1 chip for being burned
        - [ ] 3 chips per secret set of 4
        - [X] 3 chips for winning a Draw after a challenge
    - [ ] Bisaklat
    - [ ] "Two Hits"

* Misc <br />
    - [X] Research a way to get access to [C++ documentation offline](http://en.cppreference.com/w/Cppreference%3aArchives) (e.g., vector methods)
    - [ ] Better organize Tong_Its_Player class
        - [ ] Alphabetize members/methods among public/private?
        - [ ] Organize methods into function groups?
    - [ ] Configuration file/user request for optional rules/gameplay?

## Refactoring
* Less hacky method to get the index number into a vector while also getting the vector (see: show_all_melds())
* Better way to move cards from a game-private deck member variable to a player-private hand member variable than "tempDeck"?
* Current method of moving cards is clunky!
* Template parent class to derive Player and Game classes from so that Player and Game may share common functionality (and use protected?)
    [ ] move_one_card()?
* Better user input tactics in the user interface?
* Better way to store currentPlayer/currentDealer?  Pointer?  Reference?
* Dynamically size printed cards?  Is this even necessary?
* Research the C++ proper way of converting an int to a string in print_a_meld()
* Refactor show_all_*() to stop using currMeldNum.  We're using class member vectors now.
* As a shortcut, could I just take the shared_pointer from playersMelds and move it to playersExposedMelds in T_I_P::expose_a_meld()?!?!
* Vector of player objects instead of hard-coded player1, 2, and 3.  Then, you can iterate through the vector instead of copy/paste player actions (see: game_state() printing exposed melds, is_game_over() checking for Tongits and Draw)
* Tong_Its_Game::deal_players_hands() should utilize the newly created vector of players instead of hard-coded player references
* Apparently, I should never include "using namespace" in a header file.  Research this more indepth later.
* Modify T_I_G::user_interface() to take a Tong_Its_Player& humanPlayer as a parameter instead of always banking on the fact that players[0] is the human player

## Bugs
- [ ] User interface is allowing a player to call Draw if they have 0 exposed melds
- [ ] There must be a better solution to allow a Tong_Its_Game to count the number of potential melds for a player than calling a public method since *anyone* could call that method to gain insight into what a player has
- [ ] There must be a better solution than to find a cardnumber using a public member function in Tong_Its_Player class.  Anyone could figure out what's in your hand.
- [ ] Does sorting change after I expose a meld?
- [ ] When no one wins, exposes a meld, draws, or Tongits... Player 0:  won with a score of 2147483647!  CONGRATULATIONS! Segmentation fault

- [ ] When NUM_CARDS_PER_ROW is 1... terminate called after throwing an instance of 'std::out_of_range' what():  vector::_M_range_check: __n (which is 16) >= this->size() (which is 13)
