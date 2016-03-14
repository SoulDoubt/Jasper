#include <memory>

#include <Jasper\Scene.h>
#include <Jasper\Quad.h>
#include <Jasper\Cube.h>
#include <Jasper\Material.h>
#include <Jasper\BasicShader.h>
#include <Jasper\LitShader.h>
#include <Jasper\SkyboxShader.h>
#include <Jasper\TextureShader.h>
#include <Jasper\MeshRenderer.h>
#include <Jasper\SkyboxRenderer.h>
#include <Jasper\Model.h>
#include <Jasper\PlaneCollider.h>
#include <Jasper\BoxCollider.h>
#include <Jasper\Sphere.h>
#include <Jasper\SphereCollider.h>
#include <Jasper\PointLight.h>


namespace Jasper {

using namespace std;

Scene::Scene() : m_camera(Camera::CameraType::FLYING)
{
	

}

Scene::~Scene()
{
	m_meshManager.Clear();
	m_shaderManager.Clear();
	m_materialManager.Clear();
}

void DrawText(char* text, float x, float y, float r, float g, float b) {
	
}

void Scene::Initialize() {
	m_rootNode = make_unique<GameObject>("Root_Node");
	m_rootNode->SetScene(this);

	m_physicsWorld = make_unique<PhysicsWorld>(this);
	m_physicsWorld->Initialize();
	m_camera.SetPhysicsWorld(m_physicsWorld.get());
	m_camera.Awake();	
	auto pm = Matrix4();
	auto om = Matrix4();

	pm.CreatePerspectiveProjection(60.0f, 800.0f / 600.0f, 0.1f, 1000.0f);
	om.CreateOrthographicProjection(800.f / 600.f, 800.f / 600.f, 600.f, 0.f, -1.0f, 1.0f);
	m_projectionMatrix = pm;
	m_orthoMatrix = om;

	auto debugShader = m_shaderManager.CreateInstance<BasicShader>();
	m_physicsWorld->debugDrawer->debugShader = debugShader;
	m_physicsWorld->debugDrawer->Initialize();

	// perform actual game object initialization

	// create the skybox
	auto skybox = CreateEmptyGameObject("skybox", m_rootNode.get());
	auto skyboxMesh = m_meshManager.CreateInstance<Cube>(Vector3(100.0f, 100.0f, 100.0f), true);
	skyboxMesh->SetCubemap(true); // we want to render the inside of the cube
	auto skyboxShader = m_shaderManager.CreateInstance<SkyboxShader>();
	auto skyboxMaterial = m_materialManager.CreateInstance<Material>(skyboxShader);
	string posx = "./textures/Yokohama2/posx.jpg";
	string negx = "./textures/Yokohama2/negx.jpg";
	string posy = "./textures/Yokohama2/posy.jpg";
	string negy = "./textures/Yokohama2/negy.jpg";
	string posz = "./textures/Yokohama2/posz.jpg";
	string negz = "./textures/Yokohama2/negz.jpg";
	skyboxMaterial->SetCubemapTextures(posx, negx, posy, negy, posz, negz);	
	auto skyboxRenderer = skybox->AttachNewComponent<SkyboxRenderer>(skyboxMesh, skyboxMaterial);
	
	// create the Basic Shader Instance to render most objects
	auto defaultShader = m_shaderManager.CreateInstance<TextureShader>();	

	// Floor
	auto floor = CreateEmptyGameObject("floor", m_rootNode.get());
	auto quadMesh = m_meshManager.CreateInstance<Quad>(Vector2(100.0f, 100.0f), 10, 10, Quad::AxisAlignment::XZ);	
	auto floorMaterial = m_materialManager.CreateInstance<Material>(defaultShader);
	floorMaterial->SetTexture2D("./textures/tile.jpg");
	auto mr = floor->AttachNewComponent<MeshRenderer>(quadMesh, floorMaterial);		
	floor->GetLocalTransform().Translate(Vector3( 0.0f, -1.f, 0.0f ));
	auto floorP = floor->AttachNewComponent<PlaneCollider>("plane_collider", quadMesh, m_physicsWorld.get());
	floorP->Friction = 0.9f;
	floorMaterial->Ambient = { 1.0f, 1.0f, 1.0f };

	auto wall = m_rootNode->AttachNewChild<GameObject>("wall_0");
	auto wallMesh = m_meshManager.CreateInstance<Cube>( Vector3(10.f, 10.f, 0.2f));
	auto wallRenderer = wall->AttachNewComponent<MeshRenderer>(wallMesh, floorMaterial);
	auto wallCollider = wall->AttachNewComponent<BoxCollider>("wall_0_collider", wallMesh, m_physicsWorld.get());
	wallCollider->Mass = 10.0f;
	wall->GetLocalTransform().Translate({ 0.0f, 7.0f, -10.0f });

	auto cube = m_rootNode->AttachNewChild<GameObject>("cube_0");
	auto cubeMesh = m_meshManager.CreateInstance<Cube>(Vector3({ 1.5f, 1.5f, 1.5f }));
	auto m1 = m_materialManager.CreateInstance<Material>(defaultShader);
	m1->SetTexture2D("./textures/crate.png");
	m1->Shine = 36.0f;
	m1->Diffuse = { 0.8f, 0.8f, 0.8f };
	m1->Ambient = { 0.25f, 0.25f, 0.25f };
	m1->Specular = Vector3(0.843f, 0.843f, 0.843f);	
	auto mr1 = cube->AttachNewComponent<MeshRenderer>(cubeMesh, m1);
	cube->GetLocalTransform().Translate({ 0.0f, 1.0f, -3.0f });
	cube->GetLocalTransform().Rotate({ 1.f, 0.f, 0.f }, DEG_TO_RAD(30.f));
	auto bc = cube->AttachNewComponent<BoxCollider>("cube_box_collider", cubeMesh, m_physicsWorld.get());
	bc->Mass = 10.0f;

	/*auto cubechild = CreateEmptyGameObject("child_cube0", cube);
	auto childMesh = m_meshManager.CreateInstance<Cube>(Vector3(0.5f, 0.5f, 0.5f));
	childMesh->FlipTextureCoords();
	auto m2 = m_materialManager.CreateInstance<Material>(defaultShader);
	m2->SetTexture2D("./textures/jasper.jpg");	
	auto mr2 = cubechild->AttachNewComponent<MeshRenderer>(childMesh, m2);
	cubechild->GetLocalTransform().Translate({0.0f, 2.5f + 5.f, 0.0f});	*/

	auto model = m_rootNode->AttachNewChild<Model>("lara", "./models/lara/lara.dae", defaultShader, true, m_physicsWorld.get());	
	model->GetLocalTransform().Translate(Vector3(0.0f, 0.0f, 0.0f));
	//model->GetLocalTransform().Scale = Vector3{ 0.025f, 0.025f, 0.025f };
	model->GetLocalTransform().Rotate(Vector3(1.f, 0.f, 0.f), -DEG_TO_RAD(90));	
	
	//auto trooper = m_rootNode->AttachNewChild<Model>("Stormtrooper", "./models/Stormtrooper/Stormtrooper.obj", defaultShader, true, m_physicsWorld.get());
	//trooper->GetLocalTransform().Translate({ 0.0f, 2.0f, 0.0f });

	//auto sphereObject = CreateEmptyGameObject("sphere0", m_rootNode.get());
	auto sphereObject = m_rootNode->AttachNewChild<GameObject>("sphere_0");
	auto sphereMesh = m_meshManager.CreateInstance<Sphere>(1.0f);
	auto sphereMat = m_materialManager.CreateInstance<Material>(defaultShader);
	sphereMat->SetTexture2D("./textures/red.png");
	auto sphereRenderer = sphereObject->AttachNewComponent<MeshRenderer>(sphereMesh, sphereMat);
	auto sc = sphereObject->AttachNewComponent<SphereCollider>("sphere0_collider", sphereMesh, m_physicsWorld.get());
	sc->Mass = 5.f;
	sphereObject->GetLocalTransform().Translate({ 0.f, 25.f, -3.f });

	auto light0 = m_rootNode->AttachNewChild<PointLight>("light0");	
	light0->GetLocalTransform().Translate({ 0.0f, 10.0f, 0.0f });	
	light0->ConstAtten = 0.025f;
	auto lightMesh = m_meshManager.CreateInstance<Cube>(Vector3(0.1f, 0.1f, 0.1f));
	auto lightMaterial = m_materialManager.CreateInstance<Material>(defaultShader);
	lightMaterial->SetTexture2D("./textures/white.png");
	light0->AttachNewComponent<MeshRenderer>(lightMesh, lightMaterial);

}

void Scene::AddGameObject(std::unique_ptr<GameObject> go)
{
	m_rootNode->AttachChild(move(go));
}

GameObject* Scene::GetGameObjectByName(string name)
{
	auto gp = m_rootNode->FindChildByName(name);
	if (gp)
	{
		return gp;
	}
	return nullptr;
}

void Scene::Update(float dt)
{	
	m_physicsWorld->Update(dt);			
	m_rootNode->Update(dt);	
}

void Scene::Awake()
{
	Initialize();
	m_rootNode->Awake();
}

void Scene::Start()
{
	m_rootNode->Start();
}

void Scene::Destroy()
{
	m_rootNode->Destroy();
	m_meshManager.Clear();
	m_shaderManager.Clear();
}

GameObject* Scene::CreateEmptyGameObject(std::string name, GameObject* parent)
{
	auto goup = make_unique<GameObject>(name);
	auto ret = goup.get();
	parent->AttachChild(move(goup));
	return ret;
}

} // namespace Jasper