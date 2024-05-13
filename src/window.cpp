#include "common.h"

GLFWwindow* window;
i32 window_width, window_height;

void window_size_callback(GLFWwindow* win, int w, int h) {
	glfwGetWindowSize(window, &window_width, &window_height);
	glViewport(0,0,window_width,window_height);
}

void init_gfx() {
	if(!glfwInit()) {
		std::cout << "failed to initialize GLFW." << std::endl;
		exit(1);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	window = glfwCreateWindow(640, 480, "Petunia", NULL, NULL);
	if(!window) {
		std::cout << "failed to create window." << std::endl;
		exit(1);
	}
	glfwMakeContextCurrent(window);
	glfwGetWindowSize(window, &window_width, &window_height);
	glViewport(0,0,window_width,window_height);
	glfwSetWindowSizeCallback(window, window_size_callback);
}

void terminate_gfx() {
	glfwDestroyWindow(window);
	glfwTerminate();
}

u8 next_frame() {
	glfwSwapBuffers(window);
	glClearColor(.5, .5, .5, 1.);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glfwPollEvents();
	return !glfwWindowShouldClose(window);
}

f32 get_aspect() {
	return (f32)window_width / window_height;
}
