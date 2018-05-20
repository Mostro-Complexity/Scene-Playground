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
			WindowGroup(int width, int height, const std::string &title);

			virtual ~WindowGroup();

			void addGroup(std::shared_ptr<Group> group);

			void setCameraGroup(std::shared_ptr<CameraGroup> cameraGroup);

			void addModelGroup(std::shared_ptr<ModelGroup> modelGroup);

			std::shared_ptr<CameraGroup> getCameraGroup();


			// 主窗体渲染
			void render() override;

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

			void cameraProcess();

		};
	}
}
