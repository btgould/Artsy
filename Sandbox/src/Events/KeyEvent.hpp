#pragma once

#include "Events/Event.hpp"

// class representing an abstract key event
class KeyEvent : public Event {
  public:
	// gets the code of the key the event represents
	inline int GetKeyCode() {
		return m_KeyCode;
	}

	inline int GetMods() {
		return m_Mods;
	}

	inline bool modDown(int mod) {
		return mod & m_Mods;
	}

	EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

  protected:
	KeyEvent(int keycode, int mods = 0) : m_KeyCode(keycode), m_Mods(mods) {}

	int m_KeyCode;
	int m_Mods;
};

// class representing a key pressed event
class KeyPressedEvent : public KeyEvent {
  public:
	// constructs a new key pressed event with the given code and repeat
	// count
	KeyPressedEvent(int keycode, int repeatCount, int mods = 0)
		: KeyEvent(keycode, mods), m_RepeatCount(repeatCount) {};

	// gets the amount of times the event has repeated
	inline int GetRepeatCount() {
		return m_RepeatCount;
	}

	std::string ToString() const override {
		std::stringstream ss;
		ss << "KeyPressedEvent Code: " << m_KeyCode << " (" << m_RepeatCount << " repeats)";
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
	KeyReleasedEvent(int keycode, int mods = 0) : KeyEvent(keycode, mods) {};

	std::string ToString() const override {
		std::stringstream ss;
		ss << "KeyReleasedEvent Code: " << m_KeyCode;
		return ss.str();
	}

	EVENT_CLASS_TYPE(KeyReleased)
};

class KeyTypedEvent : public KeyEvent {
  public:
	// constructs a new key typed event with the given code
	KeyTypedEvent(int keycode, int mods = 0) : KeyEvent(keycode, mods) {};

	std::string ToString() const override {
		std::stringstream ss;
		ss << "KeyTypedEvent Code: " << m_KeyCode;
		return ss.str();
	}

	EVENT_CLASS_TYPE(KeyTyped);
};
