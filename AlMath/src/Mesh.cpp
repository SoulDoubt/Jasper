#include <gl\glew.h>
#include <Jasper\Mesh.h>
#include <Jasper\Shader.h>
#include <Jasper\GameObject.h>
#include <Jasper\GLError.h>

namespace Jasper {
using namespace std;

Mesh::Mesh()
{
	Initialize();
}

Mesh::Mesh(const std::string& name) {
	Initialize();
	CalculateHalfExtents();
}


Mesh::~Mesh()
{
	Destroy();
}

void Mesh::Initialize() {

}

void Mesh::Destroy() {
	Vertices.clear();
	Indices.clear();
}

void Mesh::FlipTextureCoords()
{
	for (auto& v : Vertices) {
		auto& tc = v.TexCoords;
		tc.y *= -1;
	}
}

void Mesh::CalculateFaceNormals()
{
	for (unsigned int i = 0; i < Indices.size(); i += 3) {
		unsigned index0 = Indices[i];
		unsigned index1 = Indices[i + 1];
		unsigned index2 = Indices[i + 2];
		Vertex& v0 = Vertices[index0];
		Vertex& v1 = Vertices[index1];
		Vertex& v2 = Vertices[index2];

		const Vector2 uv0 = v1.TexCoords - v0.TexCoords;
		const Vector2 uv1 = v2.TexCoords - v1.TexCoords;

		const float c = 1.0f / (uv0.x * uv1.y - uv0.y * uv1.x);

		const Vector3 normal = (v1.Position - v0.Position).Cross(v2.Position - v0.Position);
		const Vector3 tangent = Normalize((v0.Position * uv1.y - v1.Position * uv1.y) * c);

		v0.Normal += normal;
		v0.Tangent = tangent;
		v1.Normal += normal;
		v1.Tangent = tangent;
		v2.Normal += normal;
		v2.Tangent = tangent;
	}

	for (auto& v : Vertices) {
		v.Normal = Normalize(v.Normal);
		v.Tangent = Normalize(v.Tangent);
	}

	printf("Calculated some normals and Tangents\n");
}

void Mesh::CalculateHalfExtents()
{

	float xmin, ymin, zmin;
	xmin = ymin = zmin = 1000000.0f;
	float xmax, ymax, zmax;
	xmax = ymax = zmax = -1000000.0f;
	for (auto v : Vertices) {
		auto& vp = v.Position;
		if (vp.x < xmin) xmin = vp.x;
		if (vp.y < ymin) ymin = vp.y;
		if (vp.z < zmin) zmin = vp.z;
		if (vp.x > xmax) xmax = vp.x;
		if (vp.y > ymax) ymax = vp.y;
		if (vp.z > zmax) zmax = vp.z;
	}
	float x = (xmax - xmin) / 2;
	float y = (ymax - ymin) / 2;
	float z = (zmax - zmin) / 2;
	m_Extents = Vector3(x, y, z);
}
}