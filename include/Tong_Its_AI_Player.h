#ifndef __TONG_ITS_AI_PLAYER__
#define __TONG_ITS_AI_PLAYER__

#include <memory>
#include <string>
#include "Tong_Its.h"
#include "Tong_Its_Playing_Card.h"
#include "Tong_Its_Player.h"
#include "Tong_Its_Game.h"
#include <vector>

using namespace std;

// class Tong_Its_Game;  // Forward declared
// class Tong_Its_Player;  // Forward declared

class Tong_Its_AI_Player : public Tong_Its_Player
{
public:
	Tong_Its_AI_Player(string playerName, int stratNum);
	void ai_interface(Tong_Its_Game& theGame);

private:
	// 1 - Entirely random.  Never takes a discard.  Never exposes a meld.
	int strategy;  // Strategy being used by the computer
	string strategyDesc;  // Stores a brief description of this strategy
	void ai_random_interface(Tong_Its_Game& theGame);
};

#endif  // __TONG_ITS_AI_PLAYER__