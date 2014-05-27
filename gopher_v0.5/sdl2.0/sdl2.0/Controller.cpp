#include "Controller.h"
#include "Agent_AI.h"
#include <iostream>
#include <exception>
#include <ctime>



//add your code here!
int main(void* data)
{
	Shmem* shmem = (Shmem*)data;

	int player = shmem->player;

	int grid_w = shmem->grid_w;
	int grid_h = shmem->grid_h;

	char* grid = shmem->grid;

	Agent_AI *link;
	link = new Agent_AI(grid_h, grid_w, player, 4);
	/*
	//these values update, so keep a pointer to them
	int score;

	Uint32 last_update;
	int events;	
		
	int action;	
	*/
	int delay = INPUTWAITTIME+10;
	Delay(delay);

	//Uint32* prev_time = &shmem->last_update;
	Uint32  time = GetTime();

	int num = 2;
	int zero = 0;
	int* ptr = NULL;

	//your code will essentially start here, 
	while(!shmem->quit)
	{
		/*link->listen_mem(shmem->events);*/
		/*shmem->action = rand()%4 + 1;*/
		link->agent_act(shmem);
		link->listen_mem(shmem->events);
		//wait on deciding next move until **this** move has been read
		shmem->read = false;
		while (!shmem->read && !shmem->quit) {link->listen_mem(shmem->events);}
	}

	
	link->~Agent_AI();
	return 0;
}

// simple random movement
int wander(void* data)
{
	Shmem* shmem = (Shmem*)data;
	srand(time(NULL)*shmem->player);

	//your code will essentially start here, 
	while(!shmem->quit)
	{
		shmem->action = rand()%4 + 1;
		//wait on deciding next move until **this** move has been read
		shmem->read = false;
		while (!shmem->read && !shmem->quit) {}
	}
	return 0;
}