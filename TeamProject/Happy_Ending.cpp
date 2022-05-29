#include "Happy_ending.h"
#include "GameFunc.h"

extern int metal_count;
Happy_ending::Happy_ending()
{
	// For Texture

	SDL_Surface* temp_surface = IMG_Load("../../Resources/ending_happy.png");
	texture_ = SDL_CreateTextureFromSurface(g_renderer, temp_surface);
	SDL_FreeSurface(temp_surface);
	source_rectangle_ = { 0,0,640,640 };
	destination_rectangle_ = { 0,0,640,640 };
}

Happy_ending::~Happy_ending()
{
	SDL_DestroyTexture(metal_texture);
	
	SDL_DestroyTexture(texture_);
}

void Happy_ending::Score_metal(int metal_count) {
	TTF_Font* font1 = TTF_OpenFont("../../Resources/DungGeunMo.ttf", 50);
	SDL_Color black = { 0, 0, 0, 0 };
	SDL_Surface* tmp_surface = TTF_RenderText_Blended(font1, std::to_string((long long)metal_count).c_str(), black);
	metal_texture_rect = {0,0,150,100 };
	metal_texture = SDL_CreateTextureFromSurface(g_renderer, tmp_surface);
	SDL_FreeSurface(tmp_surface);
	TTF_CloseFont(font1);
}

void Happy_ending::Update()
{
	
	Score_metal(metal_count);
}


void Happy_ending::Render()
{
	SDL_Rect tmp_;
	tmp_ = { 350,190,80,80 };

	SDL_RenderCopy(g_renderer, texture_, &source_rectangle_, &destination_rectangle_);
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
			
			if (event.key.keysym.sym == SDLK_RETURN) {
				
				g_current_game_phase = PHASE_INTRO;
				metal_count = 0;
			}
			
		default:
			break;
		}
	}
}
