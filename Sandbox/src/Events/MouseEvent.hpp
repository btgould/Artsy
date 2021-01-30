#pragma once

#include "Events/Event.hpp"

// class representing a mouse moved event
class MouseMovedEvent : public Event {
  public:
	// constructs a new mouse moved event with the given x and y coords
	MouseMovedEvent(float x, float y) : m_MouseX(x), m_MouseY(y) {};

	// gets the x coord the mouse was moved to
	inline float GetX() const {
		return m_MouseX;
	}

	// gets the y coord the mouse was moved to
	inline float GetY() const {
		return m_MouseY;
	}

	std::string ToString() const override {
		std::stringstream ss;
		ss << "MouseMovedEvent: (" << m_MouseX << ", " << m_MouseY << ")";
		return ss.str();
	}

	EVENT_CLASS_TYPE(MouseMoved)
	EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

  private:
	float m_MouseX, m_MouseY;
};

// class representing a mouse scrolled event
class MouseScrolledEvent : public Event {
  public:
	// constructs a new mouse scrolled event with the given x and y offsets
	MouseScrolledEvent(float xOffset, float yOffset)
		: m_XOffset(xOffset), m_YOffset(yOffset) {};

	// gets the x offset of the scroll
	inline float GetXOffset() {
		return m_XOffset;
	}

	// gets the y offset of the scroll
	inline float GetYOffset() {
		return m_YOffset;
	}

	std::string ToString() const override {
		std::stringstream ss;
		ss << "MouseScrolledEvent: (" << m_XOffset << ", " << m_YOffset << ")";
		return ss.str();
	}

	EVENT_CLASS_TYPE(MouseScrolled)
	EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

  private:
	float m_XOffset, m_YOffset;
};

// class representing an abstract mouse button event
class MouseButtonEvent : public Event {
  public:
	// gets the mouse button of this event
	inline int GetMouseButton() const {
		return m_Button;
	}

	EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

  protected:
	MouseButtonEvent(int button) : m_Button(button) {};

	int m_Button;
};

// class representing a mouse button pressed event
class MouseButtonPressedEvent : public MouseButtonEvent {
  public:
	// constructs a new mouse button pressed event for the given button
	MouseButtonPressedEvent(int button) : MouseButtonEvent(button) {};

	std::string ToString() const override {
		std::stringstream ss;
		ss << "MouseButtonPressedEvent: " << m_Button;
		return ss.str();
	}

	EVENT_CLASS_TYPE(MouseButtonPressed)
};

// class representing a mouse button pressed event
class MouseButtonReleasedEvent : public MouseButtonEvent {
  public:
	// constructs a new mouse button pressed event for the given button
	MouseButtonReleasedEvent(int button) : MouseButtonEvent(button) {};

	std::string ToString() const override {
		std::stringstream ss;
		ss << "MouseButtonReleasedEvent: " << m_Button;
		return ss.str();
	}

	EVENT_CLASS_TYPE(MouseButtonReleased)
};
