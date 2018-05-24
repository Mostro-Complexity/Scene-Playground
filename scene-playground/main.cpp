#include <random>
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
	shared_ptr<WindowGroup> window(new WindowGroup(1024, 768, "Planet movement"));

	// Draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	shared_ptr<Shader> shader(
		ShaderGenerator::ptrFromFile("TransformVertexShader.vertexshader",
			"TextureFragmentShader.fragmentshader"));

	shared_ptr<ModelGroup> ourModel(new ModelGroup("face.obj", shader));
	std::default_random_engine random(time(NULL));
	std::uniform_real_distribution<double> posRange(-9e2f, 9e2f), messRange(5.965e10f, 5.965e11f);

	vector<shared_ptr<Planet>> p(20);
	for (size_t i = 0; i < 20; i++)
	{
		p[i] = shared_ptr<Planet>(new Planet(ourModel, messRange(random), glm::vec3(posRange(random), posRange(random), posRange(random)), glm::vec3(0, 0, 0)));
	}

	shared_ptr<Universe> universe(new Universe(p, shader));
	window->addModelGroup(universe);

	for (size_t i = 0; i < universe->tracks.size(); i++)
	{
		window->addModelGroup(universe->tracks[i]);
	}
	window->addModelGroup(shared_ptr<TrackGroup>(new TrackGroup(shader)));

	window->setCameraGroup(shared_ptr<CameraGroup>(new CameraGroup()));

	// Game loop
	window->render();

	return 0;
}
