#include "MainStage.h"
#include "SDL_image.h"
#include <windows.h>
#include <string>
#include <atlstr.h> // �ѱ��� ������ �ʿ���

#define SIZE 8
#define CELL 50
using namespace std;

enum Key
{
	LEFT,
	RIGHT,
	UP,
	DOWN
};

// Init() ��� �����ڸ� �����
MainStage::MainStage()
{
	MakeGameObjTextures();

	InitTexts();
	InitGameObjectState();

	g_bg_1_source_rect.x = 0;
	g_bg_1_source_rect.y = 0;
	g_bg_1_source_rect.w = 700;
	g_bg_1_source_rect.h = 700;

	g_bg_2_source_rect.x = 50;
	g_bg_2_source_rect.y = 50;
	g_bg_2_source_rect.w = 600;
	g_bg_2_source_rect.h = 600;

	g_truck_source_rect.x = 0; // Ʈ�� ��������
	g_truck_source_rect.y = 0;
	g_truck_source_rect.w = 160;
	g_truck_source_rect.h = 80;

	g_fuel_source_rect.x = 0; // ���� ��������
	g_fuel_source_rect.y = 0;
	g_fuel_source_rect.w = 64;
	g_fuel_source_rect.h = 64;

	g_iron_source_rect.x = 145; // ��� ��������
	g_iron_source_rect.y = 138;
	g_iron_source_rect.w = 392;
	g_iron_source_rect.h = 421;


	//���� ������Ʈ���� �ʱ�ȭ
	InitGameObjectState();

	// Clear the console screen.
	// ǥ����� ȭ���� ������ �����.
	system("cls");
}

/////////////////////////////////////////////////////////////
// Update() 
// ������ ������ ������Ʈ�ϴ� �Լ�.
// ���� ������ ���� �����ؾ��ϴ� ��.
// ���ӿ��� �Ͼ�� ��ȭ�� ��� �� ������ �����Ѵ�.
// main �Լ��� while loop�� ���ؼ� ������ �ݺ� ȣ��ȴٴ� ���� ����.
void MainStage::Update() {

	g_elapsed_time_ms += 33;
}

/////////////////////////////////////////////////////////////
// Render() 
// �׸��� �׸��� �Լ�.
// main �Լ��� while loop�� ���ؼ� ������ �ݺ� ȣ��ȴٴ� ���� ����.
void MainStage::Render() {

	SDL_SetRenderDrawColor(g_renderer, 0, 255, 0, 0);
	SDL_RenderFillRect(g_renderer, &g_bg_1_source_rect);

	SDL_SetRenderDrawColor(g_renderer, 255, 255, 255, 0);
	SDL_RenderFillRect(g_renderer, &g_bg_2_source_rect);

	//Ʈ���� �׸�
	SDL_RenderCopy(g_renderer, g_truck_sheet_texture, &g_truck_source_rect, g_truck->getRect());

	//���Ḧ �׸�
	SDL_RenderCopy(g_renderer, g_fuel_sheet_texture, &g_fuel_source_rect, g_fuel->getRect());

	//��ö�� �׸�
	SDL_RenderCopy(g_renderer, g_iron_sheet_texture, &g_iron_source_rect, g_iron->getRect());
	

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
			if (event.key.keysym.sym == SDLK_LEFT && g_cur_key != Key::RIGHT) {
				g_cur_key = Key::LEFT;
			}
			if (event.key.keysym.sym == SDLK_RIGHT && g_cur_key != Key::LEFT) {
				g_cur_key = Key::RIGHT;
			}
			if (event.key.keysym.sym == SDLK_UP && g_cur_key != Key::DOWN) {
				g_cur_key = Key::UP;
			}
			if (event.key.keysym.sym == SDLK_DOWN && g_cur_key != Key::UP) {
				g_cur_key = Key::DOWN;
			}
			if (event.key.keysym.sym == SDLK_SPACE) {
				//g_current_game_phase = PHASE_ENDING;
			}
			break;

		case SDL_MOUSEBUTTONDOWN:

			// If the mouse left button is pressed. 
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				if (!g_stage_flag_running)
				{
					g_current_game_phase = PHASE_ENDING;
					InitGameObjectState();
				}
				/*delete game_phases[g_current_game_phase];
				game_phases[g_current_game_phase] = new Ending;*/
			}
			break;

		default:
			break;
		}
	}
}

/////////////////////////////////////////////////////////////
// ClearGame() 
// ���α׷��� ���� �� �� �� ȣ��Ǵ� �Լ�.
// �� �Լ����� ���� �ڿ�(�̹���, ���� ��)�� �޸� ���� �����ؾ��Ѵ�.
// ClearGame() ��� �Ҹ��� ���
MainStage::~MainStage()
{
	SDL_DestroyTexture(g_truck_sheet_texture); // Ʈ�� �޸� ����
	SDL_DestroyTexture(g_fuel_sheet_texture); // ���� �޸� ����
	SDL_DestroyTexture(g_iron_sheet_texture); // ��ö �޸� ����
	SDL_DestroyTexture(g_gameover_text_kr); // ���ӿ��� �ؽ�Ʈ �޸� ����


	TTF_CloseFont(g_font_gameover); // ��Ʈ �޸� ����

	//Mix_FreeChunk(g_missile_fire_sound);
}

void MainStage::InitGameObjectState() // ��Ʈ�ο��� ����������� �Ѿ�� �� �ʱ�ȭ�� �ʿ��� �������� �ʱ�ȭ 
{
	SDL_Rect truck_dst_init = { 0, 500, 160, 80 };
	g_truck = new Truck(truck_dst_init);

	//pair<int, int> tempPos = CreateRandomPosition(); // ������ ��ġ ����
	SDL_Rect fuel_dst_init = { 100, 100, 50, 50 };
	g_fuel = new Fuel(fuel_dst_init);

	SDL_Rect iron_dst_init = { 350, 350, 50, 50 };
	g_iron = new Iron(iron_dst_init);

	/*tempPos = CreateRandomPosition(); // ����� ��ġ ����
	g_destination_apple.x = tempPos.first;
	g_destination_apple.y = tempPos.second;
	g_destination_apple.w = 50;
	g_destination_apple.h = 50;*/
	

	/*g_cur_key = -1;
	snakeList.clear();
	g_stage_flag_running = true;
	g_stage_is_colliding = false;
	MakeSnake();*/
}
void MainStage::MakeGameObjTextures()
{
	SDL_Surface* truck_sheet_surface = IMG_Load("../../Resources/Truck.png"); // �̹��� ������ ������
	SDL_SetColorKey(truck_sheet_surface, SDL_TRUE, SDL_MapRGB(truck_sheet_surface->format, 0, 155, 133));
	g_truck_sheet_texture = SDL_CreateTextureFromSurface(g_renderer, truck_sheet_surface);

	SDL_Surface* fuel_sheet_surface = IMG_Load("../../Resources/Fuel.png"); // �̹��� ������ ������
	SDL_SetColorKey(fuel_sheet_surface, SDL_TRUE, SDL_MapRGB(fuel_sheet_surface->format, 223, 113, 38));
	g_fuel_sheet_texture = SDL_CreateTextureFromSurface(g_renderer, fuel_sheet_surface);

	SDL_Surface* iron_sheet_surface = IMG_Load("../../Resources/apple.png"); // �̹��� ������ ������
	SDL_SetColorKey(iron_sheet_surface, SDL_TRUE, SDL_MapRGB(iron_sheet_surface->format, 255, 255, 255));
	g_iron_sheet_texture = SDL_CreateTextureFromSurface(g_renderer, iron_sheet_surface);

	SDL_FreeSurface(truck_sheet_surface);
	SDL_FreeSurface(fuel_sheet_surface);
	SDL_FreeSurface(iron_sheet_surface);
	

}

/*void MainStage::CreateApple()
{

}
bool MainStage::GetApple()
{
	int snake_x = snakeList.front().destination_snake.x;
	int snake_y = snakeList.front().destination_snake.y;

	int apple_x = g_destination_apple.x;
	int apple_y = g_destination_apple.y;

	if (snake_x == apple_x && snake_y == apple_y)
	{
		if (g_stage_is_colliding) return false;

		g_stage_is_colliding = true;
		return true;
	}
	else return false;
}*/
pair<int, int> MainStage::CreateRandomPosition()
{
	//CheckIsSnakeBody();

	srand((unsigned int)time(NULL));

	int x, y;
	while (true)
	{
		int rnd1 = rand(); int rnd2 = rand();
		x = (int)(rnd1 % 12 + 1); y = (int)(rnd2 % 12 + 1);

		if (!visited[x][y]) break;
	}

	return { x * CELL, y * CELL };
}
/*void MainStage::CheckIsSnakeBody()
{
	fill(&visited[0][0], &visited[12][13], false);

	for (auto iter = snakeList.begin(); iter != snakeList.end(); iter++)
	{
		visited[iter->destination_snake.x / 50][iter->destination_snake.y / 50] = true;
	}
}*/
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
