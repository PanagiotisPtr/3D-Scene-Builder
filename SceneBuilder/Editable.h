#ifndef OBJECT_EDITABLE_H
#define OBJECT_EDITABLE_H

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Globals.h"

class Editable : public Object {
public:
	constexpr static float Sensitivity = 0.01f;

	enum class Axis {
		NONE,
		X,
		Y,
		Z
	};

	Editable(glm::vec3 p, glm::vec3 c)
		: Object(p, c), prevPos(p), prevRot({0.0f, 0.0f, 0.0f}), prevScale(scale),
		prevCursor(GlobalCursor), axis(Axis::NONE) {
		GlobalEventBus.addEventHandler<Event::CursorPos>([this](const Event::Base& baseEvent) -> void {
			const Event::CursorPos& e = static_cast<const Event::CursorPos&>(baseEvent);

			const glm::dvec2 newPos{ e.xpos, e.ypos };
			const glm::dvec2 diff = newPos - this->prevCursor;
			this->prevCursor = newPos;

			if (this->selected == false) this->place();

			if (this->moving) {
				if (this->axis == Axis::X) this->moveObject(glm::vec3{ -diff.x, 0.0f, 0.0f });
				if (this->axis == Axis::Y) this->moveObject(glm::vec3{ 0.0f, -diff.y, 0.0f });
				if (this->axis == Axis::Z) this->moveObject(glm::vec3{ 0.0f, 0.0f, -(diff.x + diff.y) });
			}

			if (this->scaling) {
				if (this->axis == Axis::NONE)
					this->scaleObject(
						glm::vec3{ -(-diff.x + diff.y),
						-(-diff.x + diff.y),
						-(-diff.x + diff.y)
						}
					);
				if (this->axis == Axis::X) this->scaleObject(glm::vec3{ diff.x, 0.0f, 0.0f });
				if (this->axis == Axis::Y) this->scaleObject(glm::vec3{ 0.0f, -diff.y, 0.0f });
				if (this->axis == Axis::Z) this->scaleObject(glm::vec3{ 0.0f, 0.0f, -(-diff.x + diff.y) });
			}

			if (this->rotating) {
				if (this->axis == Axis::X) this->rotatebject(glm::vec3{ diff.x, 0.0f, 0.0f });
				if (this->axis == Axis::Y) this->rotatebject(glm::vec3{ 0.0f, -diff.y, 0.0f });
				if (this->axis == Axis::Z) this->rotatebject(glm::vec3{ 0.0f, 0.0f, -(-diff.x + diff.y) });
			}

		}, this->getObjectId());

		GlobalEventBus.addEventHandler<Event::KeyPress>([this](const Event::Base& baseEvent) -> void {
			const Event::KeyPress& e = static_cast<const Event::KeyPress&>(baseEvent);

			if (e.action == GLFW_REPEAT) { return; }
			if (this->selected == false) { return; }

			if (e.key == GLFW_KEY_X && e.action == GLFW_PRESS) { this->axis = Axis::X; }
			if (e.key == GLFW_KEY_Y && e.action == GLFW_PRESS) { this->axis = Axis::Y; }
			if (e.key == GLFW_KEY_Z && e.action == GLFW_PRESS) { this->axis = Axis::Z; }

			if (e.key == GLFW_KEY_G && e.action == GLFW_PRESS) {
				if (this->moving) {
					this->place();
				}
				else {
					this->pos = this->prevPos;
				}
				this->moving = !this->moving;
			}

			if (e.key == GLFW_KEY_S && e.action == GLFW_PRESS) {
				if (this->scaling) {
					this->place();
				}
				else {
					this->scale = this->prevScale;
				}
				this->scaling = !this->scaling;
			}

			if (e.key == GLFW_KEY_R && e.action == GLFW_PRESS) {
				if (this->rotating) {
					this->place();
				}
				else {
					this->rot = this->prevRot;
				}
				this->rotating = !this->rotating;
			}

			if (e.key == GLFW_KEY_ESCAPE && e.action == GLFW_PRESS) {
				this->reset();
			}
		}, this->getObjectId());
	}

	void deselect() override {
		if (this->moving || this->rotating || this->scaling) {
			this->place();
		}
		else {
			this->selected = false;
		}
	}
protected:
	Axis axis;
	glm::vec3 prevPos;
	glm::vec3 prevRot;
	glm::vec3 prevScale;
	glm::dvec2 prevCursor;

	void place() {
		this->prevPos = this->pos;
		this->prevRot = this->rot;
		this->prevScale = this->scale;

		this->reset();
	}

	void reset() {
		this->moving = false;
		this->scaling = false;
		this->rotating = false;

		this->pos = this->prevPos;
		this->scale = this->prevScale;
		this->rot = this->prevRot;
		this->axis = Axis::NONE;
	}

	void moveObject(glm::vec3 changeVector) {
		changeVector = changeVector * Editable::Sensitivity;
		this->pos = glm::translate(this->getTransform(), changeVector) * glm::vec4{ 0.0f,0.0f,0.0f,1.0f };
	}

	void scaleObject(glm::vec3 changeVector) {
		changeVector = changeVector * Editable::Sensitivity;
		this->scale += changeVector;
	}

	void rotatebject(glm::vec3 changeVector) {
		changeVector = changeVector * Editable::Sensitivity;
		this->rot += changeVector;
	}

	glm::mat4 getTransform() const override {
		glm::mat4 out = glm::identity<glm::mat4>();
		if (moving || scaling || rotating) {
			out = glm::translate(out, this->pos);
			out = glm::rotate(out, this->rot.x, glm::vec3{ 0,1,0 });
			out = glm::rotate(out, this->rot.y, glm::vec3{ 1,0,0 });
			out = glm::rotate(out, this->rot.z, glm::vec3{ 0,0,1 });
			out = glm::scale(out, { this->scale.x, this->scale.y, this->scale.z });
		}
		else {
			out = glm::translate(out, this->prevPos);
			out = glm::rotate(out, this->prevRot.x, glm::vec3{ 0,1,0 });
			out = glm::rotate(out, this->prevRot.y, glm::vec3{ 1,0,0 });
			out = glm::rotate(out, this->prevRot.z, glm::vec3{ 0,0,1 });
			out = glm::scale(out, { this->prevScale.x, this->prevScale.y, this->prevScale.z });
		}

		return out;
	}
};

#endif