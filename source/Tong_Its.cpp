// #include "../include/Tong_Its.h"
// #include "include/Tong_Its.h"
#include "Tong_Its.h"
#include <iostream>
#include <string>
#include <vector>

#define SPADE U+2660	// ♠
#define HEART U+2661	// ♡
#define DIAMOND U+2662	// ♢
#define CLUB U+2663		// ♣

/*************************/
/* TONG ITS PLAYER BEGIN */
/*************************/
Tong_Its_Player::Tong_Its_Player(string playerName)
{
	name = playerName;
	numOfChips = 100;
}

string Tong_Its_Player::get_name(void)
{
	return name;
}

int Tong_Its_Player::count_chips(void)
{
	return numOfChips;
}

/***********************/
/* TONG ITS PLAYER END */
/***********************/


/***********************/
/* TONG ITS GAME BEGIN */
/***********************/
Tong_Its_Game::Tong_Its_Game(const shared_ptr<string>& humanPlayerName) : \
player1(*humanPlayerName), player2(string("Player2")), player3(string("Player3"))
{
	// player1 = Tong_Its_Player::Tong_Its_Player(*humanPlayerName);
	// player2 = Tong_Its_Player(string("Player 2"));
	// player3 = Tong_Its_Player(string("Player 3"));
}


shared_ptr<vector<unique_ptr<PCard>>> Tong_Its_Game::build_a_deck(void)
{
	shared_ptr<vector<unique_ptr<PCard>>> retVal;  // = make_shared<vector<unique_ptr<PCard>>>();

	retVal.emplace_back(make_unique<PCard>(PCard()))

	return retVal;
}
/*********************/
/* TONG ITS GAME END */
/*********************/
