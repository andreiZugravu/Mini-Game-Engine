#pragma once
#include "ShaderResource.h"
#include "../Utilities/utilities.h"

class Shader
{
private:
	ShaderResource * sr;

public:
	int programId;

	GLuint vertexShader;
	GLuint fragmentShader;
	GLuint program;

	GLint positionAttribute;
	GLint MatrixID;
	GLint textureUniform;
	GLint uvAttribute;
	GLint uvMapAttribute;
	GLint blendMapUniform;
	GLint colorRockUniform;
	GLint colorDirtUniform;
	GLint colorGrassUniform;
	GLint offsetUniform;

	GLint cubeTextureUniform;
	
	GLint cameraCoordUniform;
	GLint rUniform;
	GLint RUniform;
	GLint fogColorUniform;
	GLint modelMatrixUniform;

	GLint timeUniform;
	GLint maximumDisplacementUniform;
	GLint displacementTextureUniform;
	GLint fireTextureUniform;
	GLint fireMaskTextureUniform;

	GLint ambientaLightColorUniform;
	GLint diffusalLightColorUniform;
	GLint normAttribute;

public:
	Shader();
	~Shader();

	void Load(ShaderResource * res);
	void freeResources();
};

