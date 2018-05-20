#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <GL/glew.h> // Include glew to get all the required OpenGL headers

namespace mostro
{
	namespace modeling
	{
		struct Shader
		{
			void use()
			{
				glUseProgram(this->programID);
			}

			GLuint programID;
			GLuint vertexShaderID;
			GLuint fragmentShaderID;

			virtual ~Shader()
			{
				glDeleteProgram(programID);
				glDeleteShader(vertexShaderID);
				glDeleteShader(fragmentShaderID);
			}
		};

		class ShaderGenerator
		{
		public:
			static Shader fromFile(const GLchar* vertexPath, const GLchar* fragmentPath)
			{
				// 1. Retrieve the vertex/fragment source code from filePath
				std::string vertexCode;
				std::string fragmentCode;
				std::ifstream vShaderFile;
				std::ifstream fShaderFile;
				// ensures ifstream objects can throw exceptions:
				vShaderFile.exceptions(std::ifstream::badbit);
				fShaderFile.exceptions(std::ifstream::badbit);
				try
				{
					// Open files
					vShaderFile.open(vertexPath);
					fShaderFile.open(fragmentPath);
					std::stringstream vShaderStream, fShaderStream;
					// Read file's buffer contents into streams
					vShaderStream << vShaderFile.rdbuf();
					fShaderStream << fShaderFile.rdbuf();
					// close file handlers
					vShaderFile.close();
					fShaderFile.close();
					// Convert stream into string
					vertexCode = vShaderStream.str();
					fragmentCode = fShaderStream.str();
				}
				catch (std::ifstream::failure e)
				{
					std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
				}

				return fromString(vertexCode, fragmentCode);
			}

			static Shader fromString(const std::string &vertexShaderCode, const std::string &fragmentShaderCode)
			{
				Shader shader;
				const GLchar *vscp = vertexShaderCode.c_str(), *fscp = fragmentShaderCode.c_str();
				GLint success;
				GLchar infoLog[512];
				// Vertex Shader
				shader.vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
				glShaderSource(shader.vertexShaderID, 1, &vscp, NULL);
				glCompileShader(shader.vertexShaderID);
				// Print compile errors if any
				glGetShaderiv(shader.vertexShaderID, GL_COMPILE_STATUS, &success);
				if (!success)
				{
					glGetShaderInfoLog(shader.vertexShaderID, 512, NULL, infoLog);
					std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
				}
				// Fragment Shader
				shader.fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
				glShaderSource(shader.fragmentShaderID, 1, &fscp, NULL);
				glCompileShader(shader.fragmentShaderID);
				// Print compile errors if any
				glGetShaderiv(shader.fragmentShaderID, GL_COMPILE_STATUS, &success);
				if (!success)
				{
					glGetShaderInfoLog(shader.fragmentShaderID, 512, NULL, infoLog);
					std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
				}
				// Shader Program
				shader.programID = glCreateProgram();
				glAttachShader(shader.programID, shader.vertexShaderID);
				glAttachShader(shader.programID, shader.fragmentShaderID);
				glLinkProgram(shader.programID);
				// Print linking errors if any
				glGetProgramiv(shader.programID, GL_LINK_STATUS, &success);
				if (!success)
				{
					glGetProgramInfoLog(shader.programID, 512, NULL, infoLog);
					std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
				}
				// TODO:Delete the shaders as they're linked into our program now and no longer necessery??
				//glDeleteShader(shader.vertexShaderID);
				//glDeleteShader(fragment);
			}

			static Shader* ptrFromFile(const GLchar* vertexPath, const GLchar* fragmentPath)
			{
				// 1. Retrieve the vertex/fragment source code from filePath
				std::string vertexCode;
				std::string fragmentCode;
				std::ifstream vShaderFile;
				std::ifstream fShaderFile;
				// ensures ifstream objects can throw exceptions:
				vShaderFile.exceptions(std::ifstream::badbit);
				fShaderFile.exceptions(std::ifstream::badbit);
				try
				{
					// Open files
					vShaderFile.open(vertexPath);
					fShaderFile.open(fragmentPath);
					std::stringstream vShaderStream, fShaderStream;
					// Read file's buffer contents into streams
					vShaderStream << vShaderFile.rdbuf();
					fShaderStream << fShaderFile.rdbuf();
					// close file handlers
					vShaderFile.close();
					fShaderFile.close();
					// Convert stream into string
					vertexCode = vShaderStream.str();
					fragmentCode = fShaderStream.str();
				}
				catch (std::ifstream::failure e)
				{
					std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
				}

				return ptrFromString(vertexCode, fragmentCode);
			}

			static Shader* ptrFromString(const std::string &vertexShaderCode, const std::string &fragmentShaderCode)
			{
				Shader* shader = new Shader;
				const GLchar *vscp = vertexShaderCode.c_str(), *fscp = fragmentShaderCode.c_str();
				GLint success;
				GLchar infoLog[512];
				// Vertex Shader
				shader->vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
				glShaderSource(shader->vertexShaderID, 1, &vscp, NULL);
				glCompileShader(shader->vertexShaderID);
				// Print compile errors if any
				glGetShaderiv(shader->vertexShaderID, GL_COMPILE_STATUS, &success);
				if (!success)
				{
					glGetShaderInfoLog(shader->vertexShaderID, 512, NULL, infoLog);
					std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
				}
				// Fragment Shader
				shader->fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
				glShaderSource(shader->fragmentShaderID, 1, &fscp, NULL);
				glCompileShader(shader->fragmentShaderID);
				// Print compile errors if any
				glGetShaderiv(shader->fragmentShaderID, GL_COMPILE_STATUS, &success);
				if (!success)
				{
					glGetShaderInfoLog(shader->fragmentShaderID, 512, NULL, infoLog);
					std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
				}
				// Shader Program
				shader->programID = glCreateProgram();
				glAttachShader(shader->programID, shader->vertexShaderID);
				glAttachShader(shader->programID, shader->fragmentShaderID);
				glLinkProgram(shader->programID);
				// Print linking errors if any
				glGetProgramiv(shader->programID, GL_LINK_STATUS, &success);
				if (!success)
				{
					glGetProgramInfoLog(shader->programID, 512, NULL, infoLog);
					std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
				}
				// TODO:Delete the shaders as they're linked into our program now and no longer necessery??
				//glDeleteShader(shader.vertexShaderID);
				//glDeleteShader(fragment);

				return shader;
			}

			static void deleteShader(Shader &shader)
			{
				// Delete the shaders as they're linked into our program now and no longer necessery
				glDeleteShader(shader.vertexShaderID);
				glDeleteShader(shader.fragmentShaderID);
			}

			static Shader* ptrLinkFragmentFromString(const Shader &ori, const std::string &fragmentShaderCode)
			{
				GLchar infoLog[512];
				GLint success;
				Shader* shader = new Shader;
				shader->vertexShaderID = ori.vertexShaderID;
				const GLchar *fscp = fragmentShaderCode.c_str();
				shader->fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
				glShaderSource(shader->fragmentShaderID, 1, &fscp, NULL);
				glCompileShader(shader->fragmentShaderID);
				glGetShaderiv(shader->fragmentShaderID, GL_COMPILE_STATUS, &success);
				if (!success) {
					glGetShaderInfoLog(shader->fragmentShaderID, 512, NULL, infoLog);
					printf("Failed to compile fragmentShader:%s \n", infoLog);
				}

				shader->programID = glCreateProgram();
				glAttachShader(shader->programID, shader->vertexShaderID);
				glAttachShader(shader->programID, shader->fragmentShaderID);
				glLinkProgram(shader->programID);

				glGetShaderiv(shader->programID, GL_LINK_STATUS, &success);
				if (!success) {
					glGetProgramInfoLog(shader->programID, 512, NULL, infoLog);
					printf("Failed to link shaderProgram:%s \n", infoLog);
				}

				return shader;
			}

			static Shader linkFragmentFromString(const Shader &ori, const std::string &fragmentShaderCode)
			{
				GLchar infoLog[512];
				GLint success;
				Shader shader;
				shader.vertexShaderID = ori.vertexShaderID;
				const GLchar *fscp = fragmentShaderCode.c_str();
				shader.fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
				glShaderSource(shader.fragmentShaderID, 1, &fscp, NULL);
				glCompileShader(shader.fragmentShaderID);
				glGetShaderiv(shader.fragmentShaderID, GL_COMPILE_STATUS, &success);
				if (!success) {
					glGetShaderInfoLog(shader.fragmentShaderID, 512, NULL, infoLog);
					printf("Failed to compile fragmentShader:%s \n", infoLog);
				}

				shader.programID = glCreateProgram();
				glAttachShader(shader.programID, shader.vertexShaderID);
				glAttachShader(shader.programID, shader.fragmentShaderID);
				glLinkProgram(shader.programID);

				glGetShaderiv(shader.programID, GL_LINK_STATUS, &success);
				if (!success) {
					glGetProgramInfoLog(shader.programID, 512, NULL, infoLog);
					printf("Failed to link shaderProgram:%s \n", infoLog);
				}

				return shader;
			}
		};
	}
}
