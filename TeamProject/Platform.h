#pragma once

#include <iostream>
#include <string>
#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include "SDL_image.h"

class Platform
{
public:
	Platform();
	~Platform();

	//�÷��� �׸��⿡ �ʿ��� �⺻ ���
	SDL_Texture* texture_pf_;
	SDL_Rect source_rect_pf_;
	SDL_Rect dest_rect_pf_;
	
};