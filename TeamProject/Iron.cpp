#include "Iron.h"


Iron::~Iron()
{
	//����� �ڿ� ��ȯ
	SDL_DestroyTexture(iron_sheet_texture);
}