#include "MainStage.h"
#include "SDL_image.h"
#include <windows.h>
#include <string>
#include <atlstr.h> // �ѱ��� ������ �ʿ���
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

// Ÿ�� ����ü ����
typedef struct t {
	int tile_posx;
	int tile_posy;
	int state;
};
t tile[map_w][map_h];

// Init() ��� �����ڸ� �����
MainStage::MainStage()
{
	MakeGameObjTextures();

	InitTexts();
	//���
	SDL_Surface* bg_surface= IMG_Load("../../Resources/dummy_bg.png");
	bg_texture= SDL_CreateTextureFromSurface(g_renderer, bg_surface);
	SDL_FreeSurface(bg_surface);
	bg_source= { 0,0,640,640 };
	bg_destination = { 0,0,640,640 };
	//Ÿ�� (�ٴ�)
	SDL_Surface* tile_surface = IMG_Load("../../Resources/tile_m.png");
	tile_texture = SDL_CreateTextureFromSurface(g_renderer, tile_surface);
	SDL_FreeSurface(tile_surface);
	tile_source = { 0,0,500,300 };

	g_truck_source_rect.x = 0; // Ʈ�� ��������
	g_truck_source_rect.y = 0;
	g_truck_source_rect.w = 160;
	g_truck_source_rect.h = 80;

	//���� ������Ʈ���� �ʱ�ȭ
	InitGameObjectState();

	p = new Player;
	ground = new Platform;

	for (int i = 1; i <= 5; i++) // �÷��� ��ġ for������ �������ְ� arr�� �־�����!
	{
		Platform* pf = new Platform(50 * i, 90 * i);
		platform_arr.push_back(pf);
	}
	// Clear the console screen.
	// ǥ����� ȭ���� ������ �����.
	//system("cls");

	// ���� ������ ����
	SDL_Surface* status_surface = IMG_Load("../../Resources/gauge.png");
	fuel_status = SDL_CreateTextureFromSurface(g_renderer, status_surface);
	SDL_FreeSurface(status_surface);
	status_source_rect = { 0,0,200,100 };
	status_destination_rect = { 0,0,200,40 };


	//Ÿ�� ����
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

	// ���� ������ ���� ����
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
// ������ ������ ������Ʈ�ϴ� �Լ�.
// ���� ������ ���� �����ؾ��ϴ� ��.
// ���ӿ��� �Ͼ�� ��ȭ�� ��� �� ������ �����Ѵ�.
// main �Լ��� while loop�� ���ؼ� ������ �ݺ� ȣ��ȴٴ� ���� ����.
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

	{	// ������ ���� ���� ��Ʈ
		itemTime += 1 / 30.0f;
		if (itemTime >= 5)
		{
			itemTime = 0;
			//�÷��̾� �ֺ� 700, 700�� Ž�� �� ������ �����ϸ� ������ ������ ��ȯ
			ItemInterface* newItem = CreateItem(700, 700);
			if (newItem != nullptr)
			{
				item_arr.push_back(newItem);
			}
		}
	}

	//������ ����
	DistinctItem();

	g_elapsed_time_ms += 33;
	if (time_sec < 0) {
		//g_current_game_phase = PHASE_ENDING;
	}
}

/////////////////////////////////////////////////////////////
// Render() 
// �׸��� �׸��� �Լ�.
// main �Լ��� while loop�� ���ؼ� ������ �ݺ� ȣ��ȴٴ� ���� ����.
void MainStage::Render() {

	//SDL_SetRenderDrawColor(g_renderer, 0, 255, 0, 0);
	//����� �׸�
	SDL_RenderCopy(g_renderer, bg_texture, &bg_source, &bg_destination);
	

	//Ʈ���� �׸�
	SDL_RenderCopy(g_renderer, g_truck_sheet_texture, &g_truck_source_rect, g_truck->getRect());
	//���� �������� �׸�
	SDL_RenderCopy(g_renderer, fuel_status, &status_source_rect, &status_destination_rect);

	//item_arr�� ���ڵ��� �׸�(��ö,����)
	for (int i = 0; i < item_arr.size(); i++)
	{
		SDL_RenderCopy(g_renderer, item_arr[i]->getTexture(), item_arr[i]->getSrcRect(), item_arr[i]->getDstRect());
	}

	ground->draw_pf();

	//Ÿ��
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

	//�÷��̾�
	p->draw_player();

	/*if (!g_stage_flag_running)
		DrawGameOverText();*/

		// �鿡�� �׸� �׸��� �ѹ��� ������
	SDL_RenderPresent(g_renderer);
}

/////////////////////////////////////////////////////////////
// HandleEvents() 
// �̺�Ʈ�� ó���ϴ� �Լ�.
// main �Լ��� while loop�� ���ؼ� ������ �ݺ� ȣ��ȴٴ� ���� ����.
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
		// Ÿ�� �ʱ�ȭ
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
		
		//������
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
	/*if (���Ḧ ������ ���� ��) {
		fuel_num += 10;
	}*/
}

/////////////////////////////////////////////////////////////
// ClearGame() 
// ���α׷��� ���� �� �� �� ȣ��Ǵ� �Լ�.
// �� �Լ����� ���� �ڿ�(�̹���, ���� ��)�� �޸� ���� �����ؾ��Ѵ�.
// ClearGame() ��� �Ҹ��� ���
MainStage::~MainStage()
{
	SDL_DestroyTexture(g_truck_sheet_texture); // Ʈ�� �޸� ����
	/*SDL_DestroyTexture(g_fuel_sheet_texture); // ���� �޸� ����
	SDL_DestroyTexture(g_iron_sheet_texture);*/ // ��ö �޸� ����
	SDL_DestroyTexture(g_gameover_text_kr); // ���ӿ��� �ؽ�Ʈ �޸� ����
	SDL_DestroyTexture(fuel_status); // ������ �޸� ����
	SDL_DestroyTexture(bg_texture); // ���ȭ�� �޸� ����
	SDL_DestroyTexture(tile_texture); // Ÿ�� �޸� ����

	TTF_CloseFont(g_font_gameover); // ��Ʈ �޸� ����

	delete p;
	delete ground;
	//delete pf2;

	//Mix_FreeChunk(g_missile_fire_sound);
}

void MainStage::InitGameObjectState() // ��Ʈ�ο��� ����������� �Ѿ�� �� �ʱ�ȭ�� �ʿ��� �������� �ʱ�ȭ 
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
	SDL_Surface* truck_sheet_surface = IMG_Load("../../Resources/Truck.png"); // �̹��� ������ ������
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
			cout << "�߰�!!" << "\n";
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
int MainStage::Random(int n) // 0���� n - 1���� ���� �� �߻�
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
			if (p->getItem(item)) // �Դµ� �����ߴٸ�
			{
				if (item->getItemType() == FUEL)
				{
					cout << "GET FUEL!!\n";
				}
				else
				{
					cout << "GET IRON!!\n";
				}
				item_arr.erase(item_arr.begin() + i); // �����.
			}
		}
	}
}
void MainStage::DrawGameText()
{

}
void MainStage::DrawGameOverText()
{
	SDL_Rect tmp_r; // ȭ�鿡 ǥ�� �� ��ġ

	tmp_r.x = 265;
	tmp_r.y = 325;
	tmp_r.w = g_gameover_text_kr_rect.w;
	tmp_r.h = g_gameover_text_kr_rect.h;

	SDL_RenderCopy(g_renderer, g_gameover_text_kr, &g_gameover_text_kr_rect, &tmp_r); // �ؽ�Ʈ ǥ��
}
void MainStage::InitChunk()
{
	/*g_missile_fire_sound = Mix_LoadWAV("../../Resources/fire.mp3"); // ȿ���� �ε�
	Mix_VolumeChunk(g_missile_fire_sound, 24);

	g_open_box_sound = Mix_LoadWAV("../../Resources/Coin.wav"); // ȿ���� �ε�
	Mix_VolumeChunk(g_open_box_sound, 24);

	g_ride_tank_sound = Mix_LoadWAV("../../Resources/IO.mp3"); // ȿ���� �ε�
	Mix_VolumeChunk(g_ride_tank_sound, 24);*/

}

void MainStage::InitTexts()
{
	g_font_gameover = TTF_OpenFont("../../Resources/MaruBuri-SemiBold.ttf", 32);
	SDL_Surface* tmp_surface_1 = TTF_RenderUTF8_Blended(g_font_gameover, CW2A(L"Game Over!", CP_UTF8), black);
	//�ؽ�Ʈ ��������
	g_gameover_text_kr_rect.x = 0;
	g_gameover_text_kr_rect.y = 0;
	g_gameover_text_kr_rect.w = tmp_surface_1->w;
	g_gameover_text_kr_rect.h = tmp_surface_1->h;

	g_gameover_text_kr = SDL_CreateTextureFromSurface(g_renderer, tmp_surface_1);
	SDL_FreeSurface(tmp_surface_1);
}