#include <Jasper\Renderer.h>
#include <Jasper\MeshRenderer.h>
#include <Jasper\Scene.h>
#include <Jasper\Material.h>
#include <algorithm>

using namespace std;

Jasper::Renderer::Renderer(Scene * scene) : m_scene(scene)
{
}

void Jasper::Renderer::Initialize()
{
	const auto root = m_scene->GetRootNode();
	ProcessGameObject(root);
	SortByMaterial();
}

void Jasper::Renderer::SetFrameInvariants(Shader* shader)
{
	// ingariants accross all models include the
	// projection matrix, 
	// view matrix,
	// lights
	auto projectionMatrix = m_scene->ProjectionMatrix();
	auto viewMatrix = m_scene->GetCamera().GetViewMatrix().Inverted();


	
}


void Jasper::Renderer::RegisterGameObject(GameObject * obj)
{
	if (find(begin(m_renderObjects), end(m_renderObjects), obj) == end(m_renderObjects))
		m_renderObjects.push_back(obj);
}

void Jasper::Renderer::UnregisterGameObject(GameObject * obj)
{
	auto go = find(begin(m_renderObjects), end(m_renderObjects), obj);
	if (go != end(m_renderObjects)) {
		m_renderObjects.erase(go);
	}
}

void Jasper::Renderer::SortByMaterial()
{
	sort(begin(m_renderObjects), end(m_renderObjects), [&](GameObject* a, GameObject* b) {
		MeshRenderer* mra = a->FindFirstComponentByType<MeshRenderer>();
		MeshRenderer* mrb = b->FindFirstComponentByType<MeshRenderer>();
		// Game objects require a mesh renderer component in poder to be placed in thw collection
		// in the first place.
		const Material* mata = mra->GetMaterial();
		const Material* matb = mrb->GetMaterial();
		return mata > matb;
	});
}

void Jasper::Renderer::ProcessGameObject(const GameObject* root)
{
	for (const auto& child : root->Children()) {
		const auto mr = child->GetComponentByType<MeshRenderer>();
		if (mr != nullptr) {
			m_renderObjects.push_back(child.get());
		}
		ProcessGameObject(child.get());
	}
}

