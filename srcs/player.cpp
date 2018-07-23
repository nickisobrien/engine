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
	// if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
	// {
	// 	this->getChunk()->update();
	// }

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
		Block *b = c->getBlock(x,y,z);
		if (b != NULL && b->isActive())
		{
			b = c->getBlock(x, y+1, z);
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

bool Player::isGrounded()
{
	glm::vec3 current = this->getPosition();
	int x = (int)floor(current.x) % CHUNK_X;
	int y = floor(current.y);
	int z = (int)floor(current.z) % CHUNK_Z;
	if (x < 0)
		x = CHUNK_X + x;
	if (z < 0)
		z = CHUNK_Z + z;

	// cout << "Block:  (" << x << ", " << y << ", " << z << ")" << endl;
	// cout << "Player: (" << current.x << ", " << current.y << ", " << current.z << ")" << endl;
	Block *b = getChunk()->getBlock(x,y-1,z);
	if (b != NULL && (b->isActive()))
		return (true);
	return (false);
}

void Player::jump()
{
	glm::vec3 cur = this->getPosition();
	if (this->isGrounded())
		this->setPosition(glm::vec3(cur.x, cur.y+5.0f, cur.z));
}

void Player::applyGravity(float time)
{
	glm::vec3 current = this->getPosition();
	if (!this->isGrounded())
		current.y -= this->gravity * time;
	this->setPosition(current);
}

void Player::mouseClickEvent()
{
	glm::vec3 current = this->getPosition();
	int x = (int)floor(current.x) % CHUNK_X;
	int y = floor(current.y);
	int z = (int)floor(current.z) % CHUNK_Z;
	if (x < 0)
		x = CHUNK_X + x;
	if (z < 0)
		z = CHUNK_Z + z;
	cout << "Block:  (" << x << ", " << y << ", " << z << ")" << endl;
	cout << "View vector: "<< this->camera.GetViewVector().x << " " <<
	this->camera.GetViewVector().y << " " << this->camera.GetViewVector().z << endl;

	glm::vec3 currentCheck = this->getPosition();
	glm::vec3 currentView = this->camera.GetViewVector();
	int ctr = 0;

	while (ctr < 20)
	{
		cout << "Current check(s): "<< (int)currentCheck.x << " " <<
		(int)currentCheck.y << " " << (int)currentCheck.z << " floats: " << currentCheck.x << " "
		<< currentCheck.y << " " << currentCheck.z << endl;
		
		// need to implement a way to do checks where i only subtract certain axis's line bresenham
		if (this->getChunk()->getBlock(round(currentCheck.x),round(currentCheck.y),round(currentCheck.z)) &&
			this->getChunk()->getBlock(round(currentCheck.x),round(currentCheck.y),round(currentCheck.z))->getType()!=0)
		{
			this->getChunk()->getBlock(round(currentCheck.x),round(currentCheck.y),round(currentCheck.z))->setType(0);
			this->getChunk()->update();
			break;
		}
		currentCheck.x -= currentView.x/10.0f;
		currentCheck.y -= currentView.y/10.0f;
		currentCheck.z -= currentView.z/10.0f;
		ctr++;
	}



	
}
