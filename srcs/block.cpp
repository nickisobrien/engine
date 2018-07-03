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

void Block::render(void)
{
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

int Block::getType(void)
{
	return type;
}