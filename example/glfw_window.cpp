#include "glfw_window.hpp"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "app.hpp"
#include <iostream>

static void GLFWErrorCallback(int error, const char* desc) { std::cerr << "GLFW Error: " << error << " Description: " << desc << std::endl; }

bool glfw_window::init(int width, int height)
{
	int init = glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_DECORATED, true);
	glfwWindowHint(GLFW_RESIZABLE, true);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Build window
	auto*			   primaryMonitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode			  = glfwGetVideoMode(primaryMonitor);

	_window = (glfwCreateWindow(width, height, "vekt example", NULL, NULL));
	// glfwGetMonitorContentScale(primaryMonitor, &m_frameBufferScaleX, &m_frameBufferScaleY);

	if (!_window)
	{
		// Assert window creation.
		std::cerr << "GLFW window failed to initialize!" << std::endl;
		return false;
	}
	// Set error callback
	glfwSetErrorCallback(GLFWErrorCallback);

	// Set context.
	glfwMakeContextCurrent(_window);

	// Load glad
	bool loaded = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	if (!loaded)
	{
		std::cerr << "GLFW window GLAD Loader failed!" << std::endl;
		return false;
	}

	// Update OpenGL about the window data.
	glViewport(0, 0, width, height);

	glfwSwapInterval(0);

	// set user pointer for callbacks.
	glfwSetWindowUserPointer(_window, this);

	auto window_resize = [](GLFWwindow* w, int wi, int he) { app::get()->on_resize(wi, he); };

	auto window_close = [](GLFWwindow* w) { app::get()->set_should_quit(true); };

	auto window_key = [](GLFWwindow* w, int key, int scancode, int action, int modes) {
		if (action == GLFW_PRESS) {}
		app::get()->on_key(key, scancode, action);
	};

	auto window_button = [](GLFWwindow* w, int button, int action, int modes) { app::get()->on_mouse(button, action); };

	auto window_scroll = [](GLFWwindow* w, double xOff, double yOff) { app::get()->on_mouse_wheel(yOff); };

	auto window_cursor = [](GLFWwindow* w, double xPos, double yPos) { app::get()->on_mouse_cursor(xPos, yPos); };

	auto window_focus = [](GLFWwindow* w, int f) {

	};

	// Register window callbacks.
	glfwSetFramebufferSizeCallback(_window, window_resize);
	glfwSetWindowCloseCallback(_window, window_close);
	glfwSetKeyCallback(_window, window_key);
	glfwSetMouseButtonCallback(_window, window_button);
	glfwSetScrollCallback(_window, window_scroll);
	glfwSetCursorPosCallback(_window, window_cursor);

	return true;
}

void glfw_window::poll() { glfwPollEvents(); }


void glfw_window::swap_buffers() { glfwSwapBuffers(_window); }

void  glfw_window::uninit() { glfwTerminate(); }
float glfw_window::get_time() const { return static_cast<float>(glfwGetTime()); }
