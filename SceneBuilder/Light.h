#ifndef OBJECT_LIGHT_H
#define OBJECT_LIGHT_H

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

#include "Sphere.h"
#include "Globals.h"

class Light : public Sphere {
public:
	Light(glm::vec3 p) : Sphere(p, {1.0f, 1.0f, 0.0f}) {
		this->scaleObject({ -0.8f / Editable::Sensitivity, -0.8f / Editable::Sensitivity, -0.8f / Editable::Sensitivity });
		this->place();
	}
protected:
	std::string objectName() const { return "LIGHT"; }
};

#endif