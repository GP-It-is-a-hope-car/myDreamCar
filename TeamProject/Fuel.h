#pragma once

#include "GameFunc.h"

class Fuel : public ItemInterface
{
private:
	int amount;
public:
	Fuel();
	virtual ~Fuel();

	virtual int getAmount() { return amount; }
};