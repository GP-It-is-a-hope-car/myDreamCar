#include "GameFunc.h"
#include "Explain.h"

bool g_stage_flag_running;
Explain::Explain()
{
	// For Texture

	SDL_Surface* temp_surface = IMG_Load("../../Resources/dummy_introduce.png");
	texture_ = SDL_CreateTextureFromSurface(g_renderer, temp_surface);
	SDL_FreeSurface(temp_surface);
	source_rectangle_ = { 0,0,640,640 };
	destination_rectangle_ = { 0,0,640,640 };

}

Explain::~Explain()
{
	SDL_DestroyTexture(texture_);
}

void Explain::Update()
{
}

void Explain::Render()
{

	SDL_RenderCopy(g_renderer, texture_, &source_rectangle_, &destination_rectangle_);
	SDL_RenderPresent(g_renderer); // draw to the screen
}

void Explain::HandleEvents()
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
				Mix_HaltMusic();
				game_phases[PHASE_INTRO]->isOpeningPlay = false;
				g_stage_flag_running = true;
				g_current_game_phase = PHASE_MAINSTAGE;
			}
		default:
			break;
		}
	}
}
