#include "artsypch.hpp"

#include "imgui/imgui.h"

// #define IMGUI_IMPL_API
#include "examples/imgui_impl_glfw.h"
#include "examples/imgui_impl_opengl3.h"

#include "Core.hpp"
#include "Application/Application.hpp"

#include "ImGuiLayer.hpp"

// TODO: TEMPORARY
#include <GL/glew.h>
#include <GLFW/glfw3.h> // keycodes

ImGuiLayer::ImGuiLayer() : Layer("ImGui Layer"), app(Application::get()) {}

ImGuiLayer::~ImGuiLayer() {}

void ImGuiLayer::OnAttach() {
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void) io;
	io.ConfigFlags |=
		ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
	// io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable
	// Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking
	// io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable
	// Multi-Viewport / Platform Windows
	// io.ConfigViewportsNoAutoMerge = true;
	// io.ConfigViewportsNoTaskBarIcon = true;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform
	// windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	Application& app = Application::get();
	GLFWwindow* window =
		static_cast<GLFWwindow*>(app.getWindow().getNativeWindow());

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 410");
}

void ImGuiLayer::OnDetach() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void ImGuiLayer::OnImGuiRender() {
	// static bool show = true;
	// ImGui::ShowDemoWindow(&show);
}

void ImGuiLayer::begin() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void ImGuiLayer::end() {
	ImGuiIO& io = ImGui::GetIO();
	Application& app = Application::get();
	io.DisplaySize =
		ImVec2(app.getWindow().GetWidth(), app.getWindow().GetHeight());

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		GLFWwindow* currentContexBackup = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(currentContexBackup);
	}
}
