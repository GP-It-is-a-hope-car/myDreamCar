
#include "Ending.h"
#include "GameFunc.h"

Ending::Ending()
{
	// For Texture

	SDL_Surface* temp_surface = IMG_Load("../../Resources/ending_prototype.png");
	texture_ = SDL_CreateTextureFromSurface(g_renderer, temp_surface);
	SDL_FreeSurface(temp_surface);
	source_rectangle_ = { 0,0,600,600 };
	destination_rectangle_ = { 0,0,700,700 };

}

Ending::~Ending()
{
	SDL_DestroyTexture(texture_);
}

void Ending::Update()
{
}


void Ending::Render()
{
	SDL_RenderCopy(g_renderer, texture_, &source_rectangle_, &destination_rectangle_);
	SDL_RenderPresent(g_renderer); 
}



void Ending::HandleEvents()
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
				g_current_game_phase = PHASE_INTRO;
			}
		case SDL_MOUSEBUTTONDOWN:

			/*
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				g_current_game_phase = PHASE_STAGE1;
			}
			break;
			*/
		default:
			break;
		}
	}
}
