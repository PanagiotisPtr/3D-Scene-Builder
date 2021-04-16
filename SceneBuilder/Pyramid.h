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

#include "Object.h"

class Pyramid : public Object {
public:
	Pyramid(float x, float y, float z) : xSize(x), ySize(y), zSize(z) {}

	void draw() const override {
		const float xs = xSize / 2;
		const float zs = zSize / 2;
		glColor3f(1.0f, 0.0f, 1.0f);
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