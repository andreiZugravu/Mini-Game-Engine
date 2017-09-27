#include "stdafx.h"
#include "../Include/rapidxml-1.13/rapidxml.hpp"
#include "ResourceManager.h"
#include <String>
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>

ResourceManager * ResourceManager::rm_instance = NULL;

ResourceManager::ResourceManager()
{

}


ResourceManager * ResourceManager::getInstance()
{
	if (!rm_instance)
	{
		rm_instance = new ResourceManager();
	}

	return rm_instance;
}

void ResourceManager::init()
{
	//parse the xml document

	//load the xml document

	rapidxml::xml_document<> doc;
	std::ifstream fin("resourceManager.xml");
	std::stringstream buffer;
	buffer << fin.rdbuf();
	fin.close();
	std::string content(buffer.str());
	doc.parse<0>(&content[0]);

	rapidxml::xml_node<> * pRoot = doc.first_node();

	rapidxml::xml_node<> * pNodeModels = pRoot->first_node("models");
	rapidxml::xml_node<> * pNodeTextures = pRoot->first_node("textures");
	rapidxml::xml_node<> * pNodeShaders = pRoot->first_node("shaders");

	//get the information and store it

	//models
	
	for (rapidxml::xml_node<> * pNode = pNodeModels->first_node("model"); pNode; pNode = pNode->next_sibling())
	{
		rapidxml::xml_attribute<> * attr = pNode->first_attribute("id");
		int id = atoi(attr->value());

		modelsResources[id] = new ModelResource();
		rapidxml::xml_node<> * pNodePath = pNode->first_node("path");

		modelsResources[id]->path = pNodePath->value();
	}

	//textures

	for (rapidxml::xml_node<> * pNode = pNodeTextures->first_node("texture"); pNode; pNode = pNode->next_sibling())
	{
		rapidxml::xml_attribute<> * attr;

		attr = pNode->first_attribute("id");
		int id = atoi(attr->value());

		texturesResources[id] = new TextureResource();

		attr = pNode->first_attribute("type");

		if (strcmp(attr->value(), "2d") == 0) // equal
			texturesResources[id]->type = _2d;
		else if(strcmp(attr->value(), "3d") == 0)
			texturesResources[id]->type = _3d;
		else if (strcmp(attr->value(), "cubemap") == 0)
			texturesResources[id]->type = cubemap;

		rapidxml::xml_node<> * pSubNodes;
		
		pSubNodes = pNode->first_node("path");

		texturesResources[id]->path = pSubNodes->value();

		pSubNodes = pNode->first_node("min_filter");
		
		if (strcmp(pSubNodes->value(), "GL_LINEAR") == 0) // equal
			texturesResources[id]->min_filter = GL_LINEAR;
		else if(strcmp(pSubNodes->value(), "GL_LINEAR_MIPMAP_LINEAR") == 0)
			texturesResources[id]->min_filter = GL_LINEAR_MIPMAP_LINEAR;
		else
			texturesResources[id]->min_filter = GL_NEAREST;

		pSubNodes = pNode->first_node("mag_filter");

		//if (strcmp(pSubNodes->value(), "GL_LINEAR") == 0) // equal
		texturesResources[id]->mag_filter = GL_LINEAR; //don't need to check, cause mag_filter can ONLY be GL_LINEAR :)
		

		pSubNodes = pNode->first_node("wrap_s");

		if (strcmp(pSubNodes->value(), "GL_CLAMP_TO_EDGE") == 0) // equal
			texturesResources[id]->wrap_s = GL_CLAMP_TO_EDGE;
		else if(strcmp(pSubNodes->value(), "GL_REPEAT") == 0)
			texturesResources[id]->wrap_s = GL_REPEAT;
		else
			texturesResources[id]->wrap_s = GL_MIRRORED_REPEAT;

		pSubNodes = pNode->first_node("wrap_t");

		if (strcmp(pSubNodes->value(), "GL_CLAMP_TO_EDGE") == 0) // equal
			texturesResources[id]->wrap_t = GL_CLAMP_TO_EDGE;
		else if (strcmp(pSubNodes->value(), "GL_REPEAT") == 0)
			texturesResources[id]->wrap_t = GL_REPEAT;
		else
			texturesResources[id]->wrap_t = GL_MIRRORED_REPEAT;
	}
	
	//shaders

	for (rapidxml::xml_node<> * pNode = pNodeShaders->first_node("shader"); pNode; pNode = pNode->next_sibling())
	{
		rapidxml::xml_attribute<> * attr = pNode->first_attribute("id");
		int id = atoi(attr->value());

		shadersResources[id] = new ShaderResource;

		rapidxml::xml_node<> * pSubNodes;

		pSubNodes = pNode->first_node("vs");
		
		shadersResources[id]->vs = pSubNodes->value();

		pSubNodes = pNode->first_node("fs");

		shadersResources[id]->fs = pSubNodes->value();
	}
}

void ResourceManager::freeResources()
{
	
	std::map < int, ModelResource * >::iterator it1;
	std::map < int, TextureResource * >::iterator it2;
	std::map < int, ShaderResource * >::iterator it3;

	std::map < int, Model * >::iterator it4;
	std::map < int, Texture * >::iterator it5;
	std::map < int, Shader * >::iterator it6;

	for (it1 = modelsResources.begin(); it1 != modelsResources.end(); ++it1)
	{
		if (it1->second != nullptr)
		{
			delete it1->second;
		}
	}
	
	for (it2 = texturesResources.begin(); it2 != texturesResources.end(); ++it2)
	{
		if (it2->second != nullptr)
		{
			delete it2->second;
		}
	}
	
	for (it3 = shadersResources.begin(); it3 != shadersResources.end(); ++it3)
	{
		if (it3->second != nullptr)
		{
			delete it3->second;
		}
	}

	for (it4 = models.begin(); it4 != models.end(); ++it4)
	{
		if (it4->second != nullptr)
		{
			//it4->second->freeResources();
			delete it4->second;
		}
	}

	for (it5 = textures.begin(); it5 != textures.end(); ++it5)
	{
		if (it5->second != nullptr)
		{
			delete it5->second;
		}
	}

	for (it6 = shaders.begin(); it6 != shaders.end(); ++it6)
	{
		if (it6->second != nullptr && it6->second != NULL)
		{
			delete it6->second;
		}
	}

}

Model * ResourceManager::loadModel(int id)
{
	std::map < int, Model * >::iterator it;
	for (it = models.begin(); it != models.end(); ++it)
	{
		if (it->first == id)
		{
			break;
		}
	}

	if (it == models.end())
	{
		models[id] = new Model;
		models[id]->Load(modelsResources[id]);
	}

	return models[id];
}

Texture * ResourceManager::loadTexture(int id)
{
	std::map < int, Texture * >::iterator it;
	for (it = textures.begin(); it != textures.end(); ++it)
	{
		if (it->first == id)
		{
			break;
		}
	}

	if (it == textures.end())
	{
		textures[id] = new Texture;
		textures[id]->Load(texturesResources[id]); //
	}

	return textures[id];

}

Shader * ResourceManager::loadShader(int id)
{
	std::map < int, Shader * >::iterator it;
	for (it = shaders.begin(); it != shaders.end(); ++it)
	{
		if (it->first == id)
		{
			break;
		}
	}

	if (it == shaders.end())
	{
		shaders[id] = new Shader;
		shaders[id]->Load(shadersResources[id]); //tipul de la shadersResources
	}

	return shaders[id];
}

ResourceManager::~ResourceManager()
{

}
