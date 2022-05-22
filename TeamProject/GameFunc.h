#pragma once

#include<iostream>
#include<cstdlib>
#include<ctime>
#include <string>
#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include "SDL_image.h"


void InitGame();
void ClearGame();

// Game Phases
const int PHASE_INTRO = 0;
const int PHASE_STAGE1 = 1;
const int PHASE_ENDING = 2;

extern int g_current_game_phase;
extern bool g_flag_running;
extern SDL_Window* g_window;
extern SDL_Renderer* g_renderer;

class PhaseInterface
{
public:
	PhaseInterface() = default;
	virtual ~PhaseInterface() = default;

	virtual void HandleEvents() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
};

class ItemInterface
{
private:
	//아이템 그리기에 필요한 기본 요소
	SDL_Texture* texture_item_;
	SDL_Rect source_rect_item_;
	SDL_Rect dest_rect_item_;
public:
	ItemInterface() = default;
	virtual ~ItemInterface() = default;

	virtual double width() { return dest_rect_item_.w; }
	virtual double height() { return dest_rect_item_.h; }
	virtual double posX() { return dest_rect_item_.x; }
	virtual double posY() { return dest_rect_item_.y; }
};

extern PhaseInterface* game_phases[3];
