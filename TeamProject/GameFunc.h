#pragma once

#include<iostream>
#include<vector>
#include<cstdlib>
#include<ctime>
#include <string>
#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include "SDL_image.h"
#include<cmath>


void InitGame();
void ClearGame();

// Game Phases
const int PHASE_INTRO = 0;
const int PHASE_MAINSTAGE = 1;
const int PHASE_ENDING = 2;

const int FUEL = 0;
const int IRON = 1;

extern int g_current_game_phase;
extern bool g_flag_running;
extern SDL_Window* g_window;
extern SDL_Renderer* g_renderer;
extern float g_timestep_s;

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
	//SDL_Rect source_rect_item_;
	SDL_Rect dest_rect_item_;
	int item_type;
public:
	ItemInterface(SDL_Rect dst_item, int it) : dest_rect_item_(dst_item), item_type(it) {};
	virtual ~ItemInterface() = default;

	virtual SDL_Rect* getDstRect() { return &dest_rect_item_; }
	virtual int getItemType() { return item_type; }
	virtual SDL_Texture* getTexture() = 0;
	virtual SDL_Rect* getSrcRect() = 0;
};

extern PhaseInterface* game_phases[3];
