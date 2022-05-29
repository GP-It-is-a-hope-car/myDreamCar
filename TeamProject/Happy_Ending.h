#pragma once

#include "GameFunc.h"

class Happy_ending : public PhaseInterface
{
public:
	Happy_ending();
	~Happy_ending();
	virtual void HandleEvents() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Score_metal(int metal_count);

private:
	Mix_Music* g_ending_happy_mus; // 인트로 배경음악 변수 선언

	//배경
	SDL_Texture* texture_;
	SDL_Rect source_rectangle_;
	SDL_Rect destination_rectangle_;
	//고철 수 표기
	SDL_Texture* metal_texture;
	SDL_Rect metal_texture_rect;
	char s1;
};



