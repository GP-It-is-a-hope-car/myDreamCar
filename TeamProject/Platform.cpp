#include "Platform.h"

Platform::Platform()
{
	//이미지를 가져와서 텍스처에 붙이기
	//가장 바닥, 그라운드를 선언할 때 사용 x = 0, y = 0
	//테스트는 중앙
	SDL_Surface* pf_surface = IMG_Load("../../Resources/dummy_platform.png");
	texture_pf_ = SDL_CreateTextureFromSurface(g_renderer, pf_surface);
	SDL_FreeSurface(pf_surface);

	SDL_QueryTexture(texture_pf_, NULL, NULL, &source_rect_pf_.w, &source_rect_pf_.h);
	source_rect_pf_.x = source_rect_pf_.y = 0;
	dest_rect_pf_.x = 400;
	dest_rect_pf_.y = 350;
	dest_rect_pf_.w = source_rect_pf_.w;
	dest_rect_pf_.h = source_rect_pf_.h;

}

Platform::Platform(double x, double y)
{
	//공중에 떠 있는 플랫폼들을 선언할 때 사용
	//x , y 는 따로 받아야함
	SDL_Surface* pf_surface = IMG_Load("../../Resources/dummy_platform.png");
	texture_pf_ = SDL_CreateTextureFromSurface(g_renderer, pf_surface);
	SDL_FreeSurface(pf_surface);

	SDL_QueryTexture(texture_pf_, NULL, NULL, &source_rect_pf_.w, &source_rect_pf_.h);
	source_rect_pf_.x = source_rect_pf_.y = 0;
	dest_rect_pf_.x = x;
	dest_rect_pf_.y = y;
	dest_rect_pf_.w = source_rect_pf_.w;
	dest_rect_pf_.h = source_rect_pf_.h;

}

Platform::~Platform()
{
	SDL_DestroyTexture(texture_pf_);
}

void Platform::draw_pf()
{
	SDL_RenderCopy(g_renderer, texture_pf_, &source_rect_pf_, &dest_rect_pf_);
}