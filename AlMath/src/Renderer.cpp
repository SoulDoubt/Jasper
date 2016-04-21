#include <Jasper\Renderer.h>
#include <Jasper\MeshRenderer.h>
#include <Jasper\Scene.h>
#include <Jasper\Material.h>
#include <Jasper\GLError.h>
#include <algorithm>

#include <Jasper\GBuffer.h>

using namespace std;

Jasper::Renderer::Renderer(Scene * scene) : m_scene(scene)
{
	m_windowWidth = m_scene->m_windowWidth;
	m_windowHeight = m_scene->m_windowHeight;
}

void Jasper::Renderer::Initialize()
{
	//GBuffer gbuff = GBuffer(1024, 768);
	//gbuff.Initialize();
	const auto root = m_scene->GetRootNode();
	ProcessGameObject(root);

	// create a framebuffer for shadow mapping...
	//CreateShadowMapObjects();

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

	if (material->GetTextureSpecularMap()) {
		int sl = glGetUniformLocation(shader->ProgramID(), "specularMap");
		if (sl > -1) {
			glUniform1i(sl, 2);
		}
		glActiveTexture(GL_TEXTURE0 + 2);
		glBindTexture(GL_TEXTURE_2D, material->GetTextureSpecularMap()->TextureID());
	}
	glActiveTexture(GL_TEXTURE0 + 0);
	uint texID = material->GetTexture2D()->TextureID();
	glBindTexture(GL_TEXTURE_2D, texID);
	
}

void Jasper::Renderer::CullGameObjects()
{
	for (const auto x : m_renderObjects) {
		auto transform = x->GetWorldTransform();
		auto meshes = x->GetComponentsByType<Mesh>();
		for (const auto m : meshes) {
			auto max = m->GetMaxExtents();
			auto min = m->GetMinExtents();

		}
	}
}

void Jasper::Renderer::CreateShadowMapObjects()
{
	glGenFramebuffers(1, &m_shadowMapBufferID);

	glGenTextures(1, &m_shadowMapTextureID);
	glBindTexture(GL_TEXTURE_2D, m_shadowMapTextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,m_windowWidth, m_windowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindFramebuffer(GL_FRAMEBUFFER, m_shadowMapBufferID);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_shadowMapTextureID, 0);
	
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

}

void Jasper::Renderer::RenderShadowMap() {

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
	glActiveTexture(GL_TEXTURE0 + 2);
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

