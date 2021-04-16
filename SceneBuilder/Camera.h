#ifndef OBJECT_CAMERA_H
#define OBJECT_CAMERA_H

#include <functional>

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Globals.h"

class Camera : public Object {
public:
	constexpr static double mouseSensitivity = 0.005f;
	constexpr static float moveSensitivity = 1.0f;
	constexpr static float cameraSensitivity = 0.01f;
	constexpr static float zoomSensitivity = 0.1f;

	Camera(glm::vec3 p, glm::dvec2 r) : pos(p), rotation(r), panning(false), lookAt(false) {
		GlobalEventBus.addEventHandler<Event::CursorPos>([this](const Event::Base& baseEvent) -> void {
			const Event::CursorPos& e = static_cast<const Event::CursorPos&>(baseEvent);

			const glm::dvec2 newPos{ e.xpos, e.ypos };
			const glm::dvec2 diff = newPos - GlobalCursor;
			GlobalCursor = newPos;
			if (this->lookAt) {
				this->mouseInput({ -diff.x, diff.y });
			}

			if (this->panning) {
				this->pan(glm::vec3{ diff.x * Camera::cameraSensitivity, diff.y * Camera::cameraSensitivity, 0.0f });
			}
		});

		GlobalEventBus.addEventHandler<Event::MouseButton>([this](const Event::Base& baseEvent) -> void {
			const Event::MouseButton& e = static_cast<const Event::MouseButton&>(baseEvent);

			if (e.button == GLFW_MOUSE_BUTTON_RIGHT && e.action == GLFW_PRESS) {
				this->lookAt = true;
			}
			else {
				this->lookAt = false;
			}
			if (e.button == GLFW_MOUSE_BUTTON_MIDDLE && e.action == GLFW_PRESS) {
				this->panning = true;
			}
			else {
				this->panning = false;
			}
		});

		GlobalEventBus.addEventHandler<Event::MouseScroll>([this](const Event::Base& baseEvent) -> void {
			const Event::MouseScroll& e = static_cast<const Event::MouseScroll&>(baseEvent);

			this->pan(glm::vec3{ 0.0f, 0.0f, e.yoffset * Camera::zoomSensitivity });
		});
	}

	glm::mat4 getTransform() const {
		glm::mat4 out = glm::identity<glm::mat4>();
		out = glm::translate(out, pos);
		out = glm::rotate(out, (float)rotation.x, glm::vec3{ 0,1,0 });
		out = glm::rotate(out, (float)rotation.y, glm::vec3{ 1,0,0 });
		return out;
	}

	void mouseInput(glm::dvec2 rotDelta) {
		rotation += (rotDelta * Camera::mouseSensitivity);
		rotation.y = clamp(glm::radians(-80.0), glm::radians(80.0), rotation.y);
	};

	void pan(glm::vec3 movementVector) {
		movementVector = movementVector * Camera::moveSensitivity;
		pos = glm::translate(getTransform(), movementVector) * glm::vec4{ 0.0f,0.0f,0.0f,1.0f };
	}

	glm::mat4 viewMatrix() const {
		const glm::vec3 eye = getTransform() * glm::vec4{ 0.0f, 0.25f, 0.13f, 1.0f };
		const glm::vec3 cen = getTransform() * glm::vec4{ 0.0f, 0.25f, 0.25f, 1.0f };
		const glm::vec3 up{ 0.0f, 1.0f, 0.0f };
		return glm::lookAt(eye, cen, up);
	}

	void draw() const override {
		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(glm::value_ptr(glm::perspective(glm::radians(40.0f), 4.0f / 3, 0.1f, 10.0f)));
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(glm::value_ptr(this->viewMatrix()));
	}

	void drawWithId() const override {
		this->draw();
	}
private:
	glm::vec3 pos;
	glm::dvec2 rotation;
	bool panning;
	bool lookAt;

	template<typename T> T clamp(T a, T b, T x) {
		if (x < a) return a;
		if (x > b) return b;
		return x;
	}
};

#endif