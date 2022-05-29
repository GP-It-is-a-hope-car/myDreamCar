#include "Fuel.h"

Fuel::~Fuel()
{
	//사용한 자원 반환
	SDL_DestroyTexture(fuel_sheet_texture_);
}

