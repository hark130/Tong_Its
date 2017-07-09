# Tong_Its
Command line replication of the Filipino card game ["Tong Its"](https://www.pagat.com/rummy/tong-its.html) in C++


## Why?
* I haven't programmed anything in a while (I'm supposed to be on vacation)
* I could use some practice in C++
* My wife always beats me and I want to test out some algorithmic strategies


## How?
g++ --std=c++14 -I include source/Tong_Its.cpp main.cpp -o game
./game
NOTE: I wanted std::make_unique so I needed C++ 14

## To Do
[ ] Interface
    [ ] Calls card printer
    [ ] Calls hand printer
[ ] Card printer
[ ] Hand printer
    [ ] Prints horizontally, or vertically
[X] Deck builder
    [X] Allocates shared pointers to a Card class
    [X] Builds vector of cards
[ ] Shuffler
    [ ] Calls randomizer
    [ ] Randomizes the order of the vector of unique pointers to the Card objects
[ ] Randomizer
    [ ] Returns a random number from x to y

