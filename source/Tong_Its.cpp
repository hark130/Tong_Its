// #include "../include/Tong_Its.h"
// #include "include/Tong_Its.h"
#include "Tong_Its.h"
#include <iostream>
#include <string>

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
Tong_Its_Game::Tong_Its_Game(const shared_ptr<string>& humanPlayerName)
{
	player1 = Tong_Its_Player(*humanPlayerName);
	player2 = Tong_Its_Player(string("Player 2"));
	player3 = Tong_Its_Player(string("Player 3"));
}
/*********************/
/* TONG ITS GAME END */
/*********************/
