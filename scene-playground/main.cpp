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
	shared_ptr<WindowGroup> window(new WindowGroup(1024, 768, "title"));

	// Draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	Shader *shader = new Shader("TransformVertexShader.vertexshader", "TextureFragmentShader.fragmentshader");
	ModelGroup *ourModel = new ModelGroup("face.obj", shader);

	vector<shared_ptr<Planet>> p = {
		shared_ptr<Planet>(new Planet(ourModel, 5.965e11f, glm::vec3(9e2f, 9e2f, 9e2f), glm::vec3(0, 0, 0))),
		shared_ptr<Planet>(new Planet(ourModel, 5.965e10f, glm::vec3(6e2f, 6e2f, 6e2f), glm::vec3(0, 0, 0))),
		shared_ptr<Planet>(new Planet(ourModel, 5.965e11f, glm::vec3(3e2f, 3e2f, 3e2f), glm::vec3(0, 0, 0))),
		shared_ptr<Planet>(new Planet(ourModel, 5.965e10f, glm::vec3(0.f, 3e2f, 0.f), glm::vec3(0, 0, 0))),
		shared_ptr<Planet>(new Planet(ourModel, 5.965e11f, glm::vec3(3e2f, 6e2f, 3e2f), glm::vec3(0, 0, 0))),
		shared_ptr<Planet>(new Planet(ourModel, 5.965e10f, glm::vec3(6e2f, 9e2f, 6e2f), glm::vec3(0, 0, 0))),
		shared_ptr<Planet>(new Planet(ourModel, 5.965e11f, glm::vec3(0.f, 6e2f,0.f), glm::vec3(0, 0, 0))),
		shared_ptr<Planet>(new Planet(ourModel, 5.965e10f, glm::vec3(3e2f, 0.f, 0.f), glm::vec3(0, 0, 0))),
		shared_ptr<Planet>(new Planet(ourModel, 5.965e11f, glm::vec3(6e2f, 3e2f, 6e2f), glm::vec3(0, 0, 0)))
	};


	Universe *uni = new Universe(p);
	window->addModelGroup(uni);
	window->setCameraGroup(new CameraGroup());

	// Game loop
	window->render();

	return 0;
}
