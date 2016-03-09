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

namespace Jasper {

template<typename T>
class ResourceManager {

	using Cache = std::vector<std::unique_ptr<T>>;
		
private:

	Cache m_cache;

public:

	ResourceManager() {
		m_cache.reserve(100);
	}

	template<typename U, typename... Args>
	U* CreateInstance(Args&&... args) {
		if (!std::is_base_of<T, U>::value) {
			return nullptr;
		}
		std::unique_ptr<U> instance = std::make_unique<U>(args...);
		U* ret = instance.get();
		m_cache.push_back(std::move(instance));
		return ret;
	}

	void Clear() {
		m_cache.clear();
	}

	Cache& GetCache() {
		return m_cache;
	}

	T* GetLatestInstanceAdded() {
		return m_cache[m_cache.size() - 1].get();
	}

	T* GetInstance(int index) {
		assert(index < m_cache.size());
		return m_cache[index].get();
	}

	size_t GetSize() {
		return m_cache.size();
	}
};



ALIGN16
class Scene
{
public:
	Scene();
	~Scene();

	ALIGN_16_OPERATORS;

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