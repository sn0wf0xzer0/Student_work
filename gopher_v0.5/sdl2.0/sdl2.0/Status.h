//This struct keeps trach of game data for Agemt_AI.
//Author: John Palacios

#ifndef STATUS_H
#define STATUS_H

struct Status
{
	bool notif[6];
	//bool wall;			//1			= a wall neer.				0
	//bool play;			//2			= a player near.			1
	//bool flag;			//4			= a gem near.				2
	//bool capF;			//8			= I captured a flag.		3
	//bool disF;			//16		= a flag has disapeared.	4
	//bool reaF;			//32		= a flag has reappeared.	5
};

#endif /* STATUS_H */