#pragma once
#include "ModelResource.h"
#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include "Vertex.h"

class Model
{
private:
	ModelResource * mr;
	GLuint iboId;
	GLuint wiredIboId;
	GLuint vboId;
	int nrIndices;
	int nrIndicesWired;

public:
	Model();
	~Model();

	void Load(ModelResource * res);
	void freeResources();

	GLuint & getIboId();
	GLuint & getWiredIboId();
	GLuint & getVboId();
	int getNrIndices();
	int getNrIndicesWired();

	void setIboId(GLuint iboid);
	void setWiredIboId(GLuint wirediboid);
	void setVboId(GLuint vboid);
	void setNrIndices(int nrindices);
	void setNrIndicesWired(int nrindiceswired);
};

