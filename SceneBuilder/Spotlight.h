#ifndef OBJECT_SPOTLIGHT_H
#define OBJECT_SPOTLIGHT_H

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

#include "Editable.h"

class Spotlight : public Editable {
public:
	Spotlight(glm::vec3 p, glm::vec3 c) : Editable(p, c) {
		glEnable(GL_LIGHT0);

		GLfloat redDiffuse[] = { 1.f, 0.0f, 0.0f, 1.0f };
		GLfloat redSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		GLfloat redPosition[] = { -0.2f, 0.f, 0.0f, 1.0f };
		glLightfv(GL_LIGHT0, GL_DIFFUSE, redDiffuse);
		glLightfv(GL_LIGHT0, GL_SPECULAR, redSpecular);
		glLightfv(GL_LIGHT0, GL_POSITION, redPosition);
		glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.001f);

		GLfloat redSpotDir[] = { 0.f,0.f,-1.f };
		GLfloat redSpotCutoff = 90.0f;
		GLfloat redSpotExp = 7.f;
		glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, redSpotDir);
		glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, redSpotCutoff);
		glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, redSpotExp);
	}

protected:
	void drawShape() const override {
		GLfloat redDiffuse[] = { 1.f, 0.0f, 0.0f, 1.0f };
		GLfloat redSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		GLfloat redPosition[] = { -0.2f, 0.f, 0.0f, 1.0f };
		glLightfv(GL_LIGHT0, GL_DIFFUSE, redDiffuse);
		glLightfv(GL_LIGHT0, GL_SPECULAR, redSpecular);
		glLightfv(GL_LIGHT0, GL_POSITION, redPosition);
		glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.001f);

		glm::vec3 redSpotDir = { 0.f,0.f,-1.f };
		redSpotDir = this->getTransform() * glm::vec4{ 0.f,0.f,-1.f, 1.0 };

		GLfloat redSpotDirArr[] = { redSpotDir.x, redSpotDir.y, redSpotDir.z };
		GLfloat redSpotCutoff = 90.0f;
		GLfloat redSpotExp = 7.f;
		glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, redSpotDirArr);
		glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, redSpotCutoff);
		glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, redSpotExp);
	}
};

#endif