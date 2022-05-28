#pragma once

#include "GameFunc.h"

class Player
{
private:
	//플레이어 그리기에 필요한 기본 요소
	SDL_Texture* texture_player_;
	SDL_Rect source_rect_player_;
	SDL_Rect dest_rect_player_;

	//필요할지도 몰라서 넣는 머리 위의 아이템 표시
	SDL_Texture* texture_fuel_;
	SDL_Rect source_rect_fuel_;
	SDL_Rect dest_rect_fuel_;

	SDL_Texture* texture_iron_;
	SDL_Rect source_rect_iron_;
	SDL_Rect dest_rect_iron_;

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
	int ownItem_;

public:
	Player();
	~Player();

	//좌우 이동, 점프 중 위치 계산
	virtual void move_left(double timestep_s);
	virtual void move_right(double timestep_s);
	virtual void move_jump(double timestep_s);

	int posX() { return dest_rect_player_.x; }
	int posY() { return dest_rect_player_.y; }
	int width() { return dest_rect_player_.w; }
	int height() { return dest_rect_player_.h; }
	
	//점프 - 플레이어 이동에서 핵심
	virtual void jump();

	//아이템 획득과 반환
	virtual void showItem();
	virtual bool getItem(int in);
	int giveItem(); // 주는 거라 반환값이 있어야 할 것 같은데 데이터타입을 몰라서 일단 보류

	//플레이어의 충돌을 테스트하기 위한 함수
	//발판, 아이템, 트럭의 데스티네이션 렉트를 받아와야 하면 매개변수 추가
	virtual void testOnPlatform(double pf_posX, double pf_posY, double pf_width, double pf_height); //발판 위에 있는가에 대한 검사 있으면 중력의 영향 X
	virtual void draw_player();


};