#include "stdafx.h"
#include "../Include/rapidxml-1.13/rapidxml.hpp"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Terrain.h"
#include "Fire.h"
#include "SkyBox.h"
#include "Globals.h"
#include <String>
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <vector>

SceneManager * SceneManager::sm_instance = NULL;

SceneManager::SceneManager()
{
	activeCamera = 0;
	wired = false;
	fogColor = Vector3(0.0, 0.0, 0.0);
}


SceneManager * SceneManager::getInstance()
{
	if (!sm_instance)
	{
		sm_instance = new SceneManager();
	}

	return sm_instance;
}

void SceneManager::init()
{
	rapidxml::xml_document<> doc;
	std::ifstream fin("sceneManager.xml");
	std::stringstream buffer;
	buffer << fin.rdbuf();
	fin.close();
	std::string content(buffer.str());
	/*char * WHAT = NULL, * WHERE = NULL;
	//rapidxml::parse_error(&WHAT, &WHERE);
	while (*WHAT != '\0') { std::cout << *WHAT; WHAT++; }
	std::cout << "\n";
	while (*WHERE != '\0') { std::cout << *WHERE; WHERE++; }*/
	doc.parse<0>(&content[0]);
	rapidxml::xml_node<> * pRoot = doc.first_node();
	

	rapidxml::xml_node<> * pNodeBackgroundColor = pRoot->first_node("backgroundColor");
	rapidxml::xml_node<> * pNodeFogColor = pRoot->first_node("fogColor");
	rapidxml::xml_node<> * pNodeCameras = pRoot->first_node("cameras");
	rapidxml::xml_node<> * pNodeActiveCamera = pRoot->first_node("activeCamera");
	rapidxml::xml_node<> * pNodeObjects = pRoot->first_node("objects");
	rapidxml::xml_node<> * pNodeAmbientalLight = pRoot->first_node("ambientalLight");
	rapidxml::xml_node<> * pNodeLights = pRoot->first_node("lights");
	rapidxml::xml_node<> * pNodeDebugSettings = pRoot->first_node("debugSettings");

	//get the information and store it

	//backgroundColor : r, g and b
	
	double r = atof(pNodeBackgroundColor->first_node("r")->value());
	double g = atof(pNodeBackgroundColor->first_node("g")->value());
	double b = atof(pNodeBackgroundColor->first_node("b")->value());

	//fogColour

	double x = atof(pNodeFogColor->first_node("x")->value());
	double y = atof(pNodeFogColor->first_node("y")->value());
	double z = atof(pNodeFogColor->first_node("z")->value());
	
	fogColor = Vector3(x, y, z);

	//raza de claritate
	if (pRoot->first_node("razaClaritate"))
		razaClaritate = atof(pRoot->first_node("razaClaritate")->value());

	//raza de tranzitie ceata
	if (pRoot->first_node("razaTranzitieCeata"))
		razaTranzitieCeata = atof(pRoot->first_node("razaTranzitieCeata")->value());

	//cameras
	Camera cam;
	for (rapidxml::xml_node<> * pNode = pNodeCameras->first_node("camera"); pNode; pNode = pNode->next_sibling())
	{
		//iterate through all the cameras and get the : id, position, target, up
		//translationSpeed, rotationSpeed, fov, near, far

		GLfloat x, y, z;


		// !!!!!!!!!!!!!!!! Except for the id, everything else was considered to be GLfloat and we used atof, not atoi


		//id
		rapidxml::xml_attribute<> * attr = pNode->first_attribute("id");
		int id = atoi(attr->value()); // found the camera id

		//position
		rapidxml::xml_node<> * pNodePosition = pNode->first_node("position");

		x = atof(pNodePosition->first_node("x")->value()); //x, y, z of POSITION
		y = atof(pNodePosition->first_node("y")->value()); //these will change later, so save them now
		z = atof(pNodePosition->first_node("z")->value()); //!!!

		cam.setPosition(Vector4(x, y, z, 1.0f));

		//target
		rapidxml::xml_node<> * pNodeTarget = pNode->first_node("target");

		x = atof(pNodeTarget->first_node("x")->value()); //x, y, z of TARGET
		y = atof(pNodeTarget->first_node("y")->value()); //these will change later, so save them now
		z = atof(pNodeTarget->first_node("z")->value()); //!!!

		cam.setTarget(Vector4(x, y, z, 1.0f));

		//up
		rapidxml::xml_node<> * pNodeUp = pNode->first_node("up");

		x = atof(pNodeUp->first_node("x")->value()); //x, y, z of UP
		y = atof(pNodeUp->first_node("y")->value());
		z = atof(pNodeUp->first_node("z")->value());

		cam.setUp(Vector4(x, y, z, 0.0f));

		//translationSpeed
		GLfloat translationSpeed = atof(pNode->first_node("translationSpeed")->value());

		cam.setMoveSpeed(translationSpeed);

		//rotationSpeed
		GLfloat rotationSpeed = atof(pNode->first_node("rotationSpeed")->value());

		cam.setRotateSpeed(rotationSpeed);

		//fov
		GLfloat fov = atof(pNode->first_node("fov")->value());

		cam.setFov(fov);

		//near
		GLfloat _near = atof(pNode->first_node("near")->value());

		cam.setNear_(_near);

		//far
		GLfloat _far = atof(pNode->first_node("far")->value());

		cam.setFar_(_far);

		cam.updateWorldView();

		GLfloat ratio = (GLfloat)((GLfloat)Globals::screenWidth / (GLfloat)Globals::screenHeight);
		cam.getProjectionMatrix().SetPerspective(fov, ratio, cam.getNear_(), cam.getFar_());
	}

	//activeCamera
	int activeCamera = atoi(pNodeActiveCamera->value());

	/********************** Just for testing **********************/

	cameras.push_back(cam);

	/********************** Just for testing **********************/
	
	//objects
	for (rapidxml::xml_node<> * pNode = pNodeObjects->first_node("object"); pNode; pNode = pNode->next_sibling())
	{
		SceneObject * obj;

		ObjectType type;

		if (pNode->first_node("type"))
		{
			if (strcmp(pNode->first_node("type")->value(), "normal") == 0)
			{
				type = ObjectType::normalObj;
				obj = new SceneObject;
			}
			else if (strcmp(pNode->first_node("type")->value(), "terrain") == 0)
			{
				type = ObjectType::terrain;
				obj = new Terrain;
			}
			else if (strcmp(pNode->first_node("type")->value(), "skyBox") == 0)
			{
				type = ObjectType::skyBox;
				obj = new SkyBox;
			}
			else if (strcmp(pNode->first_node("type")->value(), "fire") == 0)
			{
				type = ObjectType::fire;
				obj = new Fire;
			}
		}

		obj->setType(type);

		//iterate through all the objects and get the : id, model, shader, type
		//depthTest, blend, name, textures, position, rotation, scale
		//selfRotateSpeed, followingCamera, lights
		//trajectory

		//id

		rapidxml::xml_attribute<> * attr = pNode->first_attribute("id");
		int id = atoi(attr->value());

		obj->setId(id);

		//model

		int modelId = 1; //croco

		if (pNode->first_node("model"))
		{
			if (strcmp(pNode->first_node("model")->value(), "generated") == 0)
			{
				if (type == terrain)
				{
					if (pNode->first_node("nr_celule"))
						((Terrain *)obj)->setNrCelule(atoi(pNode->first_node("nr_celule")->value()));

					if (pNode->first_node("dim_celula"))
						((Terrain *)obj)->setDimCelula(atoi(pNode->first_node("dim_celula")->value()));

					if (pNode->first_node("offsetY"))
						((Terrain *)obj)->setOffsetY(atoi(pNode->first_node("offsetY")->value()));

					if (pNode->first_node("inaltimi"))
					{
						int r = 0, g = 0, b = 0;

						if (pNode->first_node("inaltimi")->first_node("r"))
							r = atoi(pNode->first_node("inaltimi")->first_node("r")->value());

						if (pNode->first_node("inaltimi")->first_node("g"))
							g = atoi(pNode->first_node("inaltimi")->first_node("g")->value());

						if (pNode->first_node("inaltimi")->first_node("b"))
							b = atoi(pNode->first_node("inaltimi")->first_node("b")->value());

						((Terrain *)obj)->setInaltimi(Vector3(r, g, b));
					}
				}

				((Terrain *)obj)->generateModel();
			}
			else
			{
				//char * val = pNode->first_node("model")->value();

				//if (val[0] >= '0' && val[0] <= '9')
				modelId = atoi(pNode->first_node("model")->value());
				
				obj->setModel(ResourceManager::getInstance()->loadModel(modelId));
			}
		}

		//shader

		int shaderId = 1;

		if (pNode->first_node("shader"))
			shaderId = atoi(pNode->first_node("shader")->value());

		obj->setShader(ResourceManager::getInstance()->loadShader(shaderId));

		//depthTest

		bool depthTest = false;

		if (pNode->first_node("depthTest"))
		{
			if (strcmp(pNode->first_node("depthTest")->value(), "true") == 0)
				depthTest = true;
			else
				depthTest = false;
		}

		obj->setDepthTest(depthTest);
		
		//blend

		bool blend = false;
		
		if(pNode->first_node("blend"))
			blend = pNode->first_node("blend")->value() == "true" ? true : false;

		//name

		std::string name = "NO NAME";
		
		if(pNode->first_node("name"))
			name = pNode->first_node("name")->value();
		
		//textures

		std::vector < Texture * > textures;
		rapidxml::xml_node<> * pNodeTextures = pNode->first_node("textures");

		if (pNodeTextures) {
			for (rapidxml::xml_node<> * pNodeT = pNodeTextures->first_node("texture"); pNodeT; pNodeT = pNodeT->next_sibling())
			{
				rapidxml::xml_attribute<> * attr = pNodeT->first_attribute("id");

				if (attr) //just to be sure it's structured like <texture id="X">
					textures.push_back(ResourceManager::getInstance()->loadTexture(atoi(attr->value())));
			}
		}
		
		obj->setTextures(textures);
		
		//position

		GLfloat x = 0.0f, y = 0.0f, z = 0.0f;

		if (pNode->first_node("position")) {

			if(pNode->first_node("position")->first_node("x"))
				x = atof(pNode->first_node("position")->first_node("x")->value()); //save them !!!

			if(pNode->first_node("position")->first_node("y"))
				y = atof(pNode->first_node("position")->first_node("y")->value()); //will be modified immediately

			if(pNode->first_node("position")->first_node("z"))
				z = atof(pNode->first_node("position")->first_node("z")->value());

		}

		Vector3 position = Vector3(x, y, z);

		obj->setPosition(position);

		//rotation

		x = 0.0f, y = 0.0f, z = 0.0f;

		if (pNode->first_node("rotation")) {

			if (pNode->first_node("rotation")->first_node("x"))
				x = atof(pNode->first_node("rotation")->first_node("x")->value()); //save them !!!

			if (pNode->first_node("rotation")->first_node("y"))
				y = atof(pNode->first_node("rotation")->first_node("y")->value()); //will be modified immediately

			if (pNode->first_node("rotation")->first_node("z"))
				z = atof(pNode->first_node("rotation")->first_node("z")->value());

		}

		Vector3 rotation = Vector3(x, y, z);

		obj->setRotation(rotation);

		//scale

		x = 0.0f, y = 0.0f, z = 0.0f;

		if (pNode->first_node("scale")) {

			if (pNode->first_node("scale")->first_node("x"))
				x = atof(pNode->first_node("scale")->first_node("x")->value()); //save them !!!

			if (pNode->first_node("scale")->first_node("y"))
				y = atof(pNode->first_node("scale")->first_node("y")->value()); //will be modified immediately

			if (pNode->first_node("scale")->first_node("z"))
				z = atof(pNode->first_node("scale")->first_node("z")->value());

		}

		Vector3 scale = Vector3(x, y, z);

		obj->setScale(scale);

		//selfRotateSpeed

		GLfloat selfRotateSpeed = 0.0f;
		
		if(pNode->first_node("selfRotateSpeed"))
			selfRotateSpeed = atof(pNode->first_node("selfRotateSpeed")->value());

		//followingCamera

		int followingCameraId = -1;
		
		if(pNode->first_node("followingCamera") && pNode->first_node("followingCamera")->first_node("cameras")
			&& pNode->first_node("followingCamera")->first_node("cameras")->first_node("camera"))
			followingCameraId = atoi(pNode->first_node("followingCamera")->first_node("cameras")->first_node("camera")->value());
		// !!!!!!!! watch this for later, as it might be modified ( ox, oy and oz might actually appear )

		//lights

		int lightsId = -1;
		
		if(pNode->first_node("lights") && pNode->first_node("lights")->first_node("light"))
			lightsId = atoi(pNode->first_node("lights")->first_node("light")->value());

		//trajectory

		std::string trajectoryType = "NO_TRAJECTORY", direction = "NO_DIRECTION";
		int iterationCount = 0;
		double speed = 0.0f;

		Trajectory * tr = NULL;

		if (pNode->first_node("trajectory"))
		{
			rapidxml::xml_node<> * pNodeTrajectory = pNode->first_node("trajectory");

			tr = new Trajectory;

			if (pNodeTrajectory->first_attribute("type"))
			{
				if (strcmp(pNodeTrajectory->first_attribute("type")->value(), "linear") == 0)
					tr->type = linear;
				else
					tr->type = circular;
			}

			if (pNodeTrajectory->first_attribute("iteration-count"))
			{
				iterationCount = atoi(pNodeTrajectory->first_attribute("iteration-count")->value());
				tr->iterationCount = iterationCount;
			}

			if (pNodeTrajectory->first_attribute("direction"))
			{
				if(strcmp(pNodeTrajectory->first_attribute("direction")->value(), "normal") == 0)
					tr->direction = normal;
				else
					tr->direction = alternate;
			}

			if (pNodeTrajectory->first_attribute("speed"))
			{
				speed = atof(pNodeTrajectory->first_attribute("speed")->value());
				tr->speed = speed;
			}

			if (pNodeTrajectory->first_node("points"))
			{
				for (rapidxml::xml_node<> * pNodePoints = pNodeTrajectory->first_node("points")->first_node("point"); pNodePoints; pNodePoints = pNodePoints->next_sibling())
				{
					float x, y, z;

					x = atof(pNodePoints->first_node("x")->value());
					y = atof(pNodePoints->first_node("y")->value());
					z = atof(pNodePoints->first_node("z")->value());

					tr->points.push_back(Vector3(x, y, z));
				}

				obj->setStartPoint(tr->points[0]);
				obj->setEndPoint(tr->points[1]);

				obj->setPosition(tr->points[0]); // set position = startPoint
			}
		}

		obj->setTrajectory(tr);

		if (type == skyBox)
		{
			if (pNode->first_node("offset"))
				((SkyBox *)obj)->setOffset(atoi(pNode->first_node("offset")->value()));
		}

		if (type == fire)
		{
			if(pNode->first_node("maximumDisplacement"))
				((Fire *)obj)->setMaximumDisplacement(atof(pNode->first_node("maximumDisplacement")->value()));
		}
		objects.push_back(obj);
	}
	
	//ambiental light

	if (pNodeAmbientalLight)
	{
		double color_r = 0.0f, color_g = 0.0f, color_b = 0.0f, ambRatio = 0.0f;

		rapidxml::xml_node<> * pNodeAmbientalLightColor = pNodeAmbientalLight->first_node("color");
		if (pNodeAmbientalLightColor)
		{
			if (pNodeAmbientalLightColor->first_node("r"))
			{
				color_r = atof(pNodeAmbientalLightColor->first_node("r")->value());
			}

			if (pNodeAmbientalLightColor->first_node("g"))
			{
				color_g = atof(pNodeAmbientalLightColor->first_node("g")->value());
			}

			if (pNodeAmbientalLightColor->first_node("b"))
			{
				color_b = atof(pNodeAmbientalLightColor->first_node("b")->value());
			}
		}

		ambiental = Vector3(color_r, color_g, color_b);

		if (pNodeAmbientalLight->first_node("ratio"))
			ambRatio = atof(pNodeAmbientalLight->first_node("ratio")->value());

		ratio = ambRatio;
	}

	for (rapidxml::xml_node<> * pNode = pNodeLights->first_node("light"); pNode; pNode = pNode->next_sibling())
	{
		Light * light = new Light;

		int lightId = 0;
		if (pNode->first_attribute("id"))
			lightId = atoi(pNode->first_attribute("id")->value());

		light->setId(lightId);

		double color_r = 0.0f, color_g = 0.0f, color_b = 0.0f;

		rapidxml::xml_node<> * pNodeDiffuseColor = pNode->first_node("diffuseColor");

		if (pNodeDiffuseColor)
		{
			if (pNodeDiffuseColor->first_node("r"))
			{
				color_r = atof(pNodeDiffuseColor->first_node("r")->value());
			}

			if (pNodeDiffuseColor->first_node("g"))
			{
				color_g = atof(pNodeDiffuseColor->first_node("g")->value());
			}

			if (pNodeDiffuseColor->first_node("b"))
			{
				color_b = atof(pNodeDiffuseColor->first_node("b")->value());
			}
		}

		color_r = color_g = color_b = 0.0f;

		rapidxml::xml_node<> * pNodeSpecularColor = pNode->first_node("specularColor");

		if (pNodeSpecularColor)
		{
			if (pNodeSpecularColor->first_node("r"))
			{
				color_r = atof(pNodeSpecularColor->first_node("r")->value());
			}

			if (pNodeSpecularColor->first_node("g"))
			{
				color_g = atof(pNodeSpecularColor->first_node("g")->value());
			}

			if (pNodeSpecularColor->first_node("b"))
			{
				color_b = atof(pNodeSpecularColor->first_node("b")->value());
			}
		}

		light->setSpecular(Vector3(color_r, color_g, color_b));

		lights.push_back(light);
	}
}

void SceneManager::freeResources()
{
	for (SceneObject * obj : objects)
		if (obj) {
			obj->freeResources();
			delete obj;
		}

	for(Light * l : lights)
		if (l)
		{
			delete l;
		}
}

SceneManager::~SceneManager()
{

}

void SceneManager::changeWiredMode()
{
	wired = !wired;
}

bool SceneManager::wiredMode()
{
	return wired;
}

Vector3 SceneManager::getFogColor()
{
	return fogColor;
}

float SceneManager::getRazaClaritate()
{
	return razaClaritate;
}

float SceneManager::getRazaTranzitieCeata()
{
	return razaTranzitieCeata;
}

Camera & SceneManager::getActiveCamera()
{
	return cameras[0]; //change to activeCamera later
}

Vector3 SceneManager::getAmbientalLight()
{
	return ambiental;
}

void SceneManager::setAmbientalLight(Vector3 ambL)
{
	ambiental = ambL;
}

void SceneManager::Update(float deltaTime)
{
	for (SceneObject * obj : objects)
		obj->Update(deltaTime);
}

void SceneManager::Draw()
{
	for (SceneObject * obj : objects) {
		obj->Draw();
	}
}
