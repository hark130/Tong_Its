#ifndef __TONG_ITS__
#define __TONG_ITS__

#include <regex>                // regex_*
#include <string>               // stoi

using namespace std;

// #define SKINNY

#ifdef SKINNY
#define BORDER_UPPER_LEFT "┌"
#define BORDER_UPPER_RIGHT "┐"
#define BORDER_VERTICAL "│"
#define BORDER_LOWER_RIGHT "┘"
#define BORDER_LOWER_LEFT "└"
#define BORDER_HORIZONTAL "─"
#define BORDER_SPACE " "
#else
#define BORDER_UPPER_LEFT "╔"
#define BORDER_UPPER_RIGHT "╗"
#define BORDER_VERTICAL "║"
#define BORDER_LOWER_RIGHT "╝"
#define BORDER_LOWER_LEFT "╚"
#define BORDER_HORIZONTAL "═"
#define BORDER_SPACE " "
#endif // SKINNY OR FAT BOXES

#if defined(_WIN32) || defined(__MSDOS__)
    #define SPADE   "\x06"
    #define CLUB    "\x05"
    #define HEART   "\x03"
    #define DIAMOND "\x04"
    #define SPECIAL "\x245"
#else
    #define SPADE   "\xE2\x99\xA0"
    #define CLUB    "\xE2\x99\xA3"
    #define HEART   "\xE2\x99\xA5"
    #define DIAMOND "\xE2\x99\xA6"
    #define SPECIAL "\xC2\xA7"
#endif

extern string spadeString;
extern string clubString;
extern string heartString;
extern string diamondString;
extern string specialString;

#define USER_EXIT 999           // User interface indication to quit the program
#define NUM_CARDS_PER_ROW 52    // Indicates how many cards print_a_row() will print per row

int input_number(void);
int random_num(int start, int stop);

#endif // __TONG_ITS__
