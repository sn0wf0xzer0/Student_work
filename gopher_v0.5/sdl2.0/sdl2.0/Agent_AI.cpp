//Implementation file for Agent_AI class.
//Author: John Palacios.

#include "Agent_AI.h"
using namespace std;

Agent_AI::Agent_AI()
{
	num_players = 4;
	my_player = 0;
	my_score = 0;
	round = 0;
	for(int i = 0; i < 6; i++){	sense_data.notif[i] = false;	}
	//This is assuming row, then collum order to the field.
	field_hight = 16;
	field_width = 16;
	/*play_field = new char* [field_hight];
	for(int i = 0; i < field_hight; i++){
		play_field[i] = new char [field_width];
	}*/
	player_token = new char [num_players];
	for(int i = 0; i < num_players; i++)
		player_token[i] = 1<<(i + 2);
	path_coords = new vector<Coord> [num_players];
}

Agent_AI::Agent_AI(int h, int w, int player, int players)
{
	num_players = players;
	my_player = player;
	my_score = 0;
	round = 0;
	for(int i = 0; i < 6; i++){	sense_data.notif[i] = false;	}
	//This is assuming row, then collum order to the field.
	field_hight = h;
	field_width = w;
	/*play_field = new char* [field_hight];
	for(int i = 0; i < field_hight; i++){
		play_field[i] = new char [field_width];
	}*/
	player_token = new char [num_players];
	for(int i = 0; i < num_players; i++)
		player_token[i] = 1<<(i + 2);
	path_coords = new vector<Coord> [num_players];
}

Agent_AI::~Agent_AI()
{
	/*for(int i = 0; i < field_hight; i++){
		delete [] play_field;
	}*/
	//delete [] play_field;
	delete [] player_token;
	delete [] path_coords;
}

void Agent_AI::agent_act(Shmem *feed)
{	
	if(round == 0 || sense_data.notif[4] || sense_data.notif[5]){
		sense_data.notif[4] = false;
		sense_data.notif[5] = false;
	//Look at current state of map.
	read_field(feed->grid);		
	//deletes player locals... must only be called when new field.
	//Glean useful info from map.
	get_locals();
	//decide where to go.
	choose_goal();			//If goal is unobstructed, this is goal,
							//else is waypoint to navigate around
							//obstruction.
	}
	choose_action(feed->action);
}

void Agent_AI::read_field(char *field)
{
	//int count = 0;
	//for(int i = 0; i < field_hight; i++){
	//	for(int j = 0; j < field_width; j++){
	//		/*play_field[i][j]*/ = field[count];
	//		count++;
	//		//cout << play_field[i][j];
	//	}
	//	//cout << endl;
	//}
	play_field = field;
}

void Agent_AI::get_locals()
{
	//Retrieve important locations from play_field.
	//Important info: all encoded in binary.
	//obsticle = 1, flag = b10(2),
	//p1 = 100(4), p2 = 1000(8), p3 = 10000(16),
	//p4 = 100000(32).
	Coord temp;
	int goal		= 0;
	int count		= 0;
	if(player_locals.size() > 0)
		player_locals.clear();

	for(int i = 0; i < field_hight; i++){
		for(int j = 0; j < field_width; j++){
			if((OBSTICLE & play_field[i*field_width + j]) == OBSTICLE || play_field[i*field_width + j] == 0)
				continue;
			if((FLAG & play_field[i*field_width + j]) == FLAG){
				temp.x = j;
				temp.y = i;
				goal_locals[goal] = temp;
				goal++;
			}
			for(int k = 0; k < num_players; k++){
				if((player_token[k] & play_field[i*field_width + j]) == player_token[k]){
					temp.x = j;
					temp.y = i;
					player_locals.push_back(temp);
					break;
				}
			}
		}
	}
}

void Agent_AI::listen_mem(int message)
{
//	  #define LOG_ERRORS            1  // 2^0, bit 0
//    #define LOG_WARNINGS          2  // 2^1, bit 1
//    #define LOG_NOTICES           4  // 2^2, bit 2
//    #define LOG_INCOMING          8  // 2^3, bit 3
//    #define LOG_OUTGOING         16  // 2^4, bit 4
//    #define LOG_LOOPBACK         32  // and so on...
//
//// Only 6 flags/bits used, so a char is fine
//unsigned char flags;
//
//// initialising the flags
//// note that assignming a value will clobber any other flags, so you
//// should generally only use the = operator when initialising vars.
//flags = LOG_ERRORS;
//// sets to 1 i.e. bit 0
//
////initialising to multiple values with OR (|)
//flags = LOG_ERRORS | LOG_WARNINGS | LOG_INCOMING;
//// sets to 1 + 2 + 8 i.e. bits 0, 1 and 3
//
//// setting one flag on, leaving the rest untouched
//// OR bitmask with the current value
//flags |= LOG_INCOMING;
//
//// testing for a flag
//// AND with the bitmask before testing with ==
//if ((flags & LOG_WARNINGS) == LOG_WARNINGS)
//   ...
//
//// testing for multiple flags
//// as above, OR the bitmasks
//if ((flags & (LOG_INCOMING | LOG_OUTGOING))
//         == (LOG_INCOMING | LOG_OUTGOING))
//   ...
//
//// removing a flag, leaving the rest untouched
//// AND with the inverse (NOT) of the bitmask
//flags &= ~LOG_OUTGOING;
//
//// toggling a flag, leaving the rest untouched
//flags ^= LOG_LOOPBACK;

	//determine player/game status from shmem->events.
	//all events encoded in binary.
	
	if((FLASH & message) == FLASH)
	{	sense_data.notif[4] = true;	}
	if((CHEER & message) == CHEER)
	{	sense_data.notif[5] = true;	}
}

void Agent_AI::choose_goal()
{
	//choose closest flag with fewest closer players.

	int dist_to_goal[2];				//my distance to each goal.
	int num_closer[2]		= {0, 0};	//number of players closer to goal i.
	int count				= 0;		//sentry value.
	Coord temp;
	temp = player_locals[my_player - 1];
	//get my distance to the goals.
	do{
	dist_to_goal[count] = get_distance(goal_locals[count], temp);
	count++;
	} while (count < 2);
	count = 0;

	//find the number of players closer to each goal than me.
	for(int i = 0; i < 2; i++){
		for(int j = 0; j < num_players; j++){
			if(j != my_player - 1){
				count = get_distance(goal_locals[i], player_locals[j]);
				if(count < dist_to_goal[i]){
					num_closer[i]++;
				}
			}
		}
	}

	//choose goal with fewest players around it.
	//if equal, choose closest goal.
	//if equal, choose goal 1.
	if(num_closer[0] != num_closer[1]){
		current_dest = goal_locals[least(num_closer[0], num_closer[1])];
	}
	else if(num_closer[0] == num_closer[1]){
		current_dest = goal_locals[least(dist_to_goal[0], dist_to_goal[1])];
	}

}

int Agent_AI::get_distance(Coord a, Coord b)
{
	//"Manhattan" distance for comparison purposes.
	int distance = abs(a.x - b.x) + abs(a.y - b.y);
	return distance;
}

void Agent_AI::choose_action(int &action)
{
	//action is 0..3 where each is none, up, down, left or right.
	//enum KEYS {NONE, UP, DOWN, LEFT, RIGHT};
	//action = rand()%4 + 1;
	if(path_coords[my_player - 1].size() == 0){
	int least_dist	= 32;
	int direction	= 0;
	get_adjacent(player_locals[my_player]);
	
	for(int i = 0; i < adjacent_spaces.size(); i++){
		if(get_distance(adjacent_spaces[i], current_dest) < least_dist){
			least_dist = get_distance(adjacent_spaces[i], current_dest);
			direction = adjacent_spaces[i].dir;
			player_locals[my_player - 1] = adjacent_spaces[i];
		}
	}
	action = direction;
	round++;
	build_path(player_locals[my_player - 1]);
	}
	else{
		action = path_coords[my_player - 1].front().dir;
		path_coords[my_player - 1].erase(path_coords[my_player - 1].begin());
	}
}

int Agent_AI::least(int a, int b)
{
	//used to return array index of smaller of two
	//int values. default 0.
	if(b > a)
		return 1;
	else
		return 0;
}

void Agent_AI::get_adjacent(Coord current)
{
	Coord temp[2];

	//Only spaces adjacent to the current agent location are to be returned.
	if(adjacent_spaces.size() != 0){
		adjacent_spaces.clear();
	}

	//find the up and down spaces.
	temp[0].y = player_locals[my_player - 1].y - 1;
	temp[0].x = player_locals[my_player - 1].x;
	temp[0].dir = 1;
	temp[1].y = player_locals[my_player - 1].y + 1;
	temp[1].x = player_locals[my_player - 1].x;
	temp[1].dir = 2;
	for(int i = 0; i < 2; i++){
		if(temp[i].y < field_hight && temp[i].y > -1){
			if(play_field[temp[i].y * field_width + temp[i].x] != 1){
				adjacent_spaces.push_back(temp[i]);
			}
		}
	}
	//find left and right.
	temp[0].y = player_locals[my_player - 1].y;
	temp[0].x = player_locals[my_player - 1].x - 1;
	temp[0].dir = 3;
	temp[1].y = player_locals[my_player - 1].y;
	temp[1].x = player_locals[my_player - 1].x + 1;
	temp[1].dir = 4;
	for(int i = 0; i < 2; i++){
		if(temp[i].x < field_width && temp[i].x > -1){
			if(play_field[temp[i].y * field_width + temp[i].x] != 1){
				adjacent_spaces.push_back(temp[i]);
			}
		}
	}
}

void Agent_AI::build_path(Coord hyp_current)
{
	//select adjacent space that is closest to destination,
	//do this untill the closest adjacent space is our
	//destination.
	int least_dist = 32;		//any adjacent space will be closer.
	/*
	Coord hyp_current = player_locals[my_player - 1];
	Coord least_temp;
	do{
		get_adjacent(hyp_current);

		for(int i = 0; i < adjacent_spaces.size(); i++){
			if(get_distance(adjacent_spaces[i], current_dest) < least_dist){
				least_dist = get_distance(adjacent_spaces[i], current_dest);
				least_temp = adjacent_spaces[i];
			}
		}
		hyp_current = least_temp;
		path_coords[my_player - 1].push_back(hyp_current);
	}while (hyp_current.x != player_locals[my_player - 1].x
		&& hyp_current.y != player_locals[my_player - 1].y);*/
	//Terminate recursion if this is the destination.
	if(hyp_current.x == current_dest.x &&
		hyp_current.y == current_dest.y)
		return;
	else{
		//Determine safe adjacent spaces, each with a direction.
		get_adjacent(hyp_current);

		//find the one closest to the goal.
		for(int i = 0; i < adjacent_spaces.size(); i++){
			if(get_distance(adjacent_spaces[i], current_dest) < least_dist){
				least_dist = get_distance(adjacent_spaces[i], current_dest);
				hyp_current = adjacent_spaces[i];
			}
		}
		path_coords[my_player - 1].push_back(hyp_current);
		player_locals[my_player - 1] = hyp_current;
		//now find the next.
		build_path(player_locals[my_player - 1]);
	}
}