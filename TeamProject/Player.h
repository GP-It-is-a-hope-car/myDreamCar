#pragma once

#include <iostream>
#include <string>
#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include "SDL_image.h"

class Player
{
public:
	//무슨 문제가 있을지 모르니 일단 전부 퍼블릭
	Player();
	~Player();

	//좌우 이동 - 키 이벤트를 매개변수로 받아오면 하나로 합치기 가능 귀찮아서 나눠둠
	virtual void move_left(double timestep_s);
	virtual void move_right(double timestep_s);
	virtual void stop();

	//점프 - 플레이어 이동에서 핵심
	virtual void jump(double timestep_s);

	//아이템 획득과 반환
	virtual void getItem();
	virtual void giveItem(); // 주는 거라 반환값이 있어야 할 것 같은데 데이터타입을 몰라서 일단 보류

	//플레이어의 충돌을 테스트하기 위한 함수
	//발판, 아이템, 트럭의 데스티네이션 렉트를 받아와야 하면 매개변수 추가
	virtual bool testOnPlatform(double pf_posX, double pf_posY, double pf_width); //발판 위에 있는가에 대한 검사 있으면 중력의 영향 X
	virtual bool testOnItem(double it_posX, double it_posY, double it_width, double it_height); //아이템과 겹치는가에 대한 검사 겹치면 getItem() 실행
	virtual bool testOnTruck(double tr_posX, double tr_posY, double tr_width, double tr_height);

private:
	//플레이어 그리기에 필요한 기본 요소
	SDL_Texture* texture_player_;
	SDL_Rect source_rect_player_;
	SDL_Rect dest_rect_player_;

	//필요할지도 몰라서 넣는 머리 위의 아이템 표시
	SDL_Texture* texture_myItem_;
	SDL_Rect source_rects_myItem_; // 아이템코드에 따라 배열의 요소에 접근 다르게 그리기?
	SDL_Rect dest_rect_myItem_;
	
	// SFX 예상 되는 거 작성한 것
	Mix_Chunk* jump_sound_;
	Mix_Chunk* move_sound_;
	Mix_Chunk* get_item_sound_;
	Mix_Chunk* give_item_sound_;

	//필요할 것 같은 변수
	double verticalSpeed_; //수직 - 점프랑 관련
	double horizontalSpeed_; //수평 - 좌우 이동과 관련
	double mass_; // 무게
	double gravityAcc_; // 중력가속도
	bool isHoldItem_; // 아이템을 들고 있는가 들고 있다면 머리 위에 아이템 표시
	bool isJump_; // 공중에 있는가
	std::string ownItem_; // 가지고 있는 아이템 종류, 임시로 스트링으로 선언 다른 걸로 바꿀 것임
};