#pragma once

#include "Windows/Window.hpp"

#include "GLFW/glfw3.h"

// class representing a platform specific window for linux systems
class LinuxWindow : public Window {
  public:
	LinuxWindow(const WindowProps& props);
	~LinuxWindow();

	void OnUpdate() override;

	inline unsigned int GetWidth() const override {
		return m_Data.Width;
	}
	inline unsigned int GetHeight() const override {
		return m_Data.Height;
	}

	// window attributes
	// ----------------------------------------------------------------------------------

	inline void SetEventCallback(const EventCallbackFn& callback) override {
		m_Data.EventCallback = callback;
	}

	void SetVSync(bool enabled) override;
	bool IsVSync() const override;

	inline virtual void* getNativeWindow() const {
		return m_Window;
	}

  private:
	GLFWwindow* m_Window;

	virtual void Init(const WindowProps& props);
	virtual void InitGLFW();
	virtual void Shutdown();

	struct WindowData {
		std::string Title;
		unsigned int Width, Height;
		bool VSync;

		EventCallbackFn EventCallback;
	};

	WindowData m_Data;
};
