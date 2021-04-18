#include "Globals.h"

extern EventBus GlobalEventBus = EventBus();
extern glm::dvec2 GlobalCursor = { 0.0f, 0.0f };
std::vector<std::unique_ptr<Object> > GlobalObjects = {};
std::queue<ObjectClasses> GlobalObjectQueue = {};
std::queue<unsigned> GlobalDeletionQueue = {};
unsigned GlobalObjectId = 99999;