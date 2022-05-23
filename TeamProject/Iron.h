#pragma once

#include "GameFunc.h"

class Iron : public ItemInterface
{
private:
	int weight;
public:
	Iron(SDL_Rect dst_iron) : weight(0), ItemInterface(dst_iron, IRON) {
	};
	virtual ~Iron();

	virtual int getWeight() { return weight; }
};