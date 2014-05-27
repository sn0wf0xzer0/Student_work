//This is the header file for the goffer AI. agent_AI 0.1.1
//Author: John Palacios

#include <vector>
#include <iostream>
#include "Coord.h"
#include "Status.h"
#include "helper.h"
#include <ctime>
using namespace std;

#ifndef AGENT_AI
#define AGENT_AI

class Agent_AI
{
private:
	int num_players;							//number of players in this game.
	int my_player;								//of the players on the field, this is my player number.
	int my_score;								//Number of flags I've captured.
	int round;									//number of rounds played.
	int field_hight;							//number of rows on field.
	int field_width;							//number of collumns on field.
	Status sense_data;							//representation of game/player status.
	char *play_field;							//internal representation of field.
	char *player_token;							//binary representation of player on the field.
	Coord goal_locals[2];						//flag locations.
	vector<Coord> player_locals;				//starting player locations.
	Coord current_dest;							//My current destination.
	vector<Coord> way_points;					//used for obsticle navigation.
	vector<Coord> *path_coords;					//Coordinates of path steps for all players.
	vector<Coord> adjacent_spaces;				//up to four cardinal adjacent spaces.
	vector<int> intended_moves;					//moves to be attempted.
	int dest_distance;							//My distance to destination = |dest(x, y) - local(x, y)|
	void read_field(char*);						//Hopefully reads smem->grid...
	void get_locals();							//retrieves important locations and stores then in Coord form.
	void choose_goal();							//Determines which flag to approach.
	int get_distance(Coord, Coord);				//distance = |one(x, y) - two(x, y)|.
	void choose_action(int &);					//chooses an action based on known data.
	int least(int, int);						//determines which of the two ints are least. 0 or 1.
	void get_adjacent(Coord);					//checks and retrieves four cardinal adjacent legal spaces.
	void build_path(Coord);						//chooses nodes based on distance to goal.
public:
	Agent_AI();									//standard object instantialization.
	Agent_AI(int, int, int, int);				//specific object instantialization.
	~Agent_AI();								//standard destructor.
	void agent_act(Shmem *);					//used to read data from shmem. Primary scheduler.
	void listen_mem(int);					//stores game/player status.
};

#endif /* AGENT_AI */