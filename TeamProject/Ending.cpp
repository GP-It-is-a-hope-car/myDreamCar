#include "Ending.h"

extern int metal_count;
Ending::Ending()
{
	g_ending_mus = Mix_LoadMUS("../../Resources/ending.mp3"); // 배경음악 로드	
	Mix_VolumeMusic(60);

	// For Texture

	SDL_Surface* temp_surface = IMG_Load("../../Resources/ending_bad.png");
	texture_ = SDL_CreateTextureFromSurface(g_renderer, temp_surface);
	SDL_FreeSurface(temp_surface);
	source_rectangle_ = { 0,0,640,640 };
	destination_rectangle_ = { 0,0,640,640 };

}

Ending::~Ending()
{
	SDL_DestroyTexture(texture_);
}

void Ending::Update()
{
	if (isEndingPlay) return;

	if (g_current_game_phase == PHASE_ENDING)
	{
		if (Mix_PlayingMusic())
			Mix_HaltMusic();

		Mix_PlayMusic(g_ending_mus, -1); // 배경음악 플레이
		isEndingPlay = true;
	}
}


void Ending::Render()
{
	SDL_Rect tmp_;
	tmp_ = { 50,50,100,100 };
	
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
			
			if (event.key.keysym.sym == SDLK_RETURN) {
				g_current_game_phase = PHASE_INTRO;
			}
			
		default:
			break;
		}
	}
}
