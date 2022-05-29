#pragma once

#include "GameFunc.h"
#include "Player.h"
#include "Happy_Ending.h"
#include "Platform.h"
#include "Truck.h"
#include "Fuel.h"
#include "Iron.h"
#include<list>
#define map_w 60 // 맵 크기 : 가로
#define map_h 10 // 맵 크기 : 세로

using namespace std;

class MainStage : public PhaseInterface
{
private:
	
	Mix_Music* g_main_mus; // 메인 배경음악 변수 선언

	//배경
	SDL_Texture* bg_texture;
	SDL_Rect bg_destination; 
	SDL_Rect bg_source;
	//타일
	SDL_Texture* tile_texture;
	SDL_Rect tile_source;
	SDL_Rect tile_destination[map_w][map_h];

	SDL_Rect g_truck_source_rect[2]; // 트럭 이미지에서 잘라오는 부분
	SDL_Texture* g_truck_sheet_texture;

	TTF_Font* g_font_gameover; // 게임 폰트 선언
	SDL_Texture* g_gameover_text_kr;
	SDL_Rect g_gameover_text_kr_rect;
	SDL_Color white = { 255, 255, 255, 0 }; // 색깔 선언
	SDL_Color black = { 0, 0, 0, 0 }; // 색깔 선언

	//연료게이지
	SDL_Texture* fuel_status;
	SDL_Rect status_source_rect;
	SDL_Rect status_destination_rect;
	
	//진척도
	SDL_Texture* advancement_texture;
	SDL_Rect advancement_destination;
	SDL_Rect advancement_source;

	Truck* g_truck;
	
	bool visited[13][13];
	int g_cur_key;
	int g_stage_last_time_ms;
	int time_ms;
	int time_sec;
	int game_time;

	int fuel_amount; // 연료통 총량 : 200
	
	float itemTime = 0;
	// 흘러간 시간 기록
	double g_elapsed_time_ms;

	float truck_time = 0.0f;

	int tile_speed; // 타일 스피드	
	int increase; // 타일을 이동시키는 변수
	int truck_motion_num; //트럭 모션의 개수
	int truck_motion_cur; //트럭 모션의 현재 상태 
	int player_motion_Lnum; // 왼쪽으로 가는 모션
	int player_motion_Rnum; // 오른쪽으로 가는 모션
	int player_motion_cur; // 현재 모션
public:
	MainStage();
	virtual ~MainStage();

	virtual void HandleEvents() override;
	virtual void Update() override;
	virtual void Render() override;

	void InitGameObjectState();
	void DrawGameText();
	void InitChunk();
	void InitTexts();
	void UpdateTimeTexture(int ms);
	void MakeGameObjTextures();
	void CreateItem();
	void DistinctItem();
	void GiveItemToTruck();
	int Random(int n);
};
