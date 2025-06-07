#include "app.hpp"
#define VEKT_IMPL
#define VEKT_VERTEX_BASIC_PCU
#define VEKT_VERTEX_TEXT_PCU
#include "vekt.hpp"
#include "glfw_window.hpp"
#include "gl_backend.hpp"
#include <iostream>

// VEKT_VERTEX_BASIC_P
// VEKT_VERTEX_BASIC_PC
// VEKT_VERTEX_BASIC_PU
// VEKT_VERTEX_BASIC_PCU
// VEKT_VERTEX_TEXT_PU
// VEKT_VERTEX_TEXT_PCU
// VEKT_USER_DATA_SIZE
// VEKT_NO_STB_IMPL

app* app::s_app = nullptr;

void app::init()
{
	vekt::config.on_log = [](vekt::log_verbosity verb, const char* err...) { std::cerr << err << std::endl; };
	vekt::font_manager::get().init();

	s_app		  = this;
	_vekt_builder = new vekt::builder();

	_vekt_builder->init({
		.widget_buffer_sz = 1024 * 1024,
	});

	_vekt_root = _vekt_builder->allocate();
	_vekt_builder->set_root(_vekt_root);
	_vekt_builder->add_input_layer(0, _vekt_root);

	_window.init(800, 600);
	_backend.init(*_vekt_builder);
}

void app::uninit()
{
	vekt::font_manager::get().uninit();

	_backend.uninit();
	_window.uninit();

	_vekt_builder->deallocate(_vekt_root);
	_vekt_builder->uninit();
	delete _vekt_builder;
}

void app::update()
{
	_window.poll();

	const vekt::vec2 screen_size = {static_cast<float>(_screen_width), static_cast<float>(_screen_height)};

	_backend.set_display(_screen_width, _screen_height);
	_backend.start_frame();

	_vekt_builder->build(screen_size);
	_vekt_builder->flush();

	_backend.end_frame();
	_window.swap_buffers();
}

void app::on_resize(unsigned int w, unsigned int h)
{
	_screen_width  = w;
	_screen_height = h;
}

void app::on_key(int key, int scancode, int action)
{
	vekt::input_event_type type = {};
	if (action == 0)
		type = vekt::input_event_type::released;
	else if (action == 1)
		type = vekt::input_event_type::pressed;
	else
		type = vekt::input_event_type::repeated;

	_vekt_builder->on_key_event({
		.type	   = type,
		.key	   = key,
		.scan_code = scancode,
	});
}

void app::on_mouse(int button, int action)
{
	vekt::input_event_type type = {};
	if (action == 0)
		type = vekt::input_event_type::released;
	else if (action == 1)
		type = vekt::input_event_type::pressed;
	else
		type = vekt::input_event_type::repeated;

	_vekt_builder->on_mouse_event({
		.type	= type,
		.button = button,
		.x		= static_cast<unsigned int>(_mouse_x),
		.y		= static_cast<unsigned int>(_mouse_y),
	});
}

void app::on_mouse_wheel(float y)
{
	_vekt_builder->on_mouse_wheel_event({
		.amount = y,
	});
}

void app::on_mouse_cursor(float x, float y)
{
	_mouse_x = x;
	_mouse_y = y;
	_vekt_builder->on_mouse_move({x, y});
}
