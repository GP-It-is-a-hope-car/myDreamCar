#include "MainStage.h"
#include "SDL_image.h"
#include <windows.h>
#include <string>
#include <atlstr.h> // �ѱ��� ������ �ʿ���
#include<vector>

#define SIZE 8
#define CELL 50
using namespace std;

Player * p;
Platform * ground;
vector<Platform*> platform_arr;

enum Key
{
	LEFT = 1,
	RIGHT,
	UP,
	DOWN
};
vector<Platform*> flatform_tmp;

bool Left;
bool Right;
bool Jump;

// Init() ��� �����ڸ� �����
MainStage::MainStage()
{
	MakeGameObjTextures();

	InitTexts();

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

	//���� ������Ʈ���� �ʱ�ȭ
	InitGameObjectState();

	p = new Player;
	ground = new Platform;
	
	for (int i = 1; i <= 3; i++) // �÷��� ��ġ for������ �������ְ� arr�� �־�����!
	{
		Platform *pf = new Platform(50*i, 100*i);
		platform_arr.push_back(pf);		
	}
	CheckCanCreateItem(700, 700);
	// Clear the console screen.
	// ǥ����� ȭ���� ������ �����.
	//system("cls");
	
	// ������ ����
	SDL_Surface* status_surface = IMG_Load("../../Resources/gauge.png");
	fuel_status = SDL_CreateTextureFromSurface(g_renderer, status_surface);
	SDL_FreeSurface(status_surface);
	status_source_rect = { 0,0,200,100 };
	status_destination_rect = { 400,600,200,50 };

	// �ð� ���� ����
	game_time = 360; //360��
	time_ms=0; 
	time_sec=0;

	Left = false;
	Right = false;
	Jump = false;
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
	}
	if (Right)
	{
		p->move_right(g_timestep_s);
	}
	p->move_jump(g_timestep_s);
	p->testOnPlatform(ground->posX(), ground->posY(), ground->width(), ground->height());

	
	for (int i = 0; i < platform_arr.size(); i++)
	{
		p->testOnPlatform(platform_arr[i]->posX(), platform_arr[i]->posY(), platform_arr[i]->width(), platform_arr[i]->height());
	}
	
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

	SDL_SetRenderDrawColor(g_renderer, 0, 255, 0, 0);
	SDL_RenderFillRect(g_renderer, &g_bg_1_source_rect);

	SDL_SetRenderDrawColor(g_renderer, 255, 255, 255, 0);
	SDL_RenderFillRect(g_renderer, &g_bg_2_source_rect);

	//Ʈ���� �׸�
	SDL_RenderCopy(g_renderer, g_truck_sheet_texture, &g_truck_source_rect, g_truck->getRect());
	//
	SDL_RenderCopy(g_renderer, fuel_status, &status_source_rect, &status_destination_rect);

	//item_arr�� ���ڵ��� �׸�(��ö,����)
	for (int i = 0; i < item_arr.size(); i++)
	{
		SDL_RenderCopy(g_renderer, item_arr[i]->getTexture(), item_arr[i]->getSrcRect(), item_arr[i]->getDstRect());
	}
	
	//SDL_RenderCopy(g_renderer, g_iron_sheet_texture, &g_iron_source_rect, g_iron->getRect());

	//�÷���
	ground->draw_pf();
	for (int i = 0; i < platform_arr.size(); i++)
	{		
		platform_arr[i]->draw_pf();
	}
	

	//�÷��̾�
	p->draw_player();
	
	//���� �ð��� �׸�
	{
		SDL_Rect r;
		r.x = 100;
		r.y = 50;
		r.w = text_time_rect.w = 50;
		r.h = text_time_rect.h = 50;
		SDL_RenderCopy(g_renderer, text_time, 0, &r);
	}

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
			if (event.button.button == SDL_BUTTON_LEFT)
			{
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
	{// �ð�
		static Uint32 last_ticks = SDL_GetTicks();
		Uint32 current_ticks = SDL_GetTicks();
		time_ms += current_ticks - last_ticks;
		time_sec = game_time - (time_ms / 1000);
		UpdateTimeTexture(time_sec);	
		last_ticks = time_ms;
		//������
		status_source_rect.w -= 0.2;
		status_destination_rect.w -= 0.2;
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
	/*SDL_DestroyTexture(g_fuel_sheet_texture); // ���� �޸� ����
	SDL_DestroyTexture(g_iron_sheet_texture);*/ // ��ö �޸� ����
	SDL_DestroyTexture(g_gameover_text_kr); // ���ӿ��� �ؽ�Ʈ �޸� ����
	SDL_DestroyTexture(fuel_status); // ������ �޸� ����
	SDL_DestroyTexture(text_time); // ���ѽð� �ؽ�Ʈ �޸� ����
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

	//pair<int, int> tempPos = CreateRandomPosition(); // ������ ��ġ ����
	SDL_Rect fuel_dst_init = { 150, 150, 50, 50 };
	ItemInterface *fuel = new Fuel(fuel_dst_init);

	item_arr.push_back(fuel);

	SDL_Rect iron_dst_init = { 350, 350, 50, 50 };
	ItemInterface *iron = new Iron(iron_dst_init);

	item_arr.push_back(iron);

	iron_dst_init = { 500, 500, 50, 50 };

	iron = new Iron(iron_dst_init);

	item_arr.push_back(iron);
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

	

	SDL_FreeSurface(truck_sheet_surface);

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
void MainStage::CheckCanCreateItem(int windowX, int windowY)
{
	flatform_tmp.clear();

	int left_up_x = p->posX() - windowX / 2, left_up_y = p->posY() - windowY / 2;
	int right_down_x = p->posX() + windowX / 2, right_down_y = p->posY() + windowY / 2;

	cout << "p->posX(): " << p->posX() << " p->posY(): " << p->posY() << "\n";
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
			flatform_tmp.push_back(platform_arr[i]);
		}
	}
	int selected_platform_idx = Random(flatform_tmp.size());
	SDL_Rect rect = flatform_tmp[selected_platform_idx]->getRect();


}
int MainStage::Random(int n) // 0���� n - 1���� ���� �� �߻�
{
	srand((unsigned int)time(NULL));

	int rnd = rand();
	int a = (int)(rnd % (n));

	return a;
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
void MainStage::UpdateTimeTexture(int ms) {

	SDL_Surface* tmp2_surface = TTF_RenderText_Blended(g_font_gameover, std::to_string((long long)ms).c_str(), black);

	text_time_rect.x = 0;
	text_time_rect.y = 0;
	text_time_rect.w = tmp2_surface->w;
	text_time_rect.h = tmp2_surface->h;
	text_time = SDL_CreateTextureFromSurface(g_renderer, tmp2_surface);
	SDL_FreeSurface(tmp2_surface);

}