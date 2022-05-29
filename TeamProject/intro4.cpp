#include "GameFunc.h"
#include "intro4.h"

Intro4::Intro4()
{

	// For Texture
	SDL_Surface* temp_surface = IMG_Load("../../Resources/intro_3.png");
	texture_ = SDL_CreateTextureFromSurface(g_renderer, temp_surface);
	SDL_FreeSurface(temp_surface);
	source_rectangle_ = { 0,0,640,640 };
	destination_rectangle_ = { 0,0,640,640 };

}

Intro4::~Intro4()
{
	SDL_DestroyTexture(texture_);

}

void Intro4::Update()
{
	
}


void Intro4::Render()
{

	SDL_RenderCopy(g_renderer, texture_, &source_rectangle_, &destination_rectangle_);
	SDL_RenderPresent(g_renderer); // draw to the screen
}



void Intro4::HandleEvents()
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
				g_current_game_phase = PHASE_INTRO5;
			}
		default:
			break;
		}
	}
}
