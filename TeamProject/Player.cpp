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

void Player::testOnPlatform()
{
	//플레이어의 좌표와 발판의 좌표 사이에서 검사
	//발판 위에서의 충돌이 확인되면 중력이 적용 X
}

void Player::testOnItem()
{
	//동일한 방식으로 검사
	//확인되면 getItem()실행 -> isHoldItem = true -> 이 상태에선 충돌해도 의미X
}

void Player::testOnTruck()
{
	//동일한 방식으로 검사
	//isHoldItem = true일 때 트럭에 전달 -> ownItem에 따라서 다른 결과 -> isHoldItem == false 의미 X
}

void Player::getItem()
{
	//검사결과 겹치면 아이템을 획득
	//isHoldItem = true
	//ownItem = 들고 있는 아이템 종류
}

void Player::giveItem()
{
	//isHoldItem == true
	//검사결과 겹치면 트럭에 아이템에 전달 -> 반환값이 있다면 데이터타입은?
	//ownItem 종류에 따라 다른 역할
	//isHoldItem = false
	//ownItem = null
}

void Player::move_left()
{
	//간단함 왼쪽방향키 누르면 수평속도만큼 이동
}

void Player::move_right()
{
	//마찬가지
	//매개변수로 키이벤트를 받아 좌우를 하나로 묶을 수 있을 것으로 보임
}

void Player::jump()
{
	//생각해보니까 키이벤트로 받으면 이동 자체를 하나로 묶을 수 있을 것 같다.
	//조건이 좀 복잡하다
	//isJump == false
	//수직속도를 올려주고 반복문 혹은 시간을 이용해 중력가속도를 계속해서 빼줌
	//그러면 천천히 느려지다가 속도가 음수가 됨
	//바닥으로 하강할 것이고 이 때 계속해서 검사를 하고 바닥과 충돌을 하게 되면
	//isJump = false를 주고 수직속도 = 0으로 초기화
	//이 때 아래에서 뚫고 올라가는 것은 가능
	//속도가 음수일 때 검사하는 것으로 간단하게 해결 가능할 것으로 보임
}