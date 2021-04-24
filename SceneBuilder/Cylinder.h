#ifndef OBJECT_CYLINDER_H
#define OBJECT_CYLINDER_H

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

#include <cmath>
#include <math.h>
#include <vector>

#include "Colourful.h"

class Cylinder : public Colourful {
public:
	Cylinder(glm::vec3 p, glm::vec3 c) : Colourful(p, c) {
		float pi = std::acos(-1.0f);
		size_t stepCount = 16;
		float step = 2 * pi / (float)stepCount;
		float theta = 0.0f;

		for (size_t i = 0; i < stepCount; i++) {
			theta = i * step;
			this->vertices.push_back({std::cos(theta), std::sin(theta)});
		}
	}

protected:
	void drawShape() const override {
		glBegin(GL_TRIANGLE_FAN);
		float x, z;
		for (int i = this->vertices.size() - 1; i >= 0; i--) {
			x = this->vertices[i].x;
			z = this->vertices[i].y;
			glVertex3f(x, 0.5f, z);
		}
		glEnd();

		glBegin(GL_QUADS);
		float x1, z1, x2, z2;
		for (size_t i = 0; i < this->vertices.size() - 1; i++) {
			x1 = this->vertices[i].x;
			z1 = this->vertices[i].y;

			x2 = this->vertices[i + 1].x;
			z2 = this->vertices[i + 1].y;
			glVertex3f(x1, 0.5f, z1);
			glVertex3f(x2, 0.5f, z2);

			glVertex3f(x2, -0.5f, z2);
			glVertex3f(x1, -0.5f, z1);
		}

		x1 = this->vertices.back().x;
		z1 = this->vertices.back().y;

		x2 = this->vertices[0].x;
		z2 = this->vertices[0].y;

		glVertex3f(x1, 0.5f, z1);
		glVertex3f(x2, 0.5f, z2);

		glVertex3f(x2, -0.5f, z2);
		glVertex3f(x1, -0.5f, z1);

		glEnd();

		glBegin(GL_TRIANGLE_FAN);
		for (int i = 0; i < this->vertices.size(); i++) {
			x = this->vertices[i].x;
			z = this->vertices[i].y;
			glVertex3f(x, -0.5f, z);
		}
		glEnd();
	}

	std::string objectName() const { return "CYLINDER"; }
private:
	std::vector<glm::vec2> vertices;
};

#endif