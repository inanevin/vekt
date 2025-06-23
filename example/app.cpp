#include "app.hpp"

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
#define VEKT_IMPL
#define VEKT_VERTEX_BASIC_PCU
#define VEKT_VERTEX_TEXT_PCU
#include "vekt.hpp"

app* app::s_app = nullptr;

class theme
{
public:
	static constexpr vekt::vec4 color_dark0		  = {12.0f / 255.0f, 12.0f / 255.0f, 12.0f / 255.0f, 1.0f};
	static constexpr vekt::vec4 color_dark1		  = {24.0f / 255.0f, 24.0f / 255.0f, 24.0f / 255.0f, 1.0f};
	static constexpr vekt::vec4 color_dark2		  = {32.0f / 255.0f, 32.0f / 255.0f, 32.0f / 255.0f, 1.0f};
	static constexpr vekt::vec4 color_dark3		  = {42.0f / 255.0f, 42.0f / 255.0f, 42.0f / 255.0f, 1.0f};
	static constexpr vekt::vec4 color_light0	  = {200.0f / 255.0f, 200.0f / 255.0f, 200.0f / 255.0f, 1.0f};
	static constexpr float		item_height		  = 24.0f;
	static constexpr float		indent_vertical	  = 8.0f;
	static constexpr float		margin_horizontal = 20.0f;
	static constexpr float		border_size		  = 6.0f;
	static constexpr float		outline_thickness = 2.0f;
};

void app::init()
{
	vekt::config.on_log = [](vekt::log_verbosity verb, const char* log...) {
		char	buffer[1024];
		va_list args;
		va_start(args, log);
		int written_chars = vsnprintf(buffer, sizeof(buffer), log, args);
		va_end(args);
		if (written_chars >= sizeof(buffer) || written_chars < 0) { std::cerr << "Log message truncated or error: "; }
		std::cerr << buffer << std::endl;
	};

	s_app		  = this;
	_vekt_builder = new vekt::builder();

	_vekt_builder->init({
		.widget_buffer_sz = 1024 * 1024,
		.vertex_buffer_sz = 1024 * 1024,
		.index_buffer_sz  = 1024 * 1024,
		.buffer_count	  = 50,
	});

	_vekt_root								 = _vekt_builder->allocate();
	_vekt_root->get_data_widget().debug_name = "Root";
	_vekt_builder->set_root(_vekt_root);
	_vekt_builder->add_input_layer(0, _vekt_root);

	_screen_width  = 1920;
	_screen_height = 1080;
	_window.init(_screen_width, _screen_height);
	_backend.init(*_vekt_builder);

	vekt::font_manager::get().init();
	_vekt_font = vekt::font_manager::get().load_font("Roboto-Regular.ttf", 18, 0, 128);

	create_top_pane();
	create_bottom_pane();
}

void app::uninit()
{
	// vekt::font_manager::get().unload_font(_vekt_font);
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

void app::create_top_pane()
{
	_vekt_root->get_data_widget().child_positioning = vekt::child_positioning::column;
	_vekt_root->get_data_widget().spacing			= 0.0f;

	vekt::widget* top_pane = _vekt_builder->allocate();
	{
		top_pane->set_pos_x(0.0f, vekt::helper_pos_type::relative, vekt::helper_anchor_type::start);
		top_pane->set_pos_y(0.0f, vekt::helper_pos_type::relative, vekt::helper_anchor_type::start);
		top_pane->set_width(1.0f, vekt::helper_size_type::relative);
		top_pane->set_height(0.35f, vekt::helper_size_type::relative);
		top_pane->get_data_widget().debug_name = "Top Pane";

		vekt::gfx_filled_rect& left_pane_rect = top_pane->set_gfx_type_filled_rect();
		left_pane_rect.color_start = left_pane_rect.color_end = theme::color_dark1;
		_vekt_root->add_child(top_pane);
	}

	vekt::widget* divider = _vekt_builder->widget_horizontal_divider(theme::border_size, theme::color_dark0);
	_vekt_root->add_child(divider);
}

void app::create_bottom_pane()
{
	vekt::widget* bottom = _vekt_builder->allocate();
	{
		bottom->set_pos_x(0.0f, vekt::helper_pos_type::relative);
		bottom->set_width(1.0f, vekt::helper_size_type::relative);
		bottom->set_height(0.0f, vekt::helper_size_type::fill);
		bottom->get_data_widget().child_positioning = vekt::child_positioning::row;
		bottom->get_data_widget().debug_name		= "Bottom";
		_vekt_root->add_child(bottom);
	}

	auto create_pane = [&](bool create_div, const VEKT_STRING& name) -> vekt::widget* {
		{
			vekt::widget* pane = _vekt_builder->allocate();
			pane->set_pos_y(0.0f, vekt::helper_pos_type::relative);
			pane->set_height(1.0f, vekt::helper_size_type::relative);
			pane->set_width(0.0f, vekt::helper_size_type::fill);
			pane->get_data_widget().margins.left	  = theme::margin_horizontal;
			pane->get_data_widget().margins.right	  = theme::margin_horizontal;
			pane->get_data_widget().margins.top		  = theme::margin_horizontal;
			pane->get_data_widget().margins.bottom	  = theme::margin_horizontal;
			pane->get_data_widget().spacing			  = theme::margin_horizontal;
			pane->get_data_widget().child_positioning = vekt::child_positioning::column;
			pane->get_data_widget().debug_name		  = name;

			vekt::gfx_filled_rect& r = pane->get_gfx_filled_rect();
			r.color_start = r.color_end = theme::color_dark1;
			bottom->add_child(pane);

			if (create_div)
			{
				vekt::widget* divider = _vekt_builder->widget_vertical_divider(theme::border_size, theme::color_dark0);
				bottom->add_child(divider);
			}

			return pane;
		}
	};

	vekt::widget* bottom_left = create_pane(true, "BottomLeft");
	{
		vekt::widget* button0							 = _vekt_builder->widget_button(theme::item_height, _vekt_font, "Button", theme::color_dark2, theme::color_light0);
		button0->get_gfx_filled_rect().outline_thickness = theme::outline_thickness;
		button0->get_gfx_filled_rect().outline_color	 = theme::color_dark3;
		button0->get_gfx_filled_rect().hovered_color	 = theme::color_dark3;
		button0->get_gfx_filled_rect().pressed_color	 = theme::color_dark1;
		bottom_left->add_child(button0);

		vekt::widget* check							   = _vekt_builder->widget_checkbox(theme::item_height, &_backend.get_sdf_material(), theme::color_dark2, theme::color_light0);
		check->get_gfx_filled_rect().outline_thickness = theme::outline_thickness;
		check->get_gfx_filled_rect().outline_color	   = theme::color_dark3;
		bottom_left->add_child(check);
	}

	vekt::widget* bottom_center = create_pane(true, "BottomCenter");
	vekt::widget* bottom_right	= create_pane(false, "BottomRight");
}
