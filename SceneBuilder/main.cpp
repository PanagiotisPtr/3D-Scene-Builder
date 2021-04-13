#include <iostream>
#define GLEW_STATIC

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/string_cast.hpp"
#include <array>

const double mouseSensitivity = 0.005f;
const float moveSensitivity = 1.0f;
const float cameraSensitivity = 0.01f;
const float zoomSensitivity = 0.1f;

int windowWidth = 800;
int windowHeight = 600;

template<typename T> T clamp(T a, T b, T x) {
	if (x < a) return a;
	if (x > b) return b;
	return x;
}

glm::mat4 getMatrixf(int flag) {
	glm::mat4 out{};
	glGetFloatv(flag, glm::value_ptr(out));
	return out;
}

void drawPyramid(float xSize, float ySize, float zSize) {
	const float xs = xSize / 2;
	const float zs = zSize / 2;
	glBegin(GL_TRIANGLES);

	glVertex3f(-xs, 0, -zs);
	glVertex3f(-xs, 0, zs);
	glVertex3f(xs, 0, zs);

	glVertex3f(xs, 0, zs);
	glVertex3f(xs, 0, -zs);
	glVertex3f(-xs, 0, -zs);


	glVertex3f(-xs, 0, -zs);
	glVertex3f(xs, 0, -zs);
	glVertex3f(0, zSize, 0);

	glVertex3f(xs, 0, -zs);
	glVertex3f(xs, 0, zs);
	glVertex3f(0, zSize, 0);

	glVertex3f(xs, 0, zs);
	glVertex3f(-xs, 0, zs);
	glVertex3f(0, zSize, 0);

	glVertex3f(-xs, 0, zs);
	glVertex3f(-xs, 0, -zs);
	glVertex3f(0, zSize, 0);

	glEnd();
}

glm::dvec2 lastCursorPos;

bool cameraLookAt = false;
bool panning = false;

class Camera {
private:
	glm::vec3 pos;
	glm::dvec2 rotation;
	glm::vec3 colour;
	double lastUpdated;

public:
	Camera(glm::vec3 pos, glm::dvec2 rotation, glm::vec3 colour) : pos{ pos }, rotation{ rotation }, colour{ colour }, lastUpdated{ glfwGetTime() } {}

	glm::mat4 getTransform() const {
		glm::mat4 out = glm::identity<glm::mat4>();
		out = glm::translate(out, pos);
		out = glm::rotate(out, (float)rotation.x, glm::vec3{ 0,1,0 });
		out = glm::rotate(out, (float)rotation.y, glm::vec3{ 1,0,0 });
		return out;
	}

	void mouseInput(glm::dvec2 rotDelta) {
		rotation += (rotDelta * mouseSensitivity);
		rotation.y = clamp(glm::radians(-80.0), glm::radians(80.0), rotation.y);
	};

	void pan(glm::vec3 movementVector) {
		const double updateTime = glfwGetTime();
		lastUpdated = updateTime;

		movementVector = movementVector * moveSensitivity;
		pos = glm::translate(getTransform(), movementVector) * glm::vec4{ 0.0f,0.0f,0.0f,1.0f };
	}

	glm::mat4 viewMatrix() const {
		const glm::vec3 eye = getTransform() * glm::vec4{ 0.0f, 0.25f, 0.13f, 1.0f };
		const glm::vec3 cen = getTransform() * glm::vec4{ 0.0f, 0.25f, 0.25f, 1.0f };
		const glm::vec3 up{ 0.0f, 1.0f, 0.0f };
		return glm::lookAt(eye, cen, up);
	}
};

Camera camera{ {0, 0.25, -2}, {0, 0}, {0.0f, 0.0f, 1.0f} };

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) { }

void cursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
	const glm::dvec2 newPos{ xpos, ypos };
	const glm::dvec2 diff = newPos - lastCursorPos;
	lastCursorPos = newPos;
	if (cameraLookAt) {
		camera.mouseInput({ -diff.x, diff.y });
	}

	if (panning) {
		camera.pan(glm::vec3{ diff.x * cameraSensitivity, diff.y * cameraSensitivity, 0.0f });
	}
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		cameraLookAt = true;
	}
	else {
		cameraLookAt = false;
	}
	if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS) {
		panning = true;
	}
	else {
		panning = false;
	}
}

void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	camera.pan(glm::vec3{ 0.0f, 0.0f, yoffset * zoomSensitivity });
}

void windowResizeCallback(GLFWwindow* window, int width, int height) {
	windowWidth = width;
	windowHeight = height;
}

void update() { }

void drawScene() {
	glColor3f(1.0f, 0.0f, 1.0f);
	drawPyramid(0.5, 0.5, 0.5);
}

void draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Perspective camera
	glViewport(0, 0, windowWidth, windowHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(glm::value_ptr(glm::perspective(glm::radians(40.0f), 4.0f / 3, 0.1f, 10.0f)));
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(glm::value_ptr(camera.viewMatrix()));
	drawScene();
}

int main() {
	if (!glfwInit()) {
		std::cout << "GLFW initialisation failed." << std::endl;
		return -1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);	

	GLFWwindow* window = glfwCreateWindow(800, 600, "Scene Builder", NULL, NULL);
	if (!window)
	{
		std::cout << "GLFW failed to create window." << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glClearColor(0.4f, 0.5f, 0.6f, 1.0f);

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glfwSetFramebufferSizeCallback(window, windowResizeCallback);

	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, cursorPosCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetScrollCallback(window, mouseScrollCallback);

	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		std::cout << "GLEW initialisation failed: "
			<< glewGetErrorString(err) << std::endl;
		return -1;
	}

	glPushMatrix();
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, 0.4f);
	glPopMatrix();
	glfwGetCursorPos(window, &lastCursorPos.x, &lastCursorPos.y);

	while (!glfwWindowShouldClose(window))
	{
		update();
		draw();
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();

	return 0;
}