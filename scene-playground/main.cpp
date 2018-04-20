// GL includes
#include "mostro\modeling\Shader.h"

#include "mostro\utility\Group.h"
#include "mostro\utility\CameraGroup.h"
#include "mostro\utility\WindowGroup.h"
#include "mostro\utility\ModelGroup.h"

// light position
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

using namespace mostro::utility;
using namespace mostro::modeling;

int main()
{
	WindowGroup windowGroup(1024, 768, "title");
	// Draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	Shader *shader = new Shader("TransformVertexShader.vertexshader", "TextureFragmentShader.fragmentshader");
	ModelGroup *ourModel = new ModelGroup("spookytree.obj", shader);
	windowGroup.addModelGroup(ourModel);
	windowGroup.setCameraGroup(new CameraGroup());

	// Game loop
	windowGroup.render();

	return 0;
}
