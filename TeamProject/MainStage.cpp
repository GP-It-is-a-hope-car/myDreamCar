#include "MainStage.h"
#include "SDL_image.h"
#include <windows.h>
#include <string>
#include <atlstr.h> // �ѱ��� ������ �ʿ���
#include <vector>
#include "GameFunc.h"

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
extern bool g_stage_flag_running;

// Ÿ�� ����ü ����
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
// Init() ��� �����ڸ� �����
MainStage::MainStage()
{
	g_stage_flag_running = true;

	g_main_mus = Mix_LoadMUS("../../Resources/stage.mp3"); // ������� �ε�	
	Mix_VolumeMusic(40);

	MakeGameObjTextures();

	InitTexts();
	//���
	SDL_Surface* bg_surface= IMG_Load("../../Resources/BG.png");
	bg_texture= SDL_CreateTextureFromSurface(g_renderer, bg_surface);
	SDL_FreeSurface(bg_surface);
	bg_source= { 0,0,640,640 };
	bg_destination = { 0,0,640,640 };
	//Ÿ�� (�ٴ�)
	SDL_Surface* tile_surface = IMG_Load("../../Resources/tile_m.png");
	tile_texture = SDL_CreateTextureFromSurface(g_renderer, tile_surface);
	SDL_FreeSurface(tile_surface);
	tile_source = { 0,0,500,300 };
	advancement_destination = { 32 ,10,576,64 };

	g_truck_source_rect[0] = { 0,0,160,80 };
	g_truck_source_rect[1] = { 160,0,320,80 };
	
	//��ô��
	SDL_Surface* advancement_surface = IMG_Load("../../Resources/gauge.png");
	advancement_texture = SDL_CreateTextureFromSurface(g_renderer, advancement_surface);
	SDL_FreeSurface(advancement_surface);
	advancement_source = {85,305,640,65 };

	p = new Player;
	ground = new Platform;
	pf = new Platform(0, 0);
	fuel = new Fuel({ 0,0,0,0 });
	iron = new Iron({ 0,0,0,0 });
	// Clear the console screen.
	// ǥ����� ȭ���� ������ �����.
	//system("cls");

	// ���� ������ ����
	SDL_Surface* status_surface = IMG_Load("../../Resources/meter.png");
	fuel_status = SDL_CreateTextureFromSurface(g_renderer, status_surface);
	SDL_FreeSurface(status_surface);
	status_source_rect = { 29,30,180,160 };
	status_destination_rect = { 0,512,128,128 };
	
	//���� ������Ʈ���� �ʱ�ȭ
	InitGameObjectState();
	metal_count = 0;
}

/////////////////////////////////////////////////////////////
// Update() 
// ������ ������ ������Ʈ�ϴ� �Լ�.
// ���� ������ ���� �����ؾ��ϴ� ��.
// ���ӿ��� �Ͼ�� ��ȭ�� ��� �� ������ �����Ѵ�.
// main �Լ��� while loop�� ���ؼ� ������ �ݺ� ȣ��ȴٴ� ���� ����.
void MainStage::Update() {
	
	//���� ������� ������ ����
	if (!g_stage_flag_running) return;

	// Ʈ���� �������� ���
	truck_motion_cur++;
	if (truck_motion_cur >= truck_motion_num) {
		truck_motion_cur = 0;
	}

	//Ʈ���� �����̴� �ӵ�
	truck_time += 1 / 30.0f;

	// Ʈ���� ���� �����ϸ� ���ǿ���
	if (g_truck->getDstRect()->x > tile_destination[59][8].x ) {

		isMainPlay = false;
		InitGameObjectState();
		g_stage_flag_running = false;

		if (Mix_PlayingMusic())
			Mix_HaltMusic();

		g_current_game_phase = PHASE_HAPPY_ENDING;
	}

	g_truck->getDstRect()->x += 1;

	if (Left)
	{
		p->move_left(g_timestep_s);

		if (p->posX()==256) {
			increase--;
			bg_source.x -= tile_speed;
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
			bg_source.x += tile_speed;
			g_truck->getDstRect()->x -= tile_speed;
		}
		else if (p->posX() != 288) {
			bg_source.x = bg_source.x;
			increase = increase;
		}
	}
	p->move_jump(g_timestep_s);

	// �ٴ� �浹 ���� + �÷��� �浹 ���� + ���� �ð� ��� �� ������ ����
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

	{	// ������ ���� ���� ��Ʈ
		itemTime += 1 / 30.0f;
		if (itemTime >= 2.0f) // ������ ���� �ֱ�
		{
			itemTime = 0;
			CreateItem();			
		}
	}

	//������ ����
	DistinctItem();

	//Ʈ���� �÷��̾��� ��ġ�� �˻� �� ��Ҵٸ� �������� Ʈ���� ��ȯ
	GiveItemToTruck();

	g_elapsed_time_ms += 33;
	if (time_sec < 0) {
		//g_current_game_phase = PHASE_ENDING;
	}
	
	// ��ô�� ����
	if (g_truck->getDstRect()->x >= tile_destination[12][8].x && g_truck->getDstRect()->x < tile_destination[24][8].x) {
		advancement_source = { 85,245,650,65 };
	}
	else if (g_truck->getDstRect()->x >= tile_destination[24][8].x && g_truck->getDstRect()->x < tile_destination[36][8].x) {
		advancement_source = { 85,180,650,65 };
	}
	else if (g_truck->getDstRect()->x >= tile_destination[36][8].x && g_truck->getDstRect()->x < tile_destination[48][8].x) {
		advancement_source = { 85,120,650,65 };
	}
	else if (g_truck->getDstRect()->x >= tile_destination[48][8].x && g_truck->getDstRect()->x < tile_destination[59][8].x) {
		advancement_source = { 85,60,650,65 };
	}



	// ���� ������ ����
	if (fuel_amount < 3600 && fuel_amount >= 3200) { //90
		status_source_rect = {226,27,180,160};
	}
	else if (fuel_amount < 3200 && fuel_amount >= 2800) { //80
		status_source_rect = { 431,27,180,160 };
	}
	else if (fuel_amount < 2800 && fuel_amount >= 2400) {//70
		status_source_rect = { 31,204,180,160 };
	}
	else if (fuel_amount < 2400 && fuel_amount >= 2000) {//60
		status_source_rect = { 221,204,180,160 };
	}
	else if (fuel_amount < 2000 && fuel_amount >= 1600) {//50
		status_source_rect = { 431,204,180,160 };
	}
	else if (fuel_amount < 1600 && fuel_amount >= 1200) {//40
		status_source_rect = { 14,380,180,160 };
	}
	else if (fuel_amount < 1200 && fuel_amount >= 800) {//30
		status_source_rect = { 221,383,180,160 };
	}
	else if (fuel_amount < 800 && fuel_amount >= 400) {//20
		status_source_rect = { 431,383,180,160 };
	}
	else if (fuel_amount < 400 && fuel_amount >= 0) {//10
		status_source_rect = { 16,540,180,160 };
	}

	if (!isMainPlay && g_current_game_phase == PHASE_MAINSTAGE)
	{
		if (Mix_PlayingMusic())
			Mix_HaltMusic();

		Mix_FadeInMusic(g_main_mus, -1, 2000); // ������� �÷���
		isMainPlay = true;
	}

}

/////////////////////////////////////////////////////////////
// Render() 
// �׸��� �׸��� �Լ�.
// main �Լ��� while loop�� ���ؼ� ������ �ݺ� ȣ��ȴٴ� ���� ����.
void MainStage::Render() {

	//����� �׸�
	SDL_RenderCopy(g_renderer, bg_texture, &bg_source, &bg_destination);
	//��ô���� �׸�
	SDL_RenderCopy(g_renderer, advancement_texture, &advancement_source, &advancement_destination);
	//Ʈ���� �׸�
	SDL_RenderCopy(g_renderer, g_truck_sheet_texture, &g_truck_source_rect[truck_motion_cur], g_truck->getDstRect());
	
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

	//�÷��̾�
	if (!g_stage_flag_running)
	{
		p->gameover();
	}

	p->draw_player();
	p->showItem();

	//���� �������� �׸�
	SDL_RenderCopy(g_renderer, fuel_status, &status_source_rect, &status_destination_rect);

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
			}
			if (event.key.keysym.sym == SDLK_q) {
				fuel_amount = 0;
			}
			if (event.key.keysym.sym == SDLK_w) {
				g_truck->getDstRect()->x = tile_destination[59][9].x;
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
		fuel_amount -= 2;
		if (fuel_amount >= 4000) {
			fuel_amount = 4000;
		}
		else if (fuel_amount <= 0) {
			InitGameObjectState();

			if (Mix_PlayingMusic())
				Mix_HaltMusic();

			isMainPlay = false;
			g_stage_flag_running = false;

			g_current_game_phase = PHASE_ENDING;		
		}
	}
	
	if (bg_source.x==3200) {
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
// ���α׷��� ���� �� �� �� ȣ��Ǵ� �Լ�.
// �� �Լ����� ���� �ڿ�(�̹���, ���� ��)�� �޸� ���� �����ؾ��Ѵ�.
// ClearGame() ��� �Ҹ��� ���
MainStage::~MainStage()
{
	SDL_DestroyTexture(g_truck_sheet_texture); // Ʈ�� �޸� ����
	SDL_DestroyTexture(g_gameover_text_kr); // ���ӿ��� �ؽ�Ʈ �޸� ����
	SDL_DestroyTexture(fuel_status); // ������ �޸� ����
	SDL_DestroyTexture(bg_texture); // ���ȭ�� �޸� ����
	SDL_DestroyTexture(tile_texture); // Ÿ�� �޸� ����
	SDL_DestroyTexture(advancement_texture); // ��ô�� �޸� ����

	TTF_CloseFont(g_font_gameover); // ��Ʈ �޸� ����

	delete p;
	delete ground;

	//Mix_FreeChunk(g_missile_fire_sound);
}

void MainStage::InitGameObjectState() // ��Ʈ�ο��� ����������� �Ѿ�� �� �ʱ�ȭ�� �ʿ��� �������� �ʱ�ȭ 
{
	//Ÿ�� ����
	for (int i = 0; i < map_w; i++) {
		for (int j = 0; j < map_h; j++) {

			tile_destination[i][j].w = 64;
			tile_destination[i][j].h = 64;
			tile[i][j].row = i;
			tile[i][j].col = j;
			tile[i][j].state = EMPTY;
		}
	}
	// �� �׸�
	for (int k = 0; k < map_w; k++) {
		tile[k][9].state = GROUND;
	}
	// ���� �׸�
	for (int i = 0; i < 1; i++) { //1ĭ
		tile[i][7].state = PLATFORM;
		tile[i + 2][5].state = PLATFORM;
		tile[i + 11][7].state = PLATFORM;
		tile[i + 37][7].state = PLATFORM;
		tile[i + 33][3].state = PLATFORM;
		tile[i + 53][4].state = PLATFORM;
	}
	for (int i = 0; i < 2; i++) { //2ĭ
		tile[i][2].state = PLATFORM;
		tile[i+8][5].state = PLATFORM;
		tile[i+11][3].state = PLATFORM;
		tile[i + 14][4].state = PLATFORM;
		tile[i + 17][3].state = PLATFORM;
		tile[i + 21][2].state = PLATFORM;
		tile[i + 14][6].state = PLATFORM;
		tile[i + 35][2].state = PLATFORM;
		tile[i + 55][5].state = PLATFORM;
	}
	for (int i = 0; i < 3; i++) { //3ĭ
		tile[i+4][7].state = PLATFORM;
		tile[i +4][3].state = PLATFORM;
		tile[i + 19][5].state = PLATFORM;
		tile[i + 29][5].state = PLATFORM;
		tile[i + 37][5].state = PLATFORM;
		tile[i + 45][5].state = PLATFORM;
		tile[i + 17][7].state = PLATFORM;
		tile[i + 25][7].state = PLATFORM;
		tile[i + 33][7].state = PLATFORM;
		tile[i + 41][7].state = PLATFORM;
		tile[i + 49][7].state = PLATFORM;
		tile[i + 25][3].state = PLATFORM;
		tile[i + 41][3].state = PLATFORM;
		tile[i + 49][3].state = PLATFORM;
		tile[i + 57][3].state = PLATFORM;
	}
	for (int i = 0; i < 4; i++) { //4ĭ
		tile[i + 54][7].state = PLATFORM;
	}
	for (int i = 0; i < map_w; i++) {
		for (int j = 0; j < map_h; j++) {
			tile[i][j].tile_posx = i * 64;
			tile[i][j].tile_posy = j * 64;
			tile_destination[i][j].x = tile[i][j].tile_posx;
			tile_destination[i][j].y = tile[i][j].tile_posy;
		}
	}
	increase = 0;
	range = 0;
	// ���� ������ ���� ����
	fuel_amount = 4000;
	
	//��� ���� ����
	truck_motion_num = 2;
	truck_motion_cur = 0;
	Left = false;
	Right = false;
	tile_speed = 320 * g_timestep_s;

	SDL_Rect truck_dst_init = { tile_destination[0][8].x, tile_destination[0][8].y - 14, 160, 80};

	//Ʈ�� ��ġ �ʱ�ȭ
	if (g_truck == nullptr)
		g_truck = new Truck(truck_dst_init);
	else
		g_truck->setDstRect(truck_dst_init);

	//�÷��̾� �ʱ�ȭ
	p->reInit();
	p->setPosX(tile_destination[0][8].x + 100);
	p->setPosY(tile_destination[0][8].y - 14);

	// ��� ��ġ + ���� ������ + ��ô�� �ʱ�ȭ
	bg_source = { 0,0,640,640 };
	bg_destination = { 0,0,640,640 };
	status_source_rect = { 29,30,180,160 };
	advancement_source = { 85,305,640,65 };
	// ���� �� ������ ����
	for (int i = 0; i < 5; i++) // ������ 5���� ����
	{
		CreateItem();
	}
}
void MainStage::MakeGameObjTextures()
{
	SDL_Surface* truck_sheet_surface = IMG_Load("../../Resources/truck_motion.png"); // �̹��� ������ ������
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

	for (int i = 0; i < map_w; i++) {
		for (int j = 0; j < map_h; j++) {
			if (tile[i][j].state == FUEL || tile[i][j].state == IRON)
			{
				SDL_Rect item_rect = tile_destination[i][j];
				if (player_x > item_rect.x && player_y > item_rect.y &&
					player_x < item_rect.x + item_rect.w &&
					player_y < item_rect.y + item_rect.h)
				{
					if (p->getItem(tile[i][j].state)) // �Դµ� �����ߴٸ�
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
			fuel_amount += 400;
			cout << "RETURN FUEL!!\n";
		}
		else
		{
			metal_count++;
			cout << metal_count << "\n";
			cout << "RETURN IRON!!\n";
		}
	}
}
void MainStage::DrawGameText()
{

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