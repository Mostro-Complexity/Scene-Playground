#pragma once
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <vector>
#include <memory>
#include "Shader.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace mostro
{
	namespace modeling
	{
		class Batch
		{
		public:
			Batch() {}

			virtual void render(std::shared_ptr<Shader> shader) abstract {}
		protected:
			virtual ~Batch() {}

		private:

		};
	}
}