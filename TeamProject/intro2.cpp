#include "GameFunc.h"
#include "Intro2.h"

Intro2::Intro2()
{
	
	// For Texture
	SDL_Surface* temp_surface = IMG_Load("../../Resources/intro_1.png");
	texture_ = SDL_CreateTextureFromSurface(g_renderer, temp_surface);
	SDL_FreeSurface(temp_surface);
	source_rectangle_ = { 0,0,640,640 };
	destination_rectangle_ = { 0,0,640,640 };

}

Intro2::~Intro2()
{
	SDL_DestroyTexture(texture_);
}

void Intro2::Update()
{
	
}


void Intro2::Render()
{

	SDL_RenderCopy(g_renderer, texture_, &source_rectangle_, &destination_rectangle_);
	SDL_RenderPresent(g_renderer); // draw to the screen
}



void Intro2::HandleEvents()
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
				g_current_game_phase = PHASE_INTRO3;
			}
		default:
			break;
		}
	}
}
