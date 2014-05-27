#include <string>
#include <SDL.h>

#pragma once

//constant values
const int P_MOVE = 99;						//chance to move, out of 100
const int P_NMOVE = 100 - P_MOVE;			//chance to not move, out of 100

enum KEYS {NONE, UP, DOWN, LEFT, RIGHT};	//list of commands to pass to your character
const int NUMKEYS = 5;						

enum OBJS									//each grid location is a bitstring with each bit representing something
{											// use this to mask/determine which locations are valid, have the goal, or another player
	OBSTICLE=1, FLAG=1<<1,					
	P1=1<<2, P2=1<<3, P3=1<<4, P4=1<<5
};

const int INPUTWAITTIME = 500;				//amount of time to generate your move, otherwise your turn is skipped

enum EVENT									//different sensations/events depending on your location/occurances
{
	FEEL   = 1<<0, //=1						//feel a wall near you
	SMELL  = 1<<1, //=2						//smell another player around you
	TWINKLE= 1<<2, //=4						//sparkle of a gem around
	CHEER  = 1<<3, //=8						//you have captured a flag
	FLASH  = 1<<4, //=16					//flag has disappeared
	HEAR   = 1<<5, //=32					//flag has reappeared
};

const int NUMGEMS = 2;

//screen stuff
const int SCREEN_BOARDER = 64;
const int SCREEN_WIDTH = 512;
const int SCREEN_HEIGHT = 512 + SCREEN_BOARDER;
const int SCREEN_X = 100;
const int SCREEN_Y = 100;

const int TILE_W = 32;
const int TILE_H = 32;

const int MAXOBJS = 10;



//shared memory between threads
struct Shmem
{
	//to controller
	bool quit;
	
	int player;
	
	int x, y;
	int grid_w;
	int grid_h;

	char* grid;
	int score;

	//Uint32 last_update;
	int events;		//use EVENT enum to mask events
	//int gem_x[NUMGEMS];		//has grid locations of gems
	//int gem_y[NUMGEMS];

	
	//from controller
	bool read;
	int action;		//use KEYS enum to assign action
};


//helper functions
static Uint32 GetTime()
{
	return SDL_GetTicks();
}

static void Delay(Uint32 delay)
{
	SDL_Delay(delay);
}

//converts an integer into a string
static std::string toString(int val)
{
	if (val == 0)
		return "0";

	std::string str;
	bool sign = false;
	if (val < 0)
	{
		sign = true;
		val = -val;
	}

	while(val > 0)
	{
		int digit = val % 10;
		str = char(digit + '0') + str;
		val /= 10;
	}
	if (sign)
		str = '-' + str;

	return str;
}

typedef int (*func)(void*);


