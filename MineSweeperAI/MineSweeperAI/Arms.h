//This is the header file for ARMS (Automated Robotic Mine Sweeper). This is an Artificial Intelegence who attempts to solve mine sweeper.
//This AI is like an octopus with eight arms, each feeling out spaces adjacent to the one it is looking at.
//Author: John Palacios

#include <iostream>
#include <Windows.h>
#include <vector>
#include "Coord.h"
using namespace std;

#ifndef ARMS
#define ARMS

class Arms
{
private:
	vector<Coord> next_move;
	vector<Coord> adja_spac;
	int field_hight;
	int field_width;
	char **mine_field;
	void get_adjacent(int, int);
	void scan_field(int **);
	void Arms_strat(int &, int &);			//to do
	void examine_node(int **, int, int);
	void clear_zeros(int **);
	void choose_arbitrary(int **);
public:
	Arms();
	Arms(int, int);
	~Arms();
	void ARMS_action(int **, int &, int &);
	int num_moves()
	{ return next_move.size(); }
};

#endif /* ARMS */