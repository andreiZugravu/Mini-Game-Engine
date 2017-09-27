// NewTrainingFramework.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Vertex.h"
#include "Shaders.h"
#include <conio.h>
#include "Globals.h"
#include "Camera.h"
#include <String>
#include <sstream>
#include <iostream>
#include <fstream>
#include "ResourceManager.h"
#include "SceneManager.h"
#include "../Utilities/utilities.h" // if you use STL, please include this line AFTER all other include

const GLfloat pragDeltaTime = 0.05f;
GLfloat actualDeltaTime = 0.0f;

int sens = 1;

int Init ( ESContext *esContext )
{
	glClearColor ( 0.0f, 0.0f, 0.0f, 0.0f );

	ResourceManager::getInstance()->init(); //this first
	SceneManager::getInstance()->init();

	//creation of shaders and program 
	//return myShaders.Init("../Resources/Shaders/TriangleShaderVS.vs", "../Resources/Shaders/TriangleShaderFS.fs");

	return 0;
}

void Draw ( ESContext *esContext )
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //
	
	SceneManager::getInstance()->Draw();
	//std::cout << SceneManager::getInstance()->getActiveCamera().getPosition().x << " " << SceneManager::getInstance()->getActiveCamera().getPosition().y << " " << SceneManager::getInstance()->getActiveCamera().getPosition().z << "\n";
	eglSwapBuffers ( esContext->eglDisplay, esContext->eglSurface ); ///
}

void Update ( ESContext *esContext, float deltaTime )
{
	SceneManager::getInstance()->Update(deltaTime); // with deltaTime
	//camera.setDeltaTime(deltaTime);
	static POINT oldpos; //should be initialized?

	if (GetAsyncKeyState(VK_LBUTTON))
	{
		POINT curpos;
		GetCursorPos(&curpos);
		ScreenToClient(esContext->hWnd, &curpos);

		if (curpos.x < 480.0f)
			SceneManager::getInstance()->getActiveCamera().rotateOy(-1);
		else
			SceneManager::getInstance()->getActiveCamera().rotateOy(1);
	}
}

void Key ( ESContext *esContext, unsigned char key, bool bIsPressed)
{
	switch (key)
	{
	case 'W':
			if (bIsPressed)
			{
				SceneManager::getInstance()->getActiveCamera().moveOz(-1);
			}
			break;

	case 'Q':
		if (bIsPressed)
			{
				SceneManager::getInstance()->getActiveCamera().moveOx(1); SceneManager::getInstance()->getActiveCamera().moveOx(1);
				//check why it requires 2 calls...that 1 -1 stuff when calling with 1
			}

	case 'A':
			if (bIsPressed)
			{
				SceneManager::getInstance()->getActiveCamera().moveOx(-1);
			}
			break;

	case 'S':
			if (bIsPressed)
			{
				SceneManager::getInstance()->getActiveCamera().moveOz(1);
			}
			break;

	case 'R':
			if (bIsPressed)
			{
				SceneManager::getInstance()->getActiveCamera().moveOy(1);
			}
			break;

	case 'D':
			if (bIsPressed)
			{
				SceneManager::getInstance()->getActiveCamera().moveOy(-1);
			}
			break;

	case 'Z':
			if (bIsPressed)
			{
				SceneManager::getInstance()->getActiveCamera().rotateOx(sens);
			}
			break;

	case 'X':
			if (bIsPressed)
			{
				SceneManager::getInstance()->getActiveCamera().rotateOz(sens);
			}
			break;

	case 'C':
			if (bIsPressed)
			{
				SceneManager::getInstance()->getActiveCamera().rotateOy(sens);
			}
			break;

	case 'L':
			if (bIsPressed)
			{
				SceneManager::getInstance()->changeWiredMode();
			}

	case 'P':
			if (bIsPressed)
			{
				sens = -sens;
			}
	}
}

void CleanUp()
{
	//
	ResourceManager::getInstance()->freeResources();
	delete ResourceManager::getInstance();

	SceneManager::getInstance()->freeResources();
	delete SceneManager::getInstance(); // <3
}

int _tmain(int argc, _TCHAR* argv[])
{
	//identifying memory leaks
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF ); 
	
	ESContext esContext;

    esInitContext ( &esContext );

	esCreateWindow ( &esContext, "Hello Triangle", Globals::screenWidth, Globals::screenHeight, ES_WINDOW_RGB | ES_WINDOW_DEPTH);
	
	if ( Init ( &esContext ) != 0 )
		return 0;

	esRegisterDrawFunc ( &esContext, Draw );
	esRegisterUpdateFunc ( &esContext, Update );
	esRegisterKeyFunc ( &esContext, Key);

	esMainLoop ( &esContext );

	//releasing OpenGL resources
	CleanUp();


	printf("Press any key...\n");
	_getch();

	return 0;
}

