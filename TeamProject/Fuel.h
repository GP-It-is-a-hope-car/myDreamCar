#pragma once

#include "GameFunc.h"

class Fuel : public ItemInterface
{
private:
	int amount;
	// 아이템 종류	
public:
	Fuel(SDL_Rect dst_fuel) : amount(0), ItemInterface(dst_fuel, FUEL) {

	};
	~Fuel() {};

	virtual int getAmount() { return amount; }
};