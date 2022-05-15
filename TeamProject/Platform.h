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

	//플랫폼 그리기에 필요한 기본 요소
	SDL_Texture* texture_pf_;
	SDL_Rect source_rect_pf_;
	SDL_Rect dest_rect_pf_;
	
};