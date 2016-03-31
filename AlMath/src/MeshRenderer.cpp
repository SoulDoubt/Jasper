#include <Jasper\MeshRenderer.h>
#include <Jasper\Material.h>
#include <Jasper\GameObject.h>
#include <Jasper\Mesh.h>
#include <Jasper\Shader.h>
#include <Jasper\Scene.h>
#include <Jasper\GLError.h>
#include <Jasper\Texture.h>
#include <Jasper\PhysicsCollider.h>
#include <Jasper\Lights.h>
#include <iostream>

namespace Jasper {

using namespace std;

MeshRenderer::MeshRenderer(Mesh* mesh, Material* material) :
	m_vertexBuffer(GLBuffer::BufferType::VERTEX),
	m_indexBuffer(GLBuffer::BufferType::INDEX)
{
	m_mesh = mesh;
	m_material = material;
}

MeshRenderer::~MeshRenderer()
{
	Destroy();
}

void MeshRenderer::Destroy() {
	m_vertexBuffer.Destroy();
	m_indexBuffer.Destroy();
}

void MeshRenderer::Awake() {

	GLERRORCHECK;
	// gather mesh data and create GL Buffers and such for future rendering...		
	assert(m_material);
	assert(m_mesh);

	// create a VAO first
	glGenVertexArrays(1, &m_vaoID);
	glBindVertexArray(m_vaoID);
	//GLERRORCHECK;

	m_elementCount = m_mesh->Indices.size();
	auto shader = m_material->GetShader();		

	m_vertexBuffer.Create();
	m_indexBuffer.Create();

	int positionLocation = shader->PositionAttributeLocation();
	int normalLocation = shader->NormalAttributeLocation();
	int texLocation = shader->TexCoordAttributeLocation();
	int tangentLocation = shader->TangentAttributeLocation();

	// upload the data
	m_vertexBuffer.Bind();
	m_vertexBuffer.Allocate(m_mesh->Vertices.data(), m_mesh->Vertices.size() * sizeof(Vertex));
	m_indexBuffer.Bind();
	m_indexBuffer.Allocate(m_mesh->Indices.data(), m_mesh->Indices.size() * sizeof(GLuint));

	// we can now destroy the client-side vertex data of the mesh...
	m_mesh->UnloadClientData();

	size_t positionOffset = offsetof(Vertex, Position);
	size_t normalOffset = offsetof(Vertex, Normal);
	size_t texOffset = offsetof(Vertex, TexCoords);
	size_t tangentOffset = offsetof(Vertex, Tangent);

	shader->SetAttributeArray(positionLocation, GL_FLOAT, (void*)positionOffset, 3, sizeof(Vertex));
	shader->SetAttributeArray(normalLocation, GL_FLOAT, (void*)normalOffset, 3, sizeof(Vertex));
	shader->SetAttributeArray(texLocation, GL_FLOAT, (void*)texOffset, 2, sizeof(Vertex));
	shader->SetAttributeArray(tangentLocation, GL_FLOAT, (void*)tangentOffset, 4, sizeof(Vertex));

	glBindVertexArray(0);
	GLERRORCHECK;
}

void MeshRenderer::Start() {}

void MeshRenderer::FixedUpdate() {}

void MeshRenderer::Update() {
	Render();
}

void MeshRenderer::LateUpdate() {}


void MeshRenderer::Render() {

	auto gameObject = GetGameObject();
	Shader* shader = m_material->GetShader();
	int shaderID = shader->ProgramID();
	shader->Bind();

	// get the transform,  either by physics or just get the raw transform
	Transform modelTrans;
	auto physics = gameObject->GetComponentByType<PhysicsCollider>();
	if (physics) {
		modelTrans = physics->GetCurrentWorldTransform();
		
		modelTrans.Scale = gameObject->GetLocalTransform().Scale;
		gameObject->SetLocalTransform(modelTrans);
	}
	else {
		modelTrans = gameObject->GetWorldTransform();
	}
	auto scene = gameObject->GetScene();
	
	auto camPos = scene->GetCamera().GetPosition();

	auto physicsDebugTrans = modelTrans;
	physicsDebugTrans.Scale = { 1.0f, 1.0f, 1.0f };
	auto physicsDebugModelMatrix = physicsDebugTrans.TransformMatrix();
	
	auto projectionMatrix = scene->ProjectionMatrix();
	auto viewMatrix = scene->GetCamera().GetViewMatrix().Inverted();
	auto modelMatrix = modelTrans.TransformMatrix();
	auto modelViewMatrix = viewMatrix * modelMatrix;
	auto mvp = projectionMatrix * modelViewMatrix;
	auto physicsDebugMvp = projectionMatrix * viewMatrix * physicsDebugModelMatrix;
	auto normalMatrix = modelMatrix.NormalMatrix();	
	//auto viewProjection = projectionMatrix * viewMatrix;

	//GLERRORCHECK;

	shader->SetViewMatrix(viewMatrix);
	shader->SetModelMatrix(modelMatrix);
	shader->SetModelViewMatrix(modelViewMatrix);
	shader->SetModelViewProjectionMatrix(mvp);
	shader->SetNormalMatrix(normalMatrix);
	//camPos = (viewMatrix * Vector4(0.f, 0.f, 0.f, 1.0f)).AsVector3();
	shader->SetCameraPosition(camPos);
	
	auto plight = scene->GetGameObjectByName("light0");
	if (plight) {
		PointLight* pl = static_cast<PointLight*>(plight);		
		Vector3 worldPosition = pl->GetWorldTransform().Position;
		//Vector3 eslp = (modelViewMa trix * Vector4(worldPosition, 1.0f)).AsVector3();
		shader->SetPointLightUniforms(pl, worldPosition);
	}
	auto dlight = scene->GetGameObjectByName("d_light");
	if (dlight) {
		DirectionalLight* dl = static_cast<DirectionalLight*>(dlight);
		shader->SetDirectionalLightUniforms(dl);
	}
	shader->SetMaterialUniforms(m_material);

	bool hasNormalMap = m_material->GetTextureNormalMap();	
	
	glBindVertexArray(m_vaoID);		
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
		glBindTexture(GL_TEXTURE_2D, m_material->GetTextureNormalMap()->TextureID());
	}
	glActiveTexture(GL_TEXTURE0 + 0);
	uint texID = m_material->GetTexture2D()->TextureID();
	glBindTexture(GL_TEXTURE_2D, texID);		
	glDrawElements(GL_TRIANGLES, m_elementCount, GL_UNSIGNED_INT, 0);	
	glBindTexture(GL_TEXTURE_2D, 0);
	if (hasNormalMap) {
		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	shader->Release();
	glBindVertexArray(0);

#ifdef DEBUG_DRAW_PHYSICS
	if (physics) {
		btTransform debugTransform = btTransform();// physics->GetDebugTransform();
		debugTransform.setIdentity();
		btCollisionShape* shape = physics->GetCollisionShape();
		btVector3 color = { 1.0f, 0.0f, 0.0f };
		physics->GetPhysicsWorld()->debugDrawer->mvpMatrix = physicsDebugMvp;
		physics->GetPhysicsWorld()->DrawPhysicsShape(debugTransform, shape, color);
	}
	GLERRORCHECK;
#endif // DEBUG_DRAW_PHYSICS
}

} // namespace Jasper