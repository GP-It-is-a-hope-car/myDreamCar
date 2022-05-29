#pragma once

#include "GameFunc.h"
#include "Player.h"
#include "Happy_Ending.h"
#include "Platform.h"
#include "Truck.h"
#include "Fuel.h"
#include "Iron.h"
#include<list>
#define map_w 60 // �� ũ�� : ����
#define map_h 10 // �� ũ�� : ����

using namespace std;

class MainStage : public PhaseInterface
{
private:
	
	Mix_Music* g_main_mus; // ���� ������� ���� ����

	//���
	SDL_Texture* bg_texture;
	SDL_Rect bg_destination; 
	SDL_Rect bg_source;
	//Ÿ��
	SDL_Texture* tile_texture;
	SDL_Rect tile_source;
	SDL_Rect tile_destination[map_w][map_h];

	SDL_Rect g_truck_source_rect[2]; // Ʈ�� �̹������� �߶���� �κ�
	SDL_Texture* g_truck_sheet_texture;

	TTF_Font* g_font_gameover; // ���� ��Ʈ ����
	SDL_Texture* g_gameover_text_kr;
	SDL_Rect g_gameover_text_kr_rect;
	SDL_Color white = { 255, 255, 255, 0 }; // ���� ����
	SDL_Color black = { 0, 0, 0, 0 }; // ���� ����

	//���������
	SDL_Texture* fuel_status;
	SDL_Rect status_source_rect;
	SDL_Rect status_destination_rect;
	
	//��ô��
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

	int fuel_amount; // ������ �ѷ� : 200
	
	float itemTime = 0;
	// �귯�� �ð� ���
	double g_elapsed_time_ms;

	float truck_time = 0.0f;

	int tile_speed; // Ÿ�� ���ǵ�	
	int increase; // Ÿ���� �̵���Ű�� ����
	int truck_motion_num; //Ʈ�� ����� ����
	int truck_motion_cur; //Ʈ�� ����� ���� ���� 
	int player_motion_Lnum; // �������� ���� ���
	int player_motion_Rnum; // ���������� ���� ���
	int player_motion_cur; // ���� ���
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
