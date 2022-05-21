#include "Player.h"

Player::Player()
{
	//�÷��̾� �ؽ�ó, ��Ʈ, SFX�� �ʱ�ȭ
	//�Ӹ� ���� ��� ������?
	//���� ������ �ʱ�ȭ
}

Player::~Player()
{
	//����� �ڿ��� ��ȯ
}

bool Player::testOnPlatform(double pf_posX, double pf_posY, double pf_width)
{
	//�÷��̾��� ��ǥ�� ������ ��ǥ ���̿��� �˻�
	//���� �������� �浹�� Ȯ�εǸ� �߷��� ���� X
	if (dest_rect_player_.x + dest_rect_player_.w * 0.5 > pf_posX &&
		dest_rect_player_.x + dest_rect_player_.w * 0.5 < pf_posX + pf_width &&
		dest_rect_player_.y + dest_rect_player_.h == pf_posY)
	{
		if (horizontalSpeed_ <= 0)
		{
			isJump_ = false;
			return true;
		}
		
	}
	return false;

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

void Player::getItem()
{
	//�˻��� ��ġ�� �������� ȹ��
	//isHoldItem = true
	//ownItem = ��� �ִ� ������ ����
	//�̰� ���ؼ� ������ Ŭ������ �޾ƿ��ų� �ؾ��� �� ����
}

void Player::giveItem()
{
	//isHoldItem == true
	//�˻��� ��ġ�� Ʈ���� �����ۿ� ���� -> ��ȯ���� �ִٸ� ������Ÿ����?
	//ownItem ������ ���� �ٸ� ����
	//isHoldItem = false
	//ownItem = null
	//������ Ŭ������ ��ȯ�ϰų� ���� ������ �ʿ�
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

	if (isJump_)
	{
		//ball launch�� ���� �� x�࿡ ���� ���븸�� �̰��� ����
	}
	else
	{
		dest_rect_player_.x -= horizontalSpeed_;
	}

	
}

void Player::move_right(double timestep_s)
{
	//��������
	//�Ű������� Ű�̺�Ʈ�� �޾� �¿츦 �ϳ��� ���� �� ���� ������ ����
	//������ �ٶ󺸴� ��������Ʈ
	if (isJump_)
	{

	}
	else
	{
		dest_rect_player_.x += horizontalSpeed_;
	}
}

void Player::stop()
{
	horizontalSpeed_ = 0;
}

void Player::jump(double timestep_s)
{
	if (isJump_ == false)
	{
		isJump_ = true;
		//ball launch�� ���� �� y�࿡ ���� ���븸�� �̰��� ����
	}
}