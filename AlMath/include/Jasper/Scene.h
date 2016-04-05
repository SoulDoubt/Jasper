#ifndef _ALENGINE_SCENE_H_
#define _ALENGINE_SCENE_H_

#include "Common.h"
#include "GameObject.h"
#include "Lights.h"
#include "Shader.h"
#include "Camera.h"
#include "PhysicsWorld.h"
#include "Texture.h"
#include <vector>
#include <typeinfo>
#include "ResourceManager.h"
#include "Mesh.h"
#include "FontRenderer.h"

namespace Jasper {

class Scene
{
public:	
	explicit Scene(int width, int height);
	~Scene();	

	Matrix4& ProjectionMatrix() {
		return m_projectionMatrix;
	}

	Matrix4& OrthographicMatrix() {
		return m_orthoMatrix;
	}	

	GameObject* GetRootNode() const {
		return m_rootNode.get();
	}

	void AddGameObject(std::unique_ptr<GameObject> go);
	GameObject* GetGameObjectByName(std::string name);	

	void Update(float dt);
	void Awake();
	void Start();
	void Destroy();

	Camera& GetCamera() {
		if (m_camera) 
			return *m_camera;
		m_camera = GetGameObjectByType<Camera>();
		return *m_camera;
	};

	GameObject* CreateEmptyGameObject(std::string name, GameObject* parent);

	template<typename T> T* GetGameObjectByType() {
		for (auto& go : m_rootNode->Children()) {
			if (auto found = dynamic_cast<T*>(go.get())) {
				return found;
			}
		}
		return nullptr;
	}


private:
	std::unique_ptr<GameObject> m_rootNode;

	Matrix4 m_projectionMatrix;
	Matrix4 m_orthoMatrix;
	
	Camera* m_camera;

	std::unique_ptr<PhysicsWorld> m_physicsWorld;

	ResourceManager<Shader> m_shaderManager;	
	ResourceManager<Mesh> m_meshManager;
	ResourceManager<Material> m_materialManager;
	ResourceManager<Texture> m_textureManager;

	std::unique_ptr<FontRenderer> m_fontRenderer;

	int m_windowWidth, m_windowHeight;

	void Initialize();


};


}
#endif // _SCENE_H_