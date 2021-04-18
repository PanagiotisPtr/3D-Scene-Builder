#ifndef OBJECT_OBJECT_H
#define OBJECT_OBJECT_H

#include <functional>

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

#include "Globals.h"
#include "ColourIdGenerator.h"

class Object {
public:
	Object(glm::vec3 p, glm::dvec2 r, glm::vec3 c)
	: colourId(ColourIdGenerator::getColourId()), objectId(ColourIdGenerator::prevId), selected(false),
	pos(p), rotation(r), colour(c) {}

	virtual void draw() const {
		glColor3fv(glm::value_ptr(colour));
		this->drawObject();
		if (this->selected) {
			this->drawOutline();
		}
	}

	virtual void drawWithId() const {
		glColor3f(this->colourId.r, this->colourId.g, this->colourId.b);
		this->drawObject();
	}

	unsigned getObjectId() const {
		return this->objectId;
	}

	void toggleSelect() { selected = !selected; }
	void select() { selected = true; }
	void deselect() { selected = false; }
protected:
	ColourIdGenerator::ColourId colourId;
	glm::vec3 pos;
	glm::dvec2 rotation;
	glm::vec3 colour;
	unsigned objectId;
	bool selected;

	virtual void drawShape() const = 0;

	glm::mat4 getTransform() const {
		glm::mat4 out = glm::identity<glm::mat4>();
		out = glm::translate(out, pos);
		out = glm::rotate(out, (float)rotation.x, glm::vec3{ 0,1,0 });
		out = glm::rotate(out, (float)rotation.y, glm::vec3{ 1,0,0 });

		return out;
	}

	void drawObject() const {
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glMultMatrixf(glm::value_ptr(this->getTransform()));

		this->drawShape();

		glPopMatrix();
	}

	virtual void drawOutline() const {
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);
		glDisable(GL_DEPTH_TEST);

		glColor3f(0.0f, 1.0f, 0.6f);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glm::mat4 mat = this->getTransform();
		mat = glm::scale(mat, { 1.04, 1.04, 1.04 });
		glMultMatrixf(glm::value_ptr(mat));

		this->drawShape();

		glPopMatrix();

		glStencilMask(0xFF);
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glEnable(GL_DEPTH_TEST);
	}
};

#endif