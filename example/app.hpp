#pragma once
#include "glfw_window.hpp"
#include "gl_backend.hpp"

namespace vekt
{
	class builder;
	class widget;
}
class app
{
public:
	void init();
	void uninit();
	void update();
	void on_resize(unsigned int w, unsigned int h);
	void on_key(int key, int scancode, int action);
	void on_mouse(int button, int action);
	void on_mouse_wheel(float y);
	void on_mouse_cursor(float x, float y);

	inline bool		   should_quit() const { return _should_quit; }
	inline void		   set_should_quit(bool q) { _should_quit = q; }
	inline static app* get() { return s_app; }

private:
	static app*	   s_app;
	vekt::widget*  _vekt_root	 = nullptr;
	vekt::builder* _vekt_builder = nullptr;
	glfw_window	   _window;
	gl_backend	   _backend;
	bool		   _should_quit	  = false;
	unsigned int   _screen_width  = 0;
	unsigned int   _screen_height = 0;
	float		   _mouse_x		  = 0.0f;
	float		   _mouse_y		  = 0.0f;
};