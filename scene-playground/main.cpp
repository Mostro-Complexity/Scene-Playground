#include "stdafx.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
using namespace glm;
using namespace Assimp;
using namespace mostro::simulation;

int main(void)
{
	// 创建窗口
	WindowGroup windowGroup(1024, 768, "initial scene");

	// 窗口初始化
	windowGroup.init();

	// 创建着色器
	ShaderGroup *shaderGroup = new ShaderGroup("TransformVertexShader.vertexshader",
		"TextureFragmentShader.fragmentshader");

	shaderGroup->setTextureGroup(new TextureGroup("uvtemplate.DDS"));// 加入一个纹理

	windowGroup.setShaderGroup(shaderGroup);// 绑定着色器

	windowGroup.setCameraGroup(new CameraGroup(CameraGroupType::observeCamera));// 设置相机

	// 渲染，按esc结束
	windowGroup.render();

	windowGroup.exit();
	return 0;
}

