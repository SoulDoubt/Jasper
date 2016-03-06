#include <Jasper\Triangle.h>

namespace Jasper {

	Triangle::Triangle()
	{
		Initialize();
	}


	Triangle::~Triangle()
	{
		Destroy();
	}

	void Triangle::Initialize()
	{
		Vertex v0 = { -0.5f, -0.5f, 0.0f, 0.0f, 0.0f };
		Vertex v1 = { 0.f, 0.5f, 0.0f, 0.0f, 0.0f };
		Vertex v2 = { 0.5f, -0.5f, 0.0f, 0.0f, 0.0f };

		Vertices.push_back(v0);
		Vertices.push_back(v1);
		Vertices.push_back(v2);

		Indices.push_back(0);
		Indices.push_back(1);
		Indices.push_back(2);
	}

	void Triangle::Destroy()
	{
	}

}


