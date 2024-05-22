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

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();
}

void terminate_gfx() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwDestroyWindow(window);
	glfwTerminate();
}


u64 frame_count;

u8 next_frame() {
	if(frame_count != 0) {
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	glfwSwapBuffers(window);
	glClearColor(.5, .5, .5, 1.);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glfwPollEvents();

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	frame_count++;
	return !glfwWindowShouldClose(window);
}

f32 get_aspect() {
	return (f32)window_width / window_height;
}
