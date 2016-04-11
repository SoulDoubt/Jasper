#pragma once

#include <Jasper\Common.h>

#include <Jasper\GameObject.h>


namespace Jasper {

class Shader;
class Material;
class MeshRenderer;

class Renderer {

public:

	explicit Renderer(Scene* scene);
	Renderer(const Renderer& o) = delete;
	Renderer& operator=(const Renderer& o) = delete;

	void Initialize();
	void Destroy();

	

	void RenderScene();

	void RegisterGameObject(GameObject* obj);
	void UnregisterGameObject(GameObject* obj);

	
private:

	void ProcessGameObject(const GameObject* go);
	void SortByMaterial();
	void ReleaseTextures();
	void SetFrameInvariants(Material* material);
	void SetMaterialUniforms(Material* material);
	void CullGameObjects();

	Scene* m_scene;

	std::vector<GameObject*> m_renderObjects;
	std::vector<MeshRenderer*> m_renderers;

	Shader* m_currentShader = nullptr;
	Material* m_currentMaterial = nullptr;



};

}