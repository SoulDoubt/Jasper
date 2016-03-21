#include <iostream>
#include <Jasper\GLWindow.h>
#include <Jasper\GLError.h>
#include <Jasper\Scene.h>

namespace Jasper {

using namespace std;

bool MOVING_FORWARD = 0;
bool MOVING_BACKWARD = 0;
bool STRAFING_RIGHT = 0;
bool STRAFING_LEFT = 0;
bool ROTATING_RIGHT = 0;
bool ROTATING_LEFT = 0;
bool ROTATING_UP = 0;
bool ROTATING_DOWN = 0;

void ProcessInput(GLFWwindow* window, Scene* scene, float deltaTime);

void framebuffer_resize_callback(GLFWwindow* window, int width, int height);

GLWindow::GLWindow(int w, int h, string title)
{
	Height = h;
	Width = w;
	Title = title;
}

GLWindow::~GLWindow()
{
}

bool GLWindow::Init() {

	if (!glfwInit()) {
		return false;
	}

	m_window = glfwCreateWindow(Width, Height, Title.c_str(), nullptr, nullptr);

	glViewport(0, 0, Width, Height);

	if (!m_window) {
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(m_window);

	GLenum glewStatus = glewInit();
	if (glewStatus != GLEW_OK) {
		return false;
	}

	glfwSetFramebufferSizeCallback(m_window, framebuffer_resize_callback);

	printGLInfo();

	SetupGL();

	return true;
}

void GLWindow::RunLoop() {

	while (!glfwWindowShouldClose(m_window)) {

		static double lastFrameTime;
		double time = glfwGetTime();
		double deltaTime = time - lastFrameTime;
		lastFrameTime = time;

		// DO ALL THE THINGS HERE!!:)
		ProcessInput(m_window, m_scene.get(), deltaTime);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		m_scene->Update(deltaTime);		
		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}
	m_scene->Destroy();
	glfwTerminate();
}

void GLWindow::InitializeScene()
{
	m_scene = make_unique<Scene>(Width, Height);
	m_scene->Awake();
	glfwSetWindowUserPointer(m_window, m_scene.get());
}

void GLWindow::SetupGL()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_TEXTURE_CUBE_MAP);
	glEnable(GL_TEXTURE_3D);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	//glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glfwWindowHint(GLFW_SAMPLES, 4);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnable(GL_MULTISAMPLE);
	GLERRORCHECK;

}


void ProcessInput(GLFWwindow* window, Scene* scene, float deltaTime) {

	glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, 1);

	static double previousMouseX = 0;
	static double previousMouseY = 0;

	float speed = 7.5f;
	float rotSpeed = 120.0f;
	float moveBy = speed * deltaTime;
	float rotBy = rotSpeed * deltaTime;

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		MOVING_FORWARD = true;
	}
	else {
		MOVING_FORWARD = false;
	}

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		MOVING_BACKWARD = true;
	}
	else {
		MOVING_BACKWARD = false;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		STRAFING_LEFT = true;
	}
	else {
		STRAFING_LEFT = false;
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		STRAFING_RIGHT = true;
	}
	else {
		STRAFING_RIGHT = false;
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		ROTATING_LEFT = true;
	}
	else {
		ROTATING_LEFT = false;
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		ROTATING_RIGHT = true;
	}
	else {
		ROTATING_RIGHT = false;
	}

	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		ROTATING_UP = true;
	}
	else {
		ROTATING_UP = false;
	}

	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
		ROTATING_DOWN = true;
	}
	else {
		ROTATING_DOWN = false;
	}

	Camera& cam = scene->GetCamera();

	int mb = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
	if (mb == GLFW_PRESS) {
		double mouseX, mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);
		double xDelta = (mouseX - previousMouseX) * deltaTime * 30.0f;
		double yDelta = (mouseY - previousMouseY) * deltaTime * 30.0f;
		previousMouseX = mouseX;
		previousMouseY = mouseY;
		cam.Rotate(-yDelta, 0.f, xDelta);
	}

	
	if (MOVING_FORWARD) {
		cam.Translate({ 0.0f, 0.0f, -moveBy });
	}
	if (MOVING_BACKWARD) {
		cam.Translate({ 0.0f, 0.0f, moveBy });
	}
	if (STRAFING_LEFT) {
		cam.Translate({ -moveBy, 0.0f, 0.0f });
	}
	if (STRAFING_RIGHT) {
		cam.Translate({ moveBy, 0.0f, 0.0f });
	}
	if (ROTATING_LEFT) {
		cam.Rotate(0.0f, 0.0f, rotBy);
	}
	if (ROTATING_RIGHT) {
		cam.Rotate(0.0f, 0.0f, -rotBy);
	}
	if (ROTATING_UP) {
		cam.Rotate(rotBy, 0.0f, 0.0f);
	}
	if (ROTATING_DOWN) {
		cam.Rotate(-rotBy, 0.0f, 0.0f);
	}
}


void framebuffer_resize_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	Scene* scene = (Scene*)glfwGetWindowUserPointer(window);
	scene->ProjectionMatrix().SetToIdentity();
	scene->ProjectionMatrix().CreatePerspectiveProjection(80, (float)width / (float)height, 0.1f, 500.0f);
	
	scene->OrthographicMatrix().CreateOrthographicProjection(0.0f, width, height, 0.0f, 1.0f, -1.0f);
}

}
