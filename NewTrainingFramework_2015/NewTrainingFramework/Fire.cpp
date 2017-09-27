#include "stdafx.h"
#include "Fire.h"
#include "SceneManager.h"
#include <ctime>

Fire::Fire()
{
	//time = clock();
	time = 0.0f;
}


Fire::~Fire()
{

}

void Fire::setMaximumDisplacement(float maxDis)
{
	maximumDisplacement = maxDis;
}

void Fire::Draw()
{

	glUseProgram(shader->program);

	glEnable(GL_BLEND); // !!!

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // !!!

	glBindBuffer(GL_ARRAY_BUFFER, model->getVboId());

	if (SceneManager::getInstance()->wiredMode())
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->getWiredIboId());
	else
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->getIboId());

	if (shader->positionAttribute != -1)
	{
		glEnableVertexAttribArray(shader->positionAttribute);
		glVertexAttribPointer(shader->positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0); //value, not &&
	}

	if (shader->uvAttribute != -1)
	{
		glEnableVertexAttribArray(shader->uvAttribute);
		glVertexAttribPointer(shader->uvAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(4 * sizeof(Vector3)));
	}

	/* */
	sendCommonData(); // !!!
	//glDrawArrays(GL_TRIANGLES, 0, 3);

	if (shader->displacementTextureUniform != -1)
	{
		glUniform1i(shader->displacementTextureUniform, 0);
	}

	if (shader->fireTextureUniform != -1)
	{
		glUniform1i(shader->fireTextureUniform, 1);
	}

	if (shader->fireMaskTextureUniform != -1)
	{
		glUniform1i(shader->fireMaskTextureUniform, 2);
	}

	if (shader->maximumDisplacementUniform != -1)
	{
		glUniform1f(shader->maximumDisplacementUniform, maximumDisplacement);
	}

	if (shader->timeUniform != -1)
	{
		glUniform1f(shader->timeUniform, time);
	}

	glActiveTexture(GL_TEXTURE0);

	glBindTexture(GL_TEXTURE_2D, textures[0]->getTextureId());

	glActiveTexture(GL_TEXTURE1);

	glBindTexture(GL_TEXTURE_2D, textures[2]->getTextureId());

	glActiveTexture(GL_TEXTURE2);

	glBindTexture(GL_TEXTURE_2D, textures[1]->getTextureId());


	if (SceneManager::getInstance()->wiredMode())
		glDrawElements(GL_LINES, model->getNrIndicesWired(), GL_UNSIGNED_SHORT, 0);
	else
		glDrawElements(GL_TRIANGLES, model->getNrIndices(), GL_UNSIGNED_SHORT, 0);

	glDrawElements(GL_TRIANGLES, model->getNrIndices(), GL_UNSIGNED_SHORT, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Fire::Update(float deltaTime)
{
	Matrix ModelMatrixScaled = Matrix().SetScale(scale);

	Matrix ModelMatrixRotatedX = ModelMatrix.SetRotationX(rotation.x);
	Matrix ModelMatrixRotatedY = ModelMatrix.SetRotationY(rotation.y);
	Matrix ModelMatrixRotatedZ = ModelMatrix.SetRotationZ(rotation.z);

	Matrix ModelMatrixTranslated = ModelMatrix.SetTranslation(position);

	ModelMatrix = ModelMatrixScaled * ModelMatrixRotatedX * ModelMatrixRotatedY * ModelMatrixRotatedZ * ModelMatrixTranslated;

	time += 0.0035f; //to work on this increment step

	if (time >= 1.0f) //partea fractionara !!
		time -= 1.0f;

	if (trajectory && trajectory->iterationCount > 0) //compiler checks for trajectory to be not NULL, then it.cnt.
	{
		if (trajectory->type == normal)
		{
			if ((endPoint - startPoint).Dot(endPoint - position) > 0.1f)
			{
				position += (endPoint - position).Normalize() * trajectory->speed * deltaTime;
			}
			else
			{
				if (trajectory->direction == alternate)
				{
					Vector3 aux = startPoint; startPoint = endPoint; endPoint = aux;

					//rotation += Vector3(0.0f, 100.0f, 0.0f);

					if (trajectory->points[0] == startPoint) //so it executes 2 forward to back moves, not n/2, in case of alternate direction
						trajectory->iterationCount--;

					position = startPoint;
				}
				else
				{
					trajectory->iterationCount--;
					position = startPoint;
				}
			}
		}
	}
}

void Fire::freeResources()
{

}
