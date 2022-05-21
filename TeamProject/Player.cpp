#include "Player.h"

Player::Player()
{
	//플레이어 텍스처, 렉트, SFX등 초기화
	//머리 위에 띄울 아이템?
	//각종 변수들 초기화
}

Player::~Player()
{
	//사용한 자원들 반환
}

bool Player::testOnPlatform(double pf_posX, double pf_posY, double pf_width)
{
	//플레이어의 좌표와 발판의 좌표 사이에서 검사
	//발판 위에서의 충돌이 확인되면 중력이 적용 X
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
	//동일한 방식으로 검사
	//확인되면 getItem()실행 -> isHoldItem = true -> 이 상태에선 충돌해도 의미X
	//아래쪽에서 먹는 동작은 허용하지 않을 것이므로, 하단에 대한 검사는 고려X
	//좌측 + 상단, 우측 + 상단을 기준으로 검사
	//좌측상단
	if (dest_rect_player_.x + dest_rect_player_.w > it_posX &&
		it_posX + it_width > dest_rect_player_.x + dest_rect_player_.w &&
		dest_rect_player_.y + dest_rect_player_.h > it_posY &&
		it_posY + it_height > dest_rect_player_.y + dest_rect_player_.h)
	{
		return true;
	}
	//우측상단
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
	//아이템과 동일한 방식
	//isHoldItem = true일 때 트럭에 전달 -> ownItem에 따라서 다른 결과 -> isHoldItem == false 의미 X
	if (dest_rect_player_.x + dest_rect_player_.w > tr_posX &&
		tr_posX + tr_width > dest_rect_player_.x + dest_rect_player_.w &&
		dest_rect_player_.y + dest_rect_player_.h > tr_posY &&
		tr_posY + tr_height > dest_rect_player_.y + dest_rect_player_.h)
	{
		return true;
	}
	//우측상단
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
	//검사결과 겹치면 아이템을 획득
	//isHoldItem = true
	//ownItem = 들고 있는 아이템 종류
	//이걸 위해서 아이템 클래스를 받아오거나 해야할 것 같음
}

void Player::giveItem()
{
	//isHoldItem == true
	//검사결과 겹치면 트럭에 아이템에 전달 -> 반환값이 있다면 데이터타입은?
	//ownItem 종류에 따라 다른 역할
	//isHoldItem = false
	//ownItem = null
	//아이템 클래스를 반환하거나 뭔가 동작이 필요
}

void Player::move_left(double timestep_s)
{
	//간단함 왼쪽방향키 누르면 수평속도만큼 이동
	//점프랑은 다르게 따로 가속도 없이 등속으로 이동
	//프레임 단위로 시간 받아서 초당 움직이는게 더 적당?
	//점프 중에는 가속도를 기준으로 계산한 속도로 이동하게
	//이동속도는 적당한 것 찾을 예정
	//왼쪽 바라보는 스프라이트
	double dt = timestep_s;

	if (isJump_)
	{
		//ball launch의 내용 중 x축에 대한 내용만을 이곳에 구현
	}
	else
	{
		dest_rect_player_.x -= horizontalSpeed_;
	}

	
}

void Player::move_right(double timestep_s)
{
	//마찬가지
	//매개변수로 키이벤트를 받아 좌우를 하나로 묶을 수 있을 것으로 보임
	//오른쪽 바라보는 스프라이트
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
		//ball launch의 내용 중 y축에 대한 내용만을 이곳에 구현
	}
}