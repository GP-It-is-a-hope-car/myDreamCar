#include "Player.h"

Player::Player()
{
	//�÷��̾� �ؽ�ó, ��Ʈ, SFX�� �ʱ�ȭ
	SDL_Surface* player_surface = IMG_Load("../../Resources/dummy.png");
	texture_player_ = SDL_CreateTextureFromSurface(g_renderer, player_surface);
	SDL_FreeSurface(player_surface);

	SDL_QueryTexture(texture_player_, NULL, NULL, &source_rect_player_.w, &source_rect_player_.h);
	source_rect_player_.x = source_rect_player_.y = 0;
	dest_rect_player_.x = dest_rect_player_.y = 286;
	dest_rect_player_.w = source_rect_player_.w;
	dest_rect_player_.h = source_rect_player_.h;

	//�Ӹ� ���� ��� ������?
	
	//���� ������ �ʱ�ȭ
	//�߷� ���ӵ��� �츮 ���ӿ� �°� ������ �����ؾ���
	verticalSpeed_ = 0;
	horizontalSpeed_ = 160;
	mass_ = 2;
	gravityAcc_ = 980;
	isHoldItem_ = false;
	isJump_ = false;
}

Player::~Player()
{
	//����� �ڿ��� ��ȯ
	SDL_DestroyTexture(texture_player_);

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
	std::cout << dest_rect_player_.x << std::endl;
}

void Player::move_right(double timestep_s)
{
	//��������
	//�Ű������� Ű�̺�Ʈ�� �޾� �¿츦 �ϳ��� ���� �� ���� ������ ����
	//������ �ٶ󺸴� ��������Ʈ
	double dt = timestep_s;

	horizontalSpeed_ = 160;
	dest_rect_player_.x = dest_rect_player_.x + dt * horizontalSpeed_;
	std::cout << dest_rect_player_.x << std::endl;
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
		verticalSpeed_ = verticalSpeed_ - (12 * dest_rect_player_.h) / mass_;
	}
}

void Player::draw_player()
{
	SDL_RenderCopy(g_renderer, texture_player_, &source_rect_player_, &dest_rect_player_);
}