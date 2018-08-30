#include "engine.h"
#include "block.h"

bool Block::isActive(void)
{
	if (this->type == AIR_BLOCK || this->type == WATER_BLOCK)
		return false;
	return true;
}

void Block::setType(int t)
{
	this->type = t;
}

Block::~Block(void)
{
}

int Block::getType(void)
{
	return this->type;
}