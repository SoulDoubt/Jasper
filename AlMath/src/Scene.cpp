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

namespace Jasper {

using namespace std;

Scene::Scene(int width, int height)// : m_camera(Camera::CameraType::FLYING)
{
	m_windowWidth = width;
	m_windowHeight = height;	
}

Scene::~Scene()
{
	m_meshManager.Clear();
	m_shaderManager.Clear();
	m_materialManager.Clear();
	m_textureManager.Clear();
}

void Scene::Initialize() {
	
	m_rootNode = make_unique<GameObject>("Root_Node");
	m_rootNode->SetScene(this);

	m_physicsWorld = make_unique<PhysicsWorld>(this);
	m_physicsWorld->Initialize();
	m_camera = m_rootNode->AttachNewChild<Camera>(Camera::CameraType::FLYING);
	m_camera->SetPhysicsWorld(m_physicsWorld.get());
	/*{
		float mass = 80.f;

		const float width = 0.8f / 2.f;
		const float depth = 0.4f / 2.f;
		const float height = 1.9f / 2.f;		
		
		auto camCollider = m_camera->AttachNewComponent<CapsuleCollider>("Camera_Collider", Vector3(width, height, depth), m_physicsWorld.get());
		camCollider->Mass = mass;
	}*/
	
	
	auto pm = Matrix4();
	auto om = Matrix4();

	float aspectRatio = (float)m_windowWidth / (float)m_windowHeight;
	
	pm.CreatePerspectiveProjection(60.0f, aspectRatio, 0.1f, 1000.0f);
	om.CreateOrthographicProjection(0.0f, m_windowWidth, m_windowHeight, 0.0f, 1.0f, -1.0f);
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
	//auto skyboxRenderer = skybox->AttachNewComponent<SkyboxRenderer>(skyboxMesh, skyboxMaterial);
	
	// create the Basic Shader Instance to render most objects
	auto defaultShader = m_shaderManager.CreateInstance<LitShader>();	

	m_fontRenderer = make_unique<FontRenderer>();
	m_fontRenderer->Initialize();
	m_fontRenderer->SetOrthoMatrix(m_orthoMatrix);

	// Floor
	auto floor = CreateEmptyGameObject("floor", m_rootNode.get());
	auto quadMesh = m_meshManager.CreateInstance<Quad>(Vector2(100.0f, 100.0f), 10, 10, Quad::AxisAlignment::XZ);	
	auto floorMaterial = m_materialManager.CreateInstance<Material>(defaultShader);
	floorMaterial->SetTextureDiffuse("./textures/tile.jpg");
	auto mr = floor->AttachNewComponent<MeshRenderer>(quadMesh, floorMaterial);		
	floor->GetLocalTransform().Translate(Vector3( 0.0f, -1.f, 0.0f ));
	auto floorP = floor->AttachNewComponent<PlaneCollider>("plane_collider", quadMesh, m_physicsWorld.get());
	floorP->Friction = 0.9f;
	floorMaterial->Ambient = { 1.0f, 1.0f, 1.0f };	

	auto m1 = m_materialManager.CreateInstance<Material>(defaultShader);
	m1->SetTextureDiffuse("./textures/154.jpg");
	m1->SetTextureNormalMap("./textures/154_norm.jpg");
	m1->Shine = 12.0f;
	m1->Diffuse = { 0.85f, 0.85f, 0.85f };
	m1->Ambient = { 0.25f, 0.25f, 0.25f };
	m1->Specular = { 0.9f, 0.9f, 0.9f };

	auto wall = m_rootNode->AttachNewChild<GameObject>("wall_0");
	auto wallMesh = m_meshManager.CreateInstance<Cube>( Vector3(10.f, 10.f, 0.2f));
	auto wallRenderer = wall->AttachNewComponent<MeshRenderer>(wallMesh, m1);
	auto wallCollider = wall->AttachNewComponent<BoxCollider>("wall_0_collider", wallMesh, m_physicsWorld.get());
	wallCollider->Mass = 00.0f;	
	wall->GetLocalTransform().Translate({ 0.0f, 30.0f, -10.0f });
	wall->GetLocalTransform().Rotate({ 1.0f, 0.f, 0.f }, DEG_TO_RAD(90.f));

	auto cube = m_rootNode->AttachNewChild<GameObject>("cube_0");
	auto cubeMesh = m_meshManager.CreateInstance<Cube>(Vector3({ 1.5f, 1.5f, 1.5f }));
	cubeMesh->FlipTextureCoords();
	auto mr1 = cube->AttachNewComponent<MeshRenderer>(cubeMesh, m1);
	cube->GetLocalTransform().Translate({ 0.0f, 1.5f, 0.0f });
	//cube->GetLocalTransform().Rotate({ 0.f, 0.f, 1.f }, DEG_TO_RAD(45.f));
	auto bc = cube->AttachNewComponent<BoxCollider>("cube_box_collider", cubeMesh, m_physicsWorld.get());
	bc->Mass = 2.0f;

	/*auto cubechild = CreateEmptyGameObject("child_cube0", cube);
	auto childMesh = m_meshManager.CreateInstance<Cube>(Vector3(0.5f, 0.5f, 0.5f));
	childMesh->FlipTextureCoords();
	auto m2 = m_materialManager.CreateInstance<Material>(defaultShader);
	m2->SetTexture2D("./textures/jasper.jpg");	
	auto mr2 = cubechild->AttachNewComponent<MeshRenderer>(childMesh, m2);
	cubechild->GetLocalTransform().Translate({0.0f, 2.5f + 5.f, 0.0f});	*/
	

	//auto model = m_rootNode->AttachNewChild<Model>("teapot", "./models/teapot/teapot.obj", defaultShader, true, m_physicsWorld.get());	
	//model->GetLocalTransform().Translate(Vector3(0.0f, 0.0f, 0.0f));
	//model->GetLocalTransform().Scale = Vector3{ 0.025f, 0.025f, 0.025f };
	////model->GetLocalTransform().Rotate(Vector3(1.f, 0.f, 0.f), -DEG_TO_RAD(90));	
	//
	auto trooper = m_rootNode->AttachNewChild<Model>("barrel", "./models/oildrum/oildrum.obj", defaultShader, true, m_physicsWorld.get());
	trooper->GetLocalTransform().Translate({ 0.0f, 2.0f, 0.0f });

	/*auto lara = m_rootNode->AttachNewChild<Model>("lara_croft", "./models/lara/lara.dae", defaultShader, true, m_physicsWorld.get());
	lara->GetLocalTransform().Translate({ 4.0f, 1.0f, -3.0f });
	lara->GetLocalTransform().Rotate({ 0.0f, 0.0f, 1.0f }, -DEG_TO_RAD(90.f));*/

	//auto sphereObject = CreateEmptyGameObject("sphere0", m_rootNode.get());
	auto sphereObject = m_rootNode->AttachNewChild<GameObject>("sphere_0");
	auto sphereMesh = m_meshManager.CreateInstance<Sphere>(1.0f);
	auto sphereMat = m_materialManager.CreateInstance<Material>(defaultShader);
	sphereMat->SetTextureDiffuse("./textures/red.png");
	auto sphereRenderer = sphereObject->AttachNewComponent<MeshRenderer>(sphereMesh, sphereMat);
	auto sc = sphereObject->AttachNewComponent<SphereCollider>("sphere0_collider", sphereMesh, m_physicsWorld.get());
	sc->Mass = 5.f;
	sphereObject->GetLocalTransform().Translate({ 0.f, 25.f, -3.f });

	auto light0 = m_rootNode->AttachNewChild<PointLight>("light0");	
	light0->GetLocalTransform().Translate({ 0.0f, 5.f, 5.0f });	
	light0->ConstAtten = 0.002f;
	light0->Color = { 1.f, 1.f, 1.f };	
	light0->AmbientIntensity = 0.75f;

	auto lightMesh = m_meshManager.CreateInstance<Cube>(Vector3(0.1f, 0.1f, 0.1f));
	auto lightMaterial = m_materialManager.CreateInstance<Material>(defaultShader);
	lightMaterial->SetTextureDiffuse("./textures/white.png");
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

static const int max_samples = 64;
float fpsSamples[max_samples];

float CalcFPS(float dt) {	
	static int frameNumber = 0;
	fpsSamples[frameNumber % max_samples] = 1.0f / dt;
	float fps = 0;
	for (int i = 0; i < max_samples; i++) {
		fps += fpsSamples[i];
	}
	fps /= max_samples;
	frameNumber++;
	return fps;
}

void Scene::Update(float dt)
{	
	Vector3 position = m_camera->GetPosition();	
	Vector3 direction = m_camera->GetViewDirection();
	m_physicsWorld->Update(dt);			

	auto light = GetGameObjectByName("light0");
	float theta = DEG_TO_RAD(1.0f);
	float x = 5.0 * cosf(theta);
	float y = 5.0 * sinf(theta);
	//light->GetLocalTransform().Translate({ x, .0f, y });

	m_rootNode->Update(dt);		
	string pos = "Position: " + position.ToString();
	string dir = "Direction: " + direction.ToString();
	
	m_fontRenderer->RenderText(pos, 25, 25);
	m_fontRenderer->RenderText(dir, 25, 52);
	float fps = CalcFPS(dt);
	m_fontRenderer->RenderText("FPS: " + to_string(fps), 25, 75);
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
	m_materialManager.Clear();
	m_textureManager.Clear();
}

GameObject* Scene::CreateEmptyGameObject(std::string name, GameObject* parent)
{
	auto goup = make_unique<GameObject>(name);
	auto ret = goup.get();
	parent->AttachChild(move(goup));
	return ret;
}

} // namespace Jasper