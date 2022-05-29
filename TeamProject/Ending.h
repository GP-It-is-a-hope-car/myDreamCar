#pragma once

#include "GameFunc.h"

class Ending : public PhaseInterface
{
public:
	Ending();
	~Ending();
	virtual void HandleEvents() override;
	virtual void Update() override;
	virtual void Render() override;

private:
	Mix_Music* g_ending_mus; // ���� ������� ���� ����

	//���
	SDL_Texture* texture_; 
	SDL_Rect source_rectangle_;
	SDL_Rect destination_rectangle_;
	
};



