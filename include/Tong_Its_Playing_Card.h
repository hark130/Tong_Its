#ifndef __TONG_ITS_PLAYING_CARD__
#define __TONG_ITS_PLAYING_CARD__

#include <string>

using namespace std;

typedef struct Playing_Card
{
    Playing_Card(string pcRank, string pcSuit);
    string rank;     // Value as a string
    string suit;     // U+2660–2667
    int rankValue;   // Value as a number
    int suitValue;   // Ranking of the suit {1:4}
    int pointValue;  // Scoring value
    int numMelds;    // Number of melds containing this PCard
    bool sapaw;      // Played on someone else's meld
} PCard, *PCard_ptr;


#endif  // __TONG_ITS_PLAYING_CARD__
