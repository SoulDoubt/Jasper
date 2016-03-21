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
	CalculateExtents();
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
		Vertex& v1 = Vertices[index0];
		Vertex& v2 = Vertices[index1];
		Vertex& v3 = Vertices[index2];

		// Tangent Space calculations adapted from
		// Lengyel, Eric. “Computing Tangent Space Basis Vectors for an Arbitrary Mesh”. Terathon Software 3D Graphics Library, 2001. http://www.terathon.com/code/tangent.html

		float x1 = v2.Position.x - v1.Position.x;
		float x2 = v3.Position.x - v1.Position.x;
		float y1 = v2.Position.y - v1.Position.y;
		float y2 = v3.Position.y - v1.Position.y;
		float z1 = v2.Position.z - v1.Position.z;
		float z2 = v3.Position.z - v1.Position.z;

		float s1 = v2.TexCoords.x - v1.TexCoords.x;
		float s2 = v3.TexCoords.x - v1.TexCoords.x;
		float t1 = v2.TexCoords.y - v1.TexCoords.y;
		float t2 = v3.TexCoords.y - v1.TexCoords.y;

		float r = 1.0F / (s1 * t2 - s2 * t1);
		Vector3 sdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r,
			(t2 * z1 - t1 * z2) * r);
		Vector3 tdir((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r,
			(s1 * z2 - s2 * z1) * r);

		Vector4 tangent = { sdir, 0.0f };
		Vector3 bitangent = tdir;
		

		Vector3 edge1 = v2.Position - v1.Position;
		Vector3 edge2 = v3.Position - v1.Position;

		Vector3 normal = (edge1).Cross(edge2);
		normal = Normalize(normal);

		//const Vector3 tangent = Normalize((v0.Position * uv1.y - v1.Position * uv1.y) * c);

		v1.Normal += normal;
		v1.Tangent += tangent;		
		v1.Bitangent += bitangent;
		v2.Normal += normal;
		v2.Tangent += tangent;
		v2.Bitangent += bitangent;
		v3.Normal += normal;
		v3.Tangent += tangent;
		v3.Bitangent += bitangent;
	}

	for (auto& v : Vertices) {
		v.Normal = Normalize(v.Normal);
		// Gram-Schmidt orthogonalize
		v.Tangent = { (v.Tangent.AsVector3() - v.Normal * Dot(v.Normal, v.Tangent.AsVector3())).Normalized(), 0.0f };
		v.Tangent.w = (Dot(Cross(v.Normal, v.Tangent.AsVector3()), v.Bitangent) < 0.0f) ? -1.0f : 1.0f;
		Vector4 Tangent = Normalize(v.Tangent);
	}

	printf("Calculated some normals and Tangents\n");
}

void Mesh::CalculateExtents()
{		
	float xmin, ymin, zmin;
	xmin = ymin = zmin = 1000000.0f;
	float xmax, ymax, zmax;
	xmax = ymax = zmax = -1000000.0f;
	for (auto& v : Vertices) {
		auto vp = v.Position;
		if (vp.x < xmin) xmin = vp.x;
		if (vp.y < ymin) ymin = vp.y;
		if (vp.z < zmin) zmin = vp.z;
		if (vp.x > xmax) xmax = vp.x;
		if (vp.y > ymax) ymax = vp.y;
		if (vp.z > zmax) zmax = vp.z;
	}
	float x = (xmax - xmin) / 2.f;
	float y = (ymax - ymin) / 2.f;
	float z = (zmax - zmin) / 2.f;
	m_Extents = Vector3(x, y, z);
	m_minExtents = Vector3(xmin, ymin, zmin);
	m_maxExtents = Vector3(xmax, ymax, zmax);

	/*float ox = (m_maxExtents.x + m_minExtents.x) / 2.0f;
	float oy = (m_maxExtents.y + m_minExtents.y) / 2.0f;
	float oz = (m_maxExtents.z + m_minExtents.z) / 2.0f;*/

	/*m_origin = Vector3(ox, oy, oz);
	float epsilon = 0.00001f;
	if (fabs(m_origin.x) > epsilon || fabs(m_origin.y) > epsilon || fabs(m_origin.z) > epsilon) {
		for (auto& v : Vertices) {
			v.Position -= m_origin;
		}		
		CalculateExtents();
	}*/

	
}

} // namespace Jasper