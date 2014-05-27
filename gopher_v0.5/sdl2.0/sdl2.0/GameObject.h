#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include "helper.h"

#pragma once

struct Vector2
{
	float x;
	float y;
};

class Game;
class Gopher;

class GameObject
{
	friend Game;
	friend Gopher;
public:
	GameObject( );
	~GameObject( );

	void SetName(std::string);
	void SetImage(std::string);
	void SetPosition(float, float);
	
	void Move(float, float);

	Vector2 GetPosition();

	virtual void Update(float);

	//shared by all gameobjects
	static void SetRenderer(SDL_Renderer*);
	static SDL_Texture* LoadMedia(std::string);

protected:
	bool m_active;

	std::string m_name;
	Vector2		m_pos;

	SDL_Texture* m_sprite;
	SDL_Rect	 m_offset;
	SDL_Rect	 m_clip;

	//shared by all gameobjects
	static SDL_Renderer* s_renderer;
	
};

