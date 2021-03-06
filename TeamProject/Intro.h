#pragma once

#include "GameFunc.h"
class Intro : public PhaseInterface
{
public:
	Intro();
	~Intro();
	virtual void HandleEvents() override;
	virtual void Update() override;
	virtual void Render() override;

private:
	Mix_Music* g_intro_mus; // 인트로 배경음악 변수 선언

	SDL_Texture* texture_; // the SDL_Texture 
	SDL_Rect source_rectangle_; // the rectangle for source image
	SDL_Rect destination_rectangle_; // for destination

	
};


