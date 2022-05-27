#include "MainStage.h"
#include "SDL_image.h"
#include <windows.h>
#include <string>
#include <atlstr.h> // 한국어 쓰려면 필요함
#include<vector>

#define SIZE 8
#define CELL 50

using namespace std;

Player* p;
Platform* ground;
vector<Platform*> platform_arr;

enum Key
{
	LEFT = 1,
	RIGHT,
	UP,
	DOWN
};
vector<Platform*> platform_tmp;

bool Left;
bool Right;
bool Jump;

// 타일 구조체 선언
typedef struct t {
	int tile_posx;
	int tile_posy;
	int state;
};
t tile[map_w][map_h];

// Init() 대신 생성자를 사용함
MainStage::MainStage()
{
	MakeGameObjTextures();

	InitTexts();
	//배경
	SDL_Surface* bg_surface= IMG_Load("../../Resources/dummy_bg.png");
	bg_texture= SDL_CreateTextureFromSurface(g_renderer, bg_surface);
	SDL_FreeSurface(bg_surface);
	bg_source= { 0,0,640,640 };
	bg_destination = { 0,0,640,640 };
	//타일 (바닥)
	SDL_Surface* tile_surface = IMG_Load("../../Resources/tile_m.png");
	tile_texture = SDL_CreateTextureFromSurface(g_renderer, tile_surface);
	SDL_FreeSurface(tile_surface);
	tile_source = { 0,0,500,300 };

	g_truck_source_rect.x = 0; // 트럭 가져오기
	g_truck_source_rect.y = 0;
	g_truck_source_rect.w = 160;
	g_truck_source_rect.h = 80;

	//게임 오브젝트들의 초기화
	InitGameObjectState();

	p = new Player;
	ground = new Platform;

	for (int i = 1; i <= 5; i++) // 플랫폼 위치 for문으로 선언해주고 arr에 넣어주자!
	{
		Platform* pf = new Platform(50 * i, 90 * i);
		platform_arr.push_back(pf);
	}
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
			tile[i][j].state = 0;
		}
	}
	for (int k = 0; k < map_w; k++) {
		tile[k][9].state = 1;
	}
	for (int i = 10; i < 13; i++) {
		tile[i][5].state = 1;
	}
	for (int i = 0; i < map_w; i++) {
		for (int j = 0; j < map_h; j++) {
			tile[i][j].tile_posx = i * 20;
			tile[i][j].tile_posy = j * 20;
		}
	}
	increase = 0;

	// 연료 게이지 관련 변수
	fuel_amount = 200;
	fuel_num = 200;
	fuel_time = 0;
	
	Left = false;
	Right = false;
	Jump = false;
	tile_speed =160*g_timestep_s;
}

/////////////////////////////////////////////////////////////
// Update() 
// 게임의 내용을 업데이트하는 함수.
// 실제 게임의 룰을 구현해야하는 곳.
// 게임에서 일어나는 변화는 모두 이 곳에서 구현한다.
// main 함수의 while loop에 의해서 무한히 반복 호출된다는 것을 주의.
void MainStage::Update() {
	if (Left)
	{
		p->move_left(g_timestep_s);
		if (bg_source.x > 0) {
			increase--;
			bg_source.x -= tile_speed;
		}
	}
	if (Right)
	{
		p->move_right(g_timestep_s);
		if (bg_source.x < 1280) {
			increase++;
			bg_source.x += tile_speed;
		}
	}
	p->move_jump(g_timestep_s);
	p->testOnPlatform(ground->posX(), ground->posY(), ground->width(), ground->height());

	

	for (int i = 0; i < platform_arr.size(); i++)
	{
		p->testOnPlatform(platform_arr[i]->posX(), platform_arr[i]->posY(), platform_arr[i]->width(), platform_arr[i]->height());
	}

	{	// 아이템 랜덤 생성 파트
		itemTime += 1 / 30.0f;
		if (itemTime >= 5)
		{
			itemTime = 0;
			//플레이어 주변 700, 700을 탐색 후 발판이 존재하면 생성할 아이템 반환
			ItemInterface* newItem = CreateItem(700, 700);
			if (newItem != nullptr)
			{
				item_arr.push_back(newItem);
			}
		}
	}

	//아이템 먹음
	DistinctItem();

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

	//SDL_SetRenderDrawColor(g_renderer, 0, 255, 0, 0);
	//배경을 그림
	SDL_RenderCopy(g_renderer, bg_texture, &bg_source, &bg_destination);
	

	//트럭을 그림
	SDL_RenderCopy(g_renderer, g_truck_sheet_texture, &g_truck_source_rect, g_truck->getRect());
	//연료 게이지를 그림
	SDL_RenderCopy(g_renderer, fuel_status, &status_source_rect, &status_destination_rect);

	//item_arr의 인자들을 그림(고철,연료)
	for (int i = 0; i < item_arr.size(); i++)
	{
		SDL_RenderCopy(g_renderer, item_arr[i]->getTexture(), item_arr[i]->getSrcRect(), item_arr[i]->getDstRect());
	}

	ground->draw_pf();

	//타일
	for (int i = 0; i < platform_arr.size(); i++)
	{
		platform_arr[i]->draw_pf();
	}
	for (int k = 0; k < map_w; k++) {
		for (int h = 0; h < map_h; h++) {
			tile_destination[k][h].w = 64;
			tile_destination[k][h].h = 64;
			if (tile[k][h].state == 1) {
				SDL_RenderCopy(g_renderer, tile_texture, &tile_source, &tile_destination[k][h]);
			}
		}
	}

	//플레이어
	p->draw_player();

	/*if (!g_stage_flag_running)
		DrawGameOverText();*/

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
				//g_current_game_phase = PHASE_ENDING;
			}
			break;

		case SDL_MOUSEBUTTONDOWN:

			// If the mouse left button is pressed. 
			if (event.button.button == SDL_BUTTON_LEFT){

				ItemInterface* return_item = p->giveItem();

				if (return_item == nullptr) return;

				if (return_item->getItemType() == FUEL)
				{
					cout << "RETURN FUEL!!\n";
				}
				else
				{
					cout << "RETURN IRON!!\n";
				}
				/*if (!g_stage_flag_running)
				{
					g_current_game_phase = PHASE_ENDING;
					InitGameObjectState();
				}
				delete game_phases[g_current_game_phase];
				game_phases[g_current_game_phase] = new Ending;*/
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
		fuel_amount = fuel_num - (fuel_time / 1000);
		fuel_last_time = fuel_time;
		status_source_rect.w = fuel_amount;
		status_destination_rect.w = fuel_amount;
		if (fuel_amount >= 200) {
			fuel_amount = 200;
		}
	}
	/*if (연료를 가져다 놨을 시) {
		fuel_num += 10;
	}*/
}

/////////////////////////////////////////////////////////////
// ClearGame() 
// 프로그램이 끝날 때 한 번 호출되는 함수.
// 이 함수에서 사용된 자원(이미지, 사운드 등)과 메모리 등을 해제해야한다.
// ClearGame() 대신 소멸자 사용
MainStage::~MainStage()
{
	SDL_DestroyTexture(g_truck_sheet_texture); // 트럭 메모리 해제
	/*SDL_DestroyTexture(g_fuel_sheet_texture); // 연료 메모리 해제
	SDL_DestroyTexture(g_iron_sheet_texture);*/ // 고철 메모리 해제
	SDL_DestroyTexture(g_gameover_text_kr); // 게임오버 텍스트 메모리 해제
	SDL_DestroyTexture(fuel_status); // 연료통 메모리 해제
	SDL_DestroyTexture(bg_texture); // 배경화면 메모리 해제
	SDL_DestroyTexture(tile_texture); // 타일 메모리 해제

	TTF_CloseFont(g_font_gameover); // 폰트 메모리 해제

	delete p;
	delete ground;
	//delete pf2;

	//Mix_FreeChunk(g_missile_fire_sound);
}

void MainStage::InitGameObjectState() // 인트로에서 게임페이즈로 넘어올 때 초기화가 필요한 변수들의 초기화 
{
	SDL_Rect truck_dst_init = { 0, 500, 160, 80 };
	g_truck = new Truck(truck_dst_init);

	/*SDL_Rect fuel_dst_init = {150, 150, 50, 50};
	ItemInterface *fuel = new Fuel(fuel_dst_init);

	item_arr.push_back(fuel);

	SDL_Rect iron_dst_init = { 350, 350, 50, 50 };
	ItemInterface *iron = new Iron(iron_dst_init);

	item_arr.push_back(iron);*/

	item_arr.clear();
	platform_arr.clear();
}
void MainStage::MakeGameObjTextures()
{
	SDL_Surface* truck_sheet_surface = IMG_Load("../../Resources/Truck.png"); // 이미지 파일을 가져옴
	SDL_SetColorKey(truck_sheet_surface, SDL_TRUE, SDL_MapRGB(truck_sheet_surface->format, 0, 155, 133));
	g_truck_sheet_texture = SDL_CreateTextureFromSurface(g_renderer, truck_sheet_surface);

	SDL_FreeSurface(truck_sheet_surface);
}

ItemInterface* MainStage::CreateItem(int windowX, int windowY)
{
	platform_tmp.clear();

	int left_up_x = p->posX() - windowX / 2, left_up_y = p->posY() - windowY / 2;
	int right_down_x = p->posX() + windowX / 2, right_down_y = p->posY() + windowY / 2;

	cout << "p->posX(): " << p->posX() << " p->posY(): " << p->posY() << increase<<"\n";
	cout << "left_up_x: " << left_up_x << " left_up_y: " << left_up_y << "\n";
	cout << "right_down_x: " << right_down_x << " right_down_y: " << right_down_y << "\n";

	for (int i = 0; i < platform_arr.size(); i++)
	{
		int platform_x = platform_arr[i]->posX();
		int platform_y = platform_arr[i]->posY();
		if (platform_x > left_up_x && platform_y > left_up_y &&
			platform_x < right_down_x && platform_y < right_down_y)
		{
			cout << "발견!!" << "\n";
			platform_tmp.push_back(platform_arr[i]);
		}
	}
	if (platform_tmp.empty()) return nullptr;

	int selected_platform_idx = Random(platform_tmp.size());
	SDL_Rect rect = platform_tmp[selected_platform_idx]->getRect();
	cout << "rect.x: " << rect.x << " rect.y: " << rect.y << "\n";

	rect.w = 50; rect.h = 50;
	int addX = Random(rect.w);
	rect.x += addX; rect.y -= 50;
	ItemInterface* item;

	if (Random(2) % 2 == FUEL)
	{
		item = new Fuel(rect);
	}
	else
	{
		item = new Iron(rect);
	}
	return item;
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
	for (int i = 0; i < item_arr.size(); i++)
	{
		SDL_Rect item_rect = *item_arr[i]->getDstRect();

		if (player_x > item_rect.x && player_y > item_rect.y &&
			player_x < item_rect.x + item_rect.w &&
			player_y < item_rect.y + item_rect.h)
		{
			ItemInterface* item = item_arr[i];
			if (p->getItem(item)) // 먹는데 성공했다면
			{
				if (item->getItemType() == FUEL)
				{
					cout << "GET FUEL!!\n";
				}
				else
				{
					cout << "GET IRON!!\n";
				}
				item_arr.erase(item_arr.begin() + i); // 지운다.
			}
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