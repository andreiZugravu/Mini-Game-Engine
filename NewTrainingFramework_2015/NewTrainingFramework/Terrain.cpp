#include "stdafx.h"
#include "Terrain.h"
#include "SceneManager.h"
#include <iostream>

Terrain::Terrain()
{

}


Terrain::~Terrain()
{

}

int Terrain::getNrCelule()
{
	return nr_celule;
}

int Terrain::getDimCelula()
{
	return dim_celula;
}

int Terrain::getOffsetY()
{
	return offsetY;
}

Vector3 Terrain::getInaltimi()
{
	return inaltimi;
}

void Terrain::setNrCelule(int nrCelule)
{
	nr_celule = nrCelule;
}

void Terrain::setDimCelula(int dimCelula)
{
	dim_celula = dimCelula;
}

void Terrain::setOffsetY(int offsety)
{
	offsetY = offsety;
}

void Terrain::setInaltimi(Vector3 inalt)
{
	inaltimi = inalt;
}

void Terrain::generateModel()
{
	/*std::cout << this->dim_celula << "\n" << this->nr_celule << "\n" << this->offsetY << "\n";
	std::cout << inaltimi.x << " " << inaltimi.y << " " << inaltimi.z << "\n";*/

	Vector3 centru = Vector3(0, 0, 0); //camera has to exist at this point!!

	nrVertices = (nr_celule + 1) * (nr_celule + 1);

	vertexArray = new TerrainVertex[nrVertices];
	
	int k = 0;
	for (int i = 0; i < nr_celule + 1 ; ++i)
	{
		for (int j = 0; j < nr_celule + 1 ; ++j)
		{
			/*							POS							*/

			vertexArray[k].pos.x = centru.x - (nr_celule / 2 - j) * dim_celula;
			vertexArray[k].pos.y = offsetY;
			vertexArray[k].pos.z = centru.z - (nr_celule / 2 - i) * dim_celula;

			/*							NORM							*/

			vertexArray[k].norm.x = 0;
			vertexArray[k].norm.y = 1;
			vertexArray[k].norm.z = 0;

			/*							BINORM							*/

			/*							TGT							*/

			/*							UV							*/

			vertexArray[k].uv_cell.x = j;
			vertexArray[k].uv_cell.y = i;

			vertexArray[k].uv_map.x = j / (float)nr_celule;
			vertexArray[k].uv_map.y = i / (float)nr_celule;

			++k;
		}
	}
	
	nrIndici = nr_celule * nr_celule * 6;

	indexArray = new GLushort[nrIndici];

	k = 0;
	for (int i = 0; i < nr_celule ; ++i)
	{
		for (int j = 0; j < nr_celule ; ++j)
		{
			indexArray[k++] = i * (nr_celule + 1) + j;
			indexArray[k++] = i * (nr_celule + 1) + j + 1;
			indexArray[k++] = (i + 1) * (nr_celule + 1) + j;

			indexArray[k++] = (i + 1) * (nr_celule + 1) + j + 1;
			indexArray[k++] = i * (nr_celule + 1) + j + 1;
			indexArray[k++] = (i + 1) * (nr_celule + 1) + j;
		}
	}

	
	nrWiredIndices = 2 * nrIndici;

	wiredIndices = new GLushort[nrWiredIndices];

	k = 0;
	for (int i = 0; i < nrIndici; i += 3)
	{
		wiredIndices[k++] = indexArray[i];       wiredIndices[k++] = indexArray[i + 1]; //segment AB

		wiredIndices[k++] = indexArray[i + 1];   wiredIndices[k++] = indexArray[i + 2]; //segment BC

		wiredIndices[k++] = indexArray[i + 2];   wiredIndices[k++] = indexArray[i];	    //segment CA
	}

	//copy everything

	this->model = new Model; //create it here
	this->model->setNrIndices(nrIndici);
	this->model->setNrIndicesWired(nrWiredIndices);

	//this->model->setNrIndicesWired(nrWiredIndices);

glGenBuffers(1, &this->model->getVboId());
glBindBuffer(GL_ARRAY_BUFFER, this->model->getVboId());
glBufferData(GL_ARRAY_BUFFER, nrVertices * sizeof(TerrainVertex), vertexArray, GL_STATIC_DRAW);

glGenBuffers(1, &this->model->getIboId());
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->model->getIboId());
glBufferData(GL_ELEMENT_ARRAY_BUFFER, nrIndici * sizeof(GLushort), indexArray, GL_STATIC_DRAW);

glGenBuffers(1, &this->model->getWiredIboId());
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->model->getWiredIboId());
glBufferData(GL_ELEMENT_ARRAY_BUFFER, nrWiredIndices * sizeof(GLushort), wiredIndices, GL_STATIC_DRAW);

glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

position = Vector3(SceneManager::getInstance()->getActiveCamera().getPosition());
}

void Terrain::Draw()
{
	glUseProgram(shader->program);
	//send common data
	glBindBuffer(GL_ARRAY_BUFFER, model->getVboId());

	if (SceneManager::getInstance()->wiredMode())
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->getWiredIboId());
	else
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->getIboId());

	if (shader->positionAttribute != -1)
	{
		glEnableVertexAttribArray(shader->positionAttribute);
		glVertexAttribPointer(shader->positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(TerrainVertex), 0); //value, not &&
	}

	if (shader->uvAttribute != -1)
	{
		glEnableVertexAttribArray(shader->uvAttribute);
		glVertexAttribPointer(shader->uvAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(TerrainVertex), (void *)(24u));
	}

	if (shader->uvMapAttribute != -1)
	{
		glEnableVertexAttribArray(shader->uvMapAttribute);
		glVertexAttribPointer(shader->uvMapAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(TerrainVertex), (void *)(32u));
	}

	sendCommonData();

	if (shader->offsetUniform != -1)
	{
		glUniform2f(shader->offsetUniform, offset_uv.x, offset_uv.y);
	}

	if (shader->blendMapUniform != -1)
	{
		glUniform1i(shader->blendMapUniform, 0);
	}

	if (shader->colorRockUniform != -1)
	{
		glUniform1i(shader->colorRockUniform, 1);
	}

	if (shader->colorDirtUniform != -1)
	{
		glUniform1i(shader->colorDirtUniform, 2);
	}

	if (shader->colorGrassUniform != -1)
	{
		glUniform1i(shader->colorGrassUniform, 3);
	}

	glActiveTexture(GL_TEXTURE0);

	glBindTexture(GL_TEXTURE_2D, textures[0]->getTextureId());

	glActiveTexture(GL_TEXTURE1);

	glBindTexture(GL_TEXTURE_2D, textures[1]->getTextureId());

	glActiveTexture(GL_TEXTURE2);

	glBindTexture(GL_TEXTURE_2D, textures[2]->getTextureId());

	glActiveTexture(GL_TEXTURE3);

	glBindTexture(GL_TEXTURE_2D, textures[3]->getTextureId());

	if (SceneManager::getInstance()->wiredMode())
		glDrawElements(GL_LINES, model->getNrIndicesWired(), GL_UNSIGNED_SHORT, 0);
	else
		glDrawElements(GL_TRIANGLES, model->getNrIndices(), GL_UNSIGNED_SHORT, 0);

	glDrawElements(GL_TRIANGLES, model->getNrIndices(), GL_UNSIGNED_SHORT, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Terrain::Update(float deltaTime)
{
	Vector3 cameraPos = SceneManager::getInstance()->getActiveCamera().getPosition();

	if (abs(position.x - cameraPos.x) > dim_celula ||
		abs(position.z - cameraPos.z) > dim_celula)
	{
		Vector3 positionDelta = cameraPos - position;
		positionDelta.y = 0;

		positionDelta.x = signum(positionDelta.x) * dim_celula;
		positionDelta.z = signum(positionDelta.z) * dim_celula;

		position += positionDelta;

		ModelMatrix.SetTranslation(position);

		offset_uv += Vector2(positionDelta.x, positionDelta.z) / (float)(dim_celula * nr_celule);
	}

}

void Terrain::freeResources()
{
	delete[] wiredIndices;
	delete[] indexArray;
	delete[] vertexArray;
}
