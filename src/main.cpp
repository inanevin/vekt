
#define VEKT_IMPL
#include "vekt.hpp"

int main()
{
	struct test
	{
		vekt::pod_vector<int> a;
	};

	vekt::pod_vector<test> huh;

	test deneme;
	deneme.a.push_back(5);
	deneme.a.push_back(12);
	deneme.a.push_back(18);

	huh.push_back(deneme);

	test deneme2;
	deneme2.a.push_back(1);
	deneme2.a.push_back(2);
	deneme2.a.push_back(3);

	huh.push_back(deneme2);
	huh.push_back(deneme2);

	for (const test& x : huh)
	{
		for (int i : x.a)
		{
			int a = 5;
		}
	}

	huh.clear();

	vekt::builder builder;
	builder.init({
		.widget_buffer_sz = 1024 * 1024,
	});
	vekt::widget* root = builder.allocate();
	builder.set_root(root);

	vekt::widget* child = builder.allocate();
	root->add_child(child);
	child->get_data_widget().flags = vekt::widget_flags::wf_pos_x_relative | vekt::widget_flags::wf_pos_y_relative | vekt::widget_flags::wf_size_x_relative | vekt::widget_flags::wf_size_y_relative;

	builder.build();

	return 0;
}
