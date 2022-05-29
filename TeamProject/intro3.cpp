#include "GameFunc.h"
#include "intro3.h"

Intro3::Intro3()
{
	
	// For Texture
	SDL_Surface* temp_surface = IMG_Load("../../Resources/intro_2.png");
	texture_ = SDL_CreateTextureFromSurface(g_renderer, temp_surface);
	SDL_FreeSurface(temp_surface);
	source_rectangle_ = { 0,0,640,640 };
	destination_rectangle_ = { 0,0,640,640 };

}

Intro3::~Intro3()
{
	SDL_DestroyTexture(texture_);
	
}

void Intro3::Update()
{
}


void Intro3::Render()
{

	SDL_RenderCopy(g_renderer, texture_, &source_rectangle_, &destination_rectangle_);
	SDL_RenderPresent(g_renderer); // draw to the screen
}



void Intro3::HandleEvents()
{
	SDL_Event event;
	if (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			g_flag_running = false;
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_SPACE) {
				g_current_game_phase = PHASE_INTRO4;
			}
		default:
			break;
		}
	}
}
