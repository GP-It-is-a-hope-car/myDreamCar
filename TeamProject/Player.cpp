#include "Player.h"

extern int range;
Player::Player()
{
	//�÷��̾� �ؽ�ó, ��Ʈ, SFX�� �ʱ�ȭ
	int index = 0;
	SDL_Surface* player_surface = IMG_Load("../../Resources/Sprites.png");
	texture_player_ = SDL_CreateTextureFromSurface(g_renderer, player_surface);
	SDL_FreeSurface(player_surface);

	//�̿�
	source_rects_player_[0] = { 0,0,64,64 };
	source_rects_player_[1] = { 0,0,64,64 };
	source_rects_player_[2] = { 0,0,64,64 };
	source_rects_player_[3] = { 0,0,64,64 };
	source_rects_player_[4] = { 0,0,64,64 };
	source_rects_player_[5] = { 0,0,64,64 };
	source_rects_player_[6] = { 0,0,64,64 };
	source_rects_player_[7] = { 0,0,64,64 };
	source_rects_player_[8] = { 0,0,64,64 };
	source_rects_player_[9] = { 0,0,64,64 };

	dest_rect_player_.x = 0;
	dest_rect_player_.x = 0;
	dest_rect_player_.w = 64;
	dest_rect_player_.h = 64;
	

	//�Ӹ� ���� ��� ������?
	//����� ��ö�� ��������Ʈ�� �����ؾ���
	SDL_Surface* fuel_surface = IMG_Load("../../Resources/Fuel.png");
	SDL_SetColorKey(fuel_surface, SDL_TRUE, SDL_MapRGB(fuel_surface->format, 223, 113, 38));
	texture_fuel_ = SDL_CreateTextureFromSurface(g_renderer, fuel_surface);
	SDL_FreeSurface(fuel_surface);

	SDL_QueryTexture(texture_fuel_, NULL, NULL, &source_rect_fuel_.w, &source_rect_fuel_.h);
	source_rect_fuel_.x = source_rect_fuel_.y = 0;
	dest_rect_fuel_.x = 0;
	dest_rect_fuel_.y = 0;
	dest_rect_fuel_.w = source_rect_fuel_.w/2;
	dest_rect_fuel_.h = source_rect_fuel_.h/2;

	SDL_Surface* iron_surface = IMG_Load("../../Resources/Iron.png");
	SDL_SetColorKey(iron_surface, SDL_TRUE, SDL_MapRGB(iron_surface->format, 50, 60, 57));
	texture_iron_ = SDL_CreateTextureFromSurface(g_renderer, iron_surface);
	SDL_FreeSurface(iron_surface);

	SDL_QueryTexture(texture_iron_, NULL, NULL, &source_rect_iron_.w, &source_rect_iron_.h);
	source_rect_iron_.x = source_rect_iron_.y = 0;
	dest_rect_iron_.x = 0;
	dest_rect_iron_.y = 0;
	dest_rect_iron_.w = source_rect_iron_.w/2;
	dest_rect_iron_.h = source_rect_iron_.h/2;
	
	//���� ������ �ʱ�ȭ
	//�߷� ���ӵ��� �츮 ���ӿ� �°� ������ �����ؾ���
	verticalSpeed_ = 0;
	horizontalSpeed_ = 200;
	mass_ = 2;
	gravityAcc_ = 1960;
	isHoldItem_ = false;
	isJump_ = false;
	ownItem_ = -1;
}

Player::~Player()
{
	//����� �ڿ��� ��ȯ
	SDL_DestroyTexture(texture_player_);
	SDL_DestroyTexture(texture_fuel_);
	SDL_DestroyTexture(texture_iron_);
	//delete ownItem_;

}

void Player::testOnPlatform(double pf_posX, double pf_posY, double pf_width, double pf_height)
{
	//�÷��̾��� ��ǥ�� ������ ��ǥ ���̿��� �˻�
	//���� �������� �浹�� Ȯ�εǸ� �߷��� ���� X
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
	//�˻��� ��ġ�� �������� ȹ��
	//isHoldItem = true
	//ownItem = ��� �ִ� ������ ����
	//�̰� ���ؼ� ������ Ŭ������ �޾ƿ��ų� �ؾ��� �� ���� 
	if (!isHoldItem_ && ownItem_ == -1)
	{
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
			dest_rect_fuel_.x = dest_rect_player_.x + 16;
			dest_rect_fuel_.y = dest_rect_player_.y - 48;
			SDL_RenderCopy(g_renderer, texture_fuel_, &source_rect_fuel_, &dest_rect_fuel_);
		}
		else if (ownItem_ == IRON)
		{
			dest_rect_iron_.x = dest_rect_player_.x + 16;
			dest_rect_iron_.y = dest_rect_player_.y - 48;
			SDL_RenderCopy(g_renderer, texture_iron_, &source_rect_iron_, &dest_rect_iron_);
		}
	}
}

int Player::giveItem()
{
	//isHoldItem == true
	//�˻��� ��ġ�� Ʈ���� �����ۿ� ���� -> ��ȯ���� �ִٸ� ������Ÿ����?
	//ownItem ������ ���� �ٸ� ����
	//isHoldItem = false
	//ownItem = null
	//������ Ŭ������ ��ȯ�ϰų� ���� ������ �ʿ�
	if (isHoldItem_)
	{
		int tmp_own;
		tmp_own = ownItem_;
		ownItem_ = -1;
		isHoldItem_ = false;
		return tmp_own;
	}
	return -1; // �ƹ��͵� �ȵ�� �ִٸ� -1 ��ȯ
}

void Player::move_left(double timestep_s)
{
	//������ ���ʹ���Ű ������ ����ӵ���ŭ �̵�
	//�������� �ٸ��� ���� ���ӵ� ���� ������� �̵�
	//������ ������ �ð� �޾Ƽ� �ʴ� �����̴°� �� ����?
	//���� �߿��� ���ӵ��� �������� ����� �ӵ��� �̵��ϰ�
	//�̵��ӵ��� ������ �� ã�� ����
	//���� �ٶ󺸴� ��������Ʈ

	if (index < 3 || index == 5)
	{
		index = 4;
	}
	double dt = timestep_s;
	horizontalSpeed_ = 200;
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
	index--;

	if (isJump_)
	{
		index = 5;
	}
}

void Player::move_right(double timestep_s)
{
	//��������
	//�Ű������� Ű�̺�Ʈ�� �޾� �¿츦 �ϳ��� ���� �� ���� ������ ����
	//������ �ٶ󺸴� ��������Ʈ
	
	if (index > 1)
	{
		index = 0;
	}
	double dt = timestep_s;

	horizontalSpeed_ = 200;
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

	index++;

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
		//ball launch�� ���� �� y�࿡ ���� ���븸�� �̰��� ����
		verticalSpeed_ = verticalSpeed_ - (21 * dest_rect_player_.h) / mass_;
	}
}

void Player::draw_player()
{
	SDL_RenderCopy(g_renderer, texture_player_, &source_rects_player_[index], &dest_rect_player_);
}

//���ӿ��� �ÿ� ������Ʈ ���� ����
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