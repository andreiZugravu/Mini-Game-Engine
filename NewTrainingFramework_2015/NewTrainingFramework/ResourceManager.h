#pragma once
#include <map>
#include "Model.h"
#include "Texture.h"
#include "Shader.h"

class ResourceManager
{
private:
	static ResourceManager * rm_instance;
	ResourceManager();

	std::map < int, ModelResource * > modelsResources;
	std::map < int, TextureResource * > texturesResources;
	std::map < int, ShaderResource * > shadersResources;

	std::map < int, Model * > models;
	std::map < int, Texture * > textures;
	std::map < int, Shader * > shaders;

public:
	static ResourceManager * getInstance();
	
	void init();
	void freeResources();

	Model * loadModel(int id);
	Texture * loadTexture(int id);
	Shader * loadShader(int id);
	
	~ResourceManager();
};