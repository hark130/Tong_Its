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
    [ ] Returns a random number from x to y <br />
* Game Play <br />
    [ ] {re}Implement "currentDealer" functionality
    [ ] Account for runs longer than 3

## Refactoring
* Better way to move cards from a game-private deck member variable to a player-private hand member variable than "tempDeck"?
* Current method of moving cards is clunky!
* Template parent class to derive Player and Game classes from so that Player and Game may share common functionality (and use protected?)
    [ ] move_one_card()?
* Better user input tactics in the user interface?
* Better way to store currentPlayer/currentDealer?  Pointer?  Reference?
* Dynamically size printed cards?  Is this even necessary?

## Bugs
[ ] When NUM_CARDS_PER_ROW is 1
terminate called after throwing an instance of 'std::out_of_range'
  what():  vector::_M_range_check: __n (which is 16) >= this->size() (which is 13)
Aborted
