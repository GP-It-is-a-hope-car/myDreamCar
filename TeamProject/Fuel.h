#pragma once
#include "GameFunc.h"

class Fuel : public ItemInterface
{
private:
	SDL_Rect fuel_source_rect_; // ���� �̹������� �߶���� �κ�
	SDL_Texture* fuel_sheet_texture_;
	// ������ ����	
public:
	Fuel(SDL_Rect dst_fuel) : ItemInterface(dst_fuel, FUEL) {

		SDL_Surface* fuel_sheet_surface = IMG_Load("../../Resources/Fuel.png"); // �̹��� ������ ������
		SDL_SetColorKey(fuel_sheet_surface, SDL_TRUE, SDL_MapRGB(fuel_sheet_surface->format, 223, 113, 38));
		fuel_sheet_texture_ = SDL_CreateTextureFromSurface(g_renderer, fuel_sheet_surface);

		fuel_source_rect_.x = 0; // ���� ��������
		fuel_source_rect_.y = 0;
		fuel_source_rect_.w = 64;
		fuel_source_rect_.h = 64;

		SDL_FreeSurface(fuel_sheet_surface);
	};
	~Fuel();
	virtual SDL_Texture* getTexture() {return fuel_sheet_texture_;}
	virtual SDL_Rect* getSrcRect() { return &fuel_source_rect_; }
};