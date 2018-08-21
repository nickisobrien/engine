#include "engine.h"
#include "shader.h"
#include "camera.h"
#include "mesh.h"
#include "model.h"
#include "terrain.h"
#include "chunk.h"
#include "player.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h" // https://github.com/nothings/stb/blob/master/stb_image.h

#define RENDER_RADIUS 12
#define PRERENDER_RADIUS 2

float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame
float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;
bool firstMouse = true;
Terrain terr;
Player player(glm::vec3(CHUNK_X/2, (float)CHUNK_Y, CHUNK_Z/2), &terr);

static void testfunc(Shader cubeShader)
{
	if (!terr.updateList.empty())
	{
		glm::ivec2 pos = terr.updateList[terr.updateList.size()-1];
		terr.updateList.pop_back();
		terr.updateChunk(pos);
	}
}

int main(void)
{
	// glfw: initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Engine",  glfwGetPrimaryMonitor(), NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // turn on mouse capturing
	glfwSetCursorPosCallback(window, mouse_callback); // calls mouse_callback every time mouse moves
	glfwSetScrollCallback(window, scroll_callback); // calls scroll_callback every time scrolling happens
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glEnable(GL_DEPTH_TEST); // turn on z buffering
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	// glEnable(GL_CULL_FACE); // face culling only renders visible faces of closed shapes ie. cube (needs speed testing to determine if worth)
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	// build and compile our shader program
	Shader cubeShader("../resources/shaders/cube.vs", "../resources/shaders/cube.fs");
	unsigned int atlas = loadTexture("../resources/textures/atlas4.png");
	cubeShader.use();
	cubeShader.setInt("atlas", 0);	

	for (int i = -PRERENDER_RADIUS; i <= PRERENDER_RADIUS; i++)
		for (int j = -PRERENDER_RADIUS; j <= PRERENDER_RADIUS; j++)
			terr.renderChunk(glm::ivec2(player.getChunk()->getXOff()+i, player.getChunk()->getZOff()+j), cubeShader);

	for (glm::ivec2 i : terr.updateList)
		terr.updateChunk(i);

	// render loop
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		player.processInput(window, deltaTime); // could be source of SEGFAULT?

		// for physics
		// getting current chunk ensures current chunk is rendered
		Chunk *c = player.getChunk();
		player.update(deltaTime);

		// render
		glClearColor(0.5f, 0.8f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// texture atlas binding
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, atlas);

		// setup renderer
		cubeShader.use();
		glm::mat4 projection = glm::perspective(glm::radians(player.camera.Zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 1000.0f);
		glm::mat4 view = player.camera.GetViewMatrix();
		cubeShader.setMat4("projection", projection);
		cubeShader.setMat4("view", view);

		for (int i = RENDER_RADIUS; i >= 0; i--)
		{
			for (int j = RENDER_RADIUS; j >= 0; j--)
			{
				terr.renderChunk(glm::ivec2(c->getXOff() + i, c->getZOff() + j), cubeShader);
				terr.renderChunk(glm::ivec2(c->getXOff() - i, c->getZOff() - j), cubeShader);
				terr.renderChunk(glm::ivec2(c->getXOff() - i, c->getZOff() + j), cubeShader);
				terr.renderChunk(glm::ivec2(c->getXOff() + i, c->getZOff() - j), cubeShader);
			}
		}
		for (int i = RENDER_RADIUS; i >= 0; i--)
		{
			for (int j = RENDER_RADIUS; j >= 0; j--)
			{
				terr.renderWaterChunk(glm::ivec2(c->getXOff() + i, c->getZOff() + j), cubeShader);
				terr.renderWaterChunk(glm::ivec2(c->getXOff() - i, c->getZOff() - j), cubeShader);
				terr.renderWaterChunk(glm::ivec2(c->getXOff() - i, c->getZOff() + j), cubeShader);
				terr.renderWaterChunk(glm::ivec2(c->getXOff() + i, c->getZOff() - j), cubeShader);
			}
		}

		if (!terr.updateList.empty())
		{
			terr.updateChunk(terr.updateList[terr.updateList.size()-1]);
			terr.updateList.clear();
		}

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	// glfw: terminate, clearing all previously allocated GLFW resources
	glfwTerminate();
	return 0;
}

// calculate mouse movement
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	player.camera.ProcessMouseMovement(xoffset, yoffset);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	if (state == GLFW_PRESS)
		player.leftMouseClickEvent();
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	player.camera.ProcessMouseScroll(yoffset);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function execute
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

unsigned int loadTexture(char const *path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}
	return textureID;
}

// loads a cubemap texture from 6 individual texture faces
unsigned int loadCubemap(vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}

unsigned int TextureFromFile(const char *path, const string &directory)
{
	string filename = string(path);
	filename = directory + '/' + filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}
	return textureID;
}



