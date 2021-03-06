#pragma once

#include "GameFunc.h"

class Platform
{
public:
	Platform();
	Platform(double x, double y);
	~Platform();

	double width() { return dest_rect_pf_.w; }
	double height() { return dest_rect_pf_.h; }
	double posX() { return dest_rect_pf_.x; }
	double posY() { return dest_rect_pf_.y; }
	SDL_Rect* getDstRect() { return &dest_rect_pf_; }
	SDL_Rect* getSrcRect() { return &source_rect_pf_; }
	SDL_Texture* getTexture() { return texture_pf_; }
	virtual void draw_pf();
	
private:
	//플랫폼 그리기에 필요한 기본 요소
	SDL_Texture* texture_pf_;
	SDL_Rect source_rect_pf_;
	SDL_Rect dest_rect_pf_;
	
};