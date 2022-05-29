#include "Player.h"

extern int range;
Player::Player()
{
	//플레이어 텍스처, 렉트, SFX등 초기화
	SDL_Surface* player_surface = IMG_Load("../../Resources/Sprites.png");
	texture_player_ = SDL_CreateTextureFromSurface(g_renderer, player_surface);
	SDL_FreeSurface(player_surface);

	source_rects_player_[0] = { 52,28,176,224 };
	source_rects_player_[1] = { 308,24,176,224 };
	source_rects_player_[2] = { 568,20,176,224 };
	source_rects_player_[3] = { 824,20,176,224 };
	source_rects_player_[4] = { 1084,20,176,224 };
	source_rects_player_[5] = { 1311,24,176,224 };
	source_rects_player_[6] = { 1772,24,176,224 };
	source_rects_player_[7] = { 2028,24,176,224 };
	source_rects_player_[8] = { 2352,24,176,224 };
	source_rects_player_[9] = { 2636,24,176,224 };

	dest_rect_player_.x = 0;
	dest_rect_player_.x = 0;
	dest_rect_player_.w = 51;
	dest_rect_player_.h = 64;
	
	SDL_Surface* fuel_surface = IMG_Load("../../Resources/Sprites.png");
	texture_fuel_ = SDL_CreateTextureFromSurface(g_renderer, fuel_surface);
	SDL_FreeSurface(fuel_surface);

	source_rect_fuel_.x = 686;
	source_rect_fuel_.y = 842;
	source_rect_fuel_.w = source_rect_fuel_.h = 256;
	dest_rect_fuel_.x = 0;
	dest_rect_fuel_.y = 0;
	dest_rect_fuel_.w = source_rect_fuel_.w/8;
	dest_rect_fuel_.h = source_rect_fuel_.h/8;

	SDL_Surface* iron_surface = IMG_Load("../../Resources/Sprites.png");
	texture_iron_ = SDL_CreateTextureFromSurface(g_renderer, iron_surface);
	SDL_FreeSurface(iron_surface);

	source_rect_iron_.x = 117;
	source_rect_iron_.y = 932;
	source_rect_iron_.w = source_rect_iron_.h = 128;
	dest_rect_iron_.x = 0;
	dest_rect_iron_.y = 0;
	dest_rect_iron_.w = source_rect_iron_.w/4;
	dest_rect_iron_.h = source_rect_iron_.h/4;

	//SFX
	get_item_sound_ = Mix_LoadWAV("../../Resources/item_wav.wav");
	jump_sound_ = Mix_LoadWAV("../../Resources/jump_wav.wav");
	
	//각종 변수들 초기화
	//중력 가속도는 우리 게임에 맞게 적당히 수정해야함
	index = 0;
	prev = -1;
	verticalSpeed_ = 0;
	horizontalSpeed_ = 240;
	mass_ = 2;
	gravityAcc_ = 1960;
	isHoldItem_ = false;
	isJump_ = false;
	ownItem_ = -1;
}

Player::~Player()
{
	//사용한 자원들 반환
	SDL_DestroyTexture(texture_player_);
	SDL_DestroyTexture(texture_fuel_);
	SDL_DestroyTexture(texture_iron_);
	//delete ownItem_;

}

void Player::testOnPlatform(double pf_posX, double pf_posY, double pf_width, double pf_height)
{
	//플레이어의 좌표와 발판의 좌표 사이에서 검사
	//발판 위에서의 충돌이 확인되면 중력이 적용 X
	if (dest_rect_player_.x + dest_rect_player_.w * 0.95 > pf_posX &&
		dest_rect_player_.x + dest_rect_player_.w * 0.05 < pf_posX + pf_width)
	{
		if (dest_rect_player_.y + dest_rect_player_.h >= pf_posY &&
			dest_rect_player_.y + dest_rect_player_.h < pf_posY + pf_height*0.3)
		{
			if (verticalSpeed_ > 0)
			{
				isJump_ = false;
				dest_rect_player_.y = pf_posY - dest_rect_player_.h;
				verticalSpeed_ = 0;
			}
		}
	}
}

bool Player::getItem(int in)
{
	//검사결과 겹치면 아이템을 획득
	//isHoldItem = true
	//ownItem = 들고 있는 아이템 종류
	//이걸 위해서 아이템 클래스를 받아오거나 해야할 것 같음 
	if (!isHoldItem_ && ownItem_ == -1)
	{
		Mix_PlayChannel(-1, get_item_sound_, 0);
		ownItem_ = in;
		isHoldItem_ = true;
		return true;
	}
	return false;
}

void Player::showItem()
{
	if (isHoldItem_)
	{
		if (ownItem_ == FUEL)
		{
			dest_rect_fuel_.x = dest_rect_player_.x + 13;
			dest_rect_fuel_.y = dest_rect_player_.y - 48;
			SDL_RenderCopy(g_renderer, texture_fuel_, &source_rect_fuel_, &dest_rect_fuel_);
		}
		else if (ownItem_ == IRON)
		{
			dest_rect_iron_.x = dest_rect_player_.x + 13;
			dest_rect_iron_.y = dest_rect_player_.y - 48;
			SDL_RenderCopy(g_renderer, texture_iron_, &source_rect_iron_, &dest_rect_iron_);
		}
	}
}

int Player::giveItem()
{
	//isHoldItem == true
	//검사결과 겹치면 트럭에 아이템에 전달 -> 반환값이 있다면 데이터타입은?
	//ownItem 종류에 따라 다른 역할
	//isHoldItem = false
	//ownItem = null
	//아이템 클래스를 반환하거나 뭔가 동작이 필요
	if (isHoldItem_)
	{
		int tmp_own;
		tmp_own = ownItem_;
		ownItem_ = -1;
		isHoldItem_ = false;
		return tmp_own;
	}
	return -1; // 아무것도 안들고 있다면 -1 반환
}

void Player::move_left(double timestep_s)
{
	//간단함 왼쪽방향키 누르면 수평속도만큼 이동
	//점프랑은 다르게 따로 가속도 없이 등속으로 이동
	//프레임 단위로 시간 받아서 초당 움직이는게 더 적당?
	//점프 중에는 가속도를 기준으로 계산한 속도로 이동하게
	//이동속도는 적당한 것 찾을 예정
	//왼쪽 바라보는 스프라이트

	if (prev != 3)
	{
		index = 3;
	}
	else
	{
		index = 4;
	}

	double dt = timestep_s;
	horizontalSpeed_ = 240;
	dest_rect_player_.x = dest_rect_player_.x - dt * horizontalSpeed_;
	if (dest_rect_player_.x<256&&range==2) {
		dest_rect_player_.x = dest_rect_player_.x - dt * horizontalSpeed_;
	}
	if (dest_rect_player_.x > 256 && range == 1) {
		dest_rect_player_.x = dest_rect_player_.x - dt * horizontalSpeed_;
	}
	else if (dest_rect_player_.x <= 256 && range != 2) {
		dest_rect_player_.x = 256;
	}

	if (dest_rect_player_.x < 0)
	{
		dest_rect_player_.x = 0;
	}
	
	prev = index;

	if (isJump_)
	{
		index = 5;
	}
}

void Player::move_right(double timestep_s)
{
	//마찬가지
	//매개변수로 키이벤트를 받아 좌우를 하나로 묶을 수 있을 것으로 보임
	//오른쪽 바라보는 스프라이트

	if (prev != 0)
	{
		index = 0;
	}
	else
	{
		index = 1;
	}
	double dt = timestep_s;

	horizontalSpeed_ = 240;
	dest_rect_player_.x = dest_rect_player_.x + dt * horizontalSpeed_;
	if (range==1&&dest_rect_player_.x >288) {
		dest_rect_player_.x = dest_rect_player_.x + dt * horizontalSpeed_;
	}
	else if (range == 2 && dest_rect_player_.x < 288) {
		dest_rect_player_.x = dest_rect_player_.x +dt * horizontalSpeed_;
	}
	else if (range != 1 && dest_rect_player_.x >=288) {
		dest_rect_player_.x = 288;
	}

	if (dest_rect_player_.x > 589)
	{
		dest_rect_player_.x = 589;
	}

	prev = index;

	if (isJump_)
	{
		index = 2;
	}
}


void Player::move_jump(double timestep_s)
{
	double dt = timestep_s;
	
	dest_rect_player_.y = dest_rect_player_.y + dt * verticalSpeed_;	
	verticalSpeed_ = verticalSpeed_ + dt * gravityAcc_;
}


void Player::jump()
{
	if (isJump_ == false)
	{
		isJump_ = true;
		//ball launch의 내용 중 y축에 대한 내용만을 이곳에 구현
		verticalSpeed_ = verticalSpeed_ - (21 * dest_rect_player_.h) / mass_;
		Mix_PlayChannel(-1, jump_sound_, 0);
	}
}

void Player::draw_player()
{
	SDL_RenderCopy(g_renderer, texture_player_, &source_rects_player_[index], &dest_rect_player_);
}

//게임오버 시에 업데이트 넣을 내용
void Player::gameover()
{
	if (index < 3 || index == 7)
	{
		index = 6;
	}
	else if (index == 6)
	{
		index = 7;
	}

	if ((index > 2 && index < 6) || index == 9)
	{
		index = 8;
	}
	else if (index == 8)
	{
		index = 9;
	}
}

void Player::reInit()
{
	index =  0;
	prev = -1;
	verticalSpeed_ = 0;
	horizontalSpeed_ = 240;
	mass_ = 2;
	gravityAcc_ = 1960;
	isHoldItem_ = false;
	isJump_ = false;
	ownItem_ = -1;
}