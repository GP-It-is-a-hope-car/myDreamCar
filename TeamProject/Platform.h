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
	SDL_Rect getRect() { return dest_rect_pf_; }
	virtual void draw_pf();
	
private:
	//�÷��� �׸��⿡ �ʿ��� �⺻ ���
	SDL_Texture* texture_pf_;
	SDL_Rect source_rect_pf_ = {0,0,100,10};
	SDL_Rect dest_rect_pf_;
	
};