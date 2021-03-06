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
	

	m_renderer = make_unique<Renderer>(this);
		
	auto pm = Matrix4();
	auto om = Matrix4();

	float aspectRatio = (float)m_windowWidth / (float)m_windowHeight;
	
	pm.CreatePerspectiveProjection(80.0f, aspectRatio, 0.1f, 1000.0f);
	om.CreateOrthographicProjection(0.0f, m_windowWidth, m_windowHeight, 0.0f, 1.0f, -1.0f);
	m_projectionMatrix = pm;
	m_orthoMatrix = om;

	auto debugShader = m_shaderManager.CreateInstance<BasicShader>();
	m_physicsWorld->debugDrawer->debugShader = debugShader;
	m_physicsWorld->debugDrawer->Initialize();

	// create the player
	btPairCachingGhostObject* ghost = new btPairCachingGhostObject();
	//ghost->setBroadphaseHandle(m_physicsWorld->get());
	auto playerShape = new btCapsuleShape(0.5f, 1.76f);
	m_player = make_unique<CharacterController>(ghost, playerShape, 1.0f, m_physicsWorld.get());
	

	//m_player = make_unique<CharacterController>()

	m_camera = m_rootNode->AttachNewChild<Camera>(Camera::CameraType::FLYING);
	m_camera->SetPhysicsWorld(m_physicsWorld.get());
	m_camera->AttachNewComponent<CapsuleCollider>("camera_collider", Vector3(1.f, 2.f, 1.f), m_physicsWorld.get());
		
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
	auto defaultShader = m_shaderManager.CreateInstance<LitShader>();	

	m_fontRenderer = make_unique<FontRenderer>();
	m_fontRenderer->Initialize();
	m_fontRenderer->SetOrthoMatrix(m_orthoMatrix);

	auto m1 = m_materialManager.CreateInstance<Material>(defaultShader);
	m1->SetTextureDiffuse("./textures/196.jpg");
	m1->SetTextureNormalMap("./textures/196_norm.jpg");
	m1->Shine = 12.0f;
	m1->Diffuse = { 0.85f, 0.85f, 0.85f };
	m1->Ambient = { 0.25f, 0.25f, 0.25f };
	m1->Specular = { 0.1f, 0.1f, 0.1f };

	// Floor
	auto floor = CreateEmptyGameObject("floor", m_rootNode.get());
	auto quadMesh = m_meshManager.CreateInstance<Quad>(Vector2(100.0f, 100.0f), 20, 20, Quad::AxisAlignment::XZ);	
	auto floorMaterial = m_materialManager.CreateInstance<Material>(defaultShader);
	floorMaterial->SetTextureDiffuse("./textures/tile.jpg");
	auto mr = floor->AttachNewComponent<MeshRenderer>(quadMesh, m1);		
	floor->GetLocalTransform().Translate(Vector3( 0.0f, -1.f, 0.0f ));
	auto floorP = floor->AttachNewComponent<PlaneCollider>("plane_collider", quadMesh, m_physicsWorld.get());
	floorP->Friction = 0.9f;
	floorMaterial->Ambient = { 1.0f, 1.0f, 1.0f };	
	

	auto wall = m_rootNode->AttachNewChild<GameObject>("wall_0");
	auto wallMesh = m_meshManager.CreateInstance<Cube>( Vector3(50.f, 50.0f, 0.2f));
	auto wallRenderer = wall->AttachNewComponent<MeshRenderer>(wallMesh, m1);
	auto wallCollider = wall->AttachNewComponent<BoxCollider>("wall_0_collider", wallMesh, m_physicsWorld.get());
	wallCollider->Mass = 0.0f;	
	wall->GetLocalTransform().Translate(0.0f, 25.0f, -20.0f);

	//auto citadel = m_rootNode->AttachNewChild<Model>("citadel", "./models/Police - Half-Life 2/Police.obj", defaultShader, false, nullptr);
	//citadel->GetLocalTransform().Scale = { 1.01f, 1.01f, 1.01f };

	//wall->GetLocalTransform().Translate({ 0.0f, 0.0f, 0.0f });
	//wall->GetLocalTransform().Rotate({ 1.0f, 0.f, 0.f }, DEG_TO_RAD(90.f));

	//auto sponza = m_rootNode->AttachNewChild<Model>("crytek_sponza", "./models/crytek-sponza/sponza.obj", defaultShader, false, nullptr);
	//sponza->GetLocalTransform().Scale = { 0.05f, 0.05f, 0.05f };
	//sponza->GetLocalTransform().Translate(0.f, 5.f, 0.f);
	//auto cube = m_rootNode->AttachNewChild<GameObject>("cube_0");
	//auto cubeMesh = m_meshManager.CreateInstance<Cube>(Vector3({ 1.5f, 1.5f, 1.5f }));
	//cubeMesh->FlipTextureCoords();
	//auto mr1 = cube->AttachNewComponent<MeshRenderer>(cubeMesh, m1);
	//cube->GetLocalTransform().Translate({ 0.0f, 1.5f, 0.0f });
	////cube->GetLocalTransform().Rotate({ 0.f, 0.f, 1.f }, DEG_TO_RAD(45.f));
	//auto bc = cube->AttachNewComponent<BoxCollider>("cube_box_collider", cubeMesh, m_physicsWorld.get());
	//bc->Mass = 2.0f;

	/*auto cubechild = CreateEmptyGameObject("child_cube0", cube);
	auto childMesh = m_meshManager.CreateInstance<Cube>(Vector3(0.5f, 0.5f, 0.5f));
	childMesh->FlipTextureCoords();
	auto m2 = m_materialManager.CreateInstance<Material>(defaultShader);
	m2->SetTexture2D("./textures/jasper.jpg");	
	auto mr2 = cubechild->AttachNewComponent<MeshRenderer>(childMesh, m2);
	cubechild->GetLocalTransform().Translate({0.0f, 2.5f + 5.f, 0.0f});	*/
	
	/*auto model = m_rootNode->AttachNewChild<Model>("teapot", "./models/teapot/teapot.obj", defaultShader, true, m_physicsWorld.get());	
	model->GetLocalTransform().Translate(Vector3(0.0f, 30.0f, 0.0f));
	model->GetLocalTransform().UniformScale(0.02f);
	model->GetLocalTransform().Rotate(Vector3(1.f, 0.f, 0.f), -DEG_TO_RAD(90));*/
	//
	auto teapot = m_rootNode->AttachNewChild<Model>("teapot", "./models/teapot/teapot.obj", defaultShader, true, m_physicsWorld.get());
	teapot->GetLocalTransform().Translate({ 0.0f, 20.0f, 0.0f });
	teapot->GetLocalTransform().Scale = { 0.04f, 0.04f, 0.04f };
	teapot->Mass = 20.f;
	teapot->Restitution = 1.2f;
	teapot->ColliderType = PHYSICS_COLLIDER_TYPE::Box;

	/*teapot->UpdateEvent += [](float dt) {
		printf("Update Event Called.\n");
	};*/
				
	//auto lara = m_rootNode->AttachNewChild<Model>("lara_croft", "./models/Lara/lara.dae", defaultShader, true, m_physicsWorld.get());
	//auto lara = m_rootNode->AttachNewChild<Model>("lara_croft", "./models/Joslin_Reyes_Bikini/Joslin_Reyes_Bikini.dae", defaultShader, true, m_physicsWorld.get());
	//lara->GetLocalTransform().Translate({ 10.0f, 1.0f, -3.0f });
	//lara->GetLocalTransform().Scale = { 1.6f, 1.6f, 1.6f };
	//lara->GetLocalTransform().Rotate({ 1.0f, 0.0f, 0.0f }, -DEG_TO_RAD(90.f));
	//lara->GetLocalTransform().Rotate({ 0.f, 1.f, 0.f }, DEG_TO_RAD(180.f));

	auto mathias = m_rootNode->AttachNewChild<Model>("mathias", "./models/Stormtrooper/stormtrooper.obj", defaultShader, true, m_physicsWorld.get());
	mathias->GetLocalTransform().Translate({ -10.f, 20.f, -3.f });
	mathias->Restitution = 1.2f;
	//mathias->GetLocalTransform().Scale = { 2.0f, 2.0f, 2.0f };
	mathias->Mass = 80.f;
	mathias->ColliderType = PHYSICS_COLLIDER_TYPE::Capsule;
	/*mathias->SetUpdateEvent([&](float dt) {
		mathias->GetLocalTransform().RotateAround({ -8.f, 1.f, -3.f }, { 0.f, 1.f, 0.f }, 1.f);
	});*/

	//auto sponza = m_rootNode->AttachNewChild<Model>("sponza", "./models/sponza/sponza.obj", defaultShader, false, m_physicsWorld.get());
	//sponza->GetLocalTransform().UniformScale(0.05f);
	//sponza->GetLocalTransform().Translate(0.0f, 31.9f, 0.0f);

	//auto hf = m_rootNode->AttachNewChild<Model>("human_female", "./models/Human_Female/Human_Female.obj", defaultShader, true, m_physicsWorld.get());
	//hf->GetLocalTransform().Translate(10, 1, 3);
	/*
	auto sphereObject = m_rootNode->AttachNewChild<GameObject>("sphere_0");
	auto sphereMesh = m_meshManager.CreateInstance<Sphere>(1.0f);
	auto sphereMat = m_materialManager.CreateInstance<Material>(defaultShader);
	sphereMat->SetTextureDiffuse("./textures/red.png");
	auto sphereRenderer = sphereObject->AttachNewComponent<MeshRenderer>(sphereMesh, sphereMat);
	auto sc = sphereObject->AttachNewComponent<SphereCollider>("sphere0_collider", sphereMesh, m_physicsWorld.get());
	sc->Mass = 5.f;
	sphereObject->GetLocalTransform().Translate({ 0.f, 25.f, -3.f });*/

	auto light0 = m_rootNode->AttachNewChild<PointLight>("p_light");	
	light0->GetLocalTransform().Translate({ 0.0f, 10.f, 15.0f });	
	light0->ConstAtten = 0.002f;
	light0->Color = { 1.f, 1.f, 1.f };	
	light0->AmbientIntensity = 0.15f;
	light0->DiffuseIntensity = 1.f;

	auto lightMesh = m_meshManager.CreateInstance<Cube>(Vector3(0.1f, 0.1f, 0.1f));
	auto lightMaterial = m_materialManager.CreateInstance<Material>(defaultShader);
	lightMaterial->SetTextureDiffuse("./textures/white.png");
	light0->AttachNewComponent<MeshRenderer>(lightMesh, lightMaterial);

	auto dlight = m_rootNode->AttachNewChild<DirectionalLight>("d_light");
	dlight->Direction = Normalize({ 0.0, -1.f, -1.f });
	dlight->AmbientIntensity = 0.54f;
	dlight->Diffuseintensity = 0.93f;


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

Vector3 pb = { 20.f, 1.f, -20.f };
Vector3 pa = { -10.f, 1.f, -3.f };
float lerpTime = 1.0f;
float currentLerpTime = 0.f;

void Scene::Update(float dt)
{	
	auto point_light = GetGameObjectByName("p_light");
	if (point_light) {
		point_light->GetLocalTransform().RotateAround({ 0.f, 10.f, 0.f }, { 0.f, 1.f, 0.f }, 1.0f);
	}

	/*auto mathias = GetGameObjectByName("mathias");
	if (mathias) {
		mathias->GetLocalTransform().RotateAround({ -5.f, 1.f, 0.f }, { 0.f, 1.f, 0.f }, 0.5f);
	}*/
	
	Vector3 position = m_camera->GetPosition();	
	Vector3 direction = m_camera->GetViewDirection();
	m_physicsWorld->Update(dt);					
	m_rootNode->Update(dt);		
	m_renderer->RenderScene();

#ifdef DEBUG_DRAW_PHYSICS
	auto viewMatrix = m_camera->GetViewMatrix().Inverted();
	for (auto& go : m_rootNode->Children()) {
		auto phys = go->GetComponentsByType<PhysicsCollider>();
		if (phys.size() > 0) {
			auto trans = go->GetWorldTransform();
			trans.Scale = { 1.f, 1.f, 1.f };
			auto mvp = m_projectionMatrix * viewMatrix * trans.TransformMatrix();
			m_physicsWorld->debugDrawer->mvpMatrix = mvp;
			btTransform dt;
			dt.setIdentity();
			for (auto collider : phys) {
				m_physicsWorld->DrawPhysicsShape(dt, collider->GetCollisionShape(), { 1.f, 1.f, 1.f });
			}
		}
		for (auto& child : go->Children()) {
			auto phys = child->GetComponentsByType<PhysicsCollider>();
			if (phys.size() > 0) {
				auto trans = child->GetWorldTransform();
				trans.Scale = { 1.f, 1.f, 1.f };
				auto mvp = m_projectionMatrix * viewMatrix * trans.TransformMatrix();
				m_physicsWorld->debugDrawer->mvpMatrix = mvp;
				btTransform dt;
				dt.setIdentity();
				for (auto collider : phys) {
					m_physicsWorld->DrawPhysicsShape(dt, collider->GetCollisionShape(),  { 1.f, 1.f, 1.f });
				}
			}
		}
	}
	auto playerTrans = m_player->GetGhostWorldTransform();
	playerTrans.UniformScale(1.0f);
	auto mvp = m_projectionMatrix * viewMatrix * playerTrans.TransformMatrix();
	m_physicsWorld->debugDrawer->mvpMatrix = mvp;
	btTransform btt;
	btt.setIdentity();
	m_physicsWorld->DrawPhysicsShape(btt, m_player->GetCollisionShape(), {1.f, 1.f, 1.f});
#endif
	string pos = "Position: " + position.ToString();
	string dir = "Direction: " + direction.ToString();
	
	m_fontRenderer->SetColor(1.0f, 0.0f, 0.0f);
	m_fontRenderer->RenderText(pos, 25, 25);
	//fontRenderer->SetColor(0.0f, 0.0f, 1.0f);
	m_fontRenderer->RenderText(dir, 25, 52);
	float fps = CalcFPS(dt);
	m_fontRenderer->RenderText("FPS: " + to_string(fps), 25, 75);
}



void Scene::Awake()
{
	Initialize();
	m_rootNode->Awake();
	m_renderer->Initialize();

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

void Scene::DoLeftClick(double x, double y) {
	printf("\n Mouse Click at: %f.2, %f.2", x, y);
}

void Scene::DestroyGameObject(std::unique_ptr<GameObject> object) {
	GameObject* go = object.release();
	go->Destroy();
	delete go;
}

} // namespace Jasper