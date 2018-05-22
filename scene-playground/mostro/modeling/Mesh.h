#pragma once

#include "Batch.h"
#include "Shader.h"

namespace mostro
{
	namespace modeling
	{
		struct Vertex
		{
			glm::vec3 Position;
			glm::vec3 Normal;
			glm::vec2 TexCoords;
		};

		struct Texture
		{
			GLuint id;
			std::string type;
			aiString path;
		};

		class Mesh : public Batch
		{
		public:
			/* Mesh Data */
			std::vector<Vertex> vertices;
			std::vector<GLuint> indices;
			std::vector<Texture> textures;

			/* Funtions */
			Mesh(std::vector<Vertex> &vertices, std::vector<GLuint> &indices, std::vector<Texture> &textures);
			void render(const Shader &shader) override;

		private:
			// Render Data
			GLuint VAO, VBO, EBO;

			// Functions
			void setupMesh();
		};
	}
}
