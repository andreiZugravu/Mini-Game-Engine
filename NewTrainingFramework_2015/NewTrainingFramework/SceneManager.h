#pragma once

#include "Camera.h"
#include "SceneObject.h"
#include "Light.h"
#include <vector>

class SceneManager
{
	static SceneManager * sm_instance;
	SceneManager();

	std::vector < Camera > cameras;
	int activeCamera;

	std::vector < SceneObject * > objects;
	
	std::vector < Light * > lights;
	Vector3 ambiental;
	float ratio;

	bool wired;

	Vector3 fogColor;
	float razaClaritate;
	float razaTranzitieCeata;

public:

	static SceneManager * getInstance();

	void init();
	void freeResources();

	~SceneManager();

	void changeWiredMode();
	bool wiredMode();

	Vector3 getFogColor();
	float getRazaClaritate();
	float getRazaTranzitieCeata();

	Camera & getActiveCamera();

	Vector3 getAmbientalLight();

	void setAmbientalLight(Vector3 ambL);

	void Update(float deltaTime);
	void Draw();
};

