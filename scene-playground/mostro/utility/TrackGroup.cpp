#include "TrackGroup.h"

using namespace std;
using namespace mostro::utility;
using namespace mostro::modeling;

TrackGroup::TrackGroup(shared_ptr<Shader> shader) : utility::ModelGroup()
{
	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

	const string vertexShaderCode =
		"#version 330 core\n"
		"layout(location = 0) in vec3 position;\n"
		"uniform mat4 model\n;uniform mat4 view;\nuniform mat4 projection;\n"
		"void main(){gl_Position = projection * view * model * vec4(position, 1.0f);}\n\0";

	const string fragmentShaderCode =
		"#version 330 core\n"
		"out vec4 color;\n"
		"void main(){color = vec4(1.0, 1.0, 1.0, 1.0);}\n\0";

	this->shader = shared_ptr<Shader>(ShaderGenerator::ptrLinkFragmentFromString(*shader, fragmentShaderCode));
}

TrackGroup::~TrackGroup()
{
}

void TrackGroup::addVertex(float x, float y, float z)
{
	trackList.push_back(x);
	trackList.push_back(y);
	trackList.push_back(z);
}

void TrackGroup::render()
{

	glBufferData(GL_ARRAY_BUFFER, trackList.size() * sizeof(GLfloat), &trackList[0], GL_STATIC_DRAW);
	shader->use();

	glUniformMatrix4fv(
		glGetUniformLocation(this->shader->programID, "model"),
		1,
		GL_FALSE,
		glm::value_ptr(model)
	);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	//glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
		0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(GLvoid*)0            // array buffer offset
	);

	glDrawArrays(GL_LINE_STRIP, 0, trackList.size() / 3); // 12*3 indices starting at 0 -> 12 triangles
	glDisableVertexAttribArray(0);

	// glDeleteBuffers(1, &vertexbuffer);
}

