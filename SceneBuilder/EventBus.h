#ifndef EVENT_BUS_H
#define EVENT_BUS_H

#include <unordered_map>
#include <vector>
#include <functional>

#include "Event.h"

class EventBus {
public:
	using EventHandler = std::function<void(const Event::Base&)>;

	void pushEvent(const Event::Base& e) {
		if (handlers.find(e.name) == std::end(handlers)) {
			return;
		}

		for (const EventHandler& handler : handlers[e.name]) {
			handler(e);
		}
	}

	template<typename T>
	void addEventHandler(EventHandler handler) {}

	template<>
	void addEventHandler<Event::WindowResize>(EventHandler handler) {
		handlers["WindowResize"].push_back(handler);
	}

	template<>
	void addEventHandler<Event::KeyPress>(EventHandler handler) {
		handlers["KeyPress"].push_back(handler);
	}

	template<>
	void addEventHandler<Event::CursorPos>(EventHandler handler) {
		handlers["CursorPos"].push_back(handler);
	}

	template<>
	void addEventHandler<Event::MouseButton>(EventHandler handler) {
		handlers["MouseButton"].push_back(handler);
	}
	template<>
	void addEventHandler<Event::MouseScroll>(EventHandler handler) {
		handlers["MouseScroll"].push_back(handler);
	}

private:
	std::unordered_map<
		std::string,
		std::vector<EventHandler>
	> handlers;
};

#endif