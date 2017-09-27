#include "stdafx.h"
#include "Shader.h"
#include <iostream>

Shader::Shader()
{
	sr = NULL;
}


Shader::~Shader()
{

}

void Shader::Load(ShaderResource * res)
{
	this->sr = new ShaderResource;

	std::string correctFilePathVS = "../"; 
	correctFilePathVS += res->vs;

	std::string correctFilePathFS = "../";
	correctFilePathFS += res->fs;

	this->sr->vs = correctFilePathVS;
	this->sr->fs = correctFilePathFS;

	vertexShader = esLoadShader(GL_VERTEX_SHADER, &this->sr->vs[0]);

	if (vertexShader == 0)
	{
		std::cout << "EROARE LA VERTEX SHADER\n";
		return; // -1
	}

	fragmentShader = esLoadShader(GL_FRAGMENT_SHADER, &this->sr->fs[0]);

	if (fragmentShader == 0)
	{
		std::cout << "EROARE LA FRAGMENT SHADER\n";
		glDeleteShader(vertexShader);
		return ; // -2
	}

	program = esLoadProgram(vertexShader, fragmentShader);

	//finding location of uniforms / attributes
	positionAttribute = glGetAttribLocation(program, "a_posL");
	MatrixID = glGetUniformLocation(program, "u_MVP");
	textureUniform = glGetUniformLocation(program, "u_texture");
	uvAttribute = glGetAttribLocation(program, "a_uv");
	uvMapAttribute = glGetAttribLocation(program, "a_uv_map");
	blendMapUniform = glGetUniformLocation(program, "u_blend_map");
	colorRockUniform = glGetUniformLocation(program, "u_color_rock");
	colorDirtUniform = glGetUniformLocation(program, "u_color_dirt");
	colorGrassUniform = glGetUniformLocation(program, "u_color_grass");
	offsetUniform = glGetUniformLocation(program, "u_offset");
	cubeTextureUniform = glGetUniformLocation(program, "u_cube_texture");
	cameraCoordUniform = glGetUniformLocation(program, "u_camera_coord");
	rUniform = glGetUniformLocation(program, "u_r");
	RUniform = glGetUniformLocation(program, "u_R");
	fogColorUniform = glGetUniformLocation(program, "u_fog_color");
	modelMatrixUniform = glGetUniformLocation(program, "u_Model");
	timeUniform = glGetUniformLocation(program, "u_time");
	maximumDisplacementUniform = glGetUniformLocation(program, "u_disp_max");
	displacementTextureUniform = glGetUniformLocation(program, "u_disp");;
	fireTextureUniform = glGetUniformLocation(program, "u_texture_fire");;
	fireMaskTextureUniform = glGetUniformLocation(program, "u_texture_fire_mask");;
	ambientaLightColorUniform = glGetUniformLocation(program, "u_ambiental_light_color");
	diffusalLightColorUniform = glGetUniformLocation(program, "u_diffusal_light_color");
	normAttribute = glGetAttribLocation(program, "a_norm");

	return ; // 0
}

void Shader::freeResources()
{
	if (sr)
		delete sr;

	glDeleteProgram(program);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}