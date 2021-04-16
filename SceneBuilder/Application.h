#ifndef APPLICATION_H
#define APPLICATION_H

#ifndef GLEW_STATIC
	#define GLEW_STATIC
#endif

#include <iostream>
#include <vector>
#include <functional>
#include <sstream>
#include <cmath>

#include "Object.h"
#include "Pyramid.h"
#include "EventBus.h"
#include "Event.h"
#include "Globals.h"

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/string_cast.hpp"

class Application {
public:
	using objectReference = std::reference_wrapper<Object>;

	Application() {
		if (!glfwInit()) {
			throw std::exception("GLFW initialisation failed.");
		}
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

		this->window = glfwCreateWindow(800, 600, "Scene Builder", NULL, NULL);
		if (!this->window) {
			glfwTerminate();
			throw std::exception("GLFW failed to create window.");
		}

		this->windowWidth = 800;
		this->windowHeight = 600;

		glfwMakeContextCurrent(this->window);
		glClearColor(0.4f, 0.5f, 0.6f, 1.0f);

		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_STENCIL_TEST);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		glDepthFunc(GL_LESS);

		glfwSetFramebufferSizeCallback(this->window, [](GLFWwindow* window, int width, int height) {
			GlobalEventBus.pushEvent(Event::WindowResize(
				window,
				width,
				height
			));
		});

		glfwSetKeyCallback(this->window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
			GlobalEventBus.pushEvent(Event::KeyPress(
				window,
				key,
				scancode,
				action,
				mods
			));
		});

		glfwSetCursorPosCallback(this->window, [](GLFWwindow* window, double xpos, double ypos) {
			GlobalEventBus.pushEvent(Event::CursorPos(
				window,
				xpos,
				ypos
			));
		});

		glfwSetMouseButtonCallback(this->window, [](GLFWwindow* window, int button, int action, int mods) {
			GlobalEventBus.pushEvent(Event::MouseButton(
				window,
				button,
				action,
				mods
			));
		});

		glfwSetScrollCallback(this->window, [](GLFWwindow* window, double xoffset, double yoffset) {
			GlobalEventBus.pushEvent(Event::MouseScroll(
				window,
				xoffset,
				yoffset
			));
		});

		GlobalEventBus.addEventHandler<Event::WindowResize>([this](const Event::Base& baseEvent) -> void {
			const Event::WindowResize& e = static_cast<const Event::WindowResize&>(baseEvent);

			this->windowHeight = e.height;
			this->windowWidth = e.width;
		});

		GlobalEventBus.addEventHandler<Event::MouseButton>([this](const Event::Base& baseEvent) -> void {
			const Event::MouseButton& e = static_cast<const Event::MouseButton&>(baseEvent);

			if (e.button != GLFW_MOUSE_BUTTON_LEFT || e.action != GLFW_PRESS) {
				return;
			}

			float pixel[4];
			int flippedY = this->windowHeight - (int)GlobalCursor.y;
			glfwGetCursorPos(this->window, &GlobalCursor.x, &GlobalCursor.y);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glViewport(0, 0, this->windowWidth, this->windowHeight);
			// make sure that the first object is always the camera
			for (const Object& o : objects) {
				o.drawWithId();
			}
			glReadPixels((int)GlobalCursor.x, flippedY, 1, 1, GL_RGBA, GL_FLOAT, &pixel);
			for (int i = 0; i < 4; i++) pixel[i] = this->roundToTwoDecimals(pixel[i]);

			for (Object& o : objects) {
				if (o.getObjectId() == ColourIdGenerator::decodeId({ pixel[0], pixel[1], pixel[2] })) {
					o.toggleSelect();
				}
				else {
					o.deselect();
				}
			}
		});

		glewExperimental = GL_TRUE;
		GLenum err = glewInit();
		if (err != GLEW_OK)
		{
			std::stringstream ss;
			ss << "GLEW initialisation failed: "
				<< glewGetErrorString(err) << std::endl;

			throw std::exception(ss.str().c_str());
		}

		glPushMatrix();
		glLoadIdentity();
		glTranslatef(0.0f, 0.0f, 0.4f);
		glPopMatrix();
		glfwGetCursorPos(this->window, &GlobalCursor.x, &GlobalCursor.y);
	}

	void start() {
		while (!glfwWindowShouldClose(this->window))
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
			glViewport(0, 0, this->windowWidth, this->windowHeight);

			// make sure that the first object is always the camera
			for (const Object& o : objects) {
				glStencilFunc(GL_ALWAYS, 1, 0xFF);
				glStencilMask(0xFF);
				o.draw();
			}

			glfwSwapBuffers(this->window);
			glfwPollEvents();
		}
		glfwTerminate();
	}

	void addObject(Object& obj) {
		std::cout << "Added objects" << std::endl;
		objects.push_back(std::ref(obj));
	}
private:
	GLFWwindow* window;
	std::vector<objectReference> objects;
	int windowWidth;
	int windowHeight;

	float roundToTwoDecimals(float v) {
		v += 0.005;

		return std::trunc(100 * v) / 100;
	}
};

#endif