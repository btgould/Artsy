#include "artsypch.hpp"

#include "Core.hpp"

#include "Application.hpp"

#include "Events/Event.hpp"

#include "GLFW/glfw3.h"

Application* Application::s_Instance = nullptr;

Application::Application() {
	// creates an OpenGL window + initializes contex
	ARTSY_CORE_ASSERT(!s_Instance,
					  "Cannot create multiple instances of Application class");
	s_Instance = this;

	m_Window = std::unique_ptr<Window>(Window::Create());
	m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
}

Application::~Application() {}

void Application::OnEvent(Event& e) {
	EventDispatcher dispatcher(e);
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
	layer->OnAttach();
}

void Application::PopLayer(Layer* layer) {
	m_LayerStack.PopLayer(layer);
	layer->OnDetach();
}

void Application::PushOverlay(Layer* overlay) {
	m_LayerStack.PushOverlay(overlay);
	overlay->OnAttach();
}

void Application::PopOverlay(Layer* overlay) {
	m_LayerStack.PopOverlay(overlay);
	overlay->OnDetach();
}

bool Application::OnWindowClose(WindowClosedEvent& e) {
	m_Running = false;
	return true;
}
