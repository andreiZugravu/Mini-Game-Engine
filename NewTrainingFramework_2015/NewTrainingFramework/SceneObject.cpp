#include "stdafx.h"
#include "SceneObject.h"
#include "SceneManager.h"
#include <iostream>

void SceneObject::sendCommonData()
{

	Matrix MVP = ModelMatrix * SceneManager::getInstance()->getActiveCamera().getViewMatrix() * SceneManager::getInstance()->getActiveCamera().getProjectionMatrix();
	//MVP

	if (shader->MatrixID != -1)
	{
		glUniformMatrix4fv(shader->MatrixID, 1, GL_FALSE, &MVP.m[0][0]); //same, not &&
	}

	//
	if (shader->textureUniform != -1)
	{
		glUniform1i(shader->textureUniform, 0);
	}

	if (shader->modelMatrixUniform != -1)
	{
		glUniformMatrix4fv(shader->modelMatrixUniform, 1, GL_FALSE, &ModelMatrix.m[0][0]);
	}

	if (shader->cameraCoordUniform != -1)
	{
		Vector3 cameraPos = SceneManager::getInstance()->getActiveCamera().getPosition();
		glUniform3f(shader->cameraCoordUniform, cameraPos.x, cameraPos.y, cameraPos.z);
	}

	if (shader->rUniform != -1)
	{
		glUniform1f(shader->rUniform, SceneManager::getInstance()->getRazaClaritate());
	}

	if (shader->RUniform != -1)
	{
		glUniform1f(shader->RUniform, SceneManager::getInstance()->getRazaTranzitieCeata());
	}

	if (shader->fogColorUniform != -1)
	{
		Vector3 fogColor = SceneManager::getInstance()->getFogColor();
		glUniform3f(shader->fogColorUniform, fogColor.x, fogColor.y, fogColor.z);
		//optimize it
	}
}

SceneObject::SceneObject()
{
	sens = 1;
}


SceneObject::~SceneObject()
{

}

void SceneObject::setId(int Id)
{
	id = Id;
}

void SceneObject::setPosition(Vector3 Position)
{
	position = Position;
}

void SceneObject::setRotation(Vector3 Rotation)
{
	rotation = Rotation;
}

void SceneObject::setScale(Vector3 Scale)
{
	scale = Scale;
}

void SceneObject::setModel(Model * m)
{
	model = m;
}

void SceneObject::setShader(Shader * s)
{
	shader = s;
}

void SceneObject::setTextures(std::vector<Texture*> t)
{
	textures = t;
}

void SceneObject::setDepthTest(bool DepthTest)
{
	depthTest = DepthTest;
}

void SceneObject::setType(ObjectType t)
{
	type = t;
}

ObjectType SceneObject::getType()
{
	return type;
}

void SceneObject::setTrajectory(Trajectory * tr)
{
	trajectory = tr;
	//if(tr)std::cout << tr->type << " " << tr->direction << " " << tr->iterationCount << " " << tr->speed << "\n";
}

void SceneObject::setStartPoint(Vector3 sp)
{
	startPoint = sp;
}

void SceneObject::setEndPoint(Vector3 ep)
{
	endPoint = ep;
}

int SceneObject::getId()
{
	return id;
}

void SceneObject::Draw()
{
	glUseProgram(shader->program);

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
	sendCommonData();

	if (shader->ambientaLightColorUniform != -1)
	{
		Vector3 ambientalLight = SceneManager::getInstance()->getAmbientalLight();
		
		glUniform3f(shader->ambientaLightColorUniform, ambientalLight.x, ambientalLight.y, ambientalLight.z);
	}

	if (shader->diffusalLightColorUniform != -1)
	{
		Vector3 diffusalLight = Vector3(1.0, 0.0, 1.0);

		glUniform3f(shader->diffusalLightColorUniform, diffusalLight.x, diffusalLight.y, diffusalLight.z);
	}

	if (shader->normAttribute != -1)
	{
		//Vector3
	}

	//glDrawArrays(GL_TRIANGLES, 0, 3);

	glActiveTexture(GL_TEXTURE0);

	glBindTexture(GL_TEXTURE_2D, textures[0]->getTextureId()); //first texture for now

	if (SceneManager::getInstance()->wiredMode())
		glDrawElements(GL_LINES, model->getNrIndicesWired(), GL_UNSIGNED_SHORT, 0);
	else
		glDrawElements(GL_TRIANGLES, model->getNrIndices(), GL_UNSIGNED_SHORT, 0);

	glDrawElements(GL_TRIANGLES, model->getNrIndices(), GL_UNSIGNED_SHORT, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void SceneObject::Update(float deltaTime)
{
	Matrix ModelMatrixScaled = Matrix().SetScale(scale);

	Matrix ModelMatrixRotatedX = ModelMatrix.SetRotationX(rotation.x);
	Matrix ModelMatrixRotatedY = ModelMatrix.SetRotationY(rotation.y);
	Matrix ModelMatrixRotatedZ = ModelMatrix.SetRotationZ(rotation.z);

	Matrix ModelMatrixTranslated = ModelMatrix.SetTranslation(position);

	ModelMatrix = ModelMatrixScaled * ModelMatrixRotatedX * ModelMatrixRotatedY * ModelMatrixRotatedZ * ModelMatrixTranslated;

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

void SceneObject::freeResources()
{
	if (trajectory)
		delete trajectory;
}