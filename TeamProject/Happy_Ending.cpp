#include "Happy_ending.h"
#include "GameFunc.h"

extern int metal_count;
Happy_ending::Happy_ending()
{
	// For Texture

	SDL_Surface* temp_surface = IMG_Load("../../Resources/ending_prototype.png");
	texture_ = SDL_CreateTextureFromSurface(g_renderer, temp_surface);
	SDL_FreeSurface(temp_surface);
	source_rectangle_ = { 0,0,640,640 };
	destination_rectangle_ = { 0,0,640,640 };

	SDL_Surface* temp1_surface = IMG_Load("../../Resources/triangle.png");
	start_texture = SDL_CreateTextureFromSurface(g_renderer, temp1_surface);
	SDL_FreeSurface(temp1_surface);
	start_source = { 0,0,200,200 };
	start_dest = { 240,400,40,40 };

	SDL_Color white = { 0, 0, 0, 0 };
	TTF_Font* font = TTF_OpenFont("../../Resources/bmjua.ttf", 50);
	SDL_Surface* tmp_surface = TTF_RenderText_Blended(font, std::to_string((long long)metal_count).c_str(), white);
	metal_texture_rect = { 0,0,150,100 };
	metal_texture = SDL_CreateTextureFromSurface(g_renderer, tmp_surface);
	SDL_FreeSurface(tmp_surface);


	re = 0;

	//Yes
	SDL_Color red = { 255, 0, 0, 0 };
	SDL_Surface* tmp1_surface = TTF_RenderText_Blended(font, "Y", red);
	re_source_rect = { 0,0,200,200 };
	re_dest_rect = { 240,350,40,40 };
	re_texture = SDL_CreateTextureFromSurface(g_renderer, tmp1_surface);
	SDL_FreeSurface(tmp1_surface);
	//No
	SDL_Surface* tmp2_surface = TTF_RenderText_Blended(font, "N", red);
	re1_source_rect = { 0,0,200,200 };
	re1_dest_rect = { 380,350,40,40 };
	re1_texture = SDL_CreateTextureFromSurface(g_renderer, tmp2_surface);
	SDL_FreeSurface(tmp2_surface);
	//Play agin?
	SDL_Surface* tmp3_surface = TTF_RenderText_Blended(font, "Play Again?", red);
	re2_source_rect = { 0,0,200,200 };
	re2_dest_rect = { 260,250,60,60 };
	re2_texture = SDL_CreateTextureFromSurface(g_renderer, tmp3_surface);
	SDL_FreeSurface(tmp3_surface);


	TTF_CloseFont(font);
}

Happy_ending::~Happy_ending()
{
	SDL_DestroyTexture(metal_texture);
	SDL_DestroyTexture(re2_texture);
	SDL_DestroyTexture(re1_texture);
	SDL_DestroyTexture(re_texture);
	SDL_DestroyTexture(texture_);
}

void Happy_ending::Update()
{
}


void Happy_ending::Render()
{
	SDL_Rect tmp_;
	tmp_ = { 50,50,100,100 };

	SDL_RenderCopy(g_renderer, texture_, &source_rectangle_, &destination_rectangle_);
	SDL_RenderCopy(g_renderer, start_texture, &start_source, &start_dest);
	SDL_RenderCopy(g_renderer, re_texture, &re_source_rect, &re_dest_rect);
	SDL_RenderCopy(g_renderer, re1_texture, &re1_source_rect, &re1_dest_rect);
	SDL_RenderCopy(g_renderer, re2_texture, &re2_source_rect, &re2_dest_rect);
	SDL_RenderCopy(g_renderer, metal_texture, &metal_texture_rect, &tmp_);
	SDL_RenderPresent(g_renderer);
}



void Happy_ending::HandleEvents()
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
			/*if (event.key.keysym.sym == SDLK_SPACE) {
				g_current_game_phase = PHASE_INTRO;
			}*/
			if (event.key.keysym.sym == SDLK_SPACE) {
				if (re == 0) {
					g_current_game_phase = PHASE_INTRO;
				}
				else if (re == 1) {
					exit(0);
				}
			}
			if (event.key.keysym.sym == SDLK_LEFT) {
				start_dest.x = 240;
				re = 0;
			}
			if (event.key.keysym.sym == SDLK_RIGHT) {
				start_dest.x = 380;
				re = 1;
			}
		default:
			break;
		}
	}
}
