#include <iostream>
#include <array>

#include "Globals.h"
#include "Application.h"
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
#include <memory>

int main() {
	Application app;

	app.start();

	return 0;
}