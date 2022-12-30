#pragma once
#include "stdafx.h"
#include "app.h"
#include "RenderLib.h"
#include "physics.h"

class MainScene : public SOF::App
{
	// geometry
	TexturedLit* floorQuad;
	TexturedLit* cuboid; // from SolarSystem, cubes, and lights 

	// camera
	glm::vec3 eye;    
	glm::vec3 lookAt; 
	glm::vec3 up;     
	FlyCam* camera;

	// scene graph
	Node* sceneGraphRoot;

	GroupNode* dynamicObjectNode;
	TransformNode* projectileNode;

	// physics
	Physics* physics;

	void shoot();

public:
	virtual void Init();
	virtual void Draw();
	virtual void Update(float deltaT);
};