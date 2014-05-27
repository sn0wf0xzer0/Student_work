#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include "GameObject.h"
#include "FontRender.h"

#pragma once



class Game
{
public:
	Game( );
	virtual ~Game( );

	virtual bool Init();
	virtual bool Update();
	virtual void Close();

protected:
	virtual void ParseControls();
	virtual void RenderScreen();

	bool KeyUpdate();
	bool KeyDown(SDL_Keycode);
	bool KeyUp(SDL_Keycode);

	virtual void LoadLevel();
	virtual void LoadPlayers(std::string);
	virtual void LoadBackground(std::string);

	//timer stuff
	Uint32 m_prev_time;
	Uint32 m_dt;
	Uint32 m_timer;

	//screen stuff
	SDL_Window*   m_window;
	static SDL_Renderer* m_renderer;
	
	SDL_Texture*  m_background;
	SDL_Rect	  m_bgoffset;

	//object stuff
	int			m_num_objs;
	GameObject* m_objs[MAXOBJS];

	//control stuff
	bool		m_keys[NUMKEYS];
};

