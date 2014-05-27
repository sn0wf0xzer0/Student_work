#include <SDL_thread.h>
#include <fstream>
#include <iostream>
#include <ctime>
#include <Windows.h>
#include "Gopher.h"

int main(void* data);
int wander(void* data);

int (*ctrl_func[2])(void*) = {main, wander};



Gopher::Gopher( ) : Game()
{
	m_active_player = 0;
	m_timer = 0;
	m_dt = 0;
	m_latest_move = "START!!";

	m_control_wait = INPUTWAITTIME;
	m_num_players = 0;
	m_num_flags = 0;
	m_num_active_flags = 0;
	memset(m_score, 0, MAXOBJS*sizeof(int));
	
	//random seed
	srand(time(0));
}


Gopher::~Gopher()
{

}


bool Gopher::Init()
{
	bool sucess = Game::Init();

	std::string name;
	m_shmem = new Shmem[m_num_players];
	m_threads = new SDL_Thread*[m_num_players];


	SetShmemGrid(true);

	/*
	//process based controls, under development
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory( &si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory( &pi, sizeof(pi));

	TCHAR argv[10];
	LPCWSTR process = L"C:\\Windows\\System32\\notepad.exe";
	CreateProcess(process, NULL, NULL, NULL, false, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);

	//if ( !CreateProcess(NULL, argv, NULL, NULL, false, 0, NULL, NULL, &si, &pi) )
	//	exit(123);

	WaitForSingleObject( pi.hProcess, 1000);
	CloseHandle( pi.hProcess );
	CloseHandle( pi.hThread );
	*/
	return sucess;
}

/*
	clears screen, puts all textures on screen, render screen
	17ms delay gives approximately 60 frames per second
*/
bool Gopher::Update()
{
	if ( !KeyUpdate() )
		return false;

	
	//only get input every 5 seconds (5000ms)
	m_timer += m_dt;
	if (m_timer >= m_control_wait)
	{
		std::cout << "Player " << m_active_player+1 << "'s turn" << std::endl;
		ParseControls();
		//m_shmem[m_active_player].last_update = m_prev_time;

		//assigns next player
		m_active_player++;
		m_active_player %= m_num_players;

		m_timer = 0;
	}

	ParseLogic();
	RenderScreen();

	//Wait 17ms ~ 60fps
	SDL_Delay( 17 );

	return true;
}

void Gopher::Close()
{
	std::cout << "Shutting Down... " << std::endl;
	for (int i=0; i<m_num_players; i++)
	{
		m_shmem[i].quit = true;
		SDL_WaitThread(m_threads[i],NULL);
		std::cout << "Closing thread " << i << std::endl;
		//SDL_KillThread(m_thread[i]);
	}


	Game::Close();
}

//sets/resets all shmem grids
void Gopher::SetShmemGrid(bool thread)
{
	std::string name;
	for (int i=0; i<m_num_players; i++)
	{
		if (thread)
			m_shmem[i].grid = new char[m_grid_w*m_grid_h];
		
		memcpy(m_shmem[i].grid, m_grid, sizeof(char)*m_grid_w*m_grid_h);
		
		//thread based controls
		m_shmem[i].quit = false;
		GetGrid(i, m_shmem[i].x, m_shmem[i].y);
		m_shmem[i].grid_w = m_grid_w;
		m_shmem[i].grid_h = m_grid_h;
		m_shmem[i].score  = m_score[i];
		//m_shmem[i].last_update = SDL_GetTicks();
		m_shmem[i].events = EVENT::HEAR;
		
		m_shmem[i].action = KEYS::NONE;
		m_shmem[i].player = i+1;
		
		//only use for the initial setup of the grids
		if (thread)
		{
			name = "Player" + toString(i+1);
			int f = i > 0 ? 1 : 0;
			m_threads[i] = SDL_CreateThread(ctrl_func[f], name.c_str(), &m_shmem[i]);
		}
	}
}


void Gopher::RenderScreen()
{
	//clear screen
	SDL_RenderClear( m_renderer );

	//populate with images
	SDL_RenderCopy( m_renderer, m_background, NULL, &m_bgoffset);
	for (int i=0; i<m_num_objs; i++)
	{
		m_objs[i]->Update(0);
	}

	PrintGUI();

	//update screen
	SDL_RenderPresent( m_renderer );
}

void Gopher::PrintGUI()
{
	//print latest action
	FontRender::RenderText(m_latest_move, 0, SCREEN_HEIGHT - 20);

	//print scores
	std::string msg = "Score:    ";
	std::string str;
	for (int i=0; i<m_num_players; i++)
	{
		str = "P" + toString(i+1) + ": " + toString(m_score[i]);
		for (int j=str.length(); j<10; j++)
			str += ' ';
		msg += str + "  ";
	}
	
	SDL_Texture* text = FontRender::RenderText(msg, 0, SCREEN_HEIGHT-SCREEN_BOARDER, WHITE);

	//print time remaining
	msg = "Time: " + toString(SDL_GetTicks()/1000);
	int x = SCREEN_WIDTH/2 - msg.length()*8;
	FontRender::RenderText(msg, x, SCREEN_HEIGHT-SCREEN_BOARDER + 20, WHITE);
}

void Gopher::ParseLogic()
{
	int fx, fy;		//flag x,y
	int px, py;		//player x,y
	Vector2 pos;

	//go through all active flags
	for (int f=m_num_players; f<m_num_objs; f++)
	{
		if ( !m_objs[f]->m_active )
			continue;
		fx = (int)m_objs[f]->m_pos.x / TILE_W;
		fy = (int)m_objs[f]->m_pos.y / TILE_H;

		//see if it is occupied by a player
		if ( Occupied(fx, fy) & EVENT::SMELL )
		{
			//go through players
			for (int p=0; p<m_num_players; p++)
			{
				GetGrid(p, px, py);
				//player and flag in same loc
				if (px == fx && py == fy)
				{
					m_score[p]++;
					SetFlash(p);		//event: send out capture
					SetShmemGrid();		//resent grids
					break;
				}
			}

			ClearGridObj(fx, fy, 0, 1);
			m_objs[f]->m_active = false;

		}
	}

	//go through all inactive flags
	for (int f=m_num_players; f<m_num_objs; f++)
	{
		if ( m_objs[f]->m_active )
			continue;
		int r = rand() % m_flag_loc.size();
		int x,y;

		GetIndexPos(m_flag_loc[r], x, y);
		m_objs[f]->SetPosition(x, y);
		m_objs[f]->m_active = true;
		SetGridObj(x/TILE_W, y/TILE_H, -1, 0, 1);
		SetHear();		//event: send out placement
	}
}


void Gopher::ParseControls()
{
	int dx = 0;
	int dy = 0;
	int tx, ty;
	Vector2 dir = {0,0};
	//m_active_player = 0;
	GetGrid(m_active_player, tx, ty);
	//GetGrid(0, tx, ty);

	/*
	//move character
	if (m_keys[UP])
	{
		dy--;
		m_keys[UP] = false;
	}
	if (m_keys[DOWN])
	{
		dy++;
		m_keys[DOWN] = false;
	}
	if (m_keys[LEFT])
	{
		dx--;
		m_keys[LEFT] = false;
	}
	if (m_keys[RIGHT])
	{
		dx++;
		m_keys[RIGHT] = false;
	}
	*/
	
	//skip read if already read
	if (m_shmem[m_active_player].read)
		return;

	int action = m_shmem[m_active_player].action;
	//m_shmem[m_active_player].read = true;
	std::string msg = "Player" + toString(m_active_player+1);

	
	float roll = rand()%100 + 1;
	if (roll < P_MOVE)
	{
		switch(action)
		{
		case UP:
			msg += " moves up";
			dy--;
			break;

		case DOWN:
			msg += " moves down";
			dy++;
			break;

		case LEFT:
			msg += " moves left";
			dx--;
			break;

		case RIGHT:
			msg += " moves right";
			dx++;
			break;

		default:
			msg += " skips move";
		}
	}
	else
	{
		msg += " failed to move";
		m_shmem[m_active_player].read = true;
		return;
	}
	m_latest_move = msg;

	//make sure he doesn't hit stuff
	if (dx || dy)
	{	
		int o = Occupied(tx+dx, ty+dy);
		if ( o == 0 || o == EVENT::TWINKLE )
		{
			dir.x = dx*TILE_W;
			dir.y = dy*TILE_H;
			m_objs[m_active_player]->Move(dir.x, dir.y);
			
			ClearGridObj(tx, ty, m_active_player+1, 0);
			SetGridObj(tx+dx, ty+dy, -1, m_active_player+1, 0);
			tx += dx;
			ty += dy;
		}
		

		//cout console info
		for (int i=0; i<m_grid_w*m_grid_h; i++)
		{
			if (i%m_grid_w == 0)
				std::cout << std::endl;
			switch( m_grid[i] )
			{
			case OBSTICLE:
				std::cout << "O ";
				break;
			case FLAG:
				std::cout << "F ";
				break;
			case P1:
				std::cout << "1 ";
				break;
			case P2:
				std::cout << "2 ";
				break;
			case P3:
				std::cout << "3 ";
				break;
			case P4:
				std::cout << "4 ";
				break;
			default:
				std::cout << (int)m_grid[i] << " ";
			}
			
		}
		//output stuff
		std::cout << std::endl;
		std::cout << "Score: ";
		for (int i=0; i<m_num_players; i++)
		{
			std::cout << "\tPlayer" << i+1 << ": " << m_score[i];
		}
			std::cout << std::endl;

	}

	
	//clear, reset senses every move
	m_shmem[m_active_player].events &= (~FEEL) & (~SMELL) & (~TWINKLE);
	for (int x=tx-1; x<=tx+1; x++)
	{
		for (int y=ty-1; y<=ty+1; y++)
		{
			if (x == tx && y == ty)
				continue;
			m_shmem[m_active_player].events |= Occupied(x, y); 
		}
	}
	
	m_shmem[m_active_player].read = true;

}

/*
	converts player x,y position to tile coordinates
*/
void Gopher::GetGrid(int player, int& x, int& y)
{
	int i = player;

	Vector2 pos = m_objs[i]->GetPosition();
	x = (int)pos.x / TILE_W;
	y = (int)pos.y / TILE_H;

}

int Gopher::GetPosIndex(float px, float py)
{
	int x = (int)px / TILE_W;
	int y = (int)py / TILE_H;

	return y*m_grid_w + x;
}

int Gopher::GetGridIndex(int x, int y)
{
	return y*m_grid_w + x;
}

void Gopher::GetIndexPos(int i, int& x, int& y)
{
	x = (i % m_grid_w) * TILE_W;
	y = (i / m_grid_w) * TILE_H;
}


/*
	returns an event depending what objects are in current x,y loc
	returns 0 otherwise
*/
int Gopher::Occupied(int x, int y, int ignore)
{
	//out of bounds, return feel
	if (x < 0 || x >= m_grid_w || y < 0 || y >= m_grid_h)
		return EVENT::FEEL;

	int i = GetGridIndex(x,y);

	//nothing there
	if (m_grid[i] == 0)
		return NONE;

	int events = 0;
	//wall
	if (m_grid[i] & OBSTICLE)
		events |= EVENT::FEEL;
	//flag
	if (m_grid[i] & FLAG)
		events |= EVENT::TWINKLE;
	//player, ignoring this player
	if (m_grid[i] & (~OBSTICLE) & (~FLAG))
		events |= SMELL;
	return events;
}

/*
	tags a specific grid location with an obsticle, a player, or flag
*/
void Gopher::SetGridObj(int x, int y, int tile, int player, int flag)
{
	int i = GetGridIndex(x,y);
	char bits = 0;
	
	//lists which tiles are obsticles
	switch (tile)
	{
	case 0:
		bits |= OBSTICLE;
		break;
	}

	//tag which grid locations have players there
	switch (player)
	{
	case 1:
		bits |= P1;
		break;
	case 2:
		bits |= P2;
		break;
	case 3:
		bits |= P3;
		break;
	case 4:
		bits |= P4;
		break;
	}

	//tag which grid locations have flag there
	if (flag)
		bits |= FLAG;

	m_grid[i] |= bits;
}

/*
	removes moveable objects from a grid location
*/
void Gopher::ClearGridObj(int x, int y, int player, int flag)
{
	int i = GetGridIndex(x,y);
	if (player)
	{
		switch (player)
		{
		case 1:
			m_grid[i] &= ~P1;
			break;
		case 2:
			m_grid[i] &= ~P2;
			break;
		case 3:
			m_grid[i] &= ~P3;
			break;
		case 4:
			m_grid[i] &= ~P4;
			break;
		}
	}

	if (flag)
		m_grid[i] &= ~FLAG;
}


void Gopher::LoadLevel()
{
	//Game::LoadLevel();
	LoadBackground("map1.txt");
	LoadPlayers("players.txt");
}



/*
	reads file to load up all player assets
*/
void Gopher::LoadPlayers(std::string filename)
{
	std::ifstream file;
	file.open(filename.c_str());
	if (file.fail())
		exit(400);

	file >> m_num_objs;

	std::string name, sprite_file;
	int tile_x, tile_y;
	int x, y;

	for (int i=0; i<m_num_objs; i++)
	{
		file >> name >> sprite_file;
		file >> tile_x >> tile_y;

		x = tile_x*TILE_W;
		y = tile_y*TILE_H;

		//set x,y grid location tile, player, flag bits
		if (name.find("Player") != std::string::npos)
		{
			SetGridObj(tile_x, tile_y, -1, i+1, 0);
			m_num_players++;
		}
		else if (name.find("rupee") != std::string::npos)
		{
			SetGridObj(tile_x, tile_y, -1, 0, 1);
			m_num_flags++;
		}

		m_objs[i] = new GameObject;
		m_objs[i]->SetName(name);
		m_objs[i]->SetImage(sprite_file);
		m_objs[i]->SetPosition(x,y);
	}
}


/*
	loads up an RLE file that specifies the tileset and background
	stores resulting texture in m_background, also loads up info about grid
*/
void Gopher::LoadBackground(std::string rlefile)
{
	//tileset surface, background surface
	SDL_Surface* tileset;
	SDL_Surface* bg;
	
	int width, height, tile, freq=-1, rep;
	int tpr;	//tiles per row
	SDL_Rect clip = {0,0,TILE_W, TILE_H};
	SDL_Rect offset = {0,0,0,0};
	std::string bgfile;

	//open rle file
	std::ifstream file;
	file.open(rlefile.c_str());
	if (file.fail())
		exit(403);

	//read header info
	file >> width >> height;
	file >> bgfile;

	//open tileset, extract tiling info from it
	tileset = IMG_Load( bgfile.c_str() );
	if (tileset == NULL)
		exit(401);
	tpr = tileset->w / TILE_W;

	// * * specific to gopher * * //
	//m_grid = new char[width*height];
	m_grid_w = width;
	m_grid_h = height;
	memset(m_grid, 0, width*height*sizeof(char));

	//generate background image
	bg = SDL_CreateRGBSurface(0, width*TILE_W, height*TILE_H,32,0,0,0,0);

	//read body info, apply tiles
	for(int y=0; y<height; y++)
	{
		for (int x=0; x<width; x += rep)
		{
			//read in new tile/freq (prev tile multiple)
			if (freq < 0)
				file >> tile;
			//read in just freq, move current value to tile (prev tile single)
			else
				tile = freq;
			file >> freq;
			
			//indicating how many times tile repeats, at least 1
			rep = freq < 0 ? -freq : 1;

			for (int i=0; i<rep; i++)
			{
				clip.x = (tile%tpr)*TILE_W;
				clip.y = (tile/tpr)*TILE_H;
				SDL_BlitSurface(tileset, &clip, bg, &offset);

				offset.x += TILE_W;

				// ** gopher addition ** //
				SetGridObj(x+i, y, tile);
				if ( !Occupied(x+i, y) )
					m_flag_loc.push_back( GetGridIndex(x+i, y) );
			}
			
		}
		offset.y += TILE_H;
		offset.x = 0;
	}

	//convert bg into a texture, save w,h
	m_background = SDL_CreateTextureFromSurface(m_renderer, bg);
	m_bgoffset.w = width *TILE_W;
	m_bgoffset.h = height*TILE_H;

	SDL_FreeSurface(bg);
}




void Gopher::SetFlash(int i_player)
{
	m_shmem[i_player].events |= EVENT::CHEER;

	for (int i=0; i<m_num_players; i++)
	{
		m_shmem[i].events |= EVENT::FLASH;

	}
}

void Gopher::SetHear( )
{
	for (int i=0; i<m_num_players; i++)
	{
		m_shmem[i].events |= EVENT::HEAR;
	}
}