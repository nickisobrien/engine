#include "engine.h"
#include "block.h"

bool Block::isActive(void)
{
	if (this->type == Blocktype::AIR_BLOCK || this->type == Blocktype::WATER_BLOCK)
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