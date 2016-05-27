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
	
	size_t stride = sizeof(Vertex);

	shader->SetAttributeArray(positionLocation, GL_FLOAT, (void*)positionOffset, 3, stride);
	shader->SetAttributeArray(normalLocation, GL_FLOAT, (void*)normalOffset, 3, stride);
	shader->SetAttributeArray(texLocation, GL_FLOAT, (void*)texOffset, 2, stride);
	shader->SetAttributeArray(tangentLocation, GL_FLOAT, (void*)tangentOffset, 4, stride);

	glBindVertexArray(0);
	GLERRORCHECK;
}

void MeshRenderer::Start() {}

void MeshRenderer::FixedUpdate() {}

void MeshRenderer::Update() {

}

void MeshRenderer::LateUpdate() {}


void MeshRenderer::Render() {

	
	GLERRORCHECK;
	glBindVertexArray(m_vaoID);
	glDrawElements(GL_TRIANGLES, m_elementCount, GL_UNSIGNED_INT, 0);
	GLERRORCHECK;
	glBindVertexArray(0);


}

} // namespace Jasper
