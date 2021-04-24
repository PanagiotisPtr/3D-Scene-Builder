#ifndef OBJECT_OBJECT_H
#define OBJECT_OBJECT_H

#include <functional>
#include <ostream>

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

#include "ColourIdGenerator.h"

class Object {
public:
	Object(glm::vec3 p, glm::vec3 c)
	: colourId(ColourIdGenerator::getColourId()), objectId(ColourIdGenerator::prevId), selected(false),
	moving(false), rotating(false), scaling(false), pos(p), rot({0.0f, 0.0f, 0.0f}),
	scale({ 1.0f , 1.0f, 1.0f }), colour(c) {}

	virtual void draw() const {
		glColor3f(this->colour.r, this->colour.g, this->colour.b);
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

	virtual void serialise(std::ostream& stream) const {
		stream << this->objectName() << '\n';
		stream << this->colour.r << ' ' << this->colour.g << ' ' << this->colour.b << '\n';
		glm::mat4 mat = this->getTransform();
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				stream << mat[i][j] << (j == 3 ? '\n' : ' ');
			}
		}
	}

	void toggleSelect() { this->selected = !this->selected; }
	void select() { this->selected = true; }
	bool isSeleted() const { return this->selected;  }
	virtual void deselect() { this->selected = false; }
	virtual bool isChanging() const { return this->moving || this->rotating || this->scaling; }
protected:
	ColourIdGenerator::ColourId colourId;
	glm::vec3 pos;
	glm::vec3 rot;
	glm::vec3 scale;
	glm::vec3 colour;
	unsigned objectId;
	bool selected;
	bool moving;
	bool scaling;
	bool rotating;

	virtual void drawShape() const = 0;

	virtual glm::mat4 getTransform() const {
		glm::mat4 out = glm::identity<glm::mat4>();
		out = glm::translate(out, this->pos);
		out = glm::rotate(out, this->rot.x, glm::vec3{ 0,1,0 });
		out = glm::rotate(out, this->rot.y, glm::vec3{ 1,0,0 });
		out = glm::rotate(out, this->rot.z, glm::vec3{ 0,0,1 });
		out = glm::scale(out, { this->scale.x, this->scale.y, this->scale.z });

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
		glDisable(GL_LIGHTING);

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
		glEnable(GL_LIGHTING);
	}

	virtual std::string objectName() const = 0;
};

#endif