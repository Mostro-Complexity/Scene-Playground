#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h> // Include glew to get all the required OpenGL headers

namespace mostro
{
	namespace modeling
	{
		class Shader
		{
		public:
			Shader(const GLchar* vertexPath, const GLchar* fragmentPath)
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
				const GLchar* vShaderCode = vertexCode.c_str();
				const GLchar * fShaderCode = fragmentCode.c_str();
				// 2. Compile shaders
				GLuint vertex, fragment;
				GLint success;
				GLchar infoLog[512];
				// Vertex Shader
				vertex = glCreateShader(GL_VERTEX_SHADER);
				glShaderSource(vertex, 1, &vShaderCode, NULL);
				glCompileShader(vertex);
				// Print compile errors if any
				glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
				if (!success)
				{
					glGetShaderInfoLog(vertex, 512, NULL, infoLog);
					std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
				}
				// Fragment Shader
				fragment = glCreateShader(GL_FRAGMENT_SHADER);
				glShaderSource(fragment, 1, &fShaderCode, NULL);
				glCompileShader(fragment);
				// Print compile errors if any
				glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
				if (!success)
				{
					glGetShaderInfoLog(fragment, 512, NULL, infoLog);
					std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
				}
				// Shader Program
				programID = glCreateProgram();
				glAttachShader(this->programID, vertex);
				glAttachShader(this->programID, fragment);
				glLinkProgram(this->programID);
				// Print linking errors if any
				glGetProgramiv(this->programID, GL_LINK_STATUS, &success);
				if (!success)
				{
					glGetProgramInfoLog(this->programID, 512, NULL, infoLog);
					std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
				}
				// Delete the shaders as they're linked into our program now and no longer necessery
				glDeleteShader(vertex);
				glDeleteShader(fragment);
			}


			~Shader()
			{
			}

			void use()
			{
				glUseProgram(this->programID);
			}

			GLuint programID;

		};
	}
}
