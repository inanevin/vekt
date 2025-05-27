
#define VEKT_IMPL
#include "vekt.hpp"

int main()
{

	vekt::builder builder;
	builder.init(1000);
	vekt::widget* root	= builder.allocate();
    builder.set_root(root);
    
	vekt::widget* child = builder.allocate();
	root->add_child(child);
	child->get_data_widget().flags = vekt::widget_flags::wf_pos_x_relative | vekt::widget_flags::wf_pos_y_relative | vekt::widget_flags::wf_size_x_relative | vekt::widget_flags::wf_size_y_relative;
    
	builder.build();

	return 0;
}
