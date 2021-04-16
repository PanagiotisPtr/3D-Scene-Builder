#ifndef OBJECT_PYRAMID_H
#define OBJECT_PYRAMID_H

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

#include "Object.h"

class Pyramid : public Object {
public:
	Pyramid(float x, float y, float z, glm::vec3 p, glm::dvec2 r, glm::vec3 c) 
	: xSize(x), ySize(y), zSize(z), pos(p), rotation(r), colour(c) {}

	void draw() const override {
		glColor3fv(glm::value_ptr(colour));
		this->drawObject();
		if (this->selected) {
			this->drawOutline();
		}
	}

	void drawWithId() const override {
		glColor3f(this->colourId.r, this->colourId.g, this->colourId.b);
		this->drawObject();
	}
private:
	glm::vec3 pos;
	glm::dvec2 rotation;
	glm::vec3 colour;
	float xSize;
	float ySize;
	float zSize;

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

	void drawOutline() const {
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

	void drawShape() const {
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
};

#endif