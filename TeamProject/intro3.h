#pragma once

#include "GameFunc.h"
class Intro3 : public PhaseInterface
{
public:
	Intro3();
	~Intro3();
	virtual void HandleEvents() override;
	virtual void Update() override;
	virtual void Render() override;

private:
	
	SDL_Texture* texture_; // the SDL_Texture 
	SDL_Rect source_rectangle_; // the rectangle for source image
	SDL_Rect destination_rectangle_; // for destination


};


