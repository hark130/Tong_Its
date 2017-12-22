# Tong_Its
Command line replication of the Filipino card game ["Tong Its"](https://www.pagat.com/rummy/tong-its.html) in C++


## Why?
* I haven't programmed anything in a while (I'm supposed to be on vacation)
* I could use some practice in C++
* My wife always beats me and I want to test out some algorithmic strategies


## How?
g++ --std=c++14 -I include source/Tong_Its.cpp main.cpp -o game.exe
./game
NOTE: I wanted std::make_unique so I needed C++ 14

## To Do
* Interface <br />
    [X] Calls card printer <br />
    [X] Calls hand printer <br />
* Card printer <br />
    [X] Print one card
* Hand printer <br />
    [X] Prints horizontally and vertically <br />
    [X] Modular horizontal/vertical printing <br />
* Card dealer <br />
    [X] Deal cards to all the players
* Deck builder <br />
    [X] Allocates shared pointers to a Card class <br />
    [X] Builds vector of cards <br />
* Shuffler <br />
    [X] Calls randomizer <br />
    [X] Randomizes the order of the vector of unique pointers to the Card objects <br />
* Randomizer <br />
    [X] Returns a random number from x to y <br />
* Game Play <br />
    [ ] Melds
        [X] Show melds
        [X] Number melds
        [/] Expose a meld
            [ ] Remove that meld from player's hand
            [ ] Add that meld to player's exposed melds
            [ ] Recalculate player's melds
            [ ] Printing the game state should show all of the exposed melds
            [ ] Showing player's melds should also show potential 'add ons' to previously exposed melds
    [ ] {re}Implement "currentDealer" functionality <br />
    [X] Account for runs longer than 3
    [ ] Only allowed to take from the discard if you can make a meld with it
* Misc <br />
    [ ] Research a way to get access to C++ documentation offline (e.g., vector methods)

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


## Bugs
[ ] There must be a better solution to allow a Tong_Its_Game to count the number of potential melds for a player than calling a public method since *anyone* could call that method to gain insight into what a player has
[ ] There must be a better solution than to find a cardnumber using a public member function in Tong_Its_Player class.  Anyone could figure out what's in your hand.
[ ] When NUM_CARDS_PER_ROW is 1
terminate called after throwing an instance of 'std::out_of_range'
  what():  vector::_M_range_check: __n (which is 16) >= this->size() (which is 13)
