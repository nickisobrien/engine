#include "player.h"

void Player::processInput(GLFWwindow *window, float deltaTime)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	glm::vec3 savePos = this->getPosition();

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		this->camera.ProcessKeyboard(FORWARD, deltaTime, this->getChunk());
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		this->camera.ProcessKeyboard(BACKWARD, deltaTime, this->getChunk());
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		this->camera.ProcessKeyboard(LEFT, deltaTime, this->getChunk());
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		this->camera.ProcessKeyboard(RIGHT, deltaTime, this->getChunk());
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		this->jump();

	glm::vec3 newPos = this->getPosition();

	int x = (int)floor(newPos.x) % CHUNK_X;
	int y = floor(newPos.y);
	int z = (int)floor(newPos.z) % CHUNK_Z;
	if (x < 0)
		x = CHUNK_X + x;
	if (z < 0)
		z = CHUNK_Z + z;

	Chunk *c;
	if ((c = this->getChunk()) != NULL)
	{
		Block *b = c->get_block(x,y,z);
		if (b != NULL && b->isActive())
		{
			b = c->get_block(x, y+1, z);
			if (b != NULL && !b->isActive())
				this->setPosition(glm::vec3(newPos.x, newPos.y+1, newPos.z));
			else
				this->setPosition(savePos); // need to change to only reverting x/y/z, not necessarily all of them
		}
	}
}

Chunk *Player::getChunk()
{
	int cx = this->camera.Position.x >= 0.0f ? this->camera.Position.x / CHUNK_X : ceil(this->camera.Position.x) / CHUNK_X - 1.0f;
	int cz = this->camera.Position.z >= 0.0f ? this->camera.Position.z / CHUNK_Z : ceil(this->camera.Position.z) / CHUNK_X - 1.0f;
	glm::ivec2 pos(cx, cz);
	if (this->terr->world.find(pos) != this->terr->world.end())
		return (this->terr->world[pos]);
	return (NULL);
}