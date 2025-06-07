#pragma once

struct GLFWwindow;

class glfw_window
{
public:
	bool  init(int width, int height);
	void  poll();
	void  swap_buffers();
	void  uninit();
	float get_time() const;

private:
	GLFWwindow* _window = nullptr;
};