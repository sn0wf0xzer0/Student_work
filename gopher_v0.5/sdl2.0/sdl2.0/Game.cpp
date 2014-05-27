#include "Game.h"
#include <fstream>

SDL_Renderer* Game::m_renderer = NULL;

Game::Game(void) : m_bgoffset()
{
	m_window = NULL;
	m_renderer = NULL;
	m_background = NULL;
	
	m_num_objs = 0;
	for (int i=0; i<MAXOBJS; i++)
		m_objs[i] = NULL;
	
	for (int i=0; i<NUMKEYS; i++)
		m_keys[i] = false;
}


Game::~Game( )
{
} 


/*
	Starts up SDL, creates window, binds render object to window
	w		- width of window
	h		- height of window
	x		- x-location of window on screen (default 0)
	y		- y-location of window on screen (default 0)
	return true on success, false otherwise
*/
bool Game::Init()
{
	if( SDL_Init( SDL_INIT_EVERYTHING) < 0 )
		return false;

	//m_window = SDL_CreateWindow( "SDL Startup", SCREEN_X, SCREEN_Y, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
	m_window = SDL_CreateWindow( "Gopher Challenge", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
	if( m_window == NULL )
		return false;

	m_renderer = SDL_CreateRenderer( m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if ( m_renderer == NULL)
		return false;

	GameObject::SetRenderer( m_renderer );

	//initialize png support, may also add IMG_INIT_PNG | IMG_INIT_JPG | IMG_INIT_TIF
	if ( IMG_Init( IMG_INIT_PNG ) == 0 )
		return false;

	if ( FontRender::Init("fonts/arial.ttf", m_renderer, 20) == 0)
		return false;

	LoadLevel();

	m_prev_time = SDL_GetTicks();

	return true;
}


/*
	clears screen, puts all textures on screen, render screen
	17ms delay gives approximately 60 frames per second
*/
bool Game::Update()
{
	if ( !KeyUpdate() )
		return false;

	ParseControls();
	RenderScreen();

	//Wait 17ms ~ 60fps
	SDL_Delay( 17 );

	return true;
}


/*
	destroys all textures loaded, closes window/renderer, quits sdl
*/
void Game::Close()
{
	for (int i=0; i<m_num_objs; i++)
	{
		if (m_objs[i])
			delete m_objs[i];
	}

	//Destroy renderer
	SDL_DestroyRenderer( m_renderer );
	m_renderer = NULL;

	//Destroy window
	SDL_DestroyWindow( m_window );
	m_window = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
	FontRender::Quit();
}

/*
	goes through each key pressed, applying controls
*/
void Game::ParseControls()
{
	Vector2 dir = {0,0};

	if (m_keys[UP])
		dir.y -= 10;
	if (m_keys[DOWN])
		dir.y += 10;
	if (m_keys[LEFT])
		dir.x -= 10;
	if (m_keys[RIGHT])
		dir.x += 10;

	for (int i=0; i<m_num_objs; i++)
		m_objs[i]->Move(dir.x, dir.y);

}

/*
	clears screen, places everything on screen, render screen
*/
void Game::RenderScreen()
{
	//clear screen
	SDL_RenderClear( m_renderer );

	//populate with images
	SDL_RenderCopy( m_renderer, m_background, NULL, &m_bgoffset);
	for (int i=0; i<m_num_objs; i++)
	{
		m_objs[i]->Update(0);
	}

	//update screen
	SDL_RenderPresent( m_renderer );
}


/*
	reads in all key events
	returns false if any key event shuts down the program (e.g. ESC, QUIT)
	return true otherwise
*/
bool Game::KeyUpdate()
{
	SDL_Event e;
	while ( SDL_PollEvent(&e) )
	{
		switch (e.type)
		{
		case SDL_QUIT:
			return false;
			break;

		case SDL_KEYDOWN:
			if ( !KeyDown(e.key.keysym.sym) )
				return false;
			break;

		case SDL_KEYUP:
			if ( !KeyUp(e.key.keysym.sym) )
				return false;
			break;
		}
	}

	//calculate dt, probably should be here but wutevs!!
	Uint32 curr_time = SDL_GetTicks();
	m_dt = curr_time - m_prev_time;
	m_prev_time = curr_time;

	return true;
}

/*
	takes in a key press, sets the corresponding key binding to true
*/
bool Game::KeyDown(SDL_Keycode key)
{
	switch (key)
	{
	case SDLK_ESCAPE:
		return false;
		break;

	case SDLK_UP:
	case 'w':
		m_keys[UP] = true;
		break;

	case SDLK_DOWN:
	case 's':
		m_keys[DOWN] = true;
		break;

	case SDLK_LEFT:
	case 'a':
		m_keys[LEFT] = true;
		break;

	case SDLK_RIGHT:
	case 'd':
		m_keys[RIGHT] = true;
		break;
	}

	return true;
}

/*
	takes in a key press, sets the corresponding key binding to false
*/
bool Game::KeyUp(SDL_Keycode key)
{
	switch (key)
	{
	case SDLK_ESCAPE:
		return false;
		break;

	case SDLK_UP:
	case 'w':
		m_keys[UP] = false;
		break;

	case SDLK_DOWN:
	case 's':
		m_keys[DOWN] = false;
		break;

	case SDLK_LEFT:
	case 'a':
		m_keys[LEFT] = false;
		break;

	case SDLK_RIGHT:
	case 'd':
		m_keys[RIGHT] = false;
		break;
	}

	return true;
}


/*
	loads up players, loads up background
*/
void Game::LoadLevel()
{
	LoadPlayers("players.txt");
	LoadBackground("map1.txt");
}


/*
	reads file to load up all player assets
*/
void Game::LoadPlayers(std::string filename)
{
	std::ifstream file;
	file.open(filename.c_str());
	if (file.fail())
		exit(400);

	file >> m_num_objs;

	std::string name, sprite_file;
	float x, y;
	for (int i=0; i<m_num_objs; i++)
	{
		file >> name >> sprite_file;
		file >> x >> y;

		m_objs[i] = new GameObject;
		m_objs[i]->SetName(name);
		m_objs[i]->SetImage(sprite_file);
		m_objs[i]->SetPosition(x,y);
	}
}


/*
	loads up an RLE file that specifies the tileset and background
	stores resulting texture in m_background
*/
void Game::LoadBackground(std::string rlefile)
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