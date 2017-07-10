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
    [ ] Calls card printer <br />
    [ ] Calls hand printer <br />
* Card printer <br />
* Hand printer <br />
    [ ] Prints horizontally, or vertically <br />
* Deck builder <br />
    [X] Allocates shared pointers to a Card class <br />
    [X] Builds vector of cards <br />
* Shuffler <br />
    [X] Calls randomizer <br />
    [X] Randomizes the order of the vector of unique pointers to the Card objects <br />
* Randomizer <br />
    [ ] Returns a random number from x to y <br />

