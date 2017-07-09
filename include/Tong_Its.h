#ifndef __TONG_ITS__
#define __TONG_ITS__

#include <iostream>
#include <memory>
#include <string>

using namespace std;

class Tong_Its_Player
{
public:
	Tong_Its_Player(string playerName);
	string get_name(void);
	int count_chips(void);
private:
	string name;
	int numOfChips;
};


class Tong_Its_Game 
{
public:
	Tong_Its_Game(const shared_ptr<string>& humanPlayerName);
private:
	Tong_Its_Player player1;
	Tong_Its_Player player2;
	Tong_Its_Player player3;
};


#endif // __TONG_ITS__