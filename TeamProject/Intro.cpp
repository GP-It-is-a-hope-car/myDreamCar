#include "GameFunc.h"
#include "Intro.h"

Intro::Intro()
{
	g_intro_mus = Mix_LoadMUS("../../Resources/opening.mp3"); // 배경음악 로드	
	Mix_VolumeMusic(40);

	// For Texture
	SDL_Surface* temp_surface = IMG_Load("../../Resources/intro_prototype.png");
	texture_ = SDL_CreateTextureFromSurface(g_renderer, temp_surface);
	SDL_FreeSurface(temp_surface);
	source_rectangle_ = { 0,0,640,640 };
	destination_rectangle_ = { 0,0,640,640 };

}

Intro::~Intro()
{
	SDL_DestroyTexture(texture_);
	Mix_FreeMusic(g_intro_mus);
}

void Intro::Update()
{
	if (isOpeningPlay) return;

	if (g_current_game_phase == PHASE_INTRO)
	{
		if(Mix_PlayingMusic())
			Mix_HaltMusic();

		Mix_PlayMusic(g_intro_mus, -1); // 배경음악 플레이
		isOpeningPlay = true;
	}
}


void Intro::Render()
{

	SDL_RenderCopy(g_renderer, texture_, &source_rectangle_, &destination_rectangle_);
	SDL_RenderPresent(g_renderer); // draw to the screen
}



void Intro::HandleEvents()
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
				g_current_game_phase = PHASE_STORY;
			}
		default:
			break;
		}
	}
}
