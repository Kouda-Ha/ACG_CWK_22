#include "stdafx.h"
#include "RenderLib.h"
#include "spline.h"
#include "physics.h"
#include <random>

class MoveLightCallback : public UpdateCallback
{
	LightNode* node;
	float t;
public:
	MoveLightCallback(LightNode* node) : node(node), t(0.0f) {}
	virtual void operator()(float dt)
	{
		t += dt;
		float x, y, z;
		x = 50.0f * cos(t);
		y = 20.0f;
		z = 50.0f * sin(t);
		node->light.position = glm::vec3(x, y, z);
	}
};

//callback for animating a rotation about the y axis - use this for the solar system exercise
class YRotatorCallback : public UpdateCallback
{
	TransformNode* trans;
	float rate;
	float t;
public:
	YRotatorCallback(TransformNode* trans, float rate) : trans(trans), rate(rate), t(0.0f) {}
	virtual void operator()(float dt)
	{
		t += dt;
		trans->SetTransform(glm::rotate(glm::mat4(), t * rate, glm::vec3(0, 1, 0)));
	}
};

// Main Scene and Room
Node* Scene( Physics* phys )
{
	QuadGeometry quad(10);
	TexturedLit* floor = new TexturedLit(quad, "textures/marble.png");

	TransformNode* rootNode = new TransformNode(glm::mat4()); // from ex
	LightNode* light = new LightNode(glm::vec3(0, 100, 0), glm::vec4(1, 1, 1, 1), 0.1, 0.9, 0.4, 10, 200);
	rootNode->AddChild(light); // from ex
														  

	// Stars picture from https://unsplash.com/photos/qVotvbsuM_c
	// create a cubemap
	std::vector<std::string> cubeMapFiles;
	cubeMapFiles.push_back("textures/cubemaps/star.jpg");
	cubeMapFiles.push_back("textures/cubemaps/star.jpg");
	cubeMapFiles.push_back("textures/cubemaps/star.jpg");
	cubeMapFiles.push_back("textures/cubemaps/star.jpg");
	cubeMapFiles.push_back("textures/cubemaps/star.jpg");
	cubeMapFiles.push_back("textures/cubemaps/star.jpg");
	SOF::Cubemap* cubeMap = new SOF::Cubemap(cubeMapFiles);
	Skybox* sb = new Skybox(cubeMap);
	GeometryNode* sbNode = new GeometryNode(sb);

	light->AddChild(sbNode);

	GeometryNode* geomNode = new GeometryNode(floor);
	// scale the floor and add 
	glm::mat4 floorMat = glm::scale(glm::mat4(), glm::vec3(100, 100, 100));
	TransformNode* floorScale = new TransformNode(floorMat);
	light->AddChild(floorScale);
	floorScale->AddChild(geomNode);

	light->SetUpdateCallback(new MoveLightCallback(light));


	// SOLAR SYSTEM
	// geometry and drawables
	// Textures from https://www.solarsystemscope.com/textures/
	SphereGeometry sphere(20);
	Emissive* sun = new Emissive(sphere, glm::vec4(1.0f, 1.0f, 0.5f, 1.0f)); // Sun
	TexturedLit* mercury = new TexturedLit(sphere, "textures/2k_mercury.jpg"); // Mercury
	TexturedLit* venus = new TexturedLit(sphere, "textures/2k_venus_surface.jpg"); // Venus
	TexturedLit* earth = new TexturedLit(sphere, "textures/2k_Earth_daymap.jpg"); // Earth
	TexturedLit* moon = new TexturedLit(sphere, "textures/2k_moon.jpg"); // Moon
	TexturedLit* mars = new TexturedLit(sphere, "textures/2k_mars.jpg"); // Mars
	TexturedLit* jupiter = new TexturedLit(sphere, "textures/2k_jupiter.jpg"); // Jupiter
	TexturedLit* saturn = new TexturedLit(sphere, "textures/2k_saturn.jpg"); // Saturn
	TexturedLit* saturnring = new TexturedLit(sphere, "textures/2k_saturn_ring_alpha.png"); // Saturn ring
	TexturedLit* uranus = new TexturedLit(sphere, "textures/2k_uranus.jpg"); // Uranus
	TexturedLit* neptune = new TexturedLit(sphere, "textures/2k_neptune.jpg"); // Neptune
	TexturedLit* pluto = new TexturedLit(sphere, "textures/stone.png"); // Pluto


	
	
	TransformNode* sphereTrans = new TransformNode(glm::translate(glm::mat4(), glm::vec3(0.0f, 10.0f, 0.0f)));
	TransformNode* sphereScale = new TransformNode(glm::scale(glm::mat4(), glm::vec3(3.0f, 3.0f, 3.0f)));

	light->AddChild(sphereTrans);
	sphereTrans->AddChild(sphereScale);
		
	// cubemap end

	phys->AddCollider(new QuadCollider(floorMat));

	// The walls 
	// Left Wall
	glm::mat4 wtrans = glm::mat4();
	wtrans = glm::translate(wtrans, glm::vec3(50.0, 0.0, 0.0));
	wtrans = glm::rotate(wtrans, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0));
	wtrans = glm::scale(wtrans, glm::vec3(100.0f, 100.0f, 100.0f));
	TransformNode *wall0 = new TransformNode(wtrans);
	light->AddChild(wall0);
	wall0->AddChild(geomNode);
	phys->AddCollider(new QuadCollider(wtrans));

	// Right wall
	wtrans = glm::mat4();
	wtrans = glm::translate(wtrans, glm::vec3(-50.0, 0.0, 0.0));
	wtrans = glm::rotate(wtrans, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0));
	wtrans = glm::scale(wtrans, glm::vec3(100.0f, 100.0f, 100.0f));
	TransformNode* wall1 = new TransformNode(wtrans);
	light->AddChild(wall1);
	wall1->AddChild(geomNode);
	phys->AddCollider(new QuadCollider(wtrans));

	// Front Wall
	wtrans = glm::mat4();
	wtrans = glm::translate(wtrans, glm::vec3(0.0, 0.0, 50.0));
	wtrans = glm::rotate(wtrans, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0));
	wtrans = glm::scale(wtrans, glm::vec3(100.0f, 100.0f, 100.0f));
	TransformNode* wall2 = new TransformNode(wtrans);
	light->AddChild(wall2);
	wall2->AddChild(geomNode);
	phys->AddCollider(new QuadCollider(wtrans));

	// Back Wall Bottom
	wtrans = glm::mat4();
	wtrans = glm::translate(wtrans, glm::vec3(0.0, 0.0, -50.0));
	wtrans = glm::rotate(wtrans, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0));
	wtrans = glm::scale(wtrans, glm::vec3(100.0f, 20.0f, 25.0f));
	TransformNode* wall3 = new TransformNode(wtrans);
	light->AddChild(wall3);
	wall3->AddChild(geomNode);
	phys->AddCollider(new QuadCollider(wtrans));

	// Back Wall Top
	wtrans = glm::mat4();
	wtrans = glm::translate(wtrans, glm::vec3(0.0, 37.5, -50.0));
	wtrans = glm::rotate(wtrans, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0));
	wtrans = glm::scale(wtrans, glm::vec3(100.0f, 20.0f, 25.0f));
	TransformNode* wall4 = new TransformNode(wtrans);
	light->AddChild(wall4);
	wall4->AddChild(geomNode);
	phys->AddCollider(new QuadCollider(wtrans));

	CubeGeometry cube;
	CylinderGeometry cylinder(1.0f, 20);

	TexturedLit* cubeRender = new TexturedLit(cube, "textures/stone.png", "textures/stonenorms.png"); // from ex
	TexturedLit* sphereRender = new TexturedLit(sphere, "textures/stone.png", "textures/stonenorms.png"); // from ex
	TexturedLit* cylinderRender = new TexturedLit(cylinder, "textures/stone.png", "textures/stonenorms.png"); // from ex

	GroupNode* group = new GroupNode();

	glm::mat4 trans;
	// cube transform - translation 10, 3, 5, rotation 45 about y, scale 3
	trans = glm::translate(glm::mat4(), glm::vec3(10, 3, 5));
	trans = glm::rotate(trans, glm::radians(45.0f), glm::vec3(0, 1, 0));
	trans = glm::scale(trans, glm::vec3(3, 3, 3));
	group->AddChild(new TransformNode(trans));
	group->GetChild(0)->AddChild(new GeometryNode(cubeRender));

	// sphere transform - translation -5, 3, 8, scale 3
	trans = glm::translate(glm::mat4(), glm::vec3(-5, 3, 8));
	trans = glm::scale(trans, glm::vec3(3, 3, 3));
	group->AddChild(new TransformNode(trans));
	group->GetChild(1)->AddChild(new GeometryNode(sphereRender));

	// cylinder transform - translation 3, 3, -6, scale 3
	trans = glm::translate(glm::mat4(), glm::vec3(3, 3, -6));
	trans = glm::scale(trans, glm::vec3(3, 3, 3));
	group->AddChild(new TransformNode(trans));
	group->GetChild(2)->AddChild(new GeometryNode(cylinderRender));

	// make multiple copies of the group, each with a random translation
	std::mt19937 rng;
	std::uniform_real_distribution<float> dist(-25.0f, 25.0f);

	for (int i = 0; i < 10; i++)
	{
		float x, y, z;
		x = dist(rng);
		y = 0.0f;
		z = dist(rng);
		TransformNode* t = new TransformNode(glm::translate(glm::mat4(), glm::vec3(x, y, z)));
		t->AddChild(group);
		light->AddChild(t);
	}
// ENDED ADD
// 
		// The scenegraph for the animated solar system model
	TransformNode* sunScale = new TransformNode(glm::scale(glm::mat4(), glm::vec3(5, 5, 5))); // Sun
	TransformNode* mercuryScale = new TransformNode(glm::scale(glm::mat4(), glm::vec3(1, 1, 1))); // mercury
	TransformNode* venusScale = new TransformNode(glm::scale(glm::mat4(), glm::vec3(1, 1, 1))); // venus
	TransformNode* earthScale = new TransformNode(glm::scale(glm::mat4(), glm::vec3(2, 2, 2))); // Earth
	TransformNode* moonScale = new TransformNode(glm::scale(glm::mat4(), glm::vec3(1, 1, 1))); // moon
	TransformNode* marsScale = new TransformNode(glm::scale(glm::mat4(), glm::vec3(1.5, 1.5, 1.5))); // mars
	TransformNode* jupiterScale = new TransformNode(glm::scale(glm::mat4(), glm::vec3(1, 1, 1))); // jupiter
	TransformNode* saturnScale = new TransformNode(glm::scale(glm::mat4(), glm::vec3(2, 2, 2))); // saturn
	TransformNode* saturnringScale = new TransformNode(glm::scale(glm::mat4(), glm::vec3(3, 0.1, 3))); // saturn ring
	TransformNode* uranusScale = new TransformNode(glm::scale(glm::mat4(), glm::vec3(1, 1, 1))); // uranus
	TransformNode* neptuneScale = new TransformNode(glm::scale(glm::mat4(), glm::vec3(1, 1, 1))); // neptune
	TransformNode* plutoScale = new TransformNode(glm::scale(glm::mat4(), glm::vec3(1, 1, 1))); // pluto

	TransformNode* sunOrbitRotator = new TransformNode(glm::mat4()); // sun
	TransformNode* mercuryOrbitRotator = new TransformNode(glm::mat4()); // mercury
	TransformNode* venusOrbitRotator = new TransformNode(glm::mat4()); // venus
	TransformNode* earthOrbitRotator = new TransformNode(glm::mat4()); // Earth
	TransformNode* moonOrbitRotator = new TransformNode(glm::mat4()); // moon
	TransformNode* marsOrbitRotator = new TransformNode(glm::mat4()); // mars
	TransformNode* jupiterOrbitRotator = new TransformNode(glm::mat4()); // jupiter
	TransformNode* saturnOrbitRotator = new TransformNode(glm::mat4()); // saturn
	TransformNode* saturnringOrbitRotator = new TransformNode(glm::mat4()); // saturn ring
	TransformNode* uranusOrbitRotator = new TransformNode(glm::mat4()); // uranus
	TransformNode* neptuneOrbitRotator = new TransformNode(glm::mat4()); // neptune
	TransformNode* plutoOrbitRotator = new TransformNode(glm::mat4()); // pluto

	float yPosPlanets = 60.0f;
	TransformNode* sunTranslation = new TransformNode(glm::translate(glm::mat4(), glm::vec3(2, yPosPlanets, 0))); //  sun 
	TransformNode* mercuryTranslation = new TransformNode(glm::translate(glm::mat4(), glm::vec3(10, yPosPlanets, 0))); // mercury 
	TransformNode* venusTranslation = new TransformNode(glm::translate(glm::mat4(), glm::vec3(10, yPosPlanets, 0))); // venus 
	TransformNode* earthTranslation = new TransformNode(glm::translate(glm::mat4(), glm::vec3(20, yPosPlanets, 0))); // earth
	TransformNode* moonTranslation = new TransformNode(glm::translate(glm::mat4(), glm::vec3(5, yPosPlanets, 0))); // moon
	TransformNode* marsTranslation = new TransformNode(glm::translate(glm::mat4(), glm::vec3(30, yPosPlanets, 0))); // mars
	TransformNode* jupiterTranslation = new TransformNode(glm::translate(glm::mat4(), glm::vec3(35, yPosPlanets, 0))); // jupiter
	TransformNode* saturnTranslation = new TransformNode(glm::translate(glm::mat4(), glm::vec3(45, yPosPlanets, 0))); // saturn
	TransformNode* saturnringTranslation = new TransformNode(glm::translate(glm::mat4(), glm::vec3(45, yPosPlanets, 0))); // saturn ring
	TransformNode* uranusTranslation = new TransformNode(glm::translate(glm::mat4(), glm::vec3(50, yPosPlanets, 0))); // uranus
	TransformNode* neptuneTranslation = new TransformNode(glm::translate(glm::mat4(), glm::vec3(55, yPosPlanets, 0))); // neptune
	TransformNode* plutoTranslation = new TransformNode(glm::translate(glm::mat4(), glm::vec3(60, yPosPlanets, 0))); // pluto



	TransformNode* sunSpinRotator = new TransformNode(glm::mat4()); // Sun
	TransformNode* mercurySpinRotator = new TransformNode(glm::mat4()); // Mercury
	TransformNode* venusSpinRotator = new TransformNode(glm::mat4()); // Venus
	TransformNode* earthSpinRotator = new TransformNode(glm::mat4()); // Earth
	// no Moon spinRotator
	TransformNode* marsSpinRotator = new TransformNode(glm::mat4()); // Mars
	TransformNode* jupiterSpinRotator = new TransformNode(glm::mat4()); // Jupiter
	TransformNode* saturnSpinRotator = new TransformNode(glm::mat4()); // Saturn
	TransformNode* saturnringSpinRotator = new TransformNode(glm::mat4()); // Saturn ring
	TransformNode* uranusSpinRotator = new TransformNode(glm::mat4()); // Uranus
	TransformNode* neptuneSpinRotator = new TransformNode(glm::mat4()); // Neptune
	TransformNode* plutoSpinRotator = new TransformNode(glm::mat4()); // Pluto

	// hooking to graph
	// Sun
	light->AddChild(sunOrbitRotator);
	sunOrbitRotator->AddChild(sunTranslation);
	sunOrbitRotator->SetUpdateCallback(new YRotatorCallback(sunOrbitRotator, 0.0f));
	
	sunTranslation->AddChild(sunSpinRotator);
	sunSpinRotator->AddChild(sunScale);
	sunSpinRotator->SetUpdateCallback(new YRotatorCallback(sunSpinRotator, 40.0f));
	sunScale->AddChild(new GeometryNode(sun));

	// Mercury
	light->AddChild(mercuryOrbitRotator);
	mercuryOrbitRotator->AddChild(mercuryTranslation);
	mercuryOrbitRotator->SetUpdateCallback(new YRotatorCallback(mercuryOrbitRotator, 6.0f));

	mercuryTranslation->AddChild(mercurySpinRotator);
	mercurySpinRotator->AddChild(mercuryScale);
	mercurySpinRotator->SetUpdateCallback(new YRotatorCallback(mercurySpinRotator, 40.0f));
	mercuryScale->AddChild(new GeometryNode(mercury));

	// Venus
	light->AddChild(venusOrbitRotator);
	venusOrbitRotator->AddChild(venusTranslation);
	venusOrbitRotator->SetUpdateCallback(new YRotatorCallback(venusOrbitRotator, 4.0f));

	venusTranslation->AddChild(venusSpinRotator);
	venusSpinRotator->AddChild(venusScale);
	venusSpinRotator->SetUpdateCallback(new YRotatorCallback(venusSpinRotator, 20.0f));
	venusScale->AddChild(new GeometryNode(venus));

	// Earth
	light->AddChild(earthOrbitRotator);
	earthOrbitRotator->AddChild(earthTranslation);
	earthOrbitRotator->SetUpdateCallback(new YRotatorCallback(earthOrbitRotator, 1.0f));

	earthTranslation->AddChild(earthSpinRotator);
	earthSpinRotator->AddChild(earthScale);
	earthSpinRotator->SetUpdateCallback(new YRotatorCallback(earthSpinRotator, 10.0f));
	earthScale->AddChild(new GeometryNode(earth));
	// Moon
	earthTranslation->AddChild(moonOrbitRotator);
	moonOrbitRotator->AddChild(moonTranslation);
	moonOrbitRotator->SetUpdateCallback(new YRotatorCallback(moonOrbitRotator, 5.0f));
	moonTranslation->AddChild(new GeometryNode(moon));
	moonScale->AddChild(new GeometryNode(moon));
	// Mars
	light->AddChild(marsOrbitRotator);// mars
	marsOrbitRotator->AddChild(marsTranslation);
	marsOrbitRotator->SetUpdateCallback(new YRotatorCallback(marsOrbitRotator, 0.75f));

	marsTranslation->AddChild(marsSpinRotator); // mars
	marsSpinRotator->AddChild(marsScale);
	marsSpinRotator->SetUpdateCallback(new YRotatorCallback(marsSpinRotator, 10.0f));
	marsScale->AddChild(new GeometryNode(mars));

	// Jupiter
	light->AddChild(jupiterOrbitRotator);
	jupiterOrbitRotator->AddChild(jupiterTranslation);
	jupiterOrbitRotator->SetUpdateCallback(new YRotatorCallback(jupiterOrbitRotator, 1.0f));

	jupiterTranslation->AddChild(jupiterSpinRotator);
	jupiterSpinRotator->AddChild(jupiterScale);
	jupiterSpinRotator->SetUpdateCallback(new YRotatorCallback(jupiterSpinRotator, 5.0f));
	jupiterScale->AddChild(new GeometryNode(jupiter));

	// Saturn
	light->AddChild(saturnOrbitRotator);
	saturnOrbitRotator->AddChild(saturnTranslation);
	saturnOrbitRotator->SetUpdateCallback(new YRotatorCallback(saturnOrbitRotator, 0.8f));

	saturnTranslation->AddChild(saturnSpinRotator);
	saturnSpinRotator->AddChild(saturnScale);
	saturnSpinRotator->SetUpdateCallback(new YRotatorCallback(saturnSpinRotator, 1.0f));
	saturnScale->AddChild(new GeometryNode(saturn));

	// Saturn rings
	light->AddChild(saturnringOrbitRotator);
	saturnringOrbitRotator->AddChild(saturnringTranslation);
	saturnringOrbitRotator->SetUpdateCallback(new YRotatorCallback(saturnringOrbitRotator, 0.8f));

	saturnringTranslation->AddChild(saturnringSpinRotator);
	saturnringSpinRotator->AddChild(saturnringScale);
	saturnringSpinRotator->SetUpdateCallback(new YRotatorCallback(saturnringSpinRotator, 25.0f));
	saturnringScale->AddChild(new GeometryNode(saturnring));


	// Uranus
	light->AddChild(uranusOrbitRotator);
	uranusOrbitRotator->AddChild(uranusTranslation);
	uranusOrbitRotator->SetUpdateCallback(new YRotatorCallback(uranusOrbitRotator, 0.075f));

	uranusTranslation->AddChild(uranusSpinRotator);
	uranusSpinRotator->AddChild(uranusScale);
	uranusSpinRotator->SetUpdateCallback(new YRotatorCallback(uranusSpinRotator, 20.0f));
	uranusScale->AddChild(new GeometryNode(uranus));

	// Neptune
	light->AddChild(neptuneOrbitRotator);
	neptuneOrbitRotator->AddChild(neptuneTranslation);
	neptuneOrbitRotator->SetUpdateCallback(new YRotatorCallback(neptuneOrbitRotator, 0.75f));

	neptuneTranslation->AddChild(neptuneSpinRotator);
	neptuneSpinRotator->AddChild(neptuneScale);
	neptuneSpinRotator->SetUpdateCallback(new YRotatorCallback(neptuneSpinRotator, 20.0f));
	neptuneScale->AddChild(new GeometryNode(neptune));

	// PLuto
	light->AddChild(plutoOrbitRotator);
	plutoOrbitRotator->AddChild(plutoTranslation);
	plutoOrbitRotator->SetUpdateCallback(new YRotatorCallback(plutoOrbitRotator, 0.05f));

	plutoTranslation->AddChild(plutoSpinRotator);
	plutoSpinRotator->AddChild(plutoScale);
	plutoSpinRotator->SetUpdateCallback(new YRotatorCallback(plutoSpinRotator, 5.0f));
	plutoScale->AddChild(new GeometryNode(pluto));


	return light;

}