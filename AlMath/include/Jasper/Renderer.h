#pragma once

#include <Jasper\Common.h>

#include <Jasper\GameObject.h>


namespace Jasper {

class Shader;
class Material;

class Renderer {

public:

	explicit Renderer(Scene* scene);
	Renderer(const Renderer& o) = delete;
	Renderer& operator=(const Renderer& o) = delete;

	void Initialize();
	void Destroy();

	void SetFrameInvariants(Shader* shader);
	void SetMaterialUniforms(Material* material);

	void RenderScene();

	void RegisterGameObject(GameObject* obj);
	void UnregisterGameObject(GameObject* obj);

	void SortByMaterial();
private:

	void ProcessGameObject(const GameObject* go);

	Scene* m_scene;

	std::vector<GameObject*> m_renderObjects;



};

}