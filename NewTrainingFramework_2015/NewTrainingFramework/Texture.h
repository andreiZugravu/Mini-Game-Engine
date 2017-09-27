#pragma once
#include "TextureResource.h"
#include "../Utilities/utilities.h"

class Texture
{
private:
	TextureResource * tr;
	GLuint textureId;
	
public:
	Texture();
	~Texture();

	void Load(TextureResource * res);
	GLuint getTextureId();
};

