#ifndef _JASPER_MESH_H_
#define _JASPER_MESH_H_

#include "Common.h"
#include "Component.h"
#include "vector.h"
#include "GLBuffer.h"
#include "Texture.h"

#include <vector>
#include <memory>

namespace Jasper {

struct Vertex {
	Vector3 Position;
	Vector3 Normal;
	Vector3 Tangent;
	Vector2 TexCoords;
	Vector4 Color;

	Vertex(float x, float y, float z, float u, float v) {
		Position = { x, y, z };
		TexCoords = { u, v };
		Normal = { 0.0f, 0.0f, 0.0f };
		Color = { 1.0f, 1.0f, 1.0f, 1.0f };
	}

	Vertex(const Vector3& position, const Vector3& normal, float u, float v) {
		Position = position;
		Normal = normal;
		Color = { 1.0f, 1.0f, 1.0f, 1.0f };
		TexCoords.x = u;
		TexCoords.y = v;
	}

	Vertex(const Vector3& position) {
		Position = position;
		TexCoords = { 0.0f, 1.0f };
		Normal = { 0.0f, 0.0f, 0.0f };
		Color = { 1.0f, 1.0f, 1.0f, 1.0f };
	}

	Vertex(const Vector3& position, const Vector3& normal, const Vector4& color, const Vector2& texCoords) {
		Position = position;
		Normal = normal;
		Color = color;
		TexCoords = texCoords;
	}

	Vertex() {
	}

};


class Mesh
{
	friend class Model;
private:


public:
	Mesh();
	explicit Mesh(const std::string& name);
	virtual ~Mesh();

	virtual void Initialize();
	virtual void Destroy();

	void FlipTextureCoords();

	void AddVertex(const Vertex& vertex);

	Vector3 GetHalfExtents() const {
		return m_Extents;
	}	

	Vector3 GetMaxExtents() const {
		return m_maxExtents;
	}

	Vector3 GetMinExtents() const {
		return m_minExtents;
	}

	unsigned int VertexCount;

	void SetReverseWinding(bool r) {
		m_reverseWinding = r;
	}
	void ReverseWinding();

	void UnloadClientData() {
		Vertices.clear();
		Indices.clear();
	}

	std::vector<Vertex> Vertices;
	std::vector<uint> Indices;

protected:
	

	void CalculateFaceNormals();
	void CalculateExtents();

	bool m_reverseWinding = false;

	Vector3 m_Extents;
	Vector3 m_minExtents;
	Vector3 m_maxExtents;

};

inline void Mesh::AddVertex(const Vertex& vertex) {
	Vertices.push_back(vertex);
}


inline void Mesh::ReverseWinding()
{
	for (unsigned i = 0; i < Indices.size(); i += 3) {
		unsigned i0 = Indices[i];
		unsigned i1 = Indices[i + 1];
		unsigned i2 = Indices[i + 2];
		Indices[i] = i2;
		Indices[i + 1] = i1;
		Indices[i + 2] = i0;
	}
}

} // namespace Jasper

#endif // _MESH_H_

