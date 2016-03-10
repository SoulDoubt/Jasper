#ifndef _ALENGINE_SCENE_H_
#define _ALENGINE_SCENE_H_

#include "Common.h"
#include "GameObject.h"
#include "DirectionalLight.h"
#include "Shader.h"
#include "Camera.h"
#include "PhysicsWorld.h"
#include <vector>
#include <typeinfo>
#include "FontRenderer.h"
#include "ResourceManager.h"

namespace Jasper {

class Scene
{
public:
	Scene();
	~Scene();	

	Matrix4& ProjectionMatrix() {
		return m_projectionMatrix;
	}

	Matrix4& OrthographicMatrix() {
		return m_orthoMatrix;
	}	

	void AddGameObject(std::unique_ptr<GameObject> go);
	GameObject* GetGameObjectByName(std::string name);	

	void Update(float dt);
	void Awake();
	void Start();
	void Destroy();

	Camera& GetCamera() {
		return m_camera;
	};

	GameObject* CreateEmptyGameObject(std::string name, GameObject* parent);




private:
	std::unique_ptr<GameObject> m_rootNode;

	Matrix4 m_projectionMatrix;
	Matrix4 m_orthoMatrix;
	
	Camera m_camera;

	std::unique_ptr<PhysicsWorld> m_physicsWorld;

	ResourceManager<Shader> m_shaderManager;	
	ResourceManager<Mesh> m_meshManager;
	ResourceManager<Material> m_materialManager;

	std::unique_ptr<FontRenderer> m_fontRenderer;

	void Initialize();


};


}
#endif // _SCENE_H_