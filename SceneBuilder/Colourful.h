#pragma once
#ifndef OBJECT_COLOURFUL_H
#define OBJECT_COLOURFUL_H

#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/string_cast.hpp"
#include "glm/common.hpp"

#include "Editable.h"
#include "Globals.h"

class Colourful : public Editable {
public:
	Colourful(glm::vec3 p, glm::vec3 c) : Editable(p, c) {
		GlobalEventBus.addEventHandler<Event::KeyPress>([this](const Event::Base& baseEvent) -> void {
			const Event::KeyPress& e = static_cast<const Event::KeyPress&>(baseEvent);

			if (!this->selected) {
				return;
			}

			if (e.key == GLFW_KEY_V && e.action == GLFW_PRESS) {
				this->colour = { 0.8f, 0.2f, 0.2f };
			}
			else if (e.key == GLFW_KEY_B && e.action == GLFW_PRESS) {
				this->colour = { 0.2f, 0.8f, 0.2f };
			}
			else if (e.key == GLFW_KEY_N && e.action == GLFW_PRESS) {
				this->colour = { 0.2f, 0.2f, 0.8f };
			}
			else if (e.key == GLFW_KEY_M && e.action == GLFW_PRESS) {
				this->colour = { 0.6f, 0.2f, 0.6f };
			}
		}, GlobalObjectId);
	}

protected:
	virtual void drawShape() const = 0;
};

#endif