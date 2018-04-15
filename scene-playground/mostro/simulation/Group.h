#pragma once
#include "stdafx.h"
// Include standard headers
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <string.h>
#include <memory>
#include <unordered_map>
// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


namespace mostro
{
	namespace simulation
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
