#include "artsypch.hpp"

#include "Application.hpp"

#include "Events/Event.hpp"

#include "GLFW/glfw3.h"

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

Application::Application() {
	// creates an OpenGL window + initializes contex
	m_Window = std::unique_ptr<Window>(Window::Create());
	m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
}

Application::~Application() {}

void Application::OnEvent(Event& e) {
	EventDistpatcher dispatcher(e);
	dispatcher.Dispatch<WindowClosedEvent>(
		BIND_EVENT_FN(Application::OnWindowClose));

	for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
		(*--it)->OnEvent(e);
		if (e.handled)
			break;
	}
}

void Application::run() {
	while (m_Running) {
		glClear(GL_COLOR_BUFFER_BIT);

		for (Layer* layer : m_LayerStack)
			layer->OnUpdate();

		m_Window->OnUpdate();
	}
}

void Application::PushLayer(Layer* layer) {
	m_LayerStack.PushLayer(layer);
}

void Application::PopLayer(Layer* layer) {
	m_LayerStack.PopLayer(layer);
}

bool Application::OnWindowClose(WindowClosedEvent& e) {
	m_Running = false;
	return true;
}
