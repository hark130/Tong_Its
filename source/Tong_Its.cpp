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


/*
    Purpose - Randomize a number within a given range, inclusive
    Input
        start - beginning range number
        stop - ending range number
    Output - A random number between start and stop, inclusive
    Note
        This function assumes you've already seeded the random number generator
 */
int random_num(int start, int stop)
{
    // cout << "Start: " << start << "\nStop: " << stop << endl;  // DEBUGGING
    // Local Variables
    int low = start;
    int high = stop;
    int retVal = 0;

    // Input Validation
    if (low > high)
    {
        low = low ^ high;
        high = low ^ high;
        low = low ^ high;
    }
    
    if (start == stop)
    {
        retVal = start;
    }
    else
    {
        retVal = rand();
        // cout << "1. rand() == " << retVal << endl;  // DEBUGGING
        // cout << "2. " << retVal << " % " << (high - low + 1);  // DEBUGGING
        retVal = retVal % (high - low + 1);
        // cout << " == " << retVal << endl;  // DEBUGGING
        // cout << "3. " << retVal << " + (" << high << " - " << low << ")";  // DEBUGGING
        // retVal += high - low;
        // cout << "3. " << retVal << " + " << low;  // DEBUGGING
        retVal += low;
        // cout << " == " << retVal << endl;  // DEBUGGING
        // retVal = (rand() % (high - low + 1)) + (high - low);
    }

    return retVal;
}
