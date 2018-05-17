#pragma once

#include "..\modeling\Shader.h"
#include "..\modeling\Mesh.h"
#include "ModelGroup.h"
namespace mostro
{
	namespace utility
	{
		class TrackGroup : public ModelGroup
		{
		public:
			TrackGroup();

			virtual ~TrackGroup();

			void addVertex(float x, float y, float z);

			void render() override;
		private:
			glm::mat4 model;

			std::vector<GLfloat> trackList;
			GLuint VertexArrayID;
			GLuint vertexbuffer;
		};
	}
}
