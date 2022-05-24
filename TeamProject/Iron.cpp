#include "Iron.h"


Iron::~Iron()
{
	//사용한 자원 반환
	SDL_DestroyTexture(iron_sheet_texture);
}