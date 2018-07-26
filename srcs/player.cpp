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

	// DEBUGGERS
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
		this->getChunk()->update();
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
		cout << "Player position: (" << this->getPosition().x << ", " << this->getPosition().y << ", " << this->getPosition().z << ") " << endl;
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
	{
		glm::vec3 current = this->getPosition();
		int x = (int)floor(current.x) % CHUNK_X;
		int y = floor(current.y);
		int z = (int)floor(current.z) % CHUNK_Z;
		if (x < 0)
			x = CHUNK_X + x;
		if (z < 0)
			z = CHUNK_Z + z;

		cout << "Block:	(" << x << ", " << y << ", " << z << ")" << endl;
	}



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
		Block *b = c->getBlock(x,y-2,z);
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

	Block *b = getChunk()->getBlock(x,y-3,z);
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

int signum(int x)
{
		return x == 0 ? 0 : x < 0 ? -1 : 1;
}

float mod(float value, float modulus)
{
	return fmod(fmod(value, modulus) + modulus,	modulus);
}

float intbound(float s, float ds)
{
	// Find the smallest positive t such that s+t*ds is an integer.
	if (ds < 0)
	{
		return intbound(-s, -ds);
	}
	else
	{
		s = mod(s, 1);
		// problem is now s+t*ds = 1
		return (1-s)/ds;
	}
}

void Player::mouseClickEvent()
{
	glm::vec3 current = this->getPosition();
	int x = (int)floor(current.x) % CHUNK_X;
	int y = floor(current.y);
	int z = (int)floor(current.z) % CHUNK_Z;
	// if (x < 0)
	// 	x = CHUNK_X + x;
	// if (z < 0)
	// 	z = CHUNK_Z + z;
	// cout << "Block:	(" << x << ", " << y << ", " << z << ")" << endl;
	// cout << "Player: (" << current.x << ", " << current.y << ", " << current.z << ")" << endl;
	// cout << "View vector: "<< this->camera.GetViewVector().x << " " <<
	// this->camera.GetViewVector().y << " " << this->camera.GetViewVector().z << endl;
	glm::vec3 currentView = this->camera.GetViewVector();

	// This id of the first/current voxel hit by the ray.
	// Using floor (round down) is actually very important,
	// the implicit int-casting will round up for negative numbers.
	glm::ivec3 current_voxel(x, y, z);

	// Compute normalized ray direction.
	glm::vec3 ray = this->camera.GetViewVector();

	// In which direction the voxel ids are incremented.
	int stepX = ray.x > 0.0f ? 1 : ray.x < 0.0f ? -1 : 0;
	int stepY = ray.y > 0.0f ? 1 : ray.y < 0.0f ? -1 : 0;
	int stepZ = ray.z > 0.0f ? 1 : ray.z < 0.0f ? -1 : 0;

	// Distance along the ray to the next voxel border from the current position (tMaxX, tMaxY, tMaxZ).
	// float next_voxel_boundary_x = (current_voxel.x+stepX);
	// float next_voxel_boundary_y = (current_voxel.y+stepY);
	// float next_voxel_boundary_z = (current_voxel.z+stepZ);

	// tMaxX, tMaxY, tMaxZ -- distance until next intersection with voxel-border
	// the value of t at which the ray crosses the first vertical voxel boundary
	// float tMaxX = (next_voxel_boundary_x - current.x) / ray.x;
	// float tMaxY = (next_voxel_boundary_y - current.y) / ray.y;
	// float tMaxZ = (next_voxel_boundary_z - current.z) / ray.z;
	float tMaxX = intbound(current.x,ray.x);
	float tMaxY = intbound(current.y,ray.y);
	float tMaxZ = intbound(current.z,ray.z);

	// tDeltaX, tDeltaY, tDeltaZ --
	// how far along the ray we must move for the horizontal component to equal the width of a voxel
	// the direction in which we traverse the grid
	// can only be FLT_MAX if we never go in that direction
	float tDeltaX = ((float)stepX) / ray.x;
    float tDeltaY = ((float)stepY) / ray.y;
    float tDeltaZ = ((float)stepZ) / ray.z;

	// cout << "Current_voxel" << current_voxel.x << " " << current_voxel.y << " " << current_voxel.z << endl;
	// cout << "Steps:" << stepX <<" "<<stepY<<" "<<stepZ<<endl;
	// cout << "Next boundary:"<<next_voxel_boundary_x<<" "<<next_voxel_boundary_y<<" "<<next_voxel_boundary_z<<endl;
	// cout << "tMaxs:"<< tMaxX << " " << tMaxY << " " << tMaxZ << endl;
	// cout << "tDeltas:"<<tDeltaX<<" "<<tDeltaY<< " " << tDeltaZ << endl;

	Block *b = this->getChunk()->getBlock(current_voxel.x,current_voxel.y,current_voxel.z);
	if (b && b->isActive())
	{
		b->setType(0);
		this->getChunk()->update();
		return;
	}
	int ctr = 0;
	do
	{
		if (tMaxX < tMaxY)
		{
			if (tMaxX < tMaxZ)
			{
				current_voxel.x += stepX;
				tMaxX += tDeltaX;
			}
			else
			{
				current_voxel.z += stepZ;
				tMaxZ += tDeltaZ;
			}
		}
		else
		{
			if (tMaxY < tMaxZ)
			{
				current_voxel.y += stepY;
				tMaxY += tDeltaY;
			}
			else
			{
				current_voxel.z += stepZ;
				tMaxZ += tDeltaZ;
			}
		}
		b = this->getChunk()->getBlock(current_voxel.x,current_voxel.y,current_voxel.z);
		ctr++;
	} while ((!b || !b->isActive()) && ctr < 10);
	if (b && b->isActive())
	{
		// cout << "FOUND " << current_voxel.x << " " << current_voxel.y << " " << current_voxel.z << endl;
		b->setType(0);
		this->getChunk()->update();
	}
	
}
