#pragma once

#include "Windows/Window.hpp"

#include "Events/ApplicationEvent.hpp"

#include "Application/LayerStack.hpp"

class Application {
  public:
	Application();
	virtual ~Application();

	void OnEvent(Event& e);

	void run();

	void PushLayer(Layer* layer);
	void PopLayer(Layer* layer);

  private:
	bool OnWindowClose(WindowClosedEvent& e);

	std::unique_ptr<Window> m_Window;
	bool m_Running = true;

	LayerStack m_LayerStack;
};

// client decides how to obtain application
Application* CreateApplication();
