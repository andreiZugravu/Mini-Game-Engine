#include "stdafx.h"
#include "Texture.h"
#include <iostream>
#include <cstring>

Texture::Texture()
{

}


Texture::~Texture()
{
	if (tr)
		delete tr;
}

void Texture::Load(TextureResource * res)
{
	int _GL_TEXTURE_TYPE;
	if (res->type == _2d)
		_GL_TEXTURE_TYPE = GL_TEXTURE_2D;
	else if (res->type == cubemap)
		_GL_TEXTURE_TYPE = GL_TEXTURE_CUBE_MAP;
	
	this->tr = new TextureResource;

	tr->mag_filter = res->mag_filter;
	tr->min_filter = res->min_filter;
	tr->path = res->path;
	tr->type = res->type;
	tr->wrap_s = res->wrap_s;
	tr->wrap_t = res->wrap_t;

	glEnable(GL_DEPTH_TEST);

	int err1 = glGetError();

	glGenTextures(1, &this->textureId); // generam id-ul de textura

	glBindTexture(_GL_TEXTURE_TYPE, textureId); //	deschidem textura

	glTexParameteri(_GL_TEXTURE_TYPE, GL_TEXTURE_MIN_FILTER, tr->min_filter); // cum se comporta textura cand trebuie marita / micsorata
	glTexParameteri(_GL_TEXTURE_TYPE, GL_TEXTURE_MAG_FILTER, tr->mag_filter); // !! min si max

	glTexParameteri(_GL_TEXTURE_TYPE, GL_TEXTURE_WRAP_S, tr->wrap_s); // cum se comporta structura cand u,v sunt in afara intervalului [0, 1]
	glTexParameteri(_GL_TEXTURE_TYPE, GL_TEXTURE_WRAP_T, tr->wrap_t);
	
	int width, height, bpp;
	std::string correctPath = "../../" + tr->path;

	char * pOut = LoadTGA(correctPath.c_str(), &width, &height, &bpp);

	int format;
	if (bpp == 32)
		format = GL_RGBA;
	else
		format = GL_RGB;

	

	if (tr->type == _2d)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, pOut);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else if (tr->type == cubemap)
	{
		int correctbpp = bpp / 8;
		int offset[6];

		offset[0] = (((height / 3) * width) + (2 * (width / 4))) * correctbpp;
		offset[1] = ((height / 3) * width) * correctbpp;
		offset[2] = (width / 4) * correctbpp;
		offset[3] = ((2 * ((height / 3)) * width) + (width / 4)) * correctbpp;
		offset[4] = (((height / 3) * width) + (width / 4)) * correctbpp;
		offset[5] = (((height / 3) * width) + (3 * (width / 4))) * correctbpp;
		
		for (int k = 0; k < 6 ; ++k)
		{
			char * pOutCubeMap = new char[width / 4 * height / 3 * bpp / 8];
			
			for (int i = 0; i < height / 3 ; ++i)
			{
				memcpy(pOutCubeMap + i * width / 4 * correctbpp, pOut + offset[k] + i * width * correctbpp, width / 4 * correctbpp);
			}

			glTexImage2D(k + GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, format, width / 4, height / 3, 0, format, GL_UNSIGNED_BYTE, pOutCubeMap);
			
			delete pOutCubeMap;
		}
		
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}
}

GLuint Texture::getTextureId()
{
	return textureId;
}
