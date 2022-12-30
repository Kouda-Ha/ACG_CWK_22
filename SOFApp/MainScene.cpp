#include "stdafx.h"
#include "RenderLib.h"
#include "mainScene.h"
#include "Transforms.h"
#include "CubeGeom.h"
#include <random>

Node* Scene( Physics *phys);
extern Node* Scene2(); // skybox

class CamColl : public CameraCollisionCallback
{
	Physics* phys;
public:
	CamColl(Physics* phys) : phys(phys) {}
	glm::vec3 operator()(const glm::vec3& pos, const glm::vec3& newPos, float rad) const override
	{
		std::vector<ContactInfo> contacts = phys->CollideWithWorld(newPos, rad);
		glm::vec3 finalPos = newPos;
		for (auto c : contacts)
		{
			finalPos += c.normal * c.depth;
		}
		return finalPos;
	}
	
	virtual ~CamColl() {}
};

void MainScene::Init()
{

	// setup the camera
	eye = glm::vec3(0.0f, 15.0f, -40.0f);
	lookAt = glm::vec3(0.0f, 3.0f, 0.0f);
	up = glm::vec3(0.0f, 1.0f, 0.0); // y is up!

	// physics
	physics = new Physics();

	//camera = new FlyCam(eye, lookAt, true);
	camera = new FlyCam(eye, lookAt, true, new CamColl(physics));
	
	sceneGraphRoot = Scene(physics); // change to switch scenes (if applicable)
	// scene graph for dynamic objects 
	dynamicObjectNode = new GroupNode();
	sceneGraphRoot->AddChild(dynamicObjectNode);

	// initialises projectile sphere
	projectileNode = new TransformNode(glm::scale(glm::mat4(), glm::vec3(0.2f, 0.2f, 0.2f)));
	SphereGeometry geom(20);
	CubeGeometry cubeGeom;
	TexturedLit* mesh = new TexturedLit(geom, "textures/grid.jpg");
	projectileNode->AddChild(new GeometryNode(mesh));

	// Discworld Turtle/"Earth" found: https://www.thingiverse.com/thing:3145157
	meshObject = new MeshObject("meshes/turtleEarth/flatTurtle.obj");

	// render states
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void MainScene::Draw()
{
	// camera
	glm::mat4 view;
	glm::mat4 proj;

	eye = camera->GetPos();
	lookAt = camera->GetLookAt();
	up = camera->GetUp();
	view = glm::lookAt(eye, lookAt, up);
	proj = glm::perspective(glm::radians(60.0f), (float)SOF::Window::GetWidth() / (float)SOF::Window::GetHeight(), 0.1f, 1000.0f);
	
	// model
	glm::mat4 model = glm::mat4();
	Transforms trans(model, view, proj);


	glm::mat4 lightPositionMat;
	glm::mat4 lightColorMat;

	PointLight light(lightPositionMat, lightColorMat, 0.1f, 0.8f, 0.5f, 20, glm::vec4());

	model = glm::mat4();
	trans.SetModel(model);
	meshObject->Draw(trans, light);

	// Render the scene graph
	RenderVisitor rv(view, proj);
	rv.Traverse(sceneGraphRoot);
}

void MainScene::shoot() {
	TransformNode* node = new TransformNode(glm::mat4());
	dynamicObjectNode->AddChild(node);
	node->AddChild(projectileNode);
	float speed = 20.0f;
	glm::vec3 vel = glm::normalize( camera->GetLookAt() - camera->GetPos()) * speed;
	Projectile* proj = new Projectile( camera->GetPos(), vel, node);
	physics->AddProjectile(proj);
}

void MainScene::Update(float deltaT)
{
	physics->Update(deltaT);
	// Upadate the scene graph
	UpdateVisitor uv(deltaT);
	uv.Traverse(sceneGraphRoot);

	if (SOF::Window::IsKeyDown(GLFW_KEY_W))
		camera->Move(10.0f*deltaT);
	if (SOF::Window::IsKeyDown(GLFW_KEY_S))
		camera->Move(-10.0f * deltaT);
	if (SOF::Window::IsKeyDown(GLFW_KEY_A))
		camera->Strafe(-10.0f * deltaT);
	if (SOF::Window::IsKeyDown(GLFW_KEY_D))
		camera->Strafe(10.0f * deltaT);

	if (SOF::Window::IsKeyDown(GLFW_KEY_LEFT))
		camera->Pan(40.0f * deltaT);
	if (SOF::Window::IsKeyDown(GLFW_KEY_RIGHT))
		camera->Pan(-40.0f * deltaT);
	if (SOF::Window::IsKeyDown(GLFW_KEY_UP))
		camera->LookUpDown(40.0f * deltaT);
	if (SOF::Window::IsKeyDown(GLFW_KEY_DOWN))
		camera->LookUpDown(-40.0f * deltaT);

	// Projectile from player (pressing spacebar shoots projectile)
	if (SOF::Window::IsKeyPressed(GLFW_KEY_SPACE))
		shoot();

}


