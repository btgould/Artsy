#pragma once

#include "artsypch.hpp"

#include "Events/Event.hpp"

// struct storing simple information about a window
struct WindowProps {
	std::string Title;
	unsigned int Width;
	unsigned int Height;

	WindowProps(const std::string& title = "Artsy", unsigned int width = 940,
				unsigned int height = 600)
		: Title(title), Width(width), Height(height) {}
};

// platform independent interface representing a graphical window
class Window {
  public:
	using EventCallbackFn = std::function<void(Event&)>;

	virtual ~Window() {};

	// function to be called each frame to update the window
	virtual void OnUpdate() = 0;

	// gets the width of the window
	virtual unsigned int GetWidth() const = 0;
	// gets the height of the window
	virtual unsigned int GetHeight() const = 0;

	// sets the callback function used to handle events
	virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
	// sets whether the window should be VSynced
	virtual void SetVSync(bool enabled) = 0;
	// gets whether the window is VSynced
	virtual bool IsVSync() const = 0;

	// creates a new window with the given window properties
	static Window* Create(const WindowProps& props = WindowProps());
};
