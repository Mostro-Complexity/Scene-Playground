#pragma once
#include "Group.h"
#include "TextureGroup.h"

namespace mostro
{
	namespace simulation
	{
		class ShaderGroup : public Group
		{
		public:
			GLuint programID;

			// 一个着色器控制多个纹理采样器，纹理采样器ID，纹理-->采样器
			std::unordered_map<GLuint, GLuint> textureSamplerMap;

			std::vector<std::shared_ptr<TextureGroup>> textureList;

			std::string vertexFilePath, fragmentFilePath;

			ShaderGroup(const std::string &vertexFilePath, const std::string &fragmentFilePath)
				: vertexFilePath(vertexFilePath), fragmentFilePath(fragmentFilePath)
			{
				programID = LoadShaders(vertexFilePath, fragmentFilePath);
			}

			void render() override
			{
				// Use our shader
				glUseProgram(programID);
				// 设置采样器
				unsigned i = 0;
				for (std::unordered_map<GLuint, GLuint>::iterator iter = textureSamplerMap.begin();
					iter != textureSamplerMap.end(); iter++)
				{
					// Bind our texture in Texture Unit i
					glActiveTexture(GL_TEXTURE0 + i);
					glBindTexture(GL_TEXTURE_2D, iter->first);
					// Get a handle for our "myTextureSampler" uniform
					glUniform1i(iter->second, GLint(i));
				}

			}

			void setTextureGroup(TextureGroup *textureGroup)
			{
				std::string name = "TextureSampler";
				textureSamplerMap[textureGroup->textureID] = glGetUniformLocation(programID,
					std::to_string(textureList.size()).c_str());
				textureList.push_back(std::shared_ptr<TextureGroup>(textureGroup));
			}

			void destroy()
			{
				for (std::unordered_map<GLuint, GLuint>::iterator iter = textureSamplerMap.begin();
					iter != textureSamplerMap.end(); iter++)
				{// 删除采样器
					glDeleteTextures(1, &iter->second);
				}
			}
		private:
			GLuint LoadShaders(const std::string &vertex_file_path, const std::string &fragment_file_path) {
				// Create the shaders
				GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
				GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

				// Read the Vertex Shader code from the file
				std::string VertexShaderCode;
				std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
				if (VertexShaderStream.is_open()) {
					std::stringstream sstr;
					sstr << VertexShaderStream.rdbuf();
					VertexShaderCode = sstr.str();
					VertexShaderStream.close();
				}
				else {
					printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n",
						vertex_file_path.c_str());
					getchar();
					return 0;
				}

				// Read the Fragment Shader code from the file
				std::string FragmentShaderCode;
				std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
				if (FragmentShaderStream.is_open()) {
					std::stringstream sstr;
					sstr << FragmentShaderStream.rdbuf();
					FragmentShaderCode = sstr.str();
					FragmentShaderStream.close();
				}

				GLint Result = GL_FALSE;
				int InfoLogLength;


				// Compile Vertex Shader
				printf("Compiling shader : %s\n", vertex_file_path.c_str());
				char const * VertexSourcePointer = VertexShaderCode.c_str();
				glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
				glCompileShader(VertexShaderID);

				// Check Vertex Shader
				glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
				glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
				if (InfoLogLength > 0) {
					std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
					glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
					printf("%s\n", &VertexShaderErrorMessage[0]);
				}



				// Compile Fragment Shader
				printf("Compiling shader : %s\n", fragment_file_path.c_str());
				char const * FragmentSourcePointer = FragmentShaderCode.c_str();
				glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
				glCompileShader(FragmentShaderID);

				// Check Fragment Shader
				glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
				glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
				if (InfoLogLength > 0) {
					std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
					glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
					printf("%s\n", &FragmentShaderErrorMessage[0]);
				}



				// Link the program
				printf("Linking program\n");
				GLuint ProgramID = glCreateProgram();
				glAttachShader(ProgramID, VertexShaderID);
				glAttachShader(ProgramID, FragmentShaderID);
				glLinkProgram(ProgramID);

				// Check the program
				glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
				glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
				if (InfoLogLength > 0) {
					std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
					glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
					printf("%s\n", &ProgramErrorMessage[0]);
				}


				glDetachShader(ProgramID, VertexShaderID);
				glDetachShader(ProgramID, FragmentShaderID);

				glDeleteShader(VertexShaderID);
				glDeleteShader(FragmentShaderID);

				return ProgramID;
			}

		};
	}
}