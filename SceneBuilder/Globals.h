#ifndef GLOBALS_H
#define GLOBALS_H

#include <unordered_map>
#include <functional>

#include "Object.h";
#include "EventBus.h";
#include "glm/mat4x4.hpp"

extern EventBus GlobalEventBus;
extern glm::dvec2 GlobalCursor;

#endif