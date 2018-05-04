// GL includes
#include "mostro\modeling\Shader.h"

#include "mostro\utility\Group.h"
#include "mostro\utility\CameraGroup.h"
#include "mostro\utility\WindowGroup.h"
#include "mostro\utility\ModelGroup.h"

#include "mostro\Universe.h"
// light position
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

using namespace mostro::utility;
using namespace mostro::modeling;
using namespace mostro;
using namespace std;

int main()
{
	WindowGroup *windowGroup = new WindowGroup(1024, 768, "title");
	// Draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	Shader *shader = new Shader("TransformVertexShader.vertexshader", "TextureFragmentShader.fragmentshader");
	ModelGroup *ourModel = new ModelGroup("face.obj", shader);

	vector<shared_ptr<Planet>> p = {
		shared_ptr<Planet>(new Planet(ourModel, 5.965e35f, glm::vec3(0, 3e2f, 3.2e2f), glm::vec3(0, 0, 0))),
		shared_ptr<Planet>(new Planet(ourModel, 5.965e35f, glm::vec3(3.8e2f, 0.f, 4.32e2f), glm::vec3(0, 0, 0))),
		shared_ptr<Planet>(new Planet(ourModel, 5.965e35f, glm::vec3(3.54e2f, 3.86e2f, 0.f), glm::vec3(0, 0, 0))),
		//shared_ptr<Planet>(new Planet(ourModel, 5.965e4, glm::vec3(0, 1e2, 1.2e1), glm::vec3(0, 0, 0))),
		//shared_ptr<Planet>(new Planet(ourModel, 5.965e4, glm::vec3(1.8e1, 0, 1.32e2), glm::vec3(0, 0, 0))),
		//shared_ptr<Planet>(new Planet(ourModel, 5.965e4, glm::vec3(1.54e3, 1.86e2, 0), glm::vec3(0, 0, 0)))
	};


	Universe *uni = new Universe(p);
	windowGroup->addModelGroup(uni);
	windowGroup->setCameraGroup(new CameraGroup());

	// Game loop
	windowGroup->render();

	return 0;
}
