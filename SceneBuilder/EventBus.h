#ifndef EVENT_BUS_H
#define EVENT_BUS_H

#include <unordered_map>
#include <functional>
#include <vector>

#include "Event.h"

class EventBus {
public:
	using EventHandler = std::function<void(const Event::Base&)>;

	void pushEvent(const Event::Base& e) {
		if (this->handlers.find(e.name) == std::end(this->handlers)) {
			return;
		}

		for (const auto& p: this->handlers[e.name]) {
			for (const EventHandler& handler : p.second) {
				handler(e);
			}
		}
	}

	void detachHandlersForObject(unsigned objectId) {
		for (auto& p : this->handlers) {
			p.second.erase(objectId);
		}
	}

	template<typename T>
	void addEventHandler(EventHandler handler, unsigned objectId) {}

	template<>
	void addEventHandler<Event::WindowResize>(EventHandler handler, unsigned objectId) {
		if (this->handlers.find("WindowResize") == std::end(this->handlers)) {
			this->handlers["WindowResize"] = {};
		}
		this->handlers["WindowResize"][objectId].push_back(handler);
	}

	template<>
	void addEventHandler<Event::KeyPress>(EventHandler handler, unsigned objectId) {
		if (this->handlers.find("KeyPress") == std::end(this->handlers)) {
			this->handlers["KeyPress"] = {};
		}
		this->handlers["KeyPress"][objectId].push_back(handler);
	}

	template<>
	void addEventHandler<Event::CursorPos>(EventHandler handler, unsigned objectId) {
		if (this->handlers.find("CursorPos") == std::end(this->handlers)) {
			this->handlers["CursorPos"] = {};
		}
		this->handlers["CursorPos"][objectId].push_back(handler);
	}

	template<>
	void addEventHandler<Event::MouseButton>(EventHandler handler, unsigned objectId) {
		if (this->handlers.find("MouseButton") == std::end(this->handlers)) {
			this->handlers["MouseButton"] = {};
		}
		this->handlers["MouseButton"][objectId].push_back(handler);
	}
	template<>
	void addEventHandler<Event::MouseScroll>(EventHandler handler, unsigned objectId) {
		if (this->handlers.find("MouseScroll") == std::end(this->handlers)) {
			this->handlers["MouseScroll"] = {};
		}
		this->handlers["MouseScroll"][objectId].push_back(handler);
	}

private:
	std::unordered_map<
		std::string,
		std::unordered_map<unsigned, std::vector<EventHandler> >
	> handlers;
};

#endif