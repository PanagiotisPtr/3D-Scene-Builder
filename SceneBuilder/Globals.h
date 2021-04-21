#ifndef GLOBALS_H
#define GLOBALS_H

#include <unordered_map>
#include <functional>
#include <vector>
#include <queue>
#include <memory>

#include "Object.h"
#include "EventBus.h"
#include "glm/mat4x4.hpp"

enum class ObjectClasses {
	SPHERE,
	PLANE,
	CYLINDER,
	CUBE,
};

extern EventBus GlobalEventBus;
extern glm::dvec2 GlobalCursor;
extern std::vector<std::unique_ptr<Object> > GlobalObjects;
extern std::queue<ObjectClasses> GlobalObjectQueue;
extern std::queue<unsigned> GlobalDeletionQueue;
extern unsigned GlobalObjectId;

#endif