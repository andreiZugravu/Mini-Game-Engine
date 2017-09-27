#pragma once

#include "SceneObject.h"

class Terrain : public SceneObject
{
private:

	struct TerrainVertex {
		Vector3 pos;
		Vector3 norm;
		Vector2 uv_cell;
		Vector2 uv_map;
	};

	int nr_celule;
	int dim_celula;
	int	offsetY;
	Vector3 inaltimi;
	Vector2 offset_uv;

	GLushort * indexArray;
	TerrainVertex * vertexArray;
	GLushort * wiredIndices;
	int nrVertices;
	int nrIndici;
	int nrWiredIndices;

public:

	Terrain();
	~Terrain();

	int getNrCelule();
	int getDimCelula();
	int getOffsetY();
	Vector3 getInaltimi();

	void setNrCelule(int nrCelule);
	void setDimCelula(int dimCelula);
	void setOffsetY(int offsety);
	void setInaltimi(Vector3 inalt);

	void generateModel();

	void Draw();
	void Update(float deltaTime);
	void freeResources();
};

