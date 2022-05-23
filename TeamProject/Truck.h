#pragma once

#include"GameFunc.h"

class Truck {
private:
	int speed; // Ʈ�� �ӵ�
	int gauge; // ���� ������
	int ironCnt;

	//Ʈ�� �׸��⿡ �ʿ��� �⺻ ���
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