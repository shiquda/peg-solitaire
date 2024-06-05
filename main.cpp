#include <graphics.h>

#include "core.h"
#include "graphic.h"


int main()
{
	while (1)
	{
		initgraph(WINDOW_WIDTH, WINDOW_HEIGHT);
		main_menu();
	}

	return 0;
}