#include "Fuel.h"

Fuel::~Fuel()
{
	//����� �ڿ� ��ȯ
	SDL_DestroyTexture(fuel_sheet_texture_);
}

