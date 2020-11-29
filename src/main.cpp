#include "GUI.hpp"

int		main(int argc, char **argv)
{
	std::atomic<bool> isRunning;
	GUI gui;

	isRunning = true;
	if (gui.initGL() == false)
		isRunning = false;
	gui.initLines();
	while (isRunning)
	{
		gui.events(isRunning);
		gui.update();
		gui.render();
	}

    return 0;
}
