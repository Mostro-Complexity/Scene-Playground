// Include standard headers
#include <cstdio>
#include <cstdlib>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <string.h>

using namespace std;
using namespace glm;

GLuint loadShaders(const char * vertex_file_path, const char * fragment_file_path) {
	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if (VertexShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		VertexShaderCode = sstr.str();
		VertexShaderStream.close();
	}
	else {
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		getchar();
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if (FragmentShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;


	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}

enum CameraGroupType
{
	roamCamera, observeCamera
};

class Group
{
	virtual void init() abstract;
	virtual void render() abstract;

};

class CameraGroup : public Group
{
public:
	void init() override {}

	void render() override
	{
		switch (cameraGroupType)
		{
		case CameraGroupType::roamCamera:
			roamCamera(); break;
		case CameraGroupType::observeCamera:
			observeCamera(); break;
		default:
			break;
		}

	}

	void horizontalRotate(double xpos)
	{
		horizontalAngle += mouseSpeed * float(1024 / 2 - xpos); // 水平角
	}

	void verticalRotate(double ypos)
	{
		verticalAngle += mouseSpeed * float(768 / 2 - ypos);// 垂直角 
	}

	void moveForward()
	{
		position += direction * deltaTime * speed;
	}

	void moveBackward()
	{
		position -= direction * deltaTime * speed;
	}

	void strafeRight()
	{
		position += right * deltaTime * speed;
	}

	void strafeLeft()
	{
		position -= right * deltaTime * speed;
	}

	glm::mat4 getProjectionMatrix()
	{
		return ProjectionMatrix;
	}

	glm::mat4 getViewMatrix()
	{
		return ViewMatrix;
	}
private:
	// Initial position : on +Z
	glm::vec3 position = glm::vec3(0, 0, 5);
	// Initial horizontal angle : toward -Z
	float horizontalAngle = 3.14f;
	// Initial vertical angle : none
	float verticalAngle = 0.0f;
	// Initial Field of View
	float initialFoV = 45.0f;

	float speed = 3.0f; // 3 units / second
	float mouseSpeed = 0.005f;

	// Direction : Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 direction;

	float deltaTime;

	// Right vector
	glm::vec3 right;

	glm::mat4 ProjectionMatrix;
	glm::mat4 ViewMatrix;

	CameraGroupType cameraGroupType;

	void getDeltaTime()
	{
		// glfwGetTime is called only once, the first time this function is called
		static double lastTime = glfwGetTime();

		// Compute time difference between current and last frame
		double currentTime = glfwGetTime();
		deltaTime = float(currentTime - lastTime);

		// For the next frame, the "last time" will be "now"
		lastTime = currentTime;
	}

	// 漫游相机
	void roamCamera() {
		getDeltaTime();

		direction = glm::vec3(
			cos(verticalAngle) * sin(horizontalAngle),
			sin(verticalAngle),
			cos(verticalAngle) * cos(horizontalAngle)
		);

		// Right vector
		right = glm::vec3(
			sin(horizontalAngle - 3.14f / 2.0f),
			0,
			cos(horizontalAngle - 3.14f / 2.0f)
		);

		// Up vector
		glm::vec3 up = glm::cross(right, direction);

		float FoV = initialFoV;// - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.

							   // Projection matrix : 45 degree Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
		ProjectionMatrix = glm::perspective(glm::radians(FoV), 4.0f / 3.0f, 0.1f, 100.0f);
		// Camera matrix
		ViewMatrix = glm::lookAt(
			position,           // Camera is here
			position + direction, // and looks here : at the same position, plus "direction"
			up                  // Head is up (set to 0,-1,0 to look upside-down)
		);

	}

	// 观察相机
	void observeCamera()
	{
		getDeltaTime();

		position.x = 5 * cos(verticalAngle) * cos(horizontalAngle);
		position.y = 5 * sin(verticalAngle);
		position.z = 5 * cos(verticalAngle) * sin(horizontalAngle);

		float FoV = initialFoV;
		// - 5 * glfwGetMouseWheel();
		// Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.

		// Projection matrix : 45 degree Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
		ProjectionMatrix = glm::perspective(glm::radians(FoV), 4.0f / 3.0f, 0.1f, 100.0f);

		glm::vec3 up;
		if (isUpsideDown(verticalAngle))
			up = glm::vec3(0, -1, 0);// 从上向下看,头朝Y轴负方向
		else
			up = glm::vec3(0, 1, 0);// 从上向下看,头朝Y轴正方向

									// Camera matrix
		ViewMatrix = glm::lookAt(
			position,           // Camera is here
			glm::vec3(0., 0., 0.), // and looks here : at the same position, plus "direction"
			up                  // Head is up (set to 0,-1,0 to look upside-down)
		);
	}

	// 是否应该“倒立”观察
	bool isUpsideDown(float verticalAngle)
	{
		// -360°到360°的范围角
		float rangeAngle =
			fmod(verticalAngle, 2 * glm::pi<float>());

		// 90°到270°的集合
		bool set_a = rangeAngle < 3 * glm::pi<float>() / 2
			&& rangeAngle >= glm::pi<float>() / 2;

		// -90°到-270°的集合
		bool set_b = rangeAngle >= -3 * glm::pi<float>() / 2
			&& rangeAngle < -glm::pi<float>() / 2;

		return set_b || set_a;
	}
};

class WindowGroup : public Group
{
public:
	WindowGroup(int width, int height, const std::string &title)
	{
		// Initialise GLFW
		if (!glfwInit())
		{
			fprintf(stderr, "Failed to initialize GLFW\n");
			getchar();
			return;
		}

		glfwWindowHint(GLFW_SAMPLES, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		// Open a window and create its OpenGL context

		window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);

		// Open a window and create its OpenGL context
		if (window == NULL) {
			fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
			getchar();
			glfwTerminate();
		}
	}
	~WindowGroup() {}

	// 相机对象
	CameraGroup cameraGroup;

	void init() override
	{
		glfwMakeContextCurrent(window);

		// Initialize GLEW
		glewExperimental = true; // Needed for core profile
		if (glewInit() != GLEW_OK) {
			fprintf(stderr, "Failed to initialize GLEW\n");
			getchar();
			glfwTerminate();
			return;
		}

		// Ensure we can capture the escape key being pressed below
		glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
		// Hide the mouse and enable unlimited mouvement
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		// Set the mouse at the center of the screen
		glfwPollEvents();
		glfwSetCursorPos(window, 1024 / 2, 768 / 2);

		// Dark blue background
		glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

		// Enable depth test
		glEnable(GL_DEPTH_TEST);
		// Accept fragment if it closer to the camera than the former one
		glDepthFunc(GL_LESS);

		// Cull triangles which normal is not towards the camera
		glEnable(GL_CULL_FACE);
	}

	// 主窗体渲染
	void render() override
	{
		// Get mouse position
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		// Reset mouse position for next frame
		glfwSetCursorPos(window, 1024 / 2, 768 / 2);
		// Compute new orientation,将鼠标坐标输入转换为两个方向角
		cameraGroup.horizontalRotate(xpos);
		cameraGroup.verticalRotate(ypos);

		// Move forward
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS
			|| glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			cameraGroup.moveForward();
		}
		// Move backward
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS
			|| glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			cameraGroup.moveBackward();
		}
		// Strafe right
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS
			|| glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			cameraGroup.strafeRight();
		}
		// Strafe left
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS
			|| glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			cameraGroup.strafeLeft();
		}
	}

	GLFWwindow * window;
private:
};

void loadShadersFromString(const GLchar *vertexShaderSource,
	const GLchar *fragmentShaderSource_1,
	const GLchar *fragmentShaderSource_2,
	GLuint &shaderProgram_1,
	GLuint &shaderProgram_2)
{
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		printf("Failed to compile vertexShader:%s \n", infoLog);
	}

	GLuint fragmentShader_1 = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader_1, 1, &fragmentShaderSource_1, NULL);
	glCompileShader(fragmentShader_1);

	glGetShaderiv(fragmentShader_1, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader_1, 512, NULL, infoLog);
		printf("Failed to compile fragmentShader:%s \n", infoLog);
	}

	GLuint fragmentShader_2 = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader_2, 1, &fragmentShaderSource_2, NULL);
	glCompileShader(fragmentShader_2);
	glGetShaderiv(fragmentShader_2, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader_2, 512, NULL, infoLog);
		printf("Failed to compile fragmentShader:%s \n", infoLog);
	}

	shaderProgram_1 = glCreateProgram();
	glAttachShader(shaderProgram_1, vertexShader);
	glAttachShader(shaderProgram_1, fragmentShader_1);
	glLinkProgram(shaderProgram_1);

	glGetShaderiv(shaderProgram_1, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram_1, 512, NULL, infoLog);
		printf("Failed to link shaderProgram:%s \n", infoLog);
	}

	shaderProgram_2 = glCreateProgram();
	glAttachShader(shaderProgram_2, vertexShader);
	glAttachShader(shaderProgram_2, fragmentShader_2);
	glLinkProgram(shaderProgram_2);

	glGetShaderiv(shaderProgram_2, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram_2, 512, NULL, infoLog);
		printf("Failed to link shaderProgram:%s \n", infoLog);
	}


	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader_1);
	glDeleteShader(fragmentShader_2);
}

int main(void)
{
	WindowGroup windowGroup(1024, 768, "initial scene");

	windowGroup.init();

	GLuint VertexArrayID[2];
	glGenVertexArrays(2, VertexArrayID);

	const GLchar *vertexShaderSource =
		"#version 330 core\n"
		"layout(location = 0) in vec3 vertexPosition_modelspace;"
		"layout(location = 1) in vec2 vertexUV;"
		"uniform mat4 MVP;"
		"void main() {gl_Position = MVP * vec4(vertexPosition_modelspace, 1);}";

	const GLchar *fragmentShaderSource_1 =
		"#version 330 core\n"
		"out vec4 color;"
		"void main() {color = vec4(1.0, 1.0, 1.0, 1.0);	}";

	const GLchar *fragmentShaderSource_2 =
		"#version 330 core\n"
		"out vec4 color;"
		"void main() {color = vec4(0.0, 0.0, 0.0, 1.0);	}";

	GLuint shaderProgram_1, shaderProgram_2;

	loadShadersFromString(
		vertexShaderSource,
		fragmentShaderSource_1,
		fragmentShaderSource_2,
		shaderProgram_1,
		shaderProgram_2);

	// Get a handle for our "MVP" uniform
	GLuint MatrixID = glGetUniformLocation(shaderProgram_1, "MVP");

	static const vector<GLfloat> g_vertex_data_1 =
	{
		0,0,0, 1,1,1, 1,1,0,
		1,1,0, 1,1,1, 0,0,0,
		1,2,0, 1,0,1, 0,0,2,
		0,0,2, 1,0,1, 1,2,0,
	}, g_vertex_data_2 =
	{
		2,2,2, 3,2,3, 3,1,1,
		3,1,1, 3,2,3, 2,2,2,
		3,3,3, 1,3,1, 1,1,3,
		1,1,3, 1,3,1, 3,3,3,
	};

	GLuint vertexbuffer[2];
	glGenBuffers(2, vertexbuffer);

	glBindVertexArray(VertexArrayID[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[0]);
	glBufferData(GL_ARRAY_BUFFER, g_vertex_data_1.size() * sizeof(GLfloat), &g_vertex_data_1[0], GL_STATIC_DRAW);

	glBindVertexArray(VertexArrayID[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[1]);
	glBufferData(GL_ARRAY_BUFFER, g_vertex_data_2.size() * sizeof(GLfloat), &g_vertex_data_2[0], GL_STATIC_DRAW);

	do {
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(shaderProgram_1);

		windowGroup.render();
		windowGroup.cameraGroup.render();

		glm::mat4 ProjectionMatrix = windowGroup.cameraGroup.getProjectionMatrix();
		glm::mat4 ViewMatrix = windowGroup.cameraGroup.getViewMatrix();
		glm::mat4 ModelMatrix = glm::mat4(1.0);
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[0]);
		//glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(GLvoid*)0            // array buffer offset
		);
		glDrawArrays(GL_TRIANGLES, 0, (GLsizei)g_vertex_data_2.size() / 3); // 12*3 indices starting at 0 -> 12 triangles
		glDisableVertexAttribArray(0);

		glUseProgram(shaderProgram_2);

		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[1]);
		//glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(GLvoid*)0            // array buffer offset
		);

		glDrawArrays(GL_TRIANGLES, 0, (GLsizei)g_vertex_data_2.size() / 3); // 12*3 indices starting at 0 -> 12 triangles
		glDisableVertexAttribArray(0);

		// Swap buffers
		glfwSwapBuffers(windowGroup.window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(windowGroup.window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(windowGroup.window) == 0);

	// Cleanup VBO and shader
	glDeleteBuffers(2, vertexbuffer);
	glDeleteProgram(shaderProgram_1);
	glDeleteVertexArrays(2, VertexArrayID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

