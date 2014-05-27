#include "FontRender.h"


TTF_Font*	  FontRender::s_font = NULL;
SDL_Renderer* FontRender::s_renderer = NULL;
SDL_Color	  FontRender::s_color = WHITE;
SDL_Texture*  FontRender::s_text = NULL;
SDL_Rect	  FontRender::s_offset = {0,0,0,0};

FontRender*   FontRender::s_singleton = NULL;

FontRender::FontRender(void)
{

}


FontRender::~FontRender(void)
{

}



bool FontRender::Init(std::string fontfile,  SDL_Renderer* renderer, int fontsize)
{
	if (s_singleton == NULL)
	{
		s_singleton = new FontRender();

		if (TTF_Init() == -1)
			exit(100);
		
		s_font = TTF_OpenFont( fontfile.c_str(), fontsize);
		if ( !s_font )
			exit(100);

		s_color.r = 255;
		s_color.g = 255;
		s_color.b = 255;
		s_color.a = 255;

		s_renderer = renderer;
	}

	return true;
}

SDL_Texture* FontRender::RenderText(std::string text, int x, int y, SDL_Color color)
{
	SDL_Surface* text_surf = TTF_RenderText_Solid( s_font, text.c_str(), color);
	int w,h;

	if ( !text_surf )
		exit(101);
	
	if ( s_text )
		SDL_DestroyTexture( s_text );

	s_text = SDL_CreateTextureFromSurface(s_renderer, text_surf);
	if ( !s_text )
		exit(102);

	SDL_QueryTexture(s_text, NULL, NULL, &w, &h);
	SDL_FreeSurface( text_surf );
	
	s_offset.x = x;
	s_offset.y = y;
	s_offset.w = w;
	s_offset.h = h;
	
	SDL_RenderCopy(s_renderer, s_text, NULL, &s_offset);

	return s_text;
}

void FontRender::Quit()
{
	if ( s_text )
		SDL_DestroyTexture( s_text );
	TTF_Quit();
	delete s_singleton;
	s_singleton = NULL;
}