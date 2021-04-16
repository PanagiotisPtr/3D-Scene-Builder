#include "Globals.h"

extern EventBus GlobalEventBus = EventBus();
extern glm::dvec2 GlobalCursor = { 0.0f, 0.0f };
std::unordered_map<int, std::reference_wrapper<const Object> > GlobalObjectMap = {};