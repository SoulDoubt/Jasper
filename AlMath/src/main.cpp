#include <Jasper\GLWindow.h>
#include <iostream>



using namespace Jasper;

int main(int argc, char** argv) {

	std::unique_ptr<GLWindow> window = std::make_unique<GLWindow>(1024, 768, "Jasper");
	window->Init();
	window->InitializeScene();
	window->RunLoop();	
}

void printGLInfo() {
	printf("Using OpenGL Version: %s\n", glGetString(GL_VERSION));
}
