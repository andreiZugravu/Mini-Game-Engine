#include "stdafx.h"
#include "SkyBox.h"
#include "SceneManager.h"

SkyBox::SkyBox()
{
	position = Vector3(SceneManager::getInstance()->getActiveCamera().getPosition()); // should care about offset? it's initialized
	//careful here if the SkyBox might look like it "jumps" when the game starts
}


SkyBox::~SkyBox()
{

}

void SkyBox::setOffset(int o)
{
	offset = o;
}

void SkyBox::Draw()
{
	glUseProgram(shader->program);

	glDepthMask(false); // ca sa se vada mereu "in spate" skybox-ul; TREBUIE SA FIE PRIMUL DESENAT !! ( primul in lista de obiecte !! )

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

	sendCommonData();

	if (shader->cubeTextureUniform != -1)
	{
		glActiveTexture(GL_TEXTURE0);

		glBindTexture(GL_TEXTURE_CUBE_MAP, textures[0]->getTextureId());

		glUniform1i(shader->cubeTextureUniform, 0);
	}

	if (SceneManager::getInstance()->wiredMode())
		glDrawElements(GL_LINES, model->getNrIndicesWired(), GL_UNSIGNED_SHORT, 0);
	else
		glDrawElements(GL_TRIANGLES, model->getNrIndices(), GL_UNSIGNED_SHORT, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	glDepthMask(true); // !!!!!
}

void SkyBox::Update(float deltaTime)
{
	position = Vector3(SceneManager::getInstance()->getActiveCamera().getPosition()) + Vector3(0, offset, 0);
	Matrix ModelMatrixScaled = Matrix().SetScale(scale);

	Matrix ModelMatrixRotatedX = ModelMatrix.SetRotationX(rotation.x);
	Matrix ModelMatrixRotatedY = ModelMatrix.SetRotationY(rotation.y);
	Matrix ModelMatrixRotatedZ = ModelMatrix.SetRotationZ(rotation.z);

	Matrix ModelMatrixTranslated = ModelMatrix.SetTranslation(position);

	ModelMatrix = ModelMatrixScaled * ModelMatrixRotatedX * ModelMatrixRotatedY * ModelMatrixRotatedZ * ModelMatrixTranslated;
}

void SkyBox::freeResources()
{

}
