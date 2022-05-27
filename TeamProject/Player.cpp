#include "Player.h"

extern int range;
Player::Player()
{
	//�÷��̾� �ؽ�ó, ��Ʈ, SFX�� �ʱ�ȭ
	SDL_Surface* player_surface = IMG_Load("../../Resources/dummy.png");
	texture_player_ = SDL_CreateTextureFromSurface(g_renderer, player_surface);
	SDL_FreeSurface(player_surface);

	SDL_QueryTexture(texture_player_, NULL, NULL, &source_rect_player_.w, &source_rect_player_.h);
	source_rect_player_.x = source_rect_player_.y = 0;
	dest_rect_player_.x = dest_rect_player_.y = 288;
	dest_rect_player_.w = source_rect_player_.w;
	dest_rect_player_.h = source_rect_player_.h;

	//�Ӹ� ���� ��� ������?
	SDL_Surface* fuel_surface = IMG_Load("../../Resources/Fuel.png");
	SDL_SetColorKey(fuel_surface, SDL_TRUE, SDL_MapRGB(fuel_surface->format, 223, 113, 38));
	texture_fuel_ = SDL_CreateTextureFromSurface(g_renderer, fuel_surface);
	SDL_FreeSurface(fuel_surface);

	SDL_QueryTexture(texture_fuel_, NULL, NULL, &source_rect_fuel_.w, &source_rect_fuel_.h);
	source_rect_fuel_.x = source_rect_fuel_.y = 0;
	dest_rect_fuel_.x = dest_rect_player_.x;
	dest_rect_fuel_.y = dest_rect_player_.y;
	dest_rect_fuel_.w = source_rect_fuel_.w/2;
	dest_rect_fuel_.h = source_rect_fuel_.h/2;

	SDL_Surface* iron_surface = IMG_Load("../../Resources/Iron.png");
	SDL_SetColorKey(iron_surface, SDL_TRUE, SDL_MapRGB(iron_surface->format, 50, 60, 57));
	texture_iron_ = SDL_CreateTextureFromSurface(g_renderer, iron_surface);
	SDL_FreeSurface(iron_surface);

	SDL_QueryTexture(texture_iron_, NULL, NULL, &source_rect_iron_.w, &source_rect_iron_.h);
	source_rect_iron_.x = source_rect_iron_.y = 0;
	dest_rect_iron_.x = dest_rect_player_.x;
	dest_rect_iron_.y = dest_rect_player_.y;
	dest_rect_iron_.w = source_rect_iron_.w/4;
	dest_rect_iron_.h = source_rect_iron_.h/4;
	
	//���� ������ �ʱ�ȭ
	//�߷� ���ӵ��� �츮 ���ӿ� �°� ������ �����ؾ���
	verticalSpeed_ = 0;
	horizontalSpeed_ = 160;
	mass_ = 2;
	gravityAcc_ = 1960;
	isHoldItem_ = false;
	isJump_ = false;
}

Player::~Player()
{
	//����� �ڿ��� ��ȯ
	SDL_DestroyTexture(texture_player_);
	SDL_DestroyTexture(texture_fuel_);
	SDL_DestroyTexture(texture_iron_);
	delete ownItem_;

}

void Player::testOnPlatform(double pf_posX, double pf_posY, double pf_width, double pf_height)
{
	//�÷��̾��� ��ǥ�� ������ ��ǥ ���̿��� �˻�
	//���� �������� �浹�� Ȯ�εǸ� �߷��� ���� X
	if (dest_rect_player_.x + dest_rect_player_.w * 0.95 > pf_posX &&
		dest_rect_player_.x + dest_rect_player_.w * 0.05 < pf_posX + pf_width)
	{
		if (dest_rect_player_.y + dest_rect_player_.h >= pf_posY &&
			dest_rect_player_.y + dest_rect_player_.h < pf_posY + pf_height*0.5)
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

bool Player::testOnItem(double it_posX, double it_posY, double it_width, double it_height)
{
	//������ ������� �˻�
	//Ȯ�εǸ� getItem()���� -> isHoldItem = true -> �� ���¿��� �浹�ص� �ǹ�X
	//�Ʒ��ʿ��� �Դ� ������ ������� ���� ���̹Ƿ�, �ϴܿ� ���� �˻�� ���X
	//���� + ���, ���� + ����� �������� �˻�
	//�������
	if (dest_rect_player_.x + dest_rect_player_.w > it_posX &&
		it_posX + it_width > dest_rect_player_.x + dest_rect_player_.w &&
		dest_rect_player_.y + dest_rect_player_.h > it_posY &&
		it_posY + it_height > dest_rect_player_.y + dest_rect_player_.h)
	{
		return true;
	}
	//�������
	if (dest_rect_player_.x > it_posX &&
		it_posX + it_width > dest_rect_player_.x &&
		dest_rect_player_.y + dest_rect_player_.h > it_posY &&
		it_posY + it_height > dest_rect_player_.y + dest_rect_player_.h)
	{
		return true;
	}
	return false;
}

bool Player::testOnTruck(double tr_posX, double tr_posY, double tr_width, double tr_height)
{
	//�����۰� ������ ���
	//isHoldItem = true�� �� Ʈ���� ���� -> ownItem�� ���� �ٸ� ��� -> isHoldItem == false �ǹ� X
	if (dest_rect_player_.x + dest_rect_player_.w > tr_posX &&
		tr_posX + tr_width > dest_rect_player_.x + dest_rect_player_.w &&
		dest_rect_player_.y + dest_rect_player_.h > tr_posY &&
		tr_posY + tr_height > dest_rect_player_.y + dest_rect_player_.h)
	{
		return true;
	}
	//�������
	if (dest_rect_player_.x > tr_posX &&
		tr_posX + tr_width > dest_rect_player_.x &&
		dest_rect_player_.y + dest_rect_player_.h > tr_posY &&
		tr_posY + tr_height > dest_rect_player_.y + dest_rect_player_.h)
	{
		return true;
	}
	return false;
}

bool Player::getItem(ItemInterface* in)
{
	//�˻��� ��ġ�� �������� ȹ��
	//isHoldItem = true
	//ownItem = ��� �ִ� ������ ����
	//�̰� ���ؼ� ������ Ŭ������ �޾ƿ��ų� �ؾ��� �� ���� 
	if (!isHoldItem_ && ownItem_ == nullptr)
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
		if (ownItem_->getItemType() == FUEL)
		{
			dest_rect_fuel_.x = dest_rect_player_.x + 16;
			dest_rect_fuel_.y = dest_rect_player_.y - 48;
			SDL_RenderCopy(g_renderer, texture_fuel_, &source_rect_fuel_, &dest_rect_fuel_);
		}
		else if (ownItem_->getItemType() == IRON)
		{
			dest_rect_iron_.x = dest_rect_player_.x + 16;
			dest_rect_iron_.y = dest_rect_player_.y - 48;
			SDL_RenderCopy(g_renderer, texture_iron_, &source_rect_iron_, &dest_rect_iron_);
		}
	}
}

ItemInterface * Player::giveItem()
{
	//isHoldItem == true
	//�˻��� ��ġ�� Ʈ���� �����ۿ� ���� -> ��ȯ���� �ִٸ� ������Ÿ����?
	//ownItem ������ ���� �ٸ� ����
	//isHoldItem = false
	//ownItem = null
	//������ Ŭ������ ��ȯ�ϰų� ���� ������ �ʿ�
	if (isHoldItem_)
	{
		ItemInterface* tmp_own;
		tmp_own = ownItem_;
		ownItem_ = nullptr;
		isHoldItem_ = false;
		return tmp_own;
	}
	return nullptr;
}

void Player::move_left(double timestep_s)
{
	//������ ���ʹ���Ű ������ ����ӵ���ŭ �̵�
	//�������� �ٸ��� ���� ���ӵ� ���� ������� �̵�
	//������ ������ �ð� �޾Ƽ� �ʴ� �����̴°� �� ����?
	//���� �߿��� ���ӵ��� �������� ����� �ӵ��� �̵��ϰ�
	//�̵��ӵ��� ������ �� ã�� ����
	//���� �ٶ󺸴� ��������Ʈ
	double dt = timestep_s;
	horizontalSpeed_ = 160;
	dest_rect_player_.x = dest_rect_player_.x - dt * horizontalSpeed_;
	if (dest_rect_player_.x<=256&&range!=2) {
		dest_rect_player_.x = 256;
	}
	else if (dest_rect_player_.x <= 256 && range == 2) {
		dest_rect_player_.x = dest_rect_player_.x - dt * horizontalSpeed_;
	}
}

void Player::move_right(double timestep_s)
{
	//��������
	//�Ű������� Ű�̺�Ʈ�� �޾� �¿츦 �ϳ��� ���� �� ���� ������ ����
	//������ �ٶ󺸴� ��������Ʈ
	double dt = timestep_s;

	horizontalSpeed_ = 160;
	dest_rect_player_.x = dest_rect_player_.x + dt * horizontalSpeed_;
	if (range!=1&&dest_rect_player_.x >=288) {
		dest_rect_player_.x = 288;
	}
	else if (range == 1 && dest_rect_player_.x >=288) {
		dest_rect_player_.x = dest_rect_player_.x + dt * horizontalSpeed_;
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
		verticalSpeed_ = verticalSpeed_ - (20 * dest_rect_player_.h) / mass_;
	}
}

void Player::draw_player()
{
	SDL_RenderCopy(g_renderer, texture_player_, &source_rect_player_, &dest_rect_player_);
}