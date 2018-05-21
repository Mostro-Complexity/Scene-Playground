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
			TrackGroup(std::shared_ptr<modeling::Shader> shader);

			virtual ~TrackGroup();

			void addVertex(float x, float y, float z);

			void render() override;
		private:

			std::deque<GLfloat> trackList;
			GLuint vertexArray;
			GLuint vertexBuffer;
		};
	}
}
