
#define VEKT_IMPL
#include "vekt.hpp"

int main()
{

	vekt::builder builder;

	/*
		box
		row
		column
		scroll stuff
		text
		icon
	*/

	/*
		text & icon define their own sizes.
		everything else can have relative sizes.

		positions are always relative OR absolute.
	*/

	builder.init(100);
	vekt::widget* root	= builder.allocate();
	vekt::widget* child = builder.allocate();
	root->add_child(child);
	child->get_data_widget().flags = vekt::widget_flags::wf_pos_x_relative | vekt::widget_flags::wf_pos_y_relative | vekt::widget_flags::wf_size_x_relative | vekt::widget_flags::wf_size_y_relative;

	builder.build();

	return 0;
}