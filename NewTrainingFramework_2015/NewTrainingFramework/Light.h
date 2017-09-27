#pragma once

#include "../Utilities/utilities.h"

class Light
{
private:

	int id;
	Vector3 diffusal;
	Vector3 specular;

public:

	Light();
	~Light();

	void setId(int i);
	void setDiffusal(Vector3 dif);
	void setSpecular(Vector3 spec);

	int getId();
	Vector3 getDiffusal();
	Vector3 getSpecular();
};

