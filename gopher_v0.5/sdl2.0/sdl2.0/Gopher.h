#include "Game.h"
#include "Controller.h"
#include <vector>

#pragma once

//types of objects that can occupy a grid location
// support up to 8 types


class Gopher : public Game
{
public:
	Gopher(void);
	~Gopher(void);

	bool Init();
	bool Update();
	void Close();

protected:
	void SetShmemGrid(bool thread=false);

	void RenderScreen();
	void PrintGUI();

	void ParseControls();
	void ParseLogic();

	void GetGrid(int player, int& x, int& y);
	int  GetGridIndex(int x, int y);
	int  GetPosIndex(float x, float y);

	void GetIndexPos(int i, int& x, int& y);

	int Occupied(int x, int y, int ignore=0);
	void SetGridObj(int, int, int=-1, int=0, int=0);
	void ClearGridObj(int, int, int=0, int=0);

	//events
	void SetFlash(int i_player); //also does cheer
	void SetHear( );

	void LoadLevel();
	void LoadPlayers(std::string);
	void LoadBackground(std::string);

	//map stuff
	char	m_grid[16*16];
	int		m_grid_w;
	int		m_grid_h;

	//flag placement
	std::vector<int> m_flag_loc;

	// counters
	int		m_active_player;
	int		m_last_event;
	std::string m_latest_move;

	int		m_num_players;
	int		m_num_flags;
	int		m_num_active_flags;
	int		m_score[MAXOBJS];

	// controls
	int			 m_control_wait;
	SDL_Thread** m_threads;
	Shmem*		 m_shmem;
};

