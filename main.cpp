// #include "include/Tong_Its.h"
#include "Tong_Its.h"
#include <iostream>
#include <memory>
#include <string>

using namespace std;

int main(void)
{
    auto me = make_shared<string>(string("Joe"));
    // cout << *me << endl;  // DEBUGGING
    auto game = Tong_Its_Game(me);
    // cout << game.player1.count_chips() << endl;  // DEBUGGING
    // game.player1.TEST_the_hand();  // DEBUGGING
    // game.TEST_the_deck(game.discardPile);  // DEBUGGING
    game.start_the_game();

    // cout << input_number();  //TESTING

    return 0;
}
