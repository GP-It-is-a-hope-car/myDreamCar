#include "MainStage.h"
#include "SDL_image.h"
#include <windows.h>
#include <string>
#include <atlstr.h> // 한국어 쓰려면 필요함
#include<vector>

using namespace std;
int range;
int metal_count;

Player* p;
Platform* ground;
Platform* pf;
ItemInterface* fuel;
ItemInterface* iron;


enum Key
{
	LEFT = 1,
	RIGHT,
	UP,
	DOWN
};

bool Left;
bool Right;
bool Jump;

// 타일 구조체 선언
typedef struct Tile {
	int tile_posx;
	int tile_posy;
	int row;
	int col;
	float time = 0.0f;
	int state;
};
Tile tile[map_w][map_h];
vector<Tile> platform_tile;
// Init() 대신 생성자를 사용함
MainStage::MainStage()
{
	MakeGameObjTextures();

	InitTexts();
	//배경
	SDL_Surface* bg_surface= IMG_Load("../../Resources/bg_2.png");
	bg_texture= SDL_CreateTextureFromSurface(g_renderer, bg_surface);
	SDL_FreeSurface(bg_surface);
	bg_source= { 0,0,640,640 };
	bg_destination = { 0,0,640,640 };
	//타일 (바닥)
	SDL_Surface* tile_surface = IMG_Load("../../Resources/tile_m.png");
	tile_texture = SDL_CreateTextureFromSurface(g_renderer, tile_surface);
	SDL_FreeSurface(tile_surface);
	tile_source = { 0,0,500,300 };

	g_truck_source_rect[0] = { 0,0,160,80 };
	g_truck_source_rect[1] = { 160,0,320,80 };
	/*
	g_truck_source_rect.x = 0; // 트럭 가져오기
	g_truck_source_rect.y = 0;
	g_truck_source_rect.w = 160;
	g_truck_source_rect.h = 80;
	*/
	//게임 오브젝트들의 초기화
	InitGameObjectState();

	p = new Player;
	ground = new Platform;
	pf = new Platform(0, 0);
	fuel = new Fuel({ 0,0,0,0 });
	iron = new Iron({ 0,0,0,0 });
	// Clear the console screen.
	// 표준출력 화면을 깨끗히 지운다.
	//system("cls");

	// 연료 게이지 생성
	SDL_Surface* status_surface = IMG_Load("../../Resources/gauge.png");
	fuel_status = SDL_CreateTextureFromSurface(g_renderer, status_surface);
	SDL_FreeSurface(status_surface);
	status_source_rect = { 0,0,200,100 };
	status_destination_rect = { 0,0,200,40 };


	//타일 관련
	for (int i = 0; i < map_w; i++) {
		for (int j = 0; j < map_h; j++) {
			tile_destination[i][j].w = 64;
			tile_destination[i][j].h = 64;
			tile[i][j].row = i;
			tile[i][j].col = j;
			tile[i][j].state = EMPTY;
		}
	}
	for (int k = 0; k < map_w; k++) {
		tile[k][9].state = GROUND;
	}
	for (int i = 5; i < 8; i++) {
		tile[i][7].state = PLATFORM;
	}

	for (int i = 10; i < 13; i++) {
		tile[i][7].state = PLATFORM;
	}
	
	for (int i = 0; i < map_w; i++) {
		for (int j = 0; j < map_h; j++) {
			tile[i][j].tile_posx = i * 64;
			tile[i][j].tile_posy = j * 64;
		}
	}
	increase = 0;
	range = 0;
	// 연료 게이지 관련 변수
	fuel_amount = 200;
	fuel_num = 200;
	fuel_time = 0;
	metal_count = 0;
	//모션 관련 변수
	truck_motion_num = 2;
	truck_motion_cur = 0;
	player_motion_cur = 0;
	player_motion_Lnum = 2;
	player_motion_Rnum = 4;
	Left = false;
	Right = false;
	Jump = false;
	tile_speed =320*g_timestep_s;
}

/////////////////////////////////////////////////////////////
// Update() 
// 게임의 내용을 업데이트하는 함수.
// 실제 게임의 룰을 구현해야하는 곳.
// 게임에서 일어나는 변화는 모두 이 곳에서 구현한다.
// main 함수의 while loop에 의해서 무한히 반복 호출된다는 것을 주의.
void MainStage::Update() {
	// 트럭이 굴러가는 모션
	truck_motion_cur++;
	if (truck_motion_cur >= truck_motion_num) {
		truck_motion_cur = 0;
	}
	//게임 오버라면 게임을 멈춤
	if (!g_stage_flag_running) return;

	//트럭이 움직이는 속도
	truck_time += 1 / 30.0f;
	cout << tile_destination[20][8].x << "\n";

	if (truck_time >= 1.0f) {
		g_truck->getDstRect()->x += 100;
		truck_time = 0.0f;
	}
	if (g_truck->getDstRect()->x > tile_destination[20][8].x || fuel_amount <= 0) {
		g_stage_flag_running = false;
	}
	if (Left)
	{
		p->move_left(g_timestep_s);

		if (p->posX()==256) {
			increase--;
			bg_source.x -= tile_speed/2;
			g_truck->getDstRect()->x += tile_speed;
		}
		else if (p->posX() != 256) {
			bg_source.x = bg_source.x;
			increase = increase;
		}
	}
	if (Right)
	{
		p->move_right(g_timestep_s);
		if (p->posX()==288) {
			increase++;
			bg_source.x += tile_speed/2;
			g_truck->getDstRect()->x -= tile_speed;
		}
		else if (p->posX() != 288) {
			bg_source.x = bg_source.x;
			increase = increase;
		}
	}
	p->move_jump(g_timestep_s);

	// 바닥 충돌 판정 + 플랫폼 충돌 판정 + 일정 시간 경과 시 아이템 삭제
	for (int i = 0; i < map_w; i++) {
		for (int j = 0; j < map_h; j++) {
			if (tile[i][j].state == GROUND || tile[i][j].state == PLATFORM)
			{
				p->testOnPlatform(tile_destination[i][j].x, tile_destination[i][j].y, tile_destination[i][j].w, tile_destination[i][j].h);
			}
			if (tile[i][j].state == FUEL || tile[i][j].state == IRON)
			{
				tile[i][j].time += 1 / 30.0f;
				if (tile[i][j].time >= ITEMTIME)
				{
					tile[i][j].time = 0.0f;
					tile[i][j].state = EMPTY;
				}
			}
		}
	}

	{	// 아이템 랜덤 생성 파트
		itemTime += 1 / 30.0f;
		if (itemTime >= ITEMTIME) // 아이템 생성 주기
		{
			itemTime = 0;
			CreateItem();		
		}
	}

	//아이템 먹음
	DistinctItem();

	//트럭과 플레이어의 위치를 검사 후 닿았다면 아이템을 트럭에 반환
	GiveItemToTruck();

	g_elapsed_time_ms += 33;
	if (time_sec < 0) {
		//g_current_game_phase = PHASE_ENDING;
	}
}

/////////////////////////////////////////////////////////////
// Render() 
// 그림을 그리는 함수.
// main 함수의 while loop에 의해서 무한히 반복 호출된다는 것을 주의.
void MainStage::Render() {

	//배경을 그림
	SDL_RenderCopy(g_renderer, bg_texture, &bg_source, &bg_destination);

	//트럭을 그림
	SDL_RenderCopy(g_renderer, g_truck_sheet_texture, &g_truck_source_rect[truck_motion_cur], g_truck->getDstRect());
	//연료 게이지를 그림
	SDL_RenderCopy(g_renderer, fuel_status, &status_source_rect, &status_destination_rect);
	
	for (int k = 0; k < map_w; k++) {
		for (int h = 0; h < map_h; h++) {
			
			if (tile[k][h].state == GROUND) {
				SDL_RenderCopy(g_renderer, tile_texture, &tile_source, &tile_destination[k][h]);
			}
			else if (tile[k][h].state == PLATFORM) {
				SDL_RenderCopy(g_renderer, pf->getTexture(), pf->getSrcRect(), &tile_destination[k][h]);
			}
			else if (tile[k][h].state == FUEL) {
				SDL_RenderCopy(g_renderer, fuel->getTexture(), fuel->getSrcRect(), &tile_destination[k][h]);
			}
			else if (tile[k][h].state == IRON) {
				SDL_RenderCopy(g_renderer, iron->getTexture(), iron->getSrcRect(), &tile_destination[k][h]);
			}
		}
	}

	//플레이어
	p->draw_player();
	p->showItem();

	if (!g_stage_flag_running)
		DrawGameOverText();

		// 백에서 그린 그림을 한번에 가져옴
	SDL_RenderPresent(g_renderer);
}

/////////////////////////////////////////////////////////////
// HandleEvents() 
// 이벤트를 처리하는 함수.
// main 함수의 while loop에 의해서 무한히 반복 호출된다는 것을 주의.
void MainStage::HandleEvents()
{
	SDL_Event event;
	if (SDL_PollEvent(&event)) {
		switch (event.type) {

		case SDL_QUIT:
			g_flag_running = false;
			break;

		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_LEFT) {
				Left = true;
			}
			if (event.key.keysym.sym == SDLK_RIGHT) {
				Right = true;
			}
			if (event.key.keysym.sym == SDLK_SPACE) {
				p->jump();
			}
			break;
		case SDL_KEYUP:
			if (event.key.keysym.sym == SDLK_LEFT)
			{
				Left = false;
			}
			if (event.key.keysym.sym == SDLK_RIGHT)
			{
				Right = false;
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				g_current_game_phase = PHASE_ENDING;
			}
		default:
			break;
		}
	}
	{	
		// 타일 초기화
		if (increase >= 0) {
			for (int i = 0; i < map_w; i++) {
				for (int j = 0; j < map_h; j++) {
					tile[i][j].tile_posx = i * 64 - increase * tile_speed;
					tile[i][j].tile_posy = j * 64;
					tile_destination[i][j].x = tile[i][j].tile_posx;
					tile_destination[i][j].y = tile[i][j].tile_posy;

				}
			}
		}
		
		//연료통
		Uint32 fuel_cur_time = SDL_GetTicks();
		static Uint32 fuel_last_time = SDL_GetTicks();
		fuel_time += fuel_cur_time - fuel_last_time;
		fuel_amount = fuel_num - (fuel_time /100);
		fuel_last_time = fuel_time;
		status_source_rect.w = fuel_amount;
		status_destination_rect.w = fuel_amount;
		if (fuel_amount >= 200) {
			fuel_amount = 200;
		}
		else if (fuel_amount <= 0) {
			g_stage_flag_running = false;
		}
	}
	/*if (연료를 가져다 놨을 시) {
		fuel_num += 10;
	}*/

	if (bg_source.x==320) {
		range = 1;
	}
	else if (bg_source.x == 0) {
		range = 2;
	}
	else {
		range = 3;
	}
}

/////////////////////////////////////////////////////////////
// ClearGame() 
// 프로그램이 끝날 때 한 번 호출되는 함수.
// 이 함수에서 사용된 자원(이미지, 사운드 등)과 메모리 등을 해제해야한다.
// ClearGame() 대신 소멸자 사용
MainStage::~MainStage()
{
	SDL_DestroyTexture(g_truck_sheet_texture); // 트럭 메모리 해제
	SDL_DestroyTexture(g_gameover_text_kr); // 게임오버 텍스트 메모리 해제
	SDL_DestroyTexture(fuel_status); // 연료통 메모리 해제
	SDL_DestroyTexture(bg_texture); // 배경화면 메모리 해제
	SDL_DestroyTexture(tile_texture); // 타일 메모리 해제

	TTF_CloseFont(g_font_gameover); // 폰트 메모리 해제

	delete p;
	delete ground;

	//Mix_FreeChunk(g_missile_fire_sound);
}

void MainStage::InitGameObjectState() // 인트로에서 게임페이즈로 넘어올 때 초기화가 필요한 변수들의 초기화 
{
	g_stage_flag_running = true;
	SDL_Rect truck_dst_init = { 0, 500, 160, 80 };
	g_truck = new Truck(truck_dst_init);
	truck_time = 0.0f;
}
void MainStage::MakeGameObjTextures()
{
	SDL_Surface* truck_sheet_surface = IMG_Load("../../Resources/truck_motion.png"); // 이미지 파일을 가져옴
	SDL_SetColorKey(truck_sheet_surface, SDL_TRUE, SDL_MapRGB(truck_sheet_surface->format, 0, 155, 133));
	g_truck_sheet_texture = SDL_CreateTextureFromSurface(g_renderer, truck_sheet_surface);

	SDL_FreeSurface(truck_sheet_surface);
}

void MainStage::CreateItem()
{
	platform_tile.clear();

	for (int i = 0; i < map_w; i++) {
		for (int j = 0; j < map_h; j++) {
			if (tile[i][j].state == PLATFORM) //&& tile[i][j - 1].state == EMPTY)
			{
				tile[i][j].time = 0.0f;
				platform_tile.push_back(tile[i][j]);
			}
		}
	}

	int selected_platform_idx = Random(platform_tile.size());
	int x = platform_tile[selected_platform_idx].row;
	int y = platform_tile[selected_platform_idx].col - 1;
	
	if (Random(2) % 2 == 0)
	{
		tile[x][y].state = FUEL;
	}
	else
	{
		tile[x][y].state = IRON;
	}
}
int MainStage::Random(int n) // 0에서 n - 1까지 랜덤 수 발생
{
	srand((unsigned int)time(NULL));

	int rnd = rand();
	int a = (int)(rnd % (n));

	return a;
}
void MainStage::DistinctItem()
{
	int player_x = p->posX() + p->width() / 2;
	int player_y = p->posY() + p->height() / 2;

	for (int i = 0; i < map_w; i++) {
		for (int j = 0; j < map_h; j++) {
			if (tile[i][j].state == FUEL || tile[i][j].state == IRON)
			{
				SDL_Rect item_rect = tile_destination[i][j];
				if (player_x > item_rect.x && player_y > item_rect.y &&
					player_x < item_rect.x + item_rect.w &&
					player_y < item_rect.y + item_rect.h)
				{
					if (p->getItem(tile[i][j].state)) // 먹는데 성공했다면
					{
						if (tile[i][j].state == FUEL)
						{
							cout << "GET FUEL!!\n";						
						}
						else
						{
							cout << "GET IRON!!\n";
						}
						tile[i][j].state = EMPTY;
						tile[i][j].time = 0.0f;
					}
				}
			}
		}
	}
}
void MainStage::GiveItemToTruck()
{
	int player_x = p->posX() + p->width() / 2;
	int player_y = p->posY() + p->height() / 2;
	int return_item;
	SDL_Rect truck_rect = *g_truck->getDstRect();

	if (player_x > truck_rect.x && player_y > truck_rect.y &&
		player_x < truck_rect.x + truck_rect.w &&
		player_y < truck_rect.y + truck_rect.h)
	{
		return_item = p->giveItem();

		if (return_item == -1) return;

		if (return_item == FUEL)
		{
			fuel_num += 10;
			cout << "RETURN FUEL!!\n";
		}
		else
		{
			g_truck->addIronCnt(1);
			cout << g_truck->getIronCnt() << "\n";
			cout << "RETURN IRON!!\n";
			metal_count++; // 고철 수 증가 : 스코어
		}
	}
}
void MainStage::DrawGameText()
{

}
void MainStage::DrawGameOverText()
{
	SDL_Rect tmp_r; // 화면에 표시 될 위치

	tmp_r.x = 265;
	tmp_r.y = 325;
	tmp_r.w = g_gameover_text_kr_rect.w;
	tmp_r.h = g_gameover_text_kr_rect.h;

	SDL_RenderCopy(g_renderer, g_gameover_text_kr, &g_gameover_text_kr_rect, &tmp_r); // 텍스트 표시
}
void MainStage::InitChunk()
{
	/*g_missile_fire_sound = Mix_LoadWAV("../../Resources/fire.mp3"); // 효과음 로드
	Mix_VolumeChunk(g_missile_fire_sound, 24);

	g_open_box_sound = Mix_LoadWAV("../../Resources/Coin.wav"); // 효과음 로드
	Mix_VolumeChunk(g_open_box_sound, 24);

	g_ride_tank_sound = Mix_LoadWAV("../../Resources/IO.mp3"); // 효과음 로드
	Mix_VolumeChunk(g_ride_tank_sound, 24);*/

}

void MainStage::InitTexts()
{
	g_font_gameover = TTF_OpenFont("../../Resources/MaruBuri-SemiBold.ttf", 32);
	SDL_Surface* tmp_surface_1 = TTF_RenderUTF8_Blended(g_font_gameover, CW2A(L"Game Over!", CP_UTF8), black);
	//텍스트 가져오기
	g_gameover_text_kr_rect.x = 0;
	g_gameover_text_kr_rect.y = 0;
	g_gameover_text_kr_rect.w = tmp_surface_1->w;
	g_gameover_text_kr_rect.h = tmp_surface_1->h;

	g_gameover_text_kr = SDL_CreateTextureFromSurface(g_renderer, tmp_surface_1);
	SDL_FreeSurface(tmp_surface_1);
}