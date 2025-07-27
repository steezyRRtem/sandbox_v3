/*

 ▒█████   ██▓███  ▓█████  ███▄    █   ▄████  ██▓
▒██▒  ██▒▓██░  ██▒▓█   ▀  ██ ▀█   █  ██▒ ▀█▒▓██▒
▒██░  ██▒▓██░ ██▓▒▒███   ▓██  ▀█ ██▒▒██░▄▄▄░▒██░
▒██   ██░▒██▄█▓▒ ▒▒▓█  ▄ ▓██▒  ▐▌██▒░▓█  ██▓▒██░
░ ████▓▒░▒██▒ ░  ░░▒████▒▒██░   ▓██░░▒▓███▀▒░██████▒
░ ▒░▒░▒░ ▒▓▒░ ░  ░░░ ▒░ ░░ ▒░   ▒ ▒  ░▒   ▒ ░ ▒░▓  ░
  ░ ▒ ▒░ ░▒ ░      ░ ░  ░░ ░░   ░ ▒░  ░   ░ ░ ░ ▒  ░
░ ░ ░ ▒  ░░          ░      ░   ░ ░ ░ ░   ░   ░ ░
	░ ░              ░  ░         ░       ░     ░  ░
*/


//headers
#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Shader.h"

//window
const GLint WIDTH = 800, HEIGHT = 600;

//main function
int main() {
	//init GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Back at OpenGL", nullptr, nullptr);

	int screenWidth, screenHeight;
	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

	if (nullptr == window) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}
	
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;

	//enable GLEW
	if (GLEW_OK != glewInit()) {
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	//setup viewport
	glViewport(0, 0, screenWidth, screenHeight);

	//load shaders
	Shader shader("shader.vert", "shader.frag");


	// Set up vertex data (and buffer(s)) and attribute pointers
	GLfloat vertices[] =
	{
		// Positions         // Colors
		0.5f, -0.5f, 0.0f,// Bottom Right
		-0.5f, -0.5f, 0.0f,// Bottom Left
		0.0f,  0.5f, 0.0f// Top
	};
	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Color attribute
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	//glEnableVertexAttribArray(1);

	glBindVertexArray(0); // Unbind VAO


	//IMGUI Init
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	float colorNew[3] = { 0.0f, 0.0f, 0.0f};

	//game loop
	while (!glfwWindowShouldClose(window)) {
		//check for events (mouse, keyboard ... )
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();


		// Draw the triangle
		shader.Use();
		
		glUniform3f(glGetUniformLocation(shader.getShader(), "ourColor"), colorNew[0], colorNew[1], colorNew[2]);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		

		ImGui::Begin("ImGui Menu");
		ImGui::Text("Change the colors of the triangle");
		ImGui::ColorEdit3("Color", colorNew);
		ImGui::End();


		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	//terminate IMGUI
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	// Properly de-allocate all resources once they've outlived their purpose
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	//terminate
	glfwTerminate();
	return EXIT_SUCCESS;
}