#include "GameFunc.h"
#include "Story.h"

Story::Story()
{
	// For Texture

	SDL_Surface* temp_surface = IMG_Load("../../Resources/dummy_story.png");
	texture_ = SDL_CreateTextureFromSurface(g_renderer, temp_surface);
	SDL_FreeSurface(temp_surface);
	source_rectangle_ = { 0,0,640,640 };
	destination_rectangle_ = { 0,0,640,640 };

}

Story::~Story()
{
	SDL_DestroyTexture(texture_);
}

void Story::Update()
{
	source_rectangle_.y += 1;
	if (source_rectangle_.y > 1280) {
		source_rectangle_.y = 1280;
		g_current_game_phase = PHASE_MAINSTAGE;
	}
}


void Story::Render()
{

	SDL_RenderCopy(g_renderer, texture_, &source_rectangle_, &destination_rectangle_);
	SDL_RenderPresent(g_renderer); // draw to the screen
}



void Story::HandleEvents()
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
				g_current_game_phase = PHASE_EXPLAIN;
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
