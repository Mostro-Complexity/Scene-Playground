#pragma once
#include "Group.h"
#include "..\modeling\Shader.h"
#include "..\modeling\Mesh.h"


namespace mostro
{
	namespace utility
	{
		class ModelGroup : public Group
		{
		public:
			ModelGroup(const GLchar* path, std::shared_ptr<modeling::Shader> shader);

			ModelGroup();

			void render() override;

			std::shared_ptr<modeling::Shader> shader;

			void render(const std::function<glm::mat4(const glm::mat4&)> &func);

			void render(glm::mat4 transform(const glm::mat4&));

		private:
			// Model Data
			std::vector<modeling::Texture> textures_loaded;
			std::vector<modeling::Mesh> meshes;

			// Functions
			void loadModel(const std::string &path);
			void processNode(aiNode* node, const aiScene* scene);
			modeling::Mesh processMesh(aiMesh* mesh, const aiScene* scene);

			std::vector<modeling::Texture> loadMaterialTextures(
				aiMaterial* mat,
				aiTextureType type,
				std::string typeName);

			GLint TextureFromFile(const char* path);

			//virtual ~ModelGroup();

		protected:
			glm::mat4 model;

		};
	}
}

