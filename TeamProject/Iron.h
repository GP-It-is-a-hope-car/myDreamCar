#pragma once

#include "GameFunc.h"

class Iron : public ItemInterface
{
private:
	int weight;
public:
	Iron();
	virtual ~Iron();

	virtual int getWeight() { return weight; }
};