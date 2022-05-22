#pragma once

#include"GameFunc.h"

class Truck {
private:
	int speed; // 트럭 속도
	int gauge; // 연료 게이지
	int ironCnt;

	//트럭 그리기에 필요한 기본 요소
	SDL_Texture* texture_truck_;
	SDL_Rect source_rect_truck_;
	SDL_Rect dest_rect_truck_;

public:
	Truck();
	virtual ~Truck();

	int getSpeed() { return speed; }
	int getGauge() { return gauge; }
	int getIronCnt() { return ironCnt; }
	double width() { return dest_rect_truck_.w; }
	double height() { return dest_rect_truck_.h; }
	double posX() { return dest_rect_truck_.x; }
	double posY() { return dest_rect_truck_.y; }
};