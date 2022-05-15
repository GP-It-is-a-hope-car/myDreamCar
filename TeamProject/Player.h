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
	virtual void move_left();
	virtual void move_right();

	//���� - �÷��̾� �̵����� �ٽ�
	virtual void jump();

	//������ ȹ��� ��ȯ
	virtual void getItem();
	virtual void giveItem(); // �ִ� �Ŷ� ��ȯ���� �־�� �� �� ������ ������Ÿ���� ���� �ϴ� ����

	//�÷��̾��� �浹�� �׽�Ʈ�ϱ� ���� �Լ�
	//����, ������, Ʈ���� ����Ƽ���̼� ��Ʈ�� �޾ƿ;� �ϸ� �Ű����� �߰�
	virtual void testOnPlatform(); //���� ���� �ִ°��� ���� �˻� ������ �߷��� ���� X
	virtual void testOnItem(); //�����۰� ��ġ�°��� ���� �˻� ��ġ�� getItem() ����
	virtual void testOnTruck(); //Ʈ���� ��ġ�°��� ���� �˻� ��ġ�� giveItem() ����

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
	float verticalSpeed; //���� - ������ ����
	float horizontalSpeed; //���� - �¿� �̵��� ����
	float gravityAcc; // �߷°��ӵ�
	bool isHoldItem; // �������� ��� �ִ°� ��� �ִٸ� �Ӹ� ���� ������ ǥ��
	bool isJump; // ���߿� �ִ°�
	std::string ownItem; // ������ �ִ� ������ ����, �ӽ÷� ��Ʈ������ ���� �ٸ� �ɷ� �ٲ� ����
};