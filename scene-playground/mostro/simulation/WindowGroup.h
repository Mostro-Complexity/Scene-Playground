#pragma once
#include "Group.h"
#include "CameraGroup.h"
#include "ShaderGroup.h"
#include "TextureGroup.h"
#include "ModelGroup.h"

namespace mostro
{
	namespace simulation
	{
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
			virtual ~WindowGroup() {}

			void exit()
			{
				// Close OpenGL window and terminate GLFW
				glfwTerminate();
			}

			void setGroup(Group *group)
			{
				renderList.push_back(std::shared_ptr<Group>(group));
			}

			void setCameraGroup(CameraGroup *cameraGroup)
			{
				this->cameraGroup = std::shared_ptr<CameraGroup>(cameraGroup);
				renderList.push_back(this->cameraGroup);
			}

			void setModelGroup(ModelGroup *modelGroup)
			{
				// Get a handle for our "MVP" uniform,一定要在programID得到后才产生
				MatrixID = glGetUniformLocation(modelGroup->getShaderGroup()->programID, "MVP");

				renderList.push_back(std::shared_ptr<Group>(modelGroup));
			}

			std::shared_ptr<CameraGroup> getCameraGroup()
			{
				return cameraGroup;
			}

			std::shared_ptr<TextureGroup> getTextureGroup()
			{
				return textureGroup;
			}

			// 主窗体渲染
			void render() override
			{
				do {
					// Clear the screen
					glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



					cameraProcess();
					for (auto &i : renderList)
					{
						i->render();
					}

					glm::mat4 ProjectionMatrix = getCameraGroup()->getProjectionMatrix();
					glm::mat4 ViewMatrix = getCameraGroup()->getViewMatrix();
					glm::mat4 ModelMatrix = glm::mat4(1.0);
					glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

					// Send our transformation to the currently bound shader, 
					// in the "MVP" uniform
					glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

					// Swap buffers
					glfwSwapBuffers(window);
					glfwPollEvents();

				} // Check if the ESC key was pressed or the window was closed
				while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
					glfwWindowShouldClose(window) == 0);
			}

			GLFWwindow * window;
		private:
			// 相机对象
			std::shared_ptr<CameraGroup> cameraGroup;
			// 纹理对象
			std::shared_ptr<TextureGroup> textureGroup;
			// 渲染列表
			std::vector<std::shared_ptr<Group>> renderList;

			GLuint MatrixID;

			void cameraProcess()
			{
				if (cameraGroup != nullptr)
				{
					// Get mouse position
					double xpos, ypos;
					glfwGetCursorPos(window, &xpos, &ypos);

					// Reset mouse position for next frame
					glfwSetCursorPos(window, 1024 / 2, 768 / 2);
					// Compute new orientation,将鼠标坐标输入转换为两个方向角
					cameraGroup->horizontalRotate(xpos);
					cameraGroup->verticalRotate(ypos);

					// Move forward
					if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS
						|| glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
						cameraGroup->moveForward();
					}
					// Move backward
					if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS
						|| glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
						cameraGroup->moveBackward();
					}
					// Strafe right
					if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS
						|| glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
						cameraGroup->strafeRight();
					}
					// Strafe left
					if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS
						|| glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
						cameraGroup->strafeLeft();
					}
				}
			}
		};
	}
}
