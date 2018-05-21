#pragma once
// Include standard headers
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include <list>
#include <queue>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <string.h>
#include <functional>
#include <memory>
#include <unordered_map>
// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <soil.h>

namespace mostro
{
	namespace utility
	{
		enum CameraGroupType
		{
			roamCamera, observeCamera
		};

		class Group
		{
		public:

			virtual void render() abstract;

			virtual ~Group() {}
		};
	}
}
