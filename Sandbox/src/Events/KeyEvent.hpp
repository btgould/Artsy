#pragma once

#include "Events/Event.hpp"

// class representing an abstract key event
class KeyEvent : public Event {
  public:
	// gets the code of the key the event represents
	inline int GetKeyCode() {
		return m_KeyCode;
	}

	EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

  protected:
	KeyEvent(int keycode) : m_KeyCode(keycode) {}

	int m_KeyCode;
};

// class representing a key pressed event
class KeyPressedEvent : public KeyEvent {
  public:
	// constructs a new key pressed event with the given code and repeat
	// count
	KeyPressedEvent(int keycode, int repeatCount)
		: KeyEvent(keycode), m_RepeatCount(repeatCount) {};

	// gets the amount of times the event should repeat when the key is held
	inline int GetRepeatCount() {
		return m_RepeatCount;
	}

	std::string ToString() const override {
		std::stringstream ss;
		ss << "KeyPressedEvent Code: " << m_KeyCode << " (" << m_RepeatCount
		   << " repeats)";
		return ss.str();
	}

	EVENT_CLASS_TYPE(KeyPressed);

  private:
	int m_RepeatCount;
};

// class representing a key released event
class KeyReleasedEvent : public KeyEvent {
  public:
	// constructs a new key released event with the given code
	KeyReleasedEvent(int keycode) : KeyEvent(keycode) {};

	std::string ToString() const override {
		std::stringstream ss;
		ss << "KeyReleasedEvent Code: " << m_KeyCode;
		return ss.str();
	}

	EVENT_CLASS_TYPE(KeyReleased)
};
