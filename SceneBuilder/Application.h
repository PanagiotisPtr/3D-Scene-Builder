#ifndef APPLICATION_H
#define APPLICATION_H

#ifndef GLEW_STATIC
	#define GLEW_STATIC
#endif

#include <vector>
#include <functional>
#include <sstream>
#include <memory>
#include <queue>
#include <cmath>

#include "Object.h"
#include "Pyramid.h"
#include "Camera.h"
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
		}, GlobalObjectId);

		GlobalEventBus.addEventHandler<Event::MouseButton>([this](const Event::Base& baseEvent) -> void {
			const Event::MouseButton& e = static_cast<const Event::MouseButton&>(baseEvent);

			if (e.button != GLFW_MOUSE_BUTTON_LEFT || e.action != GLFW_PRESS) {
				return;
			}

			float pixel[4];
			int flippedY = this->windowHeight - (int)GlobalCursor.y;
			glfwGetCursorPos(this->window, &GlobalCursor.x, &GlobalCursor.y);

			for (auto& o : GlobalObjects) {
				o->deselect();
			}

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glViewport(0, 0, this->windowWidth, this->windowHeight);
			// make sure that the first object is always the camera
			for (const auto& o : GlobalObjects) {
				o->drawWithId();
			}
			glReadPixels((int)GlobalCursor.x, flippedY, 1, 1, GL_RGBA, GL_FLOAT, &pixel);
			for (int i = 0; i < 4; i++) pixel[i] = this->roundToTwoDecimals(pixel[i]);

			for (auto& o : GlobalObjects) {
				if (o->getObjectId() == ColourIdGenerator::decodeId({ pixel[0], pixel[1], pixel[2] })) {
					o->toggleSelect();
				}
				else {
					o->deselect();
				}
			}
		}, GlobalObjectId);

		GlobalEventBus.addEventHandler<Event::CursorPos>([this](const Event::Base& baseEvent) -> void {
			glfwGetCursorPos(this->window, &GlobalCursor.x, &GlobalCursor.y);
		}, GlobalObjectId);

		GlobalEventBus.addEventHandler<Event::KeyPress>([](const Event::Base& baseEvent) -> void {
			const Event::KeyPress& e = static_cast<const Event::KeyPress&>(baseEvent);

			if (e.key == GLFW_KEY_1 && e.action == GLFW_PRESS) {
				GlobalObjectQueue.push(ObjectClasses::PYRAMID);
			}
		}, GlobalObjectId);

		GlobalEventBus.addEventHandler<Event::KeyPress>([](const Event::Base& baseEvent) -> void {
			const Event::KeyPress& e = static_cast<const Event::KeyPress&>(baseEvent);

			if (e.key == GLFW_KEY_X && e.action == GLFW_PRESS) {
				for (auto& o : GlobalObjects) {
					if (o->isSeleted() && !o->isChanging()) GlobalDeletionQueue.push(o->getObjectId());
				}
			}
		}, GlobalObjectId);

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

		GlobalObjects.emplace_back(new Camera({ 0, 0.25, -2 }));
	}

	void start() {
		while (!glfwWindowShouldClose(this->window))
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
			glViewport(0, 0, this->windowWidth, this->windowHeight);

			while (!GlobalObjectQueue.empty()) {
				switch (GlobalObjectQueue.front()) {
				case ObjectClasses::PYRAMID:
					GlobalObjects.emplace_back(new Pyramid(0.5f, 0.5f, 0.5f, { 0, 0, 0 }, { 1.0f, 0.0f, 1.0f }));
					break;
				}
				GlobalObjectQueue.pop();
			}

			while (!GlobalDeletionQueue.empty()) {
				unsigned objectId = GlobalDeletionQueue.front();
				GlobalDeletionQueue.pop();

				GlobalEventBus.detachHandlersForObject(objectId);

				GlobalObjects.erase(std::remove_if(std::begin(GlobalObjects), std::end(GlobalObjects), [objectId](const auto& o) {
					return o->getObjectId() == objectId;
				}), std::end(GlobalObjects));
			}

			// make sure that the first object is always the camera
			for (const auto& o : GlobalObjects) {
				glStencilFunc(GL_ALWAYS, 1, 0xFF);
				glStencilMask(0xFF);
				o->draw();
			}

			glfwSwapBuffers(this->window);
			glfwPollEvents();
		}
		glfwTerminate();
	}
private:
	GLFWwindow* window;
	int windowWidth;
	int windowHeight;

	float roundToTwoDecimals(float v) {
		v += 0.005f;

		return std::trunc(100 * v) / 100;
	}
};

#endif