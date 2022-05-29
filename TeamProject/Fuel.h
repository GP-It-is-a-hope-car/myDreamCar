#pragma once
#include "GameFunc.h"

class Fuel : public ItemInterface
{
private:
	SDL_Rect fuel_source_rect_; // 연료 이미지에서 잘라오는 부분
	SDL_Texture* fuel_sheet_texture_;
	// 아이템 종류	
public:
	Fuel(SDL_Rect dst_fuel) : ItemInterface(dst_fuel, FUEL) {

		SDL_Surface* fuel_sheet_surface = IMG_Load("../../Resources/Fuel.png"); // 이미지 파일을 가져옴
		SDL_SetColorKey(fuel_sheet_surface, SDL_TRUE, SDL_MapRGB(fuel_sheet_surface->format, 223, 113, 38));
		fuel_sheet_texture_ = SDL_CreateTextureFromSurface(g_renderer, fuel_sheet_surface);

		fuel_source_rect_.x = 0; // 연료 가져오기
		fuel_source_rect_.y = 0;
		fuel_source_rect_.w = 64;
		fuel_source_rect_.h = 64;

		SDL_FreeSurface(fuel_sheet_surface);
	};
	~Fuel();
	virtual SDL_Texture* getTexture() {return fuel_sheet_texture_;}
	virtual SDL_Rect* getSrcRect() { return &fuel_source_rect_; }
};