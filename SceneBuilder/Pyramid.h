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
	: xSize(x), ySize(y), zSize(z), Object(p, r, c) {}

protected:
	void drawShape() const override {
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
private:
	float xSize;
	float ySize;
	float zSize;
};

#endif