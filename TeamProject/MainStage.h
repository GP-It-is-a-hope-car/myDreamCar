#pragma once

#include "GameFunc.h"
#include "Truck.h"
#include "Fuel.h"
#include "Iron.h"
#include<list>

using namespace std;

class MainStage : public PhaseInterface
{
private:		
	SDL_Rect g_bg_1_source_rect; // 배경
	SDL_Rect g_bg_2_source_rect;

	
	SDL_Rect g_truck_source_rect; // 트럭 이미지에서 잘라오는 부분
	SDL_Texture* g_truck_sheet_texture;
	//SDL_Rect g_destination_apple;

	SDL_Rect g_fuel_source_rect; // 연료 이미지에서 잘라오는 부분
	SDL_Texture* g_fuel_sheet_texture;
	//SDL_Rect g_destination_apple;

	SDL_Rect g_iron_source_rect; // 고철 이미지에서 잘라오는 부분
	SDL_Texture* g_iron_sheet_texture;
	//SDL_Rect g_destination_apple;

	TTF_Font* g_font_gameover; // 게임 폰트 선언
	SDL_Texture* g_gameover_text_kr;
	SDL_Rect g_gameover_text_kr_rect;
	SDL_Color white = { 255, 255, 255, 0 }; // 색깔 선언
	SDL_Color black = { 0, 0, 0, 0 }; // 색깔 선언

	Truck* g_truck;
	ItemInterface* g_fuel;
	ItemInterface* g_iron;

	bool visited[13][13];
	int g_cur_key;
	int g_stage_last_time_ms;
	bool g_stage_flag_running, g_stage_is_colliding;

	// 흘러간 시간 기록
	double g_elapsed_time_ms;

public:
	MainStage();
	virtual ~MainStage();

	virtual void HandleEvents() override;
	virtual void Update() override;
	virtual void Render() override;

	void InitGameObjectState();
	//void CheckIsSnakeBody();
	void DrawGameText();
	void DrawGameOverText();
	void InitChunk();
	void InitTexts();
	void MakeGameObjTextures();
	//bool GetApple();
	//void SnakeMove();
	//void MakeSnake();
	//void CheckIsGameOver(SDL_Rect snakeHeadRect);
	//void CreateApple();
	pair<int, int> CreateRandomPosition();
};
