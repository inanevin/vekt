#include "app.hpp"
#define VEKT_IMPL
#define VEKT_VERTEX_BASIC_PCU
#define VEKT_VERTEX_TEXT_PCU
#include "vekt.hpp"
#include "glfw_window.hpp"
#include "gl_backend.hpp"
#include <iostream>
#include <glfw/glfw3.h>
#include <cstdarg> // For va_list, va_start, va_end, va_arg
#include <cstdio>  // For vsnprintf

// VEKT_VERTEX_BASIC_P
// VEKT_VERTEX_BASIC_PC
// VEKT_VERTEX_BASIC_PU
// VEKT_VERTEX_BASIC_PCU
// VEKT_VERTEX_TEXT_PU
// VEKT_VERTEX_TEXT_PCU
// VEKT_USER_DATA_SIZE
// VEKT_NO_STB_IMPL

app* app::s_app = nullptr;

/*
	VERTEX & INDEX BUFFERS RE ALLOCATING
	DRAW PASS CLIP OPTIMIZE
	TEXT SIZE CALC EVERY FRAME NO NO

*/
vekt::widget* aq = nullptr;

void app::init()
{
	vekt::config.on_log = [](vekt::log_verbosity verb, const char* log...) {
		char buffer[1024];

		va_list args;

		va_start(args, log);

		int written_chars = vsnprintf(buffer, sizeof(buffer), log, args);

		va_end(args);

		// Handle potential truncation (optional but good practice)
		if (written_chars >= sizeof(buffer) || written_chars < 0) { std::cerr << "Log message truncated or error: "; }

		// Output the formatted string to std::cerr
		std::cerr << buffer << std::endl;
	};
	vekt::font_manager::get().init();

	s_app		  = this;
	_vekt_builder = new vekt::builder();

	_vekt_builder->init({
		.widget_buffer_sz = 1024 * 1024,
	});

	_vekt_root = _vekt_builder->allocate();
	_vekt_builder->set_root(_vekt_root);
	_vekt_builder->add_input_layer(0, _vekt_root);

	_screen_width  = 1920;
	_screen_height = 1080;
	_window.init(_screen_width, _screen_height);
	_backend.init(*_vekt_builder);

	_vekt_font = vekt::font_manager::get().load_font("NotoSans-Regular.ttf", 32);

	create_some_widgets();
}

void app::uninit()
{
	vekt::font_manager::get().unload_font(_vekt_font);
	vekt::font_manager::get().uninit();

	_backend.uninit();
	_window.uninit();

	_vekt_builder->deallocate(_vekt_root);
	_vekt_builder->uninit();
	delete _vekt_builder;
}

void app::update()
{
	// if (aq)
	// {
	// 	static float a = 0.0f;
	// 	a += 0.00001f;
	// 	aq->set_pos_x(a, vekt::helper_pos_type::relative);
	// }
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

	if (type == vekt::input_event_type::pressed && key == GLFW_KEY_T) _backend.set_wireframe_mode(!_backend.get_wireframe_mode());
	if (type == vekt::input_event_type::pressed && key == GLFW_KEY_Z)
	{
		_backend.set_zoom(1.0f);
		_backend.set_debug_offset(0.0f, 0);
		_backend.set_debug_offset(0.0f, 1);
	}

	if (type != vekt::input_event_type::released && key == GLFW_KEY_D) _backend.set_debug_offset(_backend.get_debug_offset(0) + 12.1f, 0);
	if (type != vekt::input_event_type::released && key == GLFW_KEY_A) _backend.set_debug_offset(_backend.get_debug_offset(0) - 12.1f, 0);
	if (type != vekt::input_event_type::released && key == GLFW_KEY_W) _backend.set_debug_offset(_backend.get_debug_offset(1) - 12.1f, 1);
	if (type != vekt::input_event_type::released && key == GLFW_KEY_S) _backend.set_debug_offset(_backend.get_debug_offset(1) + 12.1f, 1);

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
	_backend.set_zoom(_backend.get_zoom() - y * 0.07f);

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

void app::create_some_widgets()
{
	vekt::widget* some_bg = _vekt_builder->allocate();
	{
		some_bg->set_pos_x(0.5f, vekt::helper_pos_type::relative, vekt::helper_anchor_type::center);
		some_bg->set_pos_y(0.5f, vekt::helper_pos_type::relative, vekt::helper_anchor_type::center);
		some_bg->set_width(0.5f, vekt::helper_size_type::relative);
		some_bg->set_height(0.5f, vekt::helper_size_type::relative);
		// some_bg->get_data_widget().child_positioning = vekt::child_positioning::column;
		some_bg->get_data_widget().spacing	   = 10;
		some_bg->get_data_widget().margins.top = 10;
		some_bg->get_data_widget().debug_name  = "GreenBG";

		vekt::gfx_filled_rect& rect = some_bg->set_gfx_type_filled_rect();

		rect.color_start = vekt::vec4(0.2f, 0.2f, 0.2f, 1.0f);
		rect.color_end	 = vekt::vec4(0.2f, 0.7f, 0.2f, 1.0f);
		// rect.clip_children			= true;
		rect.rounding = 36.0f;
		//	rect.aa_thickness = 2;
		rect.outline_color	   = vekt::vec4(1, 1, 1, 1);
		rect.outline_thickness = 10;
		_vekt_root->add_child(some_bg);
		aq = some_bg;
	}

	vekt::widget* child = _vekt_builder->allocate();
	{
		child->set_pos_x(0.5f, vekt::helper_pos_type::relative, vekt::helper_anchor_type::center);
		child->set_pos_y(0.5f, vekt::helper_pos_type::relative, vekt::helper_anchor_type::center);
		child->set_width(1.0f, vekt::helper_size_type::relative);
		child->set_height(0.1f, vekt::helper_size_type::relative);
		child->get_data_widget().debug_name = "Child";

		vekt::gfx_filled_rect& rect = child->set_gfx_type_filled_rect();
		rect.color_start			= vekt::vec4(0.2f, 0.2f, 0.2f, 1.0f);
		rect.color_end				= vekt::vec4(0.2f, 0.2f, 0.2f, 1.0f);
		// rect.rounding		 = 12.0f;
		// rect.clip_children = true;
		//   rect.segments		 = 1;
		//   rect.aa_thickness			 = 2;
		some_bg->add_child(child);
	}

	vekt::widget* text = _vekt_builder->allocate();
	{
		text->set_pos_x(0.5f, vekt::helper_pos_type::relative, vekt::helper_anchor_type::center);
		text->set_pos_y(0.5f, vekt::helper_pos_type::relative, vekt::helper_anchor_type::center);
		text->get_data_widget().debug_name = "Text";

		text->set_width(1.5f, vekt::helper_size_type::relative);
		text->set_height(0.1f, vekt::helper_size_type::relative);
		vekt::gfx_text& txt = text->set_gfx_type_text();
		txt.text			= "A";
		txt.target_font		= _vekt_font;
		txt.color_start = txt.color_end = vekt::vec4(1, 1, 1, 1);
		child->add_child(text);
	}
	return;

	for (int a = 0; a < 10; a++)
	{
		vekt::widget* child2 = _vekt_builder->allocate();
		{
			child2->set_pos_x(0.5f, vekt::helper_pos_type::relative, vekt::helper_anchor_type::center);
			// child2->set_pos_y(0.0f, vekt::helper_pos_type::relative, vekt::helper_anchor_type::center);
			child2->set_width(5.5f, vekt::helper_size_type::relative);
			child2->set_height(0.1f, vekt::helper_size_type::relative);
			vekt::gfx_filled_rect& rect = child2->set_gfx_type_filled_rect();
			rect.color_start			= vekt::vec4(0.2f, 0.2f, 0.2f, 1.0f);
			rect.color_end				= vekt::vec4(0.2f, 0.2f, 0.2f, 1.0f);
			// rect.rounding		 = 12.0f;
			//	rect.clip_children = true;
			//   rect.segments		 = 1;
			//   rect.thickness				 = 24;
			//   rect.aa_thickness			 = 2;

			some_bg->add_child(child2);
		}
	}

	//	vekt::widget* child2 = _vekt_builder->allocate();
	//	{
	//		child2->set_pos_x(0.0f, vekt::helper_pos_type::relative, vekt::helper_anchor_type::start);
	//		child2->set_pos_y(1.0f, vekt::helper_pos_type::relative, vekt::helper_anchor_type::end);
	//		child2->set_width(0.5f, vekt::helper_size_type::relative);
	//		child2->set_height(1.6f, vekt::helper_size_type::relative);
	//		child2->get_gfx_data().type = vekt::gfx_type::text;
	//
	//		vekt::gfx_text& text = child2->get_gfx_text();
	//		text.text			 = "This is el sample test.";
	//		text.target_font	 = _vekt_font;
	//		text.color_start	 = vekt::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	//		text.color_end		 = vekt::vec4(1.0f, 1.0f, 0.0f, 1.0f);
	//		text.color_direction = vekt::direction::vertical;
	//
	//		child->add_child(child2);
	//	}
}
