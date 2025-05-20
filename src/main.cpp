
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

	vekt::widget* w = builder.allocate();

	return 0;
}