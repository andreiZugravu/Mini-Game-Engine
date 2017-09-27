#pragma once

#include "SceneObject.h"

class Fire : public SceneObject
{
private:

	float maximumDisplacement;
	float time;

public:

	Fire();
	~Fire();

	void setMaximumDisplacement(float maxDis);

	void Draw();
	void Update(float deltaTime);
	void freeResources();
};

