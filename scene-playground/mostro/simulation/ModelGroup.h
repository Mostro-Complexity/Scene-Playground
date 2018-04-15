#pragma once
#include "Group.h"
#include "ShaderGroup.h"

namespace mostro
{
	namespace simulation
	{
		class ModelGroup : public Group
		{
		public:
			ModelGroup(ShaderGroup *shaderGroup)
				: shaderGroup(std::shared_ptr<ShaderGroup>(shaderGroup))
			{
				init();
			}

			void render() override
			{
				// Clear the screen
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				shaderGroup->render();

				// 1rst attribute buffer : vertices
				glEnableVertexAttribArray(0);
				glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
				glVertexAttribPointer(
					0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
					3,                  // size
					GL_FLOAT,           // type
					GL_FALSE,           // normalized?
					0,                  // stride
					(void*)0            // array buffer offset
				);

				// 2nd attribute buffer : UVs
				glEnableVertexAttribArray(1);
				glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
				glVertexAttribPointer(
					1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
					2,                                // size : U+V => 2
					GL_FLOAT,                         // type
					GL_FALSE,                         // normalized?
					0,                                // stride
					(void*)0                          // array buffer offset
				);

				// Draw the triangle !
				glDrawArrays(GL_TRIANGLES, 0, 12 * 3); // 12*3 indices starting at 0 -> 12 triangles

				glDisableVertexAttribArray(0);
				glDisableVertexAttribArray(1);
			}

			void destroy()
			{
				// Cleanup VBO and shader
				glDeleteBuffers(1, &vertexbuffer);
				glDeleteBuffers(1, &uvbuffer);
				glDeleteVertexArrays(1, &VertexArrayID);

				glDeleteProgram(shaderGroup->programID);

				shaderGroup->destroy();
			}

			void setShaderGroup(ShaderGroup *shaderGroup)
			{
				this->shaderGroup = std::shared_ptr<ShaderGroup>(shaderGroup);
			}

			std::shared_ptr<ShaderGroup> getShaderGroup()
			{
				return shaderGroup;
			}
		private:
			std::shared_ptr<ShaderGroup> shaderGroup;
			GLuint VertexArrayID;
			GLuint vertexbuffer;
			GLuint uvbuffer;

			void init()
			{
				// Our vertices. Tree consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
				// A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
				static const GLfloat g_vertex_buffer_data[] = {
					-1.0f,-1.0f,-1.0f,
					-1.0f,-1.0f, 1.0f,
					-1.0f, 1.0f, 1.0f,
					1.0f, 1.0f,-1.0f,
					-1.0f,-1.0f,-1.0f,
					-1.0f, 1.0f,-1.0f,
					1.0f,-1.0f, 1.0f,
					-1.0f,-1.0f,-1.0f,
					1.0f,-1.0f,-1.0f,
					1.0f, 1.0f,-1.0f,
					1.0f,-1.0f,-1.0f,
					-1.0f,-1.0f,-1.0f,
					-1.0f,-1.0f,-1.0f,
					-1.0f, 1.0f, 1.0f,
					-1.0f, 1.0f,-1.0f,
					1.0f,-1.0f, 1.0f,
					-1.0f,-1.0f, 1.0f,
					-1.0f,-1.0f,-1.0f,
					-1.0f, 1.0f, 1.0f,
					-1.0f,-1.0f, 1.0f,
					1.0f,-1.0f, 1.0f,
					1.0f, 1.0f, 1.0f,
					1.0f,-1.0f,-1.0f,
					1.0f, 1.0f,-1.0f,
					1.0f,-1.0f,-1.0f,
					1.0f, 1.0f, 1.0f,
					1.0f,-1.0f, 1.0f,
					1.0f, 1.0f, 1.0f,
					1.0f, 1.0f,-1.0f,
					-1.0f, 1.0f,-1.0f,
					1.0f, 1.0f, 1.0f,
					-1.0f, 1.0f,-1.0f,
					-1.0f, 1.0f, 1.0f,
					1.0f, 1.0f, 1.0f,
					-1.0f, 1.0f, 1.0f,
					1.0f,-1.0f, 1.0f
				};

				// Two UV coordinatesfor each vertex. They were created with Blender.
				static const GLfloat g_uv_buffer_data[] = {
					0.000059f, 0.000004f,
					0.000103f, 0.336048f,
					0.335973f, 0.335903f,
					1.000023f, 0.000013f,
					0.667979f, 0.335851f,
					0.999958f, 0.336064f,
					0.667979f, 0.335851f,
					0.336024f, 0.671877f,
					0.667969f, 0.671889f,
					1.000023f, 0.000013f,
					0.668104f, 0.000013f,
					0.667979f, 0.335851f,
					0.000059f, 0.000004f,
					0.335973f, 0.335903f,
					0.336098f, 0.000071f,
					0.667979f, 0.335851f,
					0.335973f, 0.335903f,
					0.336024f, 0.671877f,
					1.000004f, 0.671847f,
					0.999958f, 0.336064f,
					0.667979f, 0.335851f,
					0.668104f, 0.000013f,
					0.335973f, 0.335903f,
					0.667979f, 0.335851f,
					0.335973f, 0.335903f,
					0.668104f, 0.000013f,
					0.336098f, 0.000071f,
					0.000103f, 0.336048f,
					0.000004f, 0.671870f,
					0.336024f, 0.671877f,
					0.000103f, 0.336048f,
					0.336024f, 0.671877f,
					0.335973f, 0.335903f,
					0.667969f, 0.671889f,
					1.000004f, 0.671847f,
					0.667979f, 0.335851f
				};

				glGenVertexArrays(1, &VertexArrayID);
				glBindVertexArray(VertexArrayID);

				glGenBuffers(1, &vertexbuffer);
				glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
				glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

				glGenBuffers(1, &uvbuffer);
				glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
				glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data), g_uv_buffer_data, GL_STATIC_DRAW);
			}
		};
	}
}