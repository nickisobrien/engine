#include "engine.h"
#include "block.h"

bool Block::isActive(void)
{
	return (active);
}

void Block::setActive(bool act)
{
	active = act;
}

void Block::setType(int t)
{
	this->type = t;
}

Block::Block(void)
{
	active = true;
}

Block::~Block(void)
{
}

int Block::getType(void)
{
	return type;
}