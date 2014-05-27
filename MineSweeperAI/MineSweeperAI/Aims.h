//This is the AIMS class: Artificial Intelegence MineSweeper.
//Author: John Palacios

#include "Coord.h"
#include <vector>
using namespace std;

#ifndef AIMS
#define AIMS



class Aims
{
private:
	//Do not choose array.
	//next move array.
	//view of field.
	//mode. int mode, hunt == 0, zero == 1, count == 2 maybe?
	int **gause_field;			//All revealed numbers are stored here.
	int field_hight;			//the hight of current field in queston.
	int field_width;			//The width of current field in question.
	vector<Coord> next_move;	//The next moves that the AI will take.
	vector<Coord> adj_spaces;	//all spaces adjacent to [x][y].
	char **mine_field;			//Do not choose entries in this field
	int num_unreveal;			//Number of adjacent spaces to [x][y] which have not been revealed.
	int num_knowMine;			//Number of known mines in spaces adjacent to [x][y].
	int mode;					//-1 is dead (a space which is a mine has been choosen), 0 is initial and guess hunting, 1 is zero clearing mode, 2 is ready to choose spaces.
	//collects list of adjacent spaces.
	void adja(int **, int, int);
	//clears all zero's
	//void zero(int &, int &);
	//determines safe spaces and danger spaces.
	//void count(int, int);		//Taken care of in f(x) adja.
	//pattern matching can help determine likely scenarios.
	//void match(int &, int &);
	//Specific pattern with pi/2 rotational symetry.
	//void one_1(int &, int &);
	//Specific pattern with pi/2 rotational symetry.
	//void one_2(int &, int &);
public:
	//Initializes do not choose array and next move array to a 10 by 10.
	//mode = 0;
	Aims();
	//Initializes do not choose array and next move array with height by width
	//and mode = 0;
	Aims(int, int);
	~Aims();
	//Syncronizes AI understanding of game field.
	void examine_field(int **);
	//oppener strategy.
	void hunt();
	//returns AI mode.
	int get_mode()
	{ return mode;}
	//Returns the number of moves the AI has planned out.
	int how_many()
	{ return next_move.size(); }
	//Main function for AI turn taking.
	void Aim_act(int &, int &);
};

#endif /* AIMS */

//*******************************//
//Each examination of the field will generate
//a set of next moves.
//Examine will
//	->read values from the field into gause_field		<--- maybe this is not necessary.
//	->review gause_field values.
//	->if a value is zero:															|	*Need a "Ready to make move" value. after a set of next moves are "planned."
//		->all adjacent, undiscovered spaces are pushed into next_move.				| all of these methods need set of adjacent spaces. that should be it's own method.
//	->if a value == the number of undiscovered spaces around it:					| maybe examine can determine which condition is true, then call appropriat method.
//		->all adjacent, undiscovered spaces are marked in mine_field.				|
//	->if a value == the number of known mines around it:
//		->all spaces that are not know to be mines are pushed into next_move.
//	->if a value is less than the number unmarked spaces around it:
//		->examine next position.
//	->if next_move.size() == 0 at this point:
//		->an arbitrary space is instead chosen.
