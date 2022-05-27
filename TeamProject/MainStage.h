#pragma once

#include "GameFunc.h"
#include "Player.h"
#include "Platform.h"
#include "Truck.h"
#include "Fuel.h"
#include "Iron.h"
#include<list>
#define map_w 30 // �� ũ�� : ����
#define map_h 10 // �� ũ�� : ����
#define win_w 30 // ������ ũ�� : ����
#define win_h 10 // ������ ũ�� : ����

using namespace std;

class MainStage : public PhaseInterface
{
private:

	//���
	SDL_Texture* bg_texture;
	SDL_Rect bg_destination; 
	SDL_Rect bg_source;
	//Ÿ��
	SDL_Texture* tile_texture;
	SDL_Rect tile_source;
	SDL_Rect tile_destination[win_w][win_h];

	SDL_Rect g_truck_source_rect; // Ʈ�� �̹������� �߶���� �κ�
	SDL_Texture* g_truck_sheet_texture;

	TTF_Font* g_font_gameover; // ���� ��Ʈ ����
	SDL_Texture* g_gameover_text_kr;
	SDL_Rect g_gameover_text_kr_rect;
	SDL_Color white = { 255, 255, 255, 0 }; // ���� ����
	SDL_Color black = { 0, 0, 0, 0 }; // ���� ����

	SDL_Texture* fuel_status;
	SDL_Rect status_source_rect;
	SDL_Rect status_destination_rect;

	Truck* g_truck;
	
	bool visited[13][13];
	int g_cur_key;
	int g_stage_last_time_ms;
	bool g_stage_flag_running, g_stage_is_colliding;
	int time_ms;
	int time_sec;
	int game_time;

	int fuel_amount; // ������ �ѷ� : 200
	int fuel_num; // ���ᷮ ������Ű�� ���� ����
	int fuel_time; // ������ ���� �ð� �޴� ����

	float itemTime = 0;
	// �귯�� �ð� ���
	double g_elapsed_time_ms;

	float truck_time = 0.0f;

	int tile_speed; // Ÿ�� ���ǵ�	
	int increase; // Ÿ���� �̵���Ű�� ����

public:
	MainStage();
	virtual ~MainStage();

	virtual void HandleEvents() override;
	virtual void Update() override;
	virtual void Render() override;

	void InitGameObjectState();
	void DrawGameText();
	void DrawGameOverText();
	void InitChunk();
	void InitTexts();
	void UpdateTimeTexture(int ms);
	void MakeGameObjTextures();
	void CreateItem();
	void DistinctItem();
	int Random(int n);
	pair<int, int> CreateRandomPosition();
};
