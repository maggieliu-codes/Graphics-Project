// Functionality that we created
#include "SDLGraphicsProgram.hpp"

#include <iostream>

int main(int argc, char **argv)
{

	std::cout << "Please remember:\n For this starter code you only need to work in the shader.\n"
				 "That means once you compile your .cpp files, you need only run your ./prog once,\n"
				 "because every time your program runs it will recompile the shaders.\n"
				 "So save yourself some time :)\n\n"
			  << std::endl;

	// Screen dimensions
	const int SCREEN_WIDTH = 1280;
	const int SCREEN_HEIGHT = 720;

	// Create an instance of SDLGraphicsProgram
	SDLGraphicsProgram mySDLGraphicsProgram(SCREEN_WIDTH, SCREEN_HEIGHT);

	// Determine the OBJ file path
	std::string objFilePath;
	if (argc > 1)
	{
		objFilePath = argv[1];
	}
	else
	{
		// Default OBJ file if none is provided
		objFilePath = "../common/objects/bunny_centered.obj";
	}

	// Initialize objects with the given (or default) OBJ file
	mySDLGraphicsProgram.InitObjects(objFilePath);

	// Run our program loop
	mySDLGraphicsProgram.Loop();

	return 0;
}
