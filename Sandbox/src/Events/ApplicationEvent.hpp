#pragma once

#include "Events/Event.hpp"

// class representing a window resize event
class WindowResizedEvent : public Event {
  public:
	// Constructs a new window resize event to the given width and height
	WindowResizedEvent(unsigned int width, unsigned int height)
		: m_Width(width), m_Height(height) {};

	// gets the new width of the window
	inline unsigned int GetWidth() {
		return m_Width;
	}

	// gets the new height of the window
	inline unsigned int GetHeight() {
		return m_Height;
	}

	std::string ToString() const override {
		std::stringstream ss;
		ss << "WindowResizedEvent: [" << m_Width << ", " << m_Height << "]";
		return ss.str();
	}

	EVENT_CLASS_TYPE(WindowResized)
	EVENT_CLASS_CATEGORY(EventCategoryApplication)

  private:
	unsigned int m_Width, m_Height;
};

// class representing a window close event
class WindowClosedEvent : public Event {
  public:
	WindowClosedEvent() {};

	EVENT_CLASS_TYPE(WindowClosed)
	EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

// class representing an app tick event
class AppTickEvent : public Event {
  public:
	AppTickEvent() {};

	EVENT_CLASS_TYPE(AppTick)
	EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

// class representing an app update event
class AppUpdateEvent : public Event {
  public:
	AppUpdateEvent() {};

	EVENT_CLASS_TYPE(AppUpdate)
	EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

// class representing an app render event
class AppRenderEvent : public Event {
  public:
	AppRenderEvent() {};

	EVENT_CLASS_TYPE(AppRender)
	EVENT_CLASS_CATEGORY(EventCategoryApplication)
};
