#ifndef EVENT_BUS_H
#define EVENT_BUS_H

#include <iostream>
#include <unordered_map>
#include <vector>
#include <functional>

#include "Event.h"

class EventBus {
public:
	using EventHandler = std::function<void(const Event::Base&)>;

	void pushEvent(const Event::Base& e) {
		std::cout << "Event Name: ";
		std::cout << e.name << std::endl;
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
		std::cout << "Adding WindowResize handler" << std::endl;
		handlers["WindowResize"].push_back(handler);
	}

	template<>
	void addEventHandler<Event::KeyPress>(EventHandler handler) {
		std::cout << "Adding KeyPress handler" << std::endl;
		handlers["KeyPress"].push_back(handler);
	}

	template<>
	void addEventHandler<Event::CursorPos>(EventHandler handler) {
		std::cout << "Adding CursorPos handler" << std::endl;
		handlers["CursorPos"].push_back(handler);
	}

	template<>
	void addEventHandler<Event::MouseButton>(EventHandler handler) {
		std::cout << "Adding MouseButton handler" << std::endl;
		handlers["MouseButton"].push_back(handler);
	}
	template<>
	void addEventHandler<Event::MouseScroll>(EventHandler handler) {
		std::cout << "Adding MouseScroll handler" << std::endl;
		handlers["MouseScroll"].push_back(handler);
	}

private:
	std::unordered_map<
		std::string,
		std::vector<EventHandler>
	> handlers;
};

#endif