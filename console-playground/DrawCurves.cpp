#include "stdafx.h"
#include "Group.h"
#include "WindowGroup.h"
#include "CameraGroup.h"
#include <vector>
#include <deque>
#include <memory>
using namespace std;
using namespace glm;


int main(void)
{
	WindowGroup windowGroup(1024, 768, "initial scene");

	windowGroup.init();

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders("TransformVertexShader.vertexshader", "TextureFragmentShader.fragmentshader");

	// Get a handle for our "MVP" uniform
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");

	vector<GLfloat> g_vertex_buffer_data;

	GLfloat delta = 0.1;
	GLint timer = 0;

	do {

		g_vertex_buffer_data.push_back(std::sin(delta));
		g_vertex_buffer_data.push_back(std::cos(delta));
		g_vertex_buffer_data.push_back(std::log10(delta*delta));

		GLuint vertexbuffer;
		glGenBuffers(1, &vertexbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);

		glBufferData(GL_ARRAY_BUFFER, g_vertex_buffer_data.size() * sizeof(GLfloat), &g_vertex_buffer_data[0], GL_STATIC_DRAW);
		//GLintptr vertexOffset = (g_vertex_buffer_data.size() - 1) * sizeof(GLfloat);
		//GLfloat ptr[] = { sin(delta),cos(delta),log10(delta*delta) };
		//glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		//glBufferSubData(GL_ARRAY_BUFFER, vertexOffset, 3 * sizeof(GLfloat), ptr);

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);

		windowGroup.render();
		windowGroup.cameraGroup.render();

		glm::mat4 ProjectionMatrix = windowGroup.cameraGroup.getProjectionMatrix();
		glm::mat4 ViewMatrix = windowGroup.cameraGroup.getViewMatrix();
		glm::mat4 ModelMatrix = glm::mat4(1.0);
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		//glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(GLvoid*)0            // array buffer offset
		);

		glDrawArrays(GL_LINE_STRIP, 0, g_vertex_buffer_data.size() / 3); // 12*3 indices starting at 0 -> 12 triangles

																		 //glDrawRangeElements(GL_LINE_STRIP, 0, timer, timer * 3, GL_UNSIGNED_BYTE, &g_vertex_buffer_indices[0]);

		glDisableVertexAttribArray(0);

		timer++;
		delta += 0.01;

		// Swap buffers
		glfwSwapBuffers(windowGroup.window);
		glfwPollEvents();

		// Cleanup VBO and shader
		glDeleteBuffers(1, &vertexbuffer);

		//delete[s] ptr;
	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(windowGroup.window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(windowGroup.window) == 0);

	glDeleteProgram(programID);
	glDeleteVertexArrays(1, &VertexArrayID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

