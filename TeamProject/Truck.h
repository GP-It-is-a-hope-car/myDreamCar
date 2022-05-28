#pragma once

#include"GameFunc.h"

class Truck {
private:
	int speed; // Ʈ�� �ӵ�
	int ironCnt;

	//Ʈ�� �׸��⿡ �ʿ��� �⺻ ���
	SDL_Rect dest_rect_truck_;

public:
	Truck(SDL_Rect dst)
		: speed(0), ironCnt(0), dest_rect_truck_(dst) {
	};
	virtual ~Truck();

	int getSpeed() { return speed; }
	int getIronCnt() { return ironCnt; }
	void addIronCnt(int n) { ironCnt += n; };
	SDL_Rect* getDstRect() { return &dest_rect_truck_; }
	void setDstRect(SDL_Rect dst) { dest_rect_truck_ = dst; }
	
};