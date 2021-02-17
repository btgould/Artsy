#include "artsypch.hpp"

#include "imgui/imgui.h"
#include "OpenGL/ImGuiOpenGLRenderer.h"

#include "Core.hpp"
#include "Application/Application.hpp"

#include "ImGuiLayer.hpp"

// TEMPORARY
#include <GL/glew.h>
#include <GLFW/glfw3.h> // keycodes

ImGuiLayer::ImGuiLayer() : Layer("ImGui Layer"), app(Application::get()) {}

ImGuiLayer::~ImGuiLayer() {}

void ImGuiLayer::OnAttach() {
	// initialize ImGui
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGuiIO& io = ImGui::GetIO();
	io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
	io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

	// set ImGui keybindings
	// TODO: should really use engine specific key codes
	io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
	io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
	io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
	io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
	io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
	io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
	io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
	io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
	io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
	io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
	io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
	io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
	io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
	io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
	io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
	io.KeyMap[ImGuiKey_KeyPadEnter] = GLFW_KEY_KP_ENTER;
	io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
	io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
	io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
	io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
	io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
	io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

	ImGui_ImplOpenGL3_Init("#version 410");
}

void ImGuiLayer::OnDetach() {}

void ImGuiLayer::OnUpdate() {
	// update window size
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize =
		ImVec2(app.getWindow().GetWidth(), app.getWindow().GetHeight());

	// update delta T
	float time = (float) glfwGetTime();
	io.DeltaTime = (m_Time > 0.0f) ? (time - m_Time) : (1.0f / 60.0f);
	m_Time = time;

	ImGui_ImplOpenGL3_NewFrame();
	ImGui::NewFrame();

	static bool show = true;
	ImGui::ShowDemoWindow(&show);

	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiLayer::OnEvent(Event& event) {
	EventDispatcher dispatcher(event);

	// bind dispatch functions for mouse button events
	dispatcher.Dispatch<MouseButtonPressedEvent>(
		BIND_EVENT_FN(ImGuiLayer::OnMouseButtonPressedEvent));
	dispatcher.Dispatch<MouseButtonReleasedEvent>(
		BIND_EVENT_FN(ImGuiLayer::OnMouseButtonReleasedEvent));
	dispatcher.Dispatch<MouseMovedEvent>(
		BIND_EVENT_FN(ImGuiLayer::OnMouseMovedEvent));
	dispatcher.Dispatch<MouseScrolledEvent>(
		BIND_EVENT_FN(ImGuiLayer::OnMouseScrolledEvent));

	// bind dispatch functions for key events
	dispatcher.Dispatch<KeyPressedEvent>(
		BIND_EVENT_FN(ImGuiLayer::OnKeyPressedEvent));
	dispatcher.Dispatch<KeyReleasedEvent>(
		BIND_EVENT_FN(ImGuiLayer::OnKeyReleasedEvent));
	dispatcher.Dispatch<KeyTypedEvent>(
		BIND_EVENT_FN(ImGuiLayer::OnKeyTypedEvent));

	// bind dispatch functions for window events
	dispatcher.Dispatch<WindowResizedEvent>(
		BIND_EVENT_FN(ImGuiLayer::OnWindowResizedEvent));
}

bool ImGuiLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e) {
	ImGuiIO& io = ImGui::GetIO();
	io.MouseDown[e.GetMouseButton()] = true;

	return false;
}

bool ImGuiLayer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e) {
	ImGuiIO& io = ImGui::GetIO();
	io.MouseDown[e.GetMouseButton()] = false;

	return false;
}

bool ImGuiLayer::OnMouseMovedEvent(MouseMovedEvent& e) {
	ImGuiIO& io = ImGui::GetIO();
	io.MousePos = ImVec2(e.GetX(), e.GetY());

	return false;
}

bool ImGuiLayer::OnMouseScrolledEvent(MouseScrolledEvent& e) {
	ImGuiIO& io = ImGui::GetIO();
	io.MouseWheel += e.GetYOffset();
	io.MouseWheelH += e.GetXOffset();

	return false;
}

bool ImGuiLayer::OnKeyPressedEvent(KeyPressedEvent& e) {
	ImGuiIO& io = ImGui::GetIO();
	io.KeysDown[e.GetKeyCode()] = true;

	// set any needed modifiers
	io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] ||
				 io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
	io.KeyShift =
		io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
	io.KeyAlt =
		io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
	io.KeySuper =
		io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];

	return false;
}

bool ImGuiLayer::OnKeyReleasedEvent(KeyReleasedEvent& e) {
	ImGuiIO& io = ImGui::GetIO();
	io.KeysDown[e.GetKeyCode()] = false;

	return false;
}

bool ImGuiLayer::OnKeyTypedEvent(KeyTypedEvent& e) {
	ImGuiIO& io = ImGui::GetIO();

	int keycode = e.GetKeyCode();

	if (keycode > 0 && keycode < 0x10000) {
		io.AddInputCharacter((unsigned short) keycode);
	}

	return false;
}

bool ImGuiLayer::OnWindowResizedEvent(WindowResizedEvent& e) {
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2(e.GetWidth(), e.GetHeight());
	io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);

	// FIXME: THIS SHOULD BE TEMPORARY
	glViewport(0, 0, e.GetWidth(), e.GetHeight());

	return false;
}
