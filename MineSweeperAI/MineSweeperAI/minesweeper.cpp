//*Idea! divide the field into four sectors: 5X5, choose the middle most as a test clockwise, looking for a zero.

#include <iostream>
#include <ctime>
#include <cstdlib>
#include "Arms.h"
using namespace std;

const int BOMB = 9;
const int WIDTH  = 10;
const int HEIGHT = 10;

//initializes an int 2 dimensional pointer with values of
//-1 in each position.
int **dummy_field(int width, int height);

//initializes pure int field for AI consideration
//Without accidently peeking at which spaces are mines.
int **AI_field(int width, int height);

//Counts the number of unchosen coordinants on the field
//returns true if the number of unchoosen == number of mines.
bool isWinner(int **, int);

//This f(x) returns a vecotr containing all coordinants adjacent
//to x, y.
vector<Coord> get_voidZone(int x, int y);

//This f(x) redistributes mines to create a pocket around the first
//choice of coordinats.
void redistribute_mines(int field[][HEIGHT], int numbombs, vector<Coord> void_zone);

//count all bombs around location x,y
int CountAdjBombs(int field[][HEIGHT], int x, int y)
{
	int count = 0;
	for (int i=x-1; i<= x+1; i++)
	{
		for (int j=y-1; j<= y+1; j++)
		{
			if (i<0 || i>=WIDTH || j<0 || j>= HEIGHT)
				continue;
			if (field[i][j] == BOMB)
				count++;
		}
	}
	return count;

}


//1. place numbombs randomly
//2. count surrounding bombs
//3. negate, subtract 1 (flag as hidden)
void SetupMineField(int field[ ][HEIGHT], int numbombs)
{
	int x,y;
	for (int i=0; i<numbombs; i++)
	{
		x = rand()%WIDTH;
		y = rand()%HEIGHT;

		if (field[x][y] == BOMB)
			i--;
		else
			field[x][y] = BOMB;
	}

	//count nearby bombs
	for (int y=0; y<HEIGHT; y++)
	{
		for (int x=0; x<WIDTH; x++)
		{
			if (field[x][y] == BOMB)
				continue;
			field[x][y] = CountAdjBombs(field, x, y);
		}
	}

	//hide field
	for (int y=0; y<HEIGHT; y++)
	{
		for (int x=0; x<WIDTH; x++)
		{
			field[x][y] = -field[x][y] - 1;
		}
	}
}

//double for loop, 
// print 'X' if value is < 0, 'B' if bomb,
// print value otherwise
void DisplayField(int field[ ][HEIGHT], int **aiField)
{
	cout << "  0 1 2 3 4 5 6 7 8 9 x\n";
	for (int y=0; y<HEIGHT; y++)
	{
		cout << y << " ";
		for (int x=0; x<WIDTH; x++)
		{
			if (field[x][y] < 0){
				cout << 'X';
				aiField[x][y] = -1;
			}
			else if (field[x][y] == BOMB)
				cout << 'B';
			else{
				cout << field[x][y];
				aiField[x][y] = field[x][y];
			}
			cout << " ";

		}
		cout << endl;
	}
	cout << "y \n";
}


// add one, flip sign to x, y coord
// return true if BOMB, false otherwise
bool Reveal(int field[][HEIGHT], int x, int y)
{
	//return if out of bounds entry
	if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
		return false;
	
	//reveal
	if (field[x][y] < 0)
		field[x][y] = -(field[x][y] + 1);

	//return true if bomb
	if (field[x][y] == BOMB)
		return true;
	return false;
}

int main()
{
	srand(time(0));
	int minefield[WIDTH][HEIGHT] = {0};
	int **displayedfield;					//pure int field, where an unmarked space is a -1, otherwise it is positive.
	int **dummyfield;						//This field is a blank field to display at the start of the game.
	int numbombs, x, y;
	int num_Victories = 0;					//Number of wins for this run of this program.
	int games_played = 0;					//Number of games played this run.
	double win_Ratio;						//ratio of wins to losses.
	bool gameover;
	bool victory;							//This is true when the number of unmarked spaces == numbombs.
	bool first_move;						//If this is the first move of the game, true.
	char again;
	Arms *sploder;							//This is my minesweeper solving AI. If it fails, it earns it's name.
	dummyfield = dummy_field(WIDTH, HEIGHT);

	do{
		srand(time(0));
	sploder = new Arms(WIDTH, HEIGHT);
	cout << "ten mines.\n";
	numbombs = 10;

	//place the bombs!
	//SetupMineField(minefield, numbombs);
	//displayedfield = AI_field(WIDTH, HEIGHT);
	//DisplayField(minefield, displayedfield);
	//sploder->ARMS_action(displayedfield, x, y);
	//redistribute_mines(minefield, numbombs, get_voidZone(x, y));		//Garantees first selection is a zero.
	//DisplayField(minefield, displayedfield);
	gameover = false;
	victory = false;
	first_move = true;
	while (!gameover && !victory)
	{
		if(first_move){
			displayedfield = AI_field(WIDTH, HEIGHT);
			//DisplayField(minefield, displayedfield);
			sploder->ARMS_action(dummyfield, x, y);
			redistribute_mines(minefield, numbombs, get_voidZone(x, y));		//Garantees first selection is a zero. Need to tweek this.
			//Reveal, and maybe splode.
			gameover = Reveal(minefield, x, y);
			DisplayField(minefield, displayedfield);
			first_move = false;
		}
		//ARMS sploder takes it's turn to look at board and
		//choose an 'x' and a 'y' coordinate. 
		sploder->ARMS_action(displayedfield, x, y);
		//Reveal, and maybe splode.
		gameover = Reveal(minefield, x, y);
		//check the currenty field for victory.
		victory = isWinner(displayedfield, numbombs);
		//show the field, I want to see the result of this action.
		DisplayField(minefield, displayedfield);
	}
	delete sploder;
	if(victory){
		cout << "Game Over!! YOU WON!\n";
		games_played++;
		num_Victories++;
	}
	if(!victory){
		cout << "Game Over!! you lost, better luck next time.\n";
		games_played++;
	}
	win_Ratio = num_Victories / static_cast<double>(games_played);
	cout << "Games played: " << games_played << endl
		<< "Games won: " << num_Victories << " Games lost: " << games_played - num_Victories << endl
		<< "Win ratio: " << win_Ratio << endl
		<< "Would you like to play again? (y/n):";
	cin >> again;
	}while(tolower(again) == 'y');
	cout << "Good bye.\n";
	return 0;
}

int **dummy_field(int width, int height){
	int **dumbfield;

	dumbfield = new int* [width];
	for(int i = 0; i < width; i++){
		dumbfield[i] = new int [height];
		for(int j = 0; j < width; j++){
			dumbfield[i][j] = -1;
		}
	}
	return dumbfield;
}

int **AI_field(int width, int height){
	int **ai_field;

	ai_field = new int* [width];
	for(int i = 0; i < width; i++){
		ai_field[i] = new int [height];
	}
	return ai_field;
}

bool isWinner(int **field, int numMines)
{
	int unmarked = 0;	//The number of unchosen elements on the field.

	//scan the field.
	for(int i = 0; i < WIDTH; i++){
		for(int j = 0; j < HEIGHT; j++){
			if(field[i][j] < 0)
				unmarked++;
		}
	}
	//if there are as many unmarked elements as there are mines,
	//then victory has been achieved.
	if(unmarked == numMines)
		return true;
	return false;
}

vector<Coord> get_voidZone(int x, int y)
{
	Coord temp;
	vector<Coord> adja_spac;

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

	return adja_spac;
}

void redistribute_mines(int field[][HEIGHT], int numbombs, vector<Coord> void_zone)
{
	int x,y;
	for (int i=0; i<numbombs; i++)
	{
		x = rand()%WIDTH;
		y = rand()%HEIGHT;

		
		if (field[x][y] == BOMB)
			i--;
		else{
			field[x][y] = BOMB;
			for(int j = 0; j < void_zone.size(); j++){
				if (x == void_zone[j].x && y == void_zone[j].y){
				field[x][y] = 0;
				i--;
				}
			}
		}
	}

	//count nearby bombs
	for (int y=0; y<HEIGHT; y++)
	{
		for (int x=0; x<WIDTH; x++)
		{
			if (field[x][y] == BOMB)
				continue;
			field[x][y] = CountAdjBombs(field, x, y);
		}
	}

	//for checking purposes only.
	cout << "  0 1 2 3 4 5 6 7 8 9 x\n";
	for (int y=0; y<HEIGHT; y++)
	{
		cout << y << " ";
		for (int x=0; x<WIDTH; x++)
		{
			if (field[x][y] < 0){
				cout << 'X';
			}
			else if (field[x][y] == BOMB)
				cout << 'B';
			else{
				cout << field[x][y];
			}
			cout << " ";

		}
		cout << endl;
	}
	cout << "y \n";

	//hide field
	for (int y=0; y<HEIGHT; y++)
	{
		for (int x=0; x<WIDTH; x++)
		{
			field[x][y] = -field[x][y] - 1;
		}
	}
}