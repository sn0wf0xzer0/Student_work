//Implementation file for AIMS class.
//Author: John Palacios.

#include "Aims.h"
#include <iostream>
using namespace std;

Aims::Aims()
{
	gause_field = new int* [10];
	for(int i = 0; i < 10; i++){
		gause_field[i] = new int [10];
	}
	field_hight = 10;
	field_width = 10;
	mine_field = new char* [10];
	for(int i = 0; i < 10; i++){
		mine_field[i] = new char [10];
		for(int j = 0; j < 10; j++)
			mine_field[i][j] = ' ';
	}
	mode = 0;
}

Aims::Aims(int height, int width){
	gause_field = new int* [height];
	for(int i = 0; i < 10; i++){
		gause_field[i] = new int [width];
	}
	field_hight = height;
	field_width = width;
	mine_field = new char* [height];
	for(int i = 0; i < 10; i++){
		mine_field[i] = new char [width];
		for(int j = 0; j < 10; j++)
			mine_field[i][j] = ' ';
	}
	mode = 0;
}

Aims::~Aims()
{
	for(int i = 0; i < field_hight; i++){ delete [] gause_field[i]; }
	for(int i = 0; i < field_hight; i++){ delete [] mine_field[i]; }
}

void Aims::adja(int **field, int x, int y)
{
	int val_atXY = gause_field[y][x];
	int count = 0;
	Coord temp;

	//Must start with a new set of adjacent spaces.
	if(adj_spaces.size() > 0)
		adj_spaces.clear();

	//count the number of known mines and undiscovered squares around space x, y.
	num_knowMine = 0;
	num_unreveal = 0;
	for(int i = y-1; i < y+2; i++){
		for(int j = x-1; j < x+2; j++){
			if((i > -1 && 10 > i && y > -1 && 10 > y) && (!(i == y && j == x))){
				temp.x = j;
				temp.y = i;
				adj_spaces.push_back(temp);
				if(mine_field[i][j]  == 'x')
					num_knowMine++;
				if(field[i][j] == -1)
					num_unreveal++;
			}
		}
	}

	if(val_atXY == 0){
		for(int i = 0; i < adj_spaces.size(); i++){
			if(gause_field[adj_spaces[i].y][adj_spaces[i].x] == -1)
			next_move.push_back(adj_spaces[i]);
		}
		if(mode == 1)
			mode++;
	}
	//if the number at a space is equal to the number of unrevealed
	//spaces surrounding that space, then they are all mines.
	if(val_atXY == num_unreveal){
		for(int i = 0; i < adj_spaces.size(); i++){
			mine_field[adj_spaces[i].x][adj_spaces[i].y] = 'x';
		}
	}

	//there are only val_atXY mines around field[x][y]. If there are val_atXY known
	//mines around field[x][y], then the other spaces are safe to take.
	if((val_atXY != 0) && (num_knowMine == val_atXY) && (num_unreveal > val_atXY)){ //<------------- Need to make sure that this does not execute on val_atXY == 0.
		for(int i = 0; i < adj_spaces.size(); i++){
			if((mine_field[adj_spaces[i].x][adj_spaces[i].y] != 'x') && (field[adj_spaces[i].x][adj_spaces[i].y] == -1)){
				next_move.push_back(adj_spaces[i]);
				if(mode == 1)
					mode++;
			}
		}
	}
	//This is only executed if a full scan of the gause_field
	//Reveals no sure next moves. In the future, this will 
	//Increment to include pattern matching.
	if(mode == 1 && (x == field_width && y == field_hight))
		mode--;
}

void Aims::Aim_act(int &x, int &y)
{
	x = next_move[next_move.size()-1].x;
	y = next_move[next_move.size()-1].y;
	next_move.pop_back();
	if(next_move.size() == 0 && mode != 1)
		mode--;
}

void Aims::examine_field(int **field)
{
	//syncronize gause_field with current displayed field.
	for(int i = 0; i < field_hight; i++){
		for(int j = 0; j < field_width; j++){
				gause_field[i][j] = field[i][j];
		}
	}
	for(int i = 0; i < field_hight; i++){
		for(int j = 0; j < field_width; j++){
			if(gause_field[i][j] == 9){
				mode = -1;
				return;
			}
			if(gause_field[i][j] != -1)
				adja(gause_field, j, i);
		}
	}
	if(next_move.size() == 0){
		hunt();
	}
}

void Aims::hunt()
{
	int h;
	int w;
	Coord temp;
	for(int k = 0; k < 2; k++){
		for(int l = 0; l < 2; l++){
			for(int m = 0; m < 2; m++){
				for(int i = 0; i < 2; i++){
					for(int j = 0; j < 2; j++){
						h = 2 + -l^k +5*i;
						w = 2 + -l^k +5*j;
						if(gause_field[h][w] == -1){
							temp.x = w;
							temp.y = h;
							next_move.push_back(temp);
							mode++;
							return;
							
						}
					}
				}
			}
		}
	}

}