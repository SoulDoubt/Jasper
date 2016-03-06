#ifndef _GL_WINDOW_H_
#define _GL_WINDOW_H_

#define GLEW_STATIC
#include <gl\glew.h>
#include <GLFW\glfw3.h>
#include <string>
#include <memory>
#include "Common.h"


namespace Jasper {

class Scene;

class GLWindow
{
public:
	GLWindow(int h, int w, std::string title);
	~GLWindow();

	bool Init();

	int Height, Width;
	std::string Title;

	GLFWwindow* m_window;

	void RunLoop();

	void InitializeScene();

	void SetupGL();

	void printGLInfo() {
		printf("Vendor: %s\n", glGetString(GL_VENDOR));
		printf("Renderer: %s\n", glGetString(GL_RENDERER));
		printf("Version: %s\n", glGetString(GL_VERSION));
		printf("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	}

	std::unique_ptr<Scene> m_scene;

private:
	NON_COPYABLE(GLWindow);
	double m_previousFrameTime = 0;

};
}
#endif // _GL_WINDOW_H_

