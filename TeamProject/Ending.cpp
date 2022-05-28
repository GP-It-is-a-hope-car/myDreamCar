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
	source_rectangle_[1] = { 640,0,640,640 };
	source_rectangle_[2] = { 1280,0,640,640 };
	destination_rectangle_ = { 0,0,640,640 };

	SDL_Surface* temp1_surface = IMG_Load("../../Resources/triangle.png");
	start_texture = SDL_CreateTextureFromSurface(g_renderer, temp1_surface);
	SDL_FreeSurface(temp1_surface);
	start_source = { 0,0,200,200 };
	start_dest = { 260,300,20,20 };
	
	SDL_Color white = { 0, 0, 0, 0 };
	TTF_Font* font = TTF_OpenFont("../../Resources/bmjua.ttf", 50);
	SDL_Surface* tmp_surface = TTF_RenderText_Blended(font, std::to_string((long long)metal_count).c_str(), white);
	metal_texture_rect= { 0,0,150,100 };
	metal_texture= SDL_CreateTextureFromSurface(g_renderer, tmp_surface);
	SDL_FreeSurface(tmp_surface);
	
	end_cur = 0;
	end_num = 2;
	re = 0;
	
	SDL_Color red = { 255, 0, 0, 0 };
	SDL_Surface* tmp1_surface = TTF_RenderText_Blended(font, "YES", red);
	re_source_rect = { 0,0,200,200 };
	re_dest_rect = { 260,250,40,40 };
	re_texture= SDL_CreateTextureFromSurface(g_renderer, tmp1_surface);
	SDL_FreeSurface(tmp1_surface);
	TTF_CloseFont(font);
}

Ending::~Ending()
{
	SDL_DestroyTexture(metal_texture);
	SDL_DestroyTexture(start_texture);
	SDL_DestroyTexture(re_texture);
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
	tmp_ = { 50,50,100,100 };
	
	SDL_RenderCopy(g_renderer, texture_, &source_rectangle_[end_cur], &destination_rectangle_);
	SDL_RenderCopy(g_renderer, start_texture, &start_source, &start_dest);
	SDL_RenderCopy(g_renderer, re_texture, &re_source_rect, &re_dest_rect);
	SDL_RenderCopy(g_renderer, metal_texture, &metal_texture_rect, &tmp_);
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
			if (event.key.keysym.sym == SDLK_0) {
				if (re == 0) {
					g_current_game_phase = PHASE_HAPPY_ENDING;
				}
				else if(re==1) {
					exit(0);
				}
			}
			if (event.key.keysym.sym == SDLK_LEFT) {
				start_dest.x = 260;
				re = 0;
			}
			if (event.key.keysym.sym == SDLK_RIGHT) {
				start_dest.x = 320;
				re = 1;
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
