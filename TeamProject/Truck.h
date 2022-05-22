#pragma once

#include"GameFunc.h"

class Truck {
private:
	int speed; // Ʈ�� �ӵ�
	int gauge; // ���� ������
	int ironCnt;

	//Ʈ�� �׸��⿡ �ʿ��� �⺻ ���
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