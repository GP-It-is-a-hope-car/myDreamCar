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
	//���� ������ ������ �𸣴� �ϴ� ���� �ۺ�
	Player();
	~Player();

	//�¿� �̵� - Ű �̺�Ʈ�� �Ű������� �޾ƿ��� �ϳ��� ��ġ�� ���� �����Ƽ� ������
	virtual void move_left(double timestep_s);
	virtual void move_right(double timestep_s);
	virtual void stop();

	//���� - �÷��̾� �̵����� �ٽ�
	virtual void jump(double timestep_s);

	//������ ȹ��� ��ȯ
	virtual void getItem();
	virtual void giveItem(); // �ִ� �Ŷ� ��ȯ���� �־�� �� �� ������ ������Ÿ���� ���� �ϴ� ����

	//�÷��̾��� �浹�� �׽�Ʈ�ϱ� ���� �Լ�
	//����, ������, Ʈ���� ����Ƽ���̼� ��Ʈ�� �޾ƿ;� �ϸ� �Ű����� �߰�
	virtual bool testOnPlatform(double pf_posX, double pf_posY, double pf_width); //���� ���� �ִ°��� ���� �˻� ������ �߷��� ���� X
	virtual bool testOnItem(double it_posX, double it_posY, double it_width, double it_height); //�����۰� ��ġ�°��� ���� �˻� ��ġ�� getItem() ����
	virtual bool testOnTruck(double tr_posX, double tr_posY, double tr_width, double tr_height);

private:
	//�÷��̾� �׸��⿡ �ʿ��� �⺻ ���
	SDL_Texture* texture_player_;
	SDL_Rect source_rect_player_;
	SDL_Rect dest_rect_player_;

	//�ʿ������� ���� �ִ� �Ӹ� ���� ������ ǥ��
	SDL_Texture* texture_myItem_;
	SDL_Rect source_rects_myItem_; // �������ڵ忡 ���� �迭�� ��ҿ� ���� �ٸ��� �׸���?
	SDL_Rect dest_rect_myItem_;
	
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
	std::string ownItem_; // ������ �ִ� ������ ����, �ӽ÷� ��Ʈ������ ���� �ٸ� �ɷ� �ٲ� ����
};