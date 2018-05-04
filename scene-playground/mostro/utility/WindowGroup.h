#pragma once
#include "Group.h"
#include "CameraGroup.h"
#include "..\modeling\Shader.h"
#include "..\utility\ModelGroup.h"

namespace mostro
{
	namespace utility
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
				glClearColor(0.0f, 0.0f, 0.f, 0.0f);

				// Define the viewport dimensions
				glViewport(0, 0, 1024, 768);

				// Enable depth test
				glEnable(GL_DEPTH_TEST);
				// Accept fragment if it closer to the camera than the former one
				glDepthFunc(GL_LESS);

				// Cull triangles which normal is not towards the camera
				glEnable(GL_CULL_FACE);

				//glEnable(GL_BLEND);//开启混合
				//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//设置混合因子
				//glEnable(GL_POINT_SMOOTH);//开启点平滑
				//glEnable(GL_NICEST);//设置质量优先
			}
			virtual ~WindowGroup()
			{
				//delete ourModel;
				//glfwTerminate();
			}

			void exit()
			{
				// Close OpenGL window and terminate GLFW
				glfwTerminate();
			}

			void addGroup(Group *group)
			{
				std::shared_ptr<Group> arrivalGroup(group);
				if (std::shared_ptr<CameraGroup> camera =
					std::dynamic_pointer_cast<CameraGroup>(arrivalGroup))
				{
					cameraGroup = camera;
				}
				renderList.push_back(arrivalGroup);
			}

			void setCameraGroup(CameraGroup *cameraGroup)
			{
				this->cameraGroup = std::shared_ptr<CameraGroup>(cameraGroup);
				renderList.push_back(this->cameraGroup);
			}

			void addModelGroup(ModelGroup *modelGroup)
			{
				renderList.push_back(std::shared_ptr<Group>(modelGroup));
			}

			std::shared_ptr<CameraGroup> getCameraGroup()
			{
				return cameraGroup;
			}

			// 主窗体渲染
			void render() override
			{
				do {
					glfwPollEvents();

					// Clear the colorbuffer
					glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

					cameraProcess();

					for (size_t i = 0; i < renderList.size(); i++)
					{
						if (std::shared_ptr<ModelGroup> mod =
							std::dynamic_pointer_cast<ModelGroup>(renderList[i]))
						{
							glm::mat4 projection = cameraGroup->getProjectionMatrix();
							glm::mat4 view = cameraGroup->getViewMatrix();
							glUniformMatrix4fv(
								glGetUniformLocation(mod->shader->programID, "projection"),
								1,
								GL_FALSE,
								glm::value_ptr(projection)
							);

							glUniformMatrix4fv(
								glGetUniformLocation(mod->shader->programID, "view"),
								1,
								GL_FALSE,
								glm::value_ptr(view)
							);
						}
						renderList[i]->render();
					}

					// Swap the buffers
					glfwSwapBuffers(window);
				} // Check if the ESC key was pressed or the window was closed
				while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
					glfwWindowShouldClose(window) == 0);
			}

			GLFWwindow * window;
		private:
			// 相机对象
			std::shared_ptr<CameraGroup> cameraGroup;
			// 渲染列表
			std::vector<std::shared_ptr<Group>> renderList;

			GLuint MatrixID;

			void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
			{
				//cameraGroup.ProcessMouseScroll(yoffset);
			}

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
