//https://pastebin.com/DXKEmvap

static glm::vec3 VERTICES[8] =
{
	glm::vec3(-1, -1, -1),
	glm::vec3(1, -1, -1),
	glm::vec3(1, 1, -1),
	glm::vec3(-1, 1, -1),
	glm::vec3(-1, -1, 1),
	glm::vec3(1, -1, 1),
	glm::vec3(1, 1, 1),
	glm::vec3(-1, 1, 1)
};

static glm::vec2 TEXCOORDS[6] =
{
	glm::vec2(1, 1),
	glm::vec2(0, 1),
	glm::vec2(1, 0),
	glm::vec2(1, 0),
	glm::vec2(0, 1),
	glm::vec2(0, 0)
};
 
static glm::vec3 NORMALS[6] =
{
	glm::vec3(0, 0, 1),
	glm::vec3(1, 0, 0),
	glm::vec3(0, 0, -1),
	glm::vec3(-1, 0, 0),
	glm::vec3(0, 1, 0),
	glm::vec3(0, -1, 0)
};

static int INDICES[36] =
{
	4, 5, 0, 0, 5, 1, // yneg(down)
	3, 2, 7, 7, 2, 6, // ypos(up)
	1, 5, 2, 2, 5, 6, // xpos
	5, 4, 6, 6, 4, 7, // zpos
	4, 0, 7, 7, 0, 3, // xneg
	0, 1, 3, 3, 1, 2  // zneg
};
 