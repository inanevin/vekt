
#define VEKT_IMPL
#define VEKT_VERTEX_BASIC_PCU
#define VEKT_VERTEX_TEXT_PU

// VEKT_VERTEX_BASIC_P
// VEKT_VERTEX_BASIC_PC
// VEKT_VERTEX_BASIC_PU
// VEKT_VERTEX_BASIC_PCU
// VEKT_VERTEX_TEXT_PU
// VEKT_VERTEX_TEXT_PCU
// VEKT_USER_DATA_SIZE
// VEKT_NO_STB_IMPL

#include "vekt.hpp"

void draw_basic(const vekt::builder::basic_draw_buffer& db) {}

void draw_text(const vekt::builder::text_draw_buffer& db) {}

int main()
{
	vekt::builder builder;

	builder.init({
		.widget_buffer_sz = 1024 * 1024,
	});
	vekt::widget* root = builder.allocate();
	builder.set_root(root);
	builder.add_input_layer(0, root);

	vekt::widget* child = builder.allocate();
	root->add_child(child);
	child->get_data_widget().flags = vekt::widget_flags::wf_pos_x_relative | vekt::widget_flags::wf_pos_y_relative | vekt::widget_flags::wf_size_x_relative | vekt::widget_flags::wf_size_y_relative;
	builder.build(vekt::vec2());

	bool	   exit		   = false;
	vekt::vec2 screen_size = vekt::vec2();

	builder.set_on_draw_basic(draw_basic);
	builder.set_on_draw_text(draw_text);

	while (!exit)
	{
		builder.build(screen_size);
		builder.flush();
	}

	return 0;
}
