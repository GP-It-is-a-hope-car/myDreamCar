#pragma once

#include "GameFunc.h"

class Player
{
private:
	//�÷��̾� �׸��⿡ �ʿ��� �⺻ ���
	SDL_Texture* texture_player_;
	SDL_Rect source_rect_player_;
	SDL_Rect dest_rect_player_;

	//�ʿ������� ���� �ִ� �Ӹ� ���� ������ ǥ��
	SDL_Texture* texture_fuel_;
	SDL_Rect source_rect_fuel_;
	SDL_Rect dest_rect_fuel_;

	SDL_Texture* texture_iron_;
	SDL_Rect source_rect_iron_;
	SDL_Rect dest_rect_iron_;

	// SFX ���� �Ǵ� �� �ۼ��� ��
	Mix_Chunk* jump_sound_;
	Mix_Chunk* move_sound_;
	Mix_Chunk* get_item_sound_;
	Mix_Chunk* give_item_sound_;

	//�ʿ��� �� ���� ����
	double verticalSpeed_; //���� - ������ ����
	double horizontalSpeed_; //���� - �¿� �̵��� ����
	double mass_; // ����
	double gravityAcc_; // �߷°��ӵ�
	bool isHoldItem_; // �������� ��� �ִ°� ��� �ִٸ� �Ӹ� ���� ������ ǥ��
	bool isJump_; // ���߿� �ִ°�
	int ownItem_;

public:
	Player();
	~Player();

	//�¿� �̵�, ���� �� ��ġ ���
	virtual void move_left(double timestep_s);
	virtual void move_right(double timestep_s);
	virtual void move_jump(double timestep_s);

	int posX() { return dest_rect_player_.x; }
	int posY() { return dest_rect_player_.y; }
	int width() { return dest_rect_player_.w; }
	int height() { return dest_rect_player_.h; }
	
	//���� - �÷��̾� �̵����� �ٽ�
	virtual void jump();

	//������ ȹ��� ��ȯ
	virtual void showItem();
	virtual bool getItem(int in);
	int giveItem(); // �ִ� �Ŷ� ��ȯ���� �־�� �� �� ������ ������Ÿ���� ���� �ϴ� ����

	//�÷��̾��� �浹�� �׽�Ʈ�ϱ� ���� �Լ�
	//����, ������, Ʈ���� ����Ƽ���̼� ��Ʈ�� �޾ƿ;� �ϸ� �Ű����� �߰�
	virtual void testOnPlatform(double pf_posX, double pf_posY, double pf_width, double pf_height); //���� ���� �ִ°��� ���� �˻� ������ �߷��� ���� X
	virtual void draw_player();


};