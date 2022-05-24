#pragma once

#include "GameFunc.h"

class Iron : public ItemInterface
{
private:
	SDL_Rect g_iron_source_rect; // 고철 이미지에서 잘라오는 부분
	SDL_Texture* iron_sheet_texture;
	int weight;
public:
	Iron(SDL_Rect dst_iron) : weight(0), ItemInterface(dst_iron, IRON) {
		SDL_Surface* iron_sheet_surface = IMG_Load("../../Resources/apple.png"); // 이미지 파일을 가져옴
		SDL_SetColorKey(iron_sheet_surface, SDL_TRUE, SDL_MapRGB(iron_sheet_surface->format, 255, 255, 255));
		iron_sheet_texture = SDL_CreateTextureFromSurface(g_renderer, iron_sheet_surface);

		g_iron_source_rect.x = 145; // 사과 가져오기
		g_iron_source_rect.y = 138;
		g_iron_source_rect.w = 392;
		g_iron_source_rect.h = 421;

		SDL_FreeSurface(iron_sheet_surface);
	};
	virtual ~Iron();

	virtual SDL_Texture* getTexture() { return iron_sheet_texture; }
	virtual SDL_Rect* getSrcRect() { return &g_iron_source_rect; }

	virtual int getWeight() { return weight; }
};