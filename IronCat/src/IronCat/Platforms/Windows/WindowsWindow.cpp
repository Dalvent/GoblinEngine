#include "pch.h"
#include "WindowsWindow.h"

#include "IronCat/Events/AppEvent.h"
#include "IronCat/Events/KeyEvent.h"
#include "IronCat/Events/MouseEvent.h"

#include "IronCat/Platforms/GLFW/GLFWContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace IronCat
{
	WindowsWindow::WindowsWindow(const WindowProps& props)
		: u_input(new GLFWInput())
	{
		Init(props);
	}
	
	void WindowsWindow::Init(const WindowProps& windowProps)
	{
		_data.title = windowProps.title;
		_data.width = windowProps.width;
		_data.height = windowProps.height;

		IC_LOG_CORE_INFO("Creating window {0} ({1} {2})", 
			_data.title, _data.width, _data.height);

		if (GLFW_NOT_INITIALIZED)
		{
			int success = glfwInit();
			IC_LOG_CORE_ASSERT(success, "Could not intialize GLFW!");
			glfwSetErrorCallback([](int error, const char* description) 
			{
				IC_LOG_CORE_ERROR("GLFW ERROR ({0}): {1}", error, description);
			});
		}

		_glfwWindow = (glfwCreateWindow((int)_data.width, (int)_data.height, _data.title.c_str(), nullptr, nullptr));
		u_context.reset(new GLFWContext(_glfwWindow));
		u_context->Init();

		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		IC_LOG_CORE_ASSERT(status, "Failed to initialize GLAD!");

		glfwSetWindowUserPointer(_glfwWindow, &_data);
		SetVSync(true);

		glfwSetWindowSizeCallback(_glfwWindow, [](GLFWwindow* window, int width, int height)
		{
			auto& _data = *(WindowData*)glfwGetWindowUserPointer(window);
			_data.width = width;
			_data.height = height;

			auto event = WindowResizeEvent(width, height);
			_data.eventCallback(event);
		});

		glfwSetWindowCloseCallback(_glfwWindow, [](GLFWwindow* window)
		{
			auto& _data = *(WindowData*)glfwGetWindowUserPointer(window);
			auto event = WindowCloseEvent();
			_data.eventCallback(event);
		});

		glfwSetKeyCallback(_glfwWindow, [](GLFWwindow* window, KeyCode key, int scancode, int action, int mods)
		{
			auto& _data = *(WindowData*)glfwGetWindowUserPointer(window);
			
			KeyEvent* event;

			switch (action)
			{
			case GLFW_PRESS:
				event = &KeyPressedEvent(key, 0);
				break;
			case GLFW_RELEASE:
				event = &KeyReleasedEvent(key);
				break;
			case GLFW_REPEAT:
				event = &KeyPressedEvent(key, 1);
				break;
			default:
				return;
			}

			_data.eventCallback(*event);
		});

		glfwSetCharCallback(_glfwWindow, [](GLFWwindow* window, unsigned int keycode)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			auto event = KeyTypedEvent(keycode);

			data.eventCallback(event);
		});

		glfwSetMouseButtonCallback(_glfwWindow, [](GLFWwindow* window, MouseKeyCode button, int action, int mods)
		{
			auto& _data = *(WindowData*)glfwGetWindowUserPointer(window);
			MouseButtonEvent* event;

			switch (action)
			{
			case GLFW_PRESS:
				event = &MouseButtonPressedEvent(button);
				break;
			case GLFW_RELEASE:
				event = &MouseButtonReleasedEvent(button);
				break;
			default:
				break;
			}

			_data.eventCallback(*event);
		});

		glfwSetScrollCallback(_glfwWindow, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			auto& _data = *(WindowData*)glfwGetWindowUserPointer(window);
			auto event = MouseScrolledEvent(xOffset, yOffset);
			_data.eventCallback(event);
		});

		glfwSetCursorPosCallback(_glfwWindow, [](GLFWwindow* window, double xPos, double yPos)
		{
			auto& _data = *(WindowData*)glfwGetWindowUserPointer(window);
			auto event = MouseMovedEvent(xPos, yPos);
			_data.eventCallback(event);
		});
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdow();
	}
	
	void WindowsWindow::Shutdow()
	{
		glfwPollEvents();
	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		u_context->SwapBuffer();
	}
	
	double WindowsWindow::GetTime()
	{
		return glfwGetTime();
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		if (enabled)
		{
			glfwSwapInterval(1);
		}
		else
		{
			glfwSwapInterval(0);
		}

		_data.vSync = enabled;
	}
}