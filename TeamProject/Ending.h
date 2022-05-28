#pragma once

#include "GameFunc.h"

class Ending : public PhaseInterface
{
public:
	Ending();
	~Ending();
	virtual void HandleEvents() override;
	virtual void Update() override;
	virtual void Render() override;

private:
	SDL_Texture* texture_; 
	SDL_Rect source_rectangle_[3];
	SDL_Rect destination_rectangle_; 
	SDL_Texture* metal_texture; 
	SDL_Rect metal_texture_rect;
	SDL_Texture* re_texture;
	SDL_Rect re_source_rect;
	SDL_Rect re_dest_rect;

	SDL_Texture* start_texture;
	SDL_Rect start_source; 
	SDL_Rect start_dest; 
	int end_num;
	int end_cur;
	int re;
};



