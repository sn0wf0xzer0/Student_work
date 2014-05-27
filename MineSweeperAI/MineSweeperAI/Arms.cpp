//Implementation file for the ARMS class.
//Author: John palacios

#include "Arms.h"
using namespace std;

Arms::Arms()
{
	field_hight = 10;
	field_width = 10;
	mine_field = new char* [10];
	for(int i = 0; i < 10; i++){
		mine_field[i] = new char [10];
		for(int j = 0; j < 10; j++)
			mine_field[i][j] = ' ';
	}
}

Arms::Arms(int height, int width)
{
	field_hight = height;
	field_width = width;
	mine_field = new char* [width];
	for(int i = 0; i < width; i++){
		mine_field[i] = new char [height];
		for(int j = 0; j < height; j++)
			mine_field[i][j] = ' ';
	}
}

Arms::~Arms()
{
	for(int i = 0; i < field_width; i++)
		delete [] mine_field[i];
	delete [] mine_field;
	mine_field = NULL;
}

void Arms::ARMS_action(int **field, int &x, int &y)
{
	if(num_moves() == 0){
		scan_field(field);
		Arms_strat(x, y);
	}
	else
		Arms_strat(x, y);
}

void Arms::scan_field(int **field)
{
	for(int i = 0; i < field_width; i++){
		for(int j = 0; j < field_hight; j++){
			if(field[i][j] == -1)			//Here
				continue;
			//if zero mines are detected, choose all unchoosen
			//adjacent spaces as next move.
			if(field[i][j] == 0){
				get_adjacent(i, j);
				clear_zeros(field);					//check this.
				if(next_move.size() != 0)
				return;
			}
			else{
				get_adjacent(i, j);
				examine_node(field, i, j);
			}
		}
	}
	//if no other means of determining next move have succeeded,
	//choose an arbitrary unchosen space.
	if(next_move.size() == 0){
		choose_arbitrary(field);
	}
}

void Arms::get_adjacent(int x, int y)
{
	Coord temp;

	//if there already exist adjacent spaces, delete them.
	if(adja_spac.size() != 0)
		adja_spac.clear();

	//Choose coordinates touching x, y that are on the field, and not x and y.
	for(int i = x-1; i < x+2; i++){
		for(int j = y-1; j < y+2; j++){
			if((i > -1 && 10 > i && j > -1 && 10 > j) && (!(i == x && j == y))){
				temp.x = i;
				temp.y = j;
				adja_spac.push_back(temp);
			}
		}
	}
}

void Arms::examine_node(int **field, int x, int y)
{
	int unknown_spaces = 0;				//This is the number of unrevealed adjacent spaces.
	int known_mine_qty = 0;				//This is the number of known mines adjacent to field[i][j];
	int gause_value_XY = field[x][y];	//Comparisons will be made from this number.

	//find number of unknown and number of mine spaces around this node.
	for(int i = 0; i < adja_spac.size(); i++){
		if(field[adja_spac[i].x][adja_spac[i].y] == -1){
			unknown_spaces++;
		}
		if(mine_field[adja_spac[i].x][adja_spac[i].y] == 'x'){
			known_mine_qty++;
		}
	}

	//We may infer that all unknown spaces around this node are mines if
	//the number of unknown spaces around this node equal it's gause
	//value. Mark each of these on the mine field.
	if(unknown_spaces == gause_value_XY){
		for(int i = 0; i < adja_spac.size(); i++){
			if(field[adja_spac[i].x][adja_spac[i].y] == -1){
				mine_field[adja_spac[i].x][adja_spac[i].y] = 'x';
			}
		}
	}

	//If we know all mines around this node, then the remaining adjacent
	//spaces are safe to choose and this is our next move. This is the
	//first opprotunity to choose moves this examination.
	if(known_mine_qty == gause_value_XY && unknown_spaces > gause_value_XY && gause_value_XY != 0){
		for(int i = 0; i < adja_spac.size(); i++){
			if(mine_field[adja_spac[i].x][adja_spac[i].y] != 'x' && field[adja_spac[i].x][adja_spac[i].y] == -1){
				next_move.push_back(adja_spac[i]);
				cout << "going to move " << adja_spac[i].x << ", " << adja_spac[i].y << endl;
				Sleep(1000);
			}
		}
	}
}

void Arms::clear_zeros(int **field)
{
	int unknown_spaces = 0;				//This is the number of unrevealed adjacent spaces.
	//do I need to clear the adjacent spaces vector before calling clear zero?
	//adjacent spaces vector seems to have a previously examined node's spaces.
	//push all unrevealed spaces into next move.
	for(int i = 0; i < adja_spac.size(); i++){
		if(field[adja_spac[i].x][adja_spac[i].y] == -1)
			next_move.push_back(adja_spac[i]);
	}
}

void Arms::choose_arbitrary(int **field)
{
	int h;
	int w;
	Coord temp;

	//choose[ 2 + -L^(k) + 5i, 2 + -M^(k) + 5j], if this revealed space is  0, 
	//then proceed to zero mode, else proceed to choose space [ 2 + -L^(k) + 5i, 2 + -M^(k) + 5j] 
	//where k is loop zero, L is loop one, M is loop 2, i is loop 3, and j is loop 4. L and M can not exceed 2, 
	//if they do, choose random space that has not be choosen yet, untill a zero is found or game over.
	for(int k = 0; k < 2; k++){
		for(int l = 0; l < 2; l++){
			for(int m = 0; m < 2; m++){
				for(int i = 0; i < 2; i++){
					for(int j = 0; j < 2; j++){
						h = 2 + -l^k +5*i;
						w = 2 + -l^k +5*j;
						if(field[w][h] == -1 && mine_field[w][h] != 'x'){
							temp.x = w;
							temp.y = h;
							next_move.push_back(temp);
							cout << "choosing arbitrary space.\n";
							return;
						}
					}
				}
			}
		}
	}

	//in the unlikely event that no next moves have been chosen, choose a random
	//unmarked, non mine space
	while(next_move.size() == 0){
		w = rand() % field_width;
		h = rand() % field_hight;
		if(field[w][h] == -1 && mine_field[w][h] != 'x'){
			temp.x = w;
			temp.y = h;
			next_move.push_back(temp);
			cout << "choosing random space.\n";
			return;
		}
	}
}

void Arms::Arms_strat(int &x, int &y)
{
	x = next_move[next_move.size()-1].x;
	y = next_move[next_move.size()-1].y;
	next_move.pop_back();
}