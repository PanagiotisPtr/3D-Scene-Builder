#ifndef EVENT_H
#define EVENT_H

#ifndef GLEW_STATIC
	#define GLEW_STATIC
#endif

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include <string>

namespace Event {
	struct Base {
		Base(std::string n) : name(n) {}
		std::string name;
	};

	struct WindowResize : Base {
		WindowResize(GLFWwindow* win, int w, int h)
		: Base("WindowResize"), window(win), width(w), height(h) {};
		GLFWwindow* window;
		int width;
		int height;
	};

	struct KeyPress : Base {
		KeyPress(GLFWwindow* win, int k, int s, int a, int m)
		: Base("KeyPress"), window(win), key(k), scancode(s), action(a), mods(m) {};
		GLFWwindow* window;
		int key;
		int scancode;
		int action;
		int mods;
	};

	struct CursorPos : Base {
		CursorPos(GLFWwindow* win, double x, double y)
		: Base("CursorPos"), window(win), xpos(x), ypos(y) {};
		GLFWwindow* window;
		double xpos;
		double ypos;
	};

	struct MouseButton : Base {
		MouseButton(GLFWwindow* win, int b, int a, int m)
			: Base("MouseButton"), window(win), button(b), action(a), mods(m) {};
		GLFWwindow* window;
		int button;
		int action;
		int mods;
	};

	struct MouseScroll : Base {
		MouseScroll(GLFWwindow* win, double x, double y)
		: Base("MouseScroll"), window(win), xoffset(x), yoffset(y) {};
		GLFWwindow* window;
		double xoffset;
		double yoffset;
	};
}

#endif