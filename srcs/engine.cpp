#include "engine.h"
#include "shader.h"
#include "camera.h"
#include "mesh.h"
#include "model.h"
#include "terrain.h"
#include "chunk.h"
#include "player.h"
#include "textureEngine.h"

#define RENDER_RADIUS 18

float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame
float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;
bool firstMouse = true;
TextureEngine textureEngine;
Terrain terr;
Player player(glm::vec3(CHUNK_X/2.0f, (float)CHUNK_Y-30.0f, CHUNK_Z/2.0f), &terr);

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
	// GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Engine", glfwGetPrimaryMonitor(), NULL);
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Engine", NULL, NULL);
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
	unsigned int atlas = textureEngine.loadTexture("../resources/textures/myatlas.jpg");

	cubeShader.use();
	cubeShader.setInt("atlas", 0);

	// render loop
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		player.processInput(window, deltaTime);

		// render
		glClearColor(0.5f, 0.7f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// texture atlas binding
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, atlas);

		// for physics
		player.update(deltaTime);
		// setup renderer
		cubeShader.use();
		glm::mat4 projection = glm::perspective(glm::radians(player.camera.Zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 1000.0f);
		glm::mat4 view = player.camera.GetViewMatrix();
		cubeShader.setMat4("projection", projection);
		cubeShader.setMat4("view", view);

		Chunk *c = player.getChunk();
		terr.renderChunk(glm::ivec2(c->getXOff(), c->getZOff()), cubeShader);
		// need to make sure to only render each chunk once per frame
		for (int i = 0; i < RENDER_RADIUS; i++)
		{
			for (int j = 0; j < RENDER_RADIUS; j++)
			{
				if (!i && !j)
					continue;
				if (!terr.renderChunk(glm::ivec2(c->getXOff() + i, c->getZOff() + j), cubeShader))
					break ;
				if (!terr.renderChunk(glm::ivec2(c->getXOff() - i, c->getZOff() - j), cubeShader))
					break ;
				if (!terr.renderChunk(glm::ivec2(c->getXOff() - i, c->getZOff() + j), cubeShader))
					break ;
				if (!terr.renderChunk(glm::ivec2(c->getXOff() + i, c->getZOff() - j), cubeShader))
					break ;
			}
		}
		terr.renderWaterChunk(glm::ivec2(c->getXOff(), c->getZOff()), cubeShader);
		for (int i = 0; i < RENDER_RADIUS; i++)
		{
			for (int j = 0; j < RENDER_RADIUS; j++)
			{
				if (!i && !j)
					continue;
				if (!terr.renderWaterChunk(glm::ivec2(c->getXOff() + i, c->getZOff() + j), cubeShader))
					break ;
				if (!terr.renderWaterChunk(glm::ivec2(c->getXOff() - i, c->getZOff() - j), cubeShader))
					break ;
				if (!terr.renderWaterChunk(glm::ivec2(c->getXOff() - i, c->getZOff() + j), cubeShader))
					break ;
				if (!terr.renderWaterChunk(glm::ivec2(c->getXOff() + i, c->getZOff() - j), cubeShader))
					break ;
			}
		}
		if (terr.updateList != glm::ivec2(-100000,-100000)) // could switch to running this as a while loop on a list on a seperate thread
		{
			terr.updateChunk(terr.updateList);
			terr.updateList = glm::ivec2(-100000,-100000);
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
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		player.leftMouseClickEvent();

	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
		player.rightMouseClickEvent();

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