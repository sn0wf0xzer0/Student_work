#include "GameObject.h"

SDL_Renderer* GameObject::s_renderer = NULL;

// * * * * Instance methods * * * * //
GameObject::GameObject( )
{
	m_sprite = NULL;
	m_active = true;
}


GameObject::~GameObject( )
{
	if (m_sprite)
		SDL_DestroyTexture(m_sprite);
}


/*
	sets gameobject's name... duh...
*/
void GameObject::SetName(std::string name)
{
	m_name = name;
}


/*
	assigns the image specified by filename to local sprite
*/
void GameObject::SetImage(std::string filename)
{
	m_sprite = LoadMedia(filename);
	SDL_QueryTexture(m_sprite, NULL, NULL, &m_offset.w, &m_offset.h);
}

/*
	sets gameobject's position... duh...
*/
void GameObject::SetPosition(float x, float y)
{
	m_pos.x = x;
	m_pos.y = y;
}


/*
	how about the power... to move you...
*/
void GameObject::Move(float x, float y)
{
	m_pos.x += x;
	m_pos.y += y;
}

Vector2 GameObject::GetPosition()
{
	return m_pos;
}

/*
	gameobject update logic
*/
void GameObject::Update(float dt)
{
	if ( !m_active )
		return;

	m_offset.x = m_pos.x;
	m_offset.y = m_pos.y;


	//SDL_RenderCopy(s_renderer, m_sprite, &clip, &offset);
	SDL_RenderCopy(s_renderer, m_sprite, NULL, &m_offset);
	
	//uncomment for weird stuff to happen
	/*
		m_offset.w++;
		m_offset.h++;
		static int ang = 0;
		ang++;
		SDL_RenderCopyEx(s_renderer, m_sprite, NULL, &m_offset, ang, NULL, SDL_FLIP_HORIZONTAL);
	*/
}

// * * * * End Instanced Methods * * * * //




// * * * * Static methods * * * * //

/*
	assigns the render object that all gameobjects reference
*/
void GameObject::SetRenderer(SDL_Renderer* renderer)
{
	s_renderer = renderer;
}


/*
	Loads media image, binds to render object
	filename - filename (including directory location) if image to be loaded
	returns loaded texture on success, NULL otherwise
*/
SDL_Texture* GameObject::LoadMedia(std::string filename)
{
	if ( s_renderer == NULL )
	{
		exit(101);
		return NULL;
	}

	//load image
	SDL_Surface* surf = IMG_Load( filename.c_str() );
	SDL_Texture* text = NULL;

	//verify it was loaded
	if ( surf == NULL )
	{
		exit(404);	//shuts down program if image not found
		return NULL;
	}


	//bind image to renderer
	text = SDL_CreateTextureFromSurface( s_renderer, surf );
	SDL_FreeSurface( surf );

	return text;
}

// * * * * End Static Methods * * * * //