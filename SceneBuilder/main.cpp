#include <iostream>
#include <array>

#include "Globals.h"
#include "Application.h"
#include "Pyramid.h"
#include "Camera.h"

#define GLEW_STATIC

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/string_cast.hpp"
#include <array>

int main() {
	Application app;

	app.addObject(Camera({0, 0.25, -2}, {0, 0}, {0.0f, 0.0f, 1.0f}));
	app.addObject(Pyramid(0.5f, 0.5f, 0.5f));

	app.start();

	return 0;
}