#pragma once

#include "GameFunc.h"

class Happy_ending : public PhaseInterface
{
public:
	Happy_ending();
	~Happy_ending();
	virtual void HandleEvents() override;
	virtual void Update() override;
	virtual void Render() override;

private:
	//���
	SDL_Texture* texture_;
	SDL_Rect source_rectangle_;
	SDL_Rect destination_rectangle_;
	//��ö �� ǥ��
	SDL_Texture* metal_texture;
	SDL_Rect metal_texture_rect;
	//Yes
	SDL_Texture* re_texture;
	SDL_Rect re_source_rect;
	SDL_Rect re_dest_rect;
	//No
	SDL_Texture* re1_texture;
	SDL_Rect re1_source_rect;
	SDL_Rect re1_dest_rect;
	//Play Again?
	SDL_Texture* re2_texture;
	SDL_Rect re2_source_rect;
	SDL_Rect re2_dest_rect;
	//ȭ��ǥ
	SDL_Texture* start_texture;
	SDL_Rect start_source;
	SDL_Rect start_dest;
	int re;
};



