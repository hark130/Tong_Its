#include <iostream>             // cout, cin
#include <regex>                // regex_*
#include <string>               // stoi
#include "Tong_Its.h"

using namespace std;

// Definition of global externed variables used
//  by each header utilized by main.cpp
string spadeString = string(SPADE);
string clubString = string(CLUB);
string heartString = string(HEART);
string diamondString = string(DIAMOND);
string specialString = string(SPECIAL);


/*
    Purpose - An attempt to abstract-away user input
    Input - None
    Output - Int read from user input
    Note
        Will not accept a number less than 1
        Will never give up, never surrender
 */
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
