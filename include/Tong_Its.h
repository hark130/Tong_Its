#ifndef __TONG_ITS__
#define __TONG_ITS__

#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace std;

typedef struct Playing_Card
{
	string rank;  	// Value as a string
	char16_t suit;  // U+2660–2667
	int value;		// Value as a number
} PCard, *PCard_ptr;


class Tong_Its_Player
{
public:
	Tong_Its_Player(string playerName);
	string get_name(void);
	int count_chips(void);
	// int count_cards(void);
private:
	string name;
	int numOfChips;
	// Hand == vector of unique pointers to Card objects
	// AI decision algorithm
	//	Random Plays
	//	Overt
	//	Covert
	//	Overt/Covert mix
	//	Perfect strategy (predictive)
	//	Maximize points	
};


class Tong_Its_Game 
{
public:
	Tong_Its_Game(const shared_ptr<string>& humanPlayerName);
	Tong_Its_Player player1;
	Tong_Its_Player player2;
	Tong_Its_Player player3;
private:
	// Builds a vector of unique pointers to Card objects
	shared_ptr<vector<unique_ptr<PCard>>> build_a_deck(void);
	// Discard == vector of unique pointers to Card objects
	// currentPlayer

	// Calculate points()
	// Winner?()
	// Next player()
};


#endif // __TONG_ITS__
