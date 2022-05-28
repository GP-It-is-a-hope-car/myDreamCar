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
	SDL_Texture* texture_; // the SDL_Texture 
	SDL_Rect source_rectangle_[3]; // the rectangle for source image
	SDL_Rect destination_rectangle_; // for destination
	SDL_Texture* metal_texture; // 메탈 수 텍스트
	SDL_Rect metal_texture_rect;

	int end_num;
	int end_cur;
};



