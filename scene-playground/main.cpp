#include "stdafx.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
using namespace glm;
using namespace Assimp;
using namespace mostro::simulation;

int main(void)
{
	WindowGroup windowGroup(1024, 768, "initial scene");

	windowGroup.init();
	windowGroup.setCameraGroup(new CameraGroup());
	windowGroup.setShaderGroup(new ShaderGroup("TransformVertexShader.vertexshader",
		"TextureFragmentShader.fragmentshader"));
	windowGroup.setTextureGroup(new TextureGroup("uvtemplate.DDS"));

	windowGroup.render();

	windowGroup.exit();
	return 0;
}

