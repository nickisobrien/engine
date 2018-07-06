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