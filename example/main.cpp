#include "app.hpp"

int main()
{
	app demo_app;
	demo_app.init();
	while (!demo_app.should_quit())
		demo_app.update();
	demo_app.uninit();

	return 0;
}
