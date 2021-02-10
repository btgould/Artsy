#pragma once

#include "artsypch.hpp"

// NOTE: currently, events block execution of other parts of the engine

// type of the event
enum class EventType {
	none = 0,
	WindowClosed,
	WindowResized,
	WindowFocused,
	WindowLostFocus,
	WindowMoved,
	AppTick,
	AppUpdate,
	AppRender,
	KeyPressed,
	KeyReleased,
	KeyTyped,
	MouseButtonPressed,
	MouseButtonReleased,
	MouseMoved,
	MouseScrolled
};

// category(ies) that the event is in
enum EventCategory {
	none = 0,
	EventCategoryApplication = (1 << 0),
	EventCategoryInput = (1 << 1),
	EventCategoryKeyboard = (1 << 2),
	EventCategoryMouse = (1 << 3),
	EventCategoryMouseButton = (1 << 4),
};

// set event name and type
#define EVENT_CLASS_TYPE(type)                                                 \
	static EventType GetStaticType() {                                         \
		return EventType::type;                                                \
	}                                                                          \
	virtual EventType GetEventType() const override {                          \
		return GetStaticType();                                                \
	}                                                                          \
	virtual const char* GetName() const override {                             \
		return #type;                                                          \
	}

// set event category(ies)
#define EVENT_CLASS_CATEGORY(category)                                         \
	virtual int GetCategoryFlags() const override {                            \
		return category;                                                       \
	}

// class representing an application event
class Event {
	friend class EventDistpatcher;

  public:
	// What type an event is
	// overriden in EVENT_CLASS_TYPE macro
	virtual EventType GetEventType() const = 0;
	virtual const char* GetName() const = 0;

	// What category(ies) an event is in
	// overriden in EVENT_CLASS_CATEGORY macro
	virtual int GetCategoryFlags() const = 0;

	virtual std::string ToString() const {
		return GetName();
	}

	// checks if an event is in the given category
	inline bool isInCategory(EventCategory category) {
		return GetCategoryFlags() & category;
	}

	// whether or not this event has been handled
	bool handled = false;
};

class EventDistpatcher {
	template <typename T> using EventFn = std::function<bool(T&)>;

  public:
	EventDistpatcher(Event& event) : m_Event(event) {};

	template <typename T> bool Dispatch(EventFn<T> func) {
		// check if the dispatch function is valid for current event
		if (m_Event.GetEventType() == T::GetStaticType()) {
			// call dispatch function on event, decide if handled
			m_Event.handled = func(*(T*) &m_Event);
			return true;
		}

		// dispatch function was not valid for event type
		return false;
	}

  private:
	Event& m_Event;
};

// define how Event's should be push to streams
inline std::ostream& operator<<(std::ostream& os, const Event& e) {
	return os << e.ToString();
}
