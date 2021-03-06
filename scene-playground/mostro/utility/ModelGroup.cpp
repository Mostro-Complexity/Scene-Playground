#include "ModelGroup.h"

#include <iostream>
#include <SOIL.h>

using namespace mostro::utility;
using namespace mostro::modeling;

ModelGroup::ModelGroup(const GLchar * path, std::shared_ptr<Shader> shader)
	: shader(shader)
{
	this->loadModel(path);

	// Draw the loaded model
	model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
	// It's a bit too big for our scene, so scale it down
	model = glm::rotate(model, 3.14f / 2, glm::vec3(-1.0, 0., 0.));
}

ModelGroup::ModelGroup()
{
	shader = std::shared_ptr<modeling::Shader>(
		ShaderGenerator::ptrFromFile("TransformVertexShader.vertexshader",
			"TextureFragmentShader.fragmentshader"));

	// Draw the loaded model
	model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
	// It's a bit too big for our scene, so scale it down
	model = glm::rotate(model, 3.14f / 2, glm::vec3(-1.0, 0., 0.));
}

void ModelGroup::render()
{
	shader->use();

	glUniformMatrix4fv(
		glGetUniformLocation(shader->programID, "model"),
		1,
		GL_FALSE,
		glm::value_ptr(model)
	);

	for (GLuint i = 0; i < this->meshes.size(); i++)
	{
		this->meshes[i].render(*shader);
	}
}

void ModelGroup::render(
	const std::function<glm::mat4(const glm::mat4&)>& transform)
{
	shader->use();

	glUniformMatrix4fv(
		glGetUniformLocation(shader->programID, "model"),
		1,
		GL_FALSE,
		glm::value_ptr(transform(model))
	);

	for (GLuint i = 0; i < this->meshes.size(); i++)
	{
		this->meshes[i].render(*shader);
	}
}

void ModelGroup::render(glm::mat4 transform(const glm::mat4&))
{
	shader->use();

	glUniformMatrix4fv(
		glGetUniformLocation(shader->programID, "model"),
		1,
		GL_FALSE,
		glm::value_ptr(transform(model))
	);

	for (GLuint i = 0; i < this->meshes.size(); i++)
	{
		this->meshes[i].render(*shader);
	}
}

glm::vec3 ModelGroup::getCentralVertex()
{
	return glm::vec3((min_x + max_x) / 2, (min_y + max_y) / 2, (min_z + max_z) / 2);
}

void ModelGroup::loadModel(const std::string &path)
{
	Assimp::Importer import;

	const aiScene* scene = import.ReadFile(
		path,
		aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices
	);

	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return;
	}

	this->processNode(scene->mRootNode, scene);
}

void ModelGroup::processNode(aiNode* node, const aiScene* scene)
{
	// Process all the node's meshes (if any)
	for (GLuint i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		this->meshes.push_back(this->processMesh(mesh, scene));
	}
	// Then do the same for each of its children
	for (GLuint i = 0; i < node->mNumChildren; i++)
	{
		this->processNode(node->mChildren[i], scene);
	}
}

Mesh ModelGroup::processMesh(aiMesh * mesh, const aiScene * scene)
{
	std::vector<modeling::Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<modeling::Texture> textures;

	for (GLuint i = 0; i < mesh->mNumVertices; i++)
	{
		modeling::Vertex vertex;
		// Process vertex positions, normals and texture coordinates

		//position
		glm::vec3 vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;

		max_x = vector.x > max_x ? vector.x : max_x;
		max_y = vector.y > max_y ? vector.y : max_y;
		max_z = vector.z > max_z ? vector.z : max_z;

		min_x = vector.x < min_x ? vector.x : min_x;
		min_y = vector.y < min_y ? vector.y : min_y;
		min_z = vector.z < min_z ? vector.z : min_z;

		// normal
		if (mesh->mNormals != nullptr)
		{
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.Normal = vector;
		}
		// texture coordinates
		if (mesh->mTextureCoords[0]) // Does the mesh contain texture coordinates?
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);

		vertices.push_back(vertex);
	}
	// Process indices
	for (GLuint i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (GLuint j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	// Process material
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<modeling::Texture> diffuseMaps = this->loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		std::vector<modeling::Texture> specularMaps = this->loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	return Mesh(vertices, indices, textures);
}

std::vector<Texture> ModelGroup::loadMaterialTextures(aiMaterial * mat, aiTextureType type, std::string typeName)
{
	std::vector<modeling::Texture> textures;
	for (GLuint i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		GLboolean skip = false;
		for (GLuint j = 0; j < textures_loaded.size(); j++)
		{
			if (textures_loaded[j].path == str)
			{
				textures.push_back(textures_loaded[j]);
				skip = true;
				break;
			}
		}
		if (!skip)
		{   // If texture hasn't been loaded already, load it
			Texture texture;
			texture.id = TextureFromFile(str.C_Str());
			texture.type = typeName;
			texture.path = str;
			textures.push_back(texture);
			this->textures_loaded.push_back(texture);  // Add to loaded textures
		}
	}
	return textures;
}

GLint ModelGroup::TextureFromFile(const char* path)
{
	//Generate texture ID and load texture data 
	GLuint textureID;
	glGenTextures(1, &textureID);
	int width, height;
	unsigned char* image = SOIL_load_image(path, &width, &height, 0, SOIL_LOAD_RGB);
	// Assign texture to ID
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image);
	return textureID;
}

//ModelGroup:: ~ModelGroup()
//{
//	for (size_t i = 0; i < meshes.size(); i++)
//	{
//		for (size_t j = 0; j < meshes[i].textures.size(); j++)
//		{
//			glDeleteTextures(1, &meshes[i].textures[j].id);
//		}
//	}
//}
