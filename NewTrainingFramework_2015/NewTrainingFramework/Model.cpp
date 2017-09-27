#include "stdafx.h"
#include "Model.h"
#include <fstream>
#include <iostream>

Model::Model()
{

}


Model::~Model()
{
	if (mr)
		;// delete mr;
}

void Model::freeResources()
{
	if (mr != nullptr)
		delete mr;

	glDeleteBuffers(1, &this->vboId);
	glDeleteBuffers(1, &this->iboId);
}

GLuint & Model::getIboId()
{
	return iboId;
}

GLuint & Model::getWiredIboId()
{
	return wiredIboId;
}

GLuint & Model::getVboId()
{
	return vboId;
}

int Model::getNrIndices()
{
	return nrIndices;
}

int Model::getNrIndicesWired()
{
	return nrIndicesWired;
}

void Model::setIboId(GLuint iboid)
{
	iboId = iboid;
}

void Model::setWiredIboId(GLuint wirediboid)
{
	wiredIboId = wirediboid;
}

void Model::setVboId(GLuint vboid)
{
	vboId = vboid;
}

void Model::setNrIndices(int nrindices)
{
	nrIndices = nrindices;
}

void Model::setNrIndicesWired(int nrindiceswired)
{
	nrIndicesWired = nrindiceswired;
}

void Model::Load(ModelResource * res)
{
	GLushort * indexArray;
	Vertex * vertexArray;
	GLushort * wiredIndices;

	this->mr = res; //copy the pointer
	
	std::string filePath("../../");
	filePath += res->path;

	std::ifstream fin(filePath);

	char buffer[200];
	int bufferLength, i, j, k;

	fin.getline(buffer, 200, '\n');
	
	bufferLength = strlen(buffer);
	i = bufferLength - 1;
	while (buffer[i] >= '0' && buffer[i] <= '9') i--;
	
	i++;
	int nrVertices = 0;
	while (buffer[i] >= '0' && buffer[i] <= '9')
	{
		nrVertices = nrVertices * 10 + (buffer[i] - '0');
		i++;
	}
	
	vertexArray = new Vertex[nrVertices * sizeof(Vertex)];
	char auxNr[20];
	bool negative;
	double nr;
	for (i = 0; i < nrVertices; ++i)
	{
		fin.getline(buffer, 200, '\n');

		j = 0;

		/*							POS							*/


		negative = false;
		while (buffer[j] != '[') j++;
		j++;

		if (buffer[j] == '-')
		{
			negative = true;
			j++;
		}

		k = 0;
		while ((buffer[j] >= '0' && buffer[j] <= '9') || buffer[j] == '.')
		{
			auxNr[k] = buffer[j];
			k++;
			j++;
		}
		auxNr[k] = '\0';

		nr = std::atof(auxNr);
		if (negative) nr *= -1;
		vertexArray[i].pos.x = nr;


		negative = false;
		while (buffer[j] != '-' && (buffer[j] < '0' || buffer[j] > '9')) j++;

		if (buffer[j] == '-')
		{
			negative = true;
			j++;
		}

		k = 0;
		while ((buffer[j] >= '0' && buffer[j] <= '9') || buffer[j] == '.')
		{
			auxNr[k] = buffer[j];
			k++;
			j++;
		}
		auxNr[k] = '\0';

		nr = std::atof(auxNr);
		if (negative) nr *= -1;
		vertexArray[i].pos.y = nr;

		negative = false;
		while (buffer[j] != '-' && (buffer[j] < '0' || buffer[j] > '9')) j++;

		if (buffer[j] == '-')
		{
			negative = true;
			j++;
		}

		k = 0;
		while ((buffer[j] >= '0' && buffer[j] <= '9') || buffer[j] == '.')
		{
			auxNr[k] = buffer[j];
			k++;
			j++;
		}
		auxNr[k] = '\0';

		nr = std::atof(auxNr);
		if (negative) nr *= -1;
		vertexArray[i].pos.z = nr;

		//std::cout << vertexArray[i].pos.x << " " << vertexArray[i].pos.y << " " << vertexArray[i].pos.z << "\n";
		/*							NORM							*/


		negative = false;
		while (buffer[j] != '[') j++;
		j++;

		if (buffer[j] == '-')
		{
			negative = true;
			j++;
		}

		k = 0;
		while ((buffer[j] >= '0' && buffer[j] <= '9') || buffer[j] == '.')
		{
			auxNr[k] = buffer[j];
			k++;
			j++;
		}
		auxNr[k] = '\0';

		nr = std::atof(auxNr);
		if (negative) nr *= -1;
		vertexArray[i].norm.x = nr;


		negative = false;
		while (buffer[j] != '-' && (buffer[j] < '0' || buffer[j] > '9')) j++;

		if (buffer[j] == '-')
		{
			negative = true;
			j++;
		}

		k = 0;
		while ((buffer[j] >= '0' && buffer[j] <= '9') || buffer[j] == '.')
		{
			auxNr[k] = buffer[j];
			k++;
			j++;
		}
		auxNr[k] = '\0';

		nr = std::atof(auxNr);
		if (negative) nr *= -1;
		vertexArray[i].norm.y = nr;

		negative = false;
		while (buffer[j] != '-' && (buffer[j] < '0' || buffer[j] > '9')) j++;

		if (buffer[j] == '-')
		{
			negative = true;
			j++;
		}

		k = 0;
		while ((buffer[j] >= '0' && buffer[j] <= '9') || buffer[j] == '.')
		{
			auxNr[k] = buffer[j];
			k++;
			j++;
		}
		auxNr[k] = '\0';

		nr = std::atof(auxNr);
		if (negative) nr *= -1;
		vertexArray[i].norm.z = nr;

		//std::cout << vertexArray[i].norm.x << " " << vertexArray[i].norm.y << " " << vertexArray[i].norm.z << "\n";
		/*							BINORM							*/


		negative = false;
		while (buffer[j] != '[') j++;
		j++;

		if (buffer[j] == '-')
		{
			negative = true;
			j++;
		}

		k = 0;
		while ((buffer[j] >= '0' && buffer[j] <= '9') || buffer[j] == '.')
		{
			auxNr[k] = buffer[j];
			k++;
			j++;
		}
		auxNr[k] = '\0';

		nr = std::atof(auxNr);
		if (negative) nr *= -1;
		vertexArray[i].binorm.x = nr;


		negative = false;
		while (buffer[j] != '-' && (buffer[j] < '0' || buffer[j] > '9')) j++;

		if (buffer[j] == '-')
		{
			negative = true;
			j++;
		}

		k = 0;
		while ((buffer[j] >= '0' && buffer[j] <= '9') || buffer[j] == '.')
		{
			auxNr[k] = buffer[j];
			k++;
			j++;
		}
		auxNr[k] = '\0';

		nr = std::atof(auxNr);
		if (negative) nr *= -1;
		vertexArray[i].binorm.y = nr;

		negative = false;
		while (buffer[j] != '-' && (buffer[j] < '0' || buffer[j] > '9')) j++;

		if (buffer[j] == '-')
		{
			negative = true;
			j++;
		}

		k = 0;
		while ((buffer[j] >= '0' && buffer[j] <= '9') || buffer[j] == '.')
		{
			auxNr[k] = buffer[j];
			k++;
			j++;
		}
		auxNr[k] = '\0';

		nr = std::atof(auxNr);
		if (negative) nr *= -1;
		vertexArray[i].binorm.z = nr;

		//std::cout << vertexArray[i].binorm.x << " " << vertexArray[i].binorm.y << " " << vertexArray[i].binorm.z << "\n";
		/*							TGT							*/


		negative = false;
		while (buffer[j] != '[') j++;
		j++;

		if (buffer[j] == '-')
		{
			negative = true;
			j++;
		}

		k = 0;
		while ((buffer[j] >= '0' && buffer[j] <= '9') || buffer[j] == '.')
		{
			auxNr[k] = buffer[j];
			k++;
			j++;
		}
		auxNr[k] = '\0';

		nr = std::atof(auxNr);
		if (negative) nr *= -1;
		vertexArray[i].tgt.x = nr;


		negative = false;
		while (buffer[j] != '-' && (buffer[j] < '0' || buffer[j] > '9')) j++;

		if (buffer[j] == '-')
		{
			negative = true;
			j++;
		}

		k = 0;
		while ((buffer[j] >= '0' && buffer[j] <= '9') || buffer[j] == '.')
		{
			auxNr[k] = buffer[j];
			k++;
			j++;
		}
		auxNr[k] = '\0';

		nr = std::atof(auxNr);
		if (negative) nr *= -1;
		vertexArray[i].tgt.y = nr;

		negative = false;
		while (buffer[j] != '-' && (buffer[j] < '0' || buffer[j] > '9')) j++;

		if (buffer[j] == '-')
		{
			negative = true;
			j++;
		}

		k = 0;
		while ((buffer[j] >= '0' && buffer[j] <= '9') || buffer[j] == '.')
		{
			auxNr[k] = buffer[j];
			k++;
			j++;
		}
		auxNr[k] = '\0';

		nr = std::atof(auxNr);
		if (negative) nr *= -1;
		vertexArray[i].tgt.z = nr;

		//std::cout << vertexArray[i].tgt.x << " " << vertexArray[i].tgt.y << " " << vertexArray[i].tgt.z << "\n";
		/*							UV							*/


		negative = false;
		while (buffer[j] != '[') j++;
		j++;

		if (buffer[j] == '-')
		{
			negative = true;
			j++;
		}

		k = 0;
		while ((buffer[j] >= '0' && buffer[j] <= '9') || buffer[j] == '.')
		{
			auxNr[k] = buffer[j];
			k++;
			j++;
		}
		auxNr[k] = '\0';

		nr = std::atof(auxNr);
		if (negative) nr *= -1;
		vertexArray[i].uv.x = nr;


		negative = false;
		while (buffer[j] != '-' && (buffer[j] < '0' || buffer[j] > '9')) j++;

		if (buffer[j] == '-')
		{
			negative = true;
			j++;
		}

		k = 0;
		while ((buffer[j] >= '0' && buffer[j] <= '9') || buffer[j] == '.')
		{
			auxNr[k] = buffer[j];
			k++;
			j++;
		}
		auxNr[k] = '\0';

		nr = std::atof(auxNr);
		if (negative) nr *= -1;
		vertexArray[i].uv.y = nr;

		//std::cout << vertexArray[i].uv.x << " " << vertexArray[i].uv.y << "\n\n\n";
	}

	fin.getline(buffer, 200, '\n');

	bufferLength = strlen(buffer);
	i = bufferLength - 1;
	while (buffer[i] >= '0' && buffer[i] <= '9') i--;

	i++;
	int nrIndici = 0;
	while (buffer[i] >= '0' && buffer[i] <= '9')
	{
		nrIndici = nrIndici * 10 + (buffer[i] - '0');
		i++;
	}

	indexArray = new GLushort[nrIndici * 3];
	GLushort index;

	k = 0;
	for (i = 0; i < nrIndici / 3; ++i)
	{
		fin.getline(buffer, 200, '\n');

		j = 0;
		while (buffer[j] != '.') j++;
		while (buffer[j] < '0' || buffer[j] > '9') j++;

		index = 0;
		while (buffer[j] != ',')
		{
			index = index * 10 + (buffer[j] - '0');
			j++;
		}

		indexArray[k] = index;
		k++;

		while (buffer[j] < '0' || buffer[j] > '9') j++;

		index = 0;
		while (buffer[j] != ',')
		{
			index = index * 10 + (buffer[j] - '0');
			j++;
		}

		indexArray[k] = index;
		k++;

		while (buffer[j] < '0' || buffer[j] > '9') j++;

		index = 0;
		while (buffer[j] != '\0')
		{
			index = index * 10 + (buffer[j] - '0');
			j++;
		}

		indexArray[k] = index;
		k++;

	}

	fin.close();

	int nrWiredIndices = 2 * nrIndici;

	wiredIndices = new GLushort[nrWiredIndices];

	k = 0;
	for (int i = 0; i < nrIndici; i += 3)
	{
		wiredIndices[k++] = indexArray[i];       wiredIndices[k++] = indexArray[i + 1]; //segment AB

		wiredIndices[k++] = indexArray[i + 1];   wiredIndices[k++] = indexArray[i + 2]; //segment BC

		wiredIndices[k++] = indexArray[i + 2];   wiredIndices[k++] = indexArray[i];	    //segment CA
	}

	//copy everything

	this->nrIndices = nrIndici;
	this->nrIndicesWired = nrWiredIndices;

	glGenBuffers(1, &this->vboId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, nrVertices * sizeof(Vertex), vertexArray, GL_STATIC_DRAW);

	glGenBuffers(1, &this->iboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, nrIndici * sizeof(GLushort), indexArray, GL_STATIC_DRAW);

	glGenBuffers(1, &this->wiredIboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, wiredIboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, nrWiredIndices * sizeof(GLushort), wiredIndices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//dealocate the memory

	delete[] wiredIndices;
	delete[] indexArray;
	delete[] vertexArray;
}
