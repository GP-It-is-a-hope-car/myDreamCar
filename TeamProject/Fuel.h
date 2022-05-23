#pragma once

#include "GameFunc.h"

class Fuel : public ItemInterface
{
private:
	int amount;
	
public:
	Fuel(SDL_Rect dst_fuel) : amount(0), ItemInterface(dst_fuel) {

	};
	virtual ~Fuel();

	virtual int getAmount() { return amount; }
};