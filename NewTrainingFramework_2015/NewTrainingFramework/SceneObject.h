#pragma once

#include "../Utilities/utilities.h"
#include "Model.h"
#include "Shader.h"
#include "Texture.h"
#include "Trajectory.h"
#include <vector>

enum ObjectType {
	normalObj,
	terrain,
	skyBox,
	fire
};

class SceneObject
{
protected:
	int id;
	Vector3 position;
	Vector3 rotation;
	Vector3 scale;
	Model * model;
	Shader * shader;
	std::vector < Texture * > textures;
	bool depthTest;
	ObjectType type;

	Trajectory * trajectory;
	Vector3 startPoint;
	Vector3 endPoint;
	int sens;

	Matrix ModelMatrix;

	void sendCommonData();

public:
	SceneObject();
	virtual ~SceneObject();

	void setId(int Id);
	void setPosition(Vector3 Position);
	void setRotation(Vector3 Rotation);
	void setScale(Vector3 Scale);
	void setModel(Model * m);
	void setShader(Shader * s);
	void setTextures(std::vector < Texture * > t);
	void setDepthTest(bool DepthTest);

	void setType(ObjectType t);
	ObjectType getType();

	void setTrajectory(Trajectory * tr);
	void setStartPoint(Vector3 sp);
	void setEndPoint(Vector3 ep);

	int getId();

	virtual void Draw();
	virtual void Update(float deltaTime);
	virtual void freeResources();
};

