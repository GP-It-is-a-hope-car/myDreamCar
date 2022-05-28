#include "Ending.h"
#include "GameFunc.h"

extern int metal_count;
Ending::Ending()
{
	// For Texture

	SDL_Surface* temp_surface = IMG_Load("../../Resources/ending.png");
	texture_ = SDL_CreateTextureFromSurface(g_renderer, temp_surface);
	SDL_FreeSurface(temp_surface);
	source_rectangle_[0] = { 0,0,640,640 };
	source_rectangle_[1] = { 640,0,1280,640 };
	source_rectangle_[2] = { 1280,0,1920,640 };
	destination_rectangle_ = { 0,0,640,640 };

	SDL_Color white = { 0, 0, 0, 0 };
	TTF_Font* font = TTF_OpenFont("../../Resources/bmjua.ttf", 50);
	SDL_Surface* tmp_surface = TTF_RenderText_Blended(font, std::to_string((long long)metal_count).c_str(), white);
	metal_texture_rect= { 0,0,150,100 };
	metal_texture= SDL_CreateTextureFromSurface(g_renderer, tmp_surface);
	SDL_FreeSurface(tmp_surface);
	TTF_CloseFont(font);
	end_cur = 0;
	end_num = 2;
}

Ending::~Ending()
{
	SDL_DestroyTexture(metal_texture);
	SDL_DestroyTexture(texture_);
}

void Ending::Update()
{
	end_cur++;
	if (end_cur >= end_num) {
		end_cur = 0;
	}
}


void Ending::Render()
{
	SDL_Rect tmp_;
	tmp_ = { 220,500,55,50 };
	SDL_RenderCopy(g_renderer, metal_texture, &metal_texture_rect, &tmp_);
	SDL_RenderCopy(g_renderer, texture_, &source_rectangle_[end_cur], &destination_rectangle_);
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
