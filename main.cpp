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
    cout << game.player1.count_chips() << endl;

    return 0;
}
