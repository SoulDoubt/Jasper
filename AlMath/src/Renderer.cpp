#include <Jasper\Renderer.h>
#include <Jasper\MeshRenderer.h>
#include <Jasper\Scene.h>
#include <Jasper\Material.h>
#include <Jasper\GLError.h>
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

void Jasper::Renderer::SetFrameInvariants(Material* material)
{
	// ingariants accross all models include the
	// projection matrix, 
	// view matrix,
	// lights
	Shader* shader = material->GetShader();	
	//auto projectionMatrix = m_scene->ProjectionMatrix();
	auto viewMatrix = m_scene->GetCamera().GetViewMatrix().Inverted();
	auto pointLight = m_scene->GetGameObjectByName("p_light");
	auto directionalLight = m_scene->GetGameObjectByName("d_light");
	auto cameraPosition = m_scene->GetCamera().GetWorldTransform().Position;

	shader->SetViewMatrix(viewMatrix);
	shader->SetCameraPosition(cameraPosition);

	if (pointLight) {
		shader->SetPointLightUniforms((PointLight*)pointLight, pointLight->GetWorldTransform().Position);
	}

	if (directionalLight) {
		shader->SetDirectionalLightUniforms((DirectionalLight*)directionalLight);
	}			
}

void Jasper::Renderer::SetMaterialUniforms(Material * material)
{
	Shader* shader = material->GetShader();	
	shader->SetMaterialUniforms(material);

	bool hasNormalMap = material->GetTextureNormalMap();
	
	if (hasNormalMap) {
		int dl = glGetUniformLocation(shader->ProgramID(), "colorMap");
		int nl = glGetUniformLocation(shader->ProgramID(), "normalMap");
		if (dl > -1) {
			glUniform1i(dl, 0);
		}
		if (nl > -1) {
			glUniform1i(nl, 1);
		}
		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, material->GetTextureNormalMap()->TextureID());
	}
	glActiveTexture(GL_TEXTURE0 + 0);
	uint texID = material->GetTexture2D()->TextureID();
	glBindTexture(GL_TEXTURE_2D, texID);
	
}



void Jasper::Renderer::RenderScene()
{
	GLERRORCHECK;
	auto projMatrix = m_scene->ProjectionMatrix();
	auto viewMatrix = m_scene->GetCamera().GetViewMatrix().Inverted();	
	for (auto& mr : m_renderers) {		
		auto material = mr->GetMaterial();
		if (material != m_currentMaterial) {
			ReleaseTextures();
			if (m_currentMaterial != nullptr) {
				m_currentMaterial->GetShader()->Release();
			}
			m_currentMaterial = material;
			material->GetShader()->Bind();
			SetFrameInvariants(material);	
			SetMaterialUniforms(material);
		}
		auto transform = mr->GetGameObject()->GetWorldTransform();
		auto modelMatrix = transform.TransformMatrix();
		auto mvpMatrix = projMatrix * viewMatrix * modelMatrix;
		auto normMatrix = modelMatrix.NormalMatrix();
		material->GetShader()->SetModelMatrix(modelMatrix);
		material->GetShader()->SetModelViewProjectionMatrix(mvpMatrix);
		material->GetShader()->SetNormalMatrix(normMatrix);
		mr->Render();

#ifdef DEBUG_DRAW_PHYSICS
#if 0
		auto physics = mr->GetGameObject()->GetComponentByType<PhysicsCollider>();
		if (physics) {
			transform.Scale = { 1.f, 1.f, 1.f };
			auto physMM = transform.TransformMatrix();
			auto physicsDebugMvp = projMatrix * viewMatrix * physMM;
			btTransform debugTransform = btTransform();// physics->GetDebugTransform();
			debugTransform.setIdentity();
			btCollisionShape* shape = physics->GetCollisionShape();
			btVector3 color = { 1.0f, 0.0f, 0.0f };
			physics->GetPhysicsWorld()->debugDrawer->mvpMatrix = physicsDebugMvp;
			physics->GetPhysicsWorld()->DrawPhysicsShape(debugTransform, shape, color);
		}
		GLERRORCHECK;
#endif
#endif // DEBUG_DRAW_PHYSICS
	}
}


void Jasper::Renderer::RegisterGameObject(GameObject* obj)
{

	if (find(begin(m_renderObjects), end(m_renderObjects), obj) == end(m_renderObjects)) {
		m_renderObjects.push_back(obj);
		auto& renderers = obj->GetComponentsByType<MeshRenderer>();
		if (renderers.size() > 0) {
			m_renderers.insert(end(m_renderers), begin(renderers), end(renderers));
		}
	}
}

void Jasper::Renderer::UnregisterGameObject(GameObject* obj)
{
	auto go = find(begin(m_renderObjects), end(m_renderObjects), obj);
	if (go != end(m_renderObjects)) {
		m_renderObjects.erase(go);
	}
}

void Jasper::Renderer::SortByMaterial()
{
	sort(begin(m_renderers), end(m_renderers), [&](const MeshRenderer* a, const MeshRenderer* b) {					
		const Material* mata = a->GetMaterial();
		const Material* matb = b->GetMaterial();

		return mata > matb;
	});
}

void Jasper::Renderer::ReleaseTextures()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);	
	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Jasper::Renderer::ProcessGameObject(const GameObject* root)
{
	for (const auto& child : root->Children()) {
		if (child->GetName() != "skybox") {
			const auto mr = child->GetComponentByType<MeshRenderer>();
			if (mr != nullptr) {
				RegisterGameObject(child.get());
			}
		}
		ProcessGameObject(child.get());
	}
}

