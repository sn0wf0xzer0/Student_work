#include <SDL_ttf.h>
#include <string>

#pragma once

const SDL_Color WHITE = {255,255,255,255};

class FontRender
{
public:
	~FontRender(void);
	static bool Init(std::string fontfile,  SDL_Renderer* renderer, int fontsize);
	static SDL_Texture* FontRender::RenderText(std::string text, int x, int y, SDL_Color color=WHITE);
	static void Quit();


private:
	FontRender(void);

	static TTF_Font*	 s_font;
	static SDL_Renderer* s_renderer;
	static SDL_Color	 s_color;
	static SDL_Texture*  s_text;
	static SDL_Rect		 s_offset;

	static FontRender*   s_singleton;
};

