#pragma once

#include "SceneObject.h"

class SkyBox : public SceneObject
{
private:

	int offset;

public:

	SkyBox();
	~SkyBox();

	void setOffset(int o);

	void Draw();
	void Update(float deltaTime);
	void freeResources();
};

