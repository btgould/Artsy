#include "artsypch.hpp"
#include "glutil.hpp"
#include "LinuxWindow.hpp"

#include "Events/ApplicationEvent.hpp"
#include "Events/KeyEvent.hpp"
#include "Events/MouseEvent.hpp"

#include "imgui/imgui.h"
#include "stb_image/stb_image.h"

static bool s_GLFWInitialized = false;

static void GLFWErrorCallback(int errorCode, const char* description) {
	std::cout << "GLFW Error (" << errorCode << "): " << description << std::endl;
}

Window* Window::Create(const WindowProps& props) {
	return new LinuxWindow(props);
}

LinuxWindow::LinuxWindow(const WindowProps& props) {
	Init(props);
}

LinuxWindow::~LinuxWindow() {
	Shutdown();
	glfwTerminate();
}

void LinuxWindow::Init(const WindowProps& props) {
	// save window properties
	m_Data.Title = props.Title;
	m_Data.Width = props.Width;
	m_Data.Height = props.Height;
	std::cout << "Creating window " << props.Title << "(" << props.Width << ", " << props.Height
			  << ")" << std::endl;

	// check if GLFW needs to be initialized
	if (!s_GLFWInitialized) {
		int success = glfwInit();
		ASSERT(success);

		glfwSetErrorCallback(GLFWErrorCallback);

		s_GLFWInitialized = true;
	}

	// create new GLFW window
	m_Window = glfwCreateWindow((int) props.Width, (int) props.Height, m_Data.Title.c_str(),
								nullptr, nullptr);
	if (!m_Window) {
		GL_CALL(glfwTerminate());
		std::cout << "[ERROR]: Window could not be created" << std::endl;
	}

	// set window icon
	GLFWimage images[1];
	images[0].pixels = stbi_load("dog.png", &images[0].width, &images[0].height, 0,
								 4); // rgba channels
	glfwSetWindowIcon(m_Window, 1, images);
	stbi_image_free(images[0].pixels);

	GL_CALL(glfwMakeContextCurrent(m_Window));
	GL_CALL(glfwSetWindowUserPointer(m_Window,
									 &m_Data)); // associates our window data w/ GLFW
	SetVSync(true);

	// enable blending transparent values
	GL_CALL(glEnable(GL_BLEND));
	GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	// set GLFW callbacks
	// 1. Create Nutella event from GLFW data, 2. pass to m_Data's callback
	glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
		WindowData& data = *(WindowData*) glfwGetWindowUserPointer(window);
		WindowResizedEvent event(width, height);

		data.Width = width;
		data.Height = height;
		data.EventCallback(event);
	});

	glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
		WindowData& data = *(WindowData*) glfwGetWindowUserPointer(window);
		WindowClosedEvent event;

		data.EventCallback(event);
	});

	glfwSetKeyCallback(m_Window,
					   [](GLFWwindow* window, int keycode, int scancode, int action, int mods) {
						   WindowData& data = *(WindowData*) glfwGetWindowUserPointer(window);

						   switch (action) {
							   {
							   case GLFW_PRESS:
								   KeyPressedEvent event(keycode, 0, mods);
								   data.EventCallback(event);
								   break;
							   }
							   {
							   case GLFW_RELEASE:
								   KeyReleasedEvent event(keycode, mods);
								   data.EventCallback(event);
								   break;
							   }
							   {
							   case GLFW_REPEAT:
								   KeyPressedEvent event(keycode, 1, mods);
								   data.EventCallback(event);
								   break;
							   }
						   default:
							   std::cout << "Key action type unrecognized" << std::endl;
							   break;
						   }
					   });

	glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode) {
		WindowData& data = *(WindowData*) glfwGetWindowUserPointer(window);
		KeyTypedEvent event(keycode);
		data.EventCallback(event);
	});

	glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) {
		WindowData& data = *(WindowData*) glfwGetWindowUserPointer(window);

		switch (action) {
			{
			case GLFW_PRESS:
				MouseButtonPressedEvent event(button);
				data.EventCallback(event);
				break;
			}
			{
			case GLFW_RELEASE:
				MouseButtonReleasedEvent event(button);
				data.EventCallback(event);
				break;
			}
		default:
			std::cout << "Mouse action type unrecognized" << std::endl;
			break;
		}
	});

	glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos) {
		WindowData& data = *(WindowData*) glfwGetWindowUserPointer(window);

		MouseMovedEvent event((float) xPos, (float) yPos);
		data.EventCallback(event);
	});

	glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset) {
		WindowData& data = *(WindowData*) glfwGetWindowUserPointer(window);

		MouseScrolledEvent event((float) xOffset, (float) yOffset);
		data.EventCallback(event);
	});
}

void LinuxWindow::InitGLFW() {}

void LinuxWindow::Shutdown() {
	glfwDestroyWindow(m_Window);
}

void LinuxWindow::OnUpdate() {
	glfwPollEvents();
	glfwSwapBuffers(m_Window);
}

void LinuxWindow::SetVSync(bool enabled) {
	if (enabled)
		glfwSwapInterval(1);
	else
		glfwSwapInterval(0);

	m_Data.VSync = enabled;
}

bool LinuxWindow::IsVSync() const {
	return m_Data.VSync;
}
