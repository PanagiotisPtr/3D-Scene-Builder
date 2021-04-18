#include <iostream>
#include <array>

#include "Globals.h"
#include "Application.h"
#include "Pyramid.h"
#include "Camera.h"
#include "ColourIdGenerator.h"

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
	Camera camera({ 0, 0.25, -2 });
	Pyramid pyramid(0.5f, 0.5f, 0.5f, { 0, 0, 0 }, { 1.0f, 0.0f, 1.0f });

	app.addObject(camera);
	app.addObject(pyramid);

	app.start();

	return 0;
}