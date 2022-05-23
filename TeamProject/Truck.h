#pragma once

#include"GameFunc.h"

class Truck {
private:
	int speed; // 트럭 속도
	int gauge; // 연료 게이지
	int ironCnt;

	//트럭 그리기에 필요한 기본 요소
	SDL_Rect dest_rect_truck_;

public:
	Truck(SDL_Rect dst)
		: speed(0), gauge(0), ironCnt(0), dest_rect_truck_(dst) {
	};
	virtual ~Truck();

	int getSpeed() { return speed; }
	int getGauge() { return gauge; }
	int getIronCnt() { return ironCnt; }
	SDL_Rect* getRect() { return &dest_rect_truck_; }
	
};