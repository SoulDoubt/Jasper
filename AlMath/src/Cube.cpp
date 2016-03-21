#include <Jasper\Cube.h>
#include <Jasper\GameObject.h>
#include <Jasper\Shader.h>
#include <gl\glew.h>

namespace Jasper {

Cube::Cube() : Mesh("Cube")
{
	Dimensions = Vector3(0.5f, 0.5f, 0.5f);
	Initialize();
}

Cube::Cube(const Vector3& dimensions)
{
	Dimensions = dimensions;
	Initialize();
}

Cube::Cube(const Vector3& dimensions, bool reverseWinding)
{
	m_isCubemap = reverseWinding;
	Dimensions = dimensions;
	Initialize();
}


Cube::~Cube()
{
	Destroy();
}

void Cube::Initialize() {
	// construct a unit cube centered on the axis.

	//float x = Dimensions.x;
	//float y = Dimensions.y;
	//float z = Dimensions.z;

	//// Front
	//Vertex f0 = { -x, -y, z, 0.0f, 0.0f }; // 0
	//Vertex f1 = { -x, y, z, 0.0f, 1.0f };  // 1 
	//Vertex f2 = { x, y, z, 1.0f, 1.0f };   // 2
	//Vertex f3 = { x, -y, z, 1.0f, 0.0f };  // 3

	//Vertices.push_back(f0);
	//Vertices.push_back(f1);
	//Vertices.push_back(f2);
	//Vertices.push_back(f3);

	//Indices.push_back(0);
	//Indices.push_back(3);
	//Indices.push_back(2);
	//Indices.push_back(2);
	//Indices.push_back(1);
	//Indices.push_back(0);

	//// right side
	//Vertex r0 = { x, -y, z, 0.0f, 0.0f };
	//Vertex r1 = { x, y, z, 0.0f, 1.0f };
	//Vertex r2 = { x, y, -z, 1.0f, 1.0f };
	//Vertex r3 = { x, -y, -z, 1.0f, 0.0f };

	//Vertices.push_back(r0); // 4
	//Vertices.push_back(r1); // 5
	//Vertices.push_back(r2); // 6
	//Vertices.push_back(r3); // 7

	//Indices.push_back(4);
	//Indices.push_back(7);
	//Indices.push_back(6);
	//Indices.push_back(6);
	//Indices.push_back(5);
	//Indices.push_back(4);

	//// left side
	//Vertex l0 = { -x, -y, -z, 0.0f, 0.0f }; // 8
	//Vertex l1 = { -x, -y, z, 1.0f, 0.0f };  // 9
	//Vertex l2 = { -x, y, z, 1.0f, 1.0f };   // 10
	//Vertex l3 = { -x, y, -z, 0.0f, 1.0f };  // 11

	//Vertices.push_back(l0);
	//Vertices.push_back(l1);
	//Vertices.push_back(l2);
	//Vertices.push_back(l3);

	//Indices.push_back(8);
	//Indices.push_back(9);
	//Indices.push_back(10);
	//Indices.push_back(10);
	//Indices.push_back(11);
	//Indices.push_back(8);

	//// "Back"
	//Vertex b0 = { -x, -y, -z, 1.0f, 0.0f }; // 12
	//Vertex b1 = { x, -y, -z, 0.0f, 0.0f };  // 13
	//Vertex b2 = { x, y, -z, 0.0f, 1.0f };   // 14
	//Vertex b3 = { -x, y, -z, 1.0f, 1.0f };  // 15

	//Vertices.push_back(b0);
	//Vertices.push_back(b1);
	//Vertices.push_back(b2);
	//Vertices.push_back(b3);

	//Indices.push_back(13);
	//Indices.push_back(12);
	//Indices.push_back(15);
	//Indices.push_back(15);
	//Indices.push_back(14);
	//Indices.push_back(13);

	//// TOP
	//Vertex t0 = { -x, y, z, 0.0f, 0.0f };		// 16
	//Vertex t1 = { -x, y, -z, 0.0f, 1.0f };	// 17
	//Vertex t2 = { x, y, -z, 1.0f, 1.0f };		// 18
	//Vertex t3 = { x, y, z, 1.0f, 0.0f };		// 19

	//Vertices.push_back(t0);
	//Vertices.push_back(t1);
	//Vertices.push_back(t2);
	//Vertices.push_back(t3);

	//Indices.push_back(16);
	//Indices.push_back(19);
	//Indices.push_back(18);
	//Indices.push_back(18);
	//Indices.push_back(17);
	//Indices.push_back(16);

	//// Bottom
	//Vertex u0 = { -x, -y, z, 0.0f, 0.0f };	// 20
	//Vertex u1 = { -x, -y, -z, 0.0f, 1.0f };	// 21
	//Vertex u2 = { x, -y, -z, 1.0f, 1.0f };	// 22
	//Vertex u3 = { x, -y, z, 1.0f, 0.0f };		// 23

	//Vertices.push_back(u0);
	//Vertices.push_back(u1);
	//Vertices.push_back(u2);
	//Vertices.push_back(u3);

	//Indices.push_back(20);
	//Indices.push_back(21);
	//Indices.push_back(22);
	//Indices.push_back(22);
	//Indices.push_back(23);
	//Indices.push_back(20);

	//CalculateFaceNormals();
	//if (m_reverseWinding) {
	//	ReverseWinding();
	//}
	if (m_isCubemap) {
		LoadCubemapVerts();
	}
	else {
		LoadStandardVerts();
	}
}

void Cube::LoadCubemapVerts() {
	float x = Dimensions.x;
	float y = Dimensions.y;
	float z = Dimensions.z;
	Vertices.push_back({ Vector3(-x, y, -z) });
	Vertices.push_back({ Vector3(-x, -y, -z) });
	Vertices.push_back({ Vector3(x, -y, -z) });
	Vertices.push_back({ Vector3(x, -y, -z) });
	Vertices.push_back({ Vector3(x, y, -z) });
	Vertices.push_back({ Vector3(-x, y, -z) });

	Vertices.push_back({ Vector3(-x, -y, z) });
	Vertices.push_back({ Vector3(-x, -y, -z) });
	Vertices.push_back({ Vector3(-x, y, -z) });
	Vertices.push_back({ Vector3(-x, y, -z) });
	Vertices.push_back({ Vector3(-x, y, z) });
	Vertices.push_back({ Vector3(-x, -y, z) });

	Vertices.push_back({ Vector3(x, -y, -z) });
	Vertices.push_back({ Vector3(x, -y, z) });
	Vertices.push_back({ Vector3(x, y, z) });
	Vertices.push_back({ Vector3(x, y, z) });
	Vertices.push_back({ Vector3(x, y, -z) });
	Vertices.push_back({ Vector3(x, -y, -z) });

	Vertices.push_back({ Vector3(-x, -y, z) });
	Vertices.push_back({ Vector3(-x, y, z) });
	Vertices.push_back({ Vector3(x, y, z) });
	Vertices.push_back({ Vector3(x, y, z) });
	Vertices.push_back({ Vector3(x, -y, z) });
	Vertices.push_back({ Vector3(-x, -y, z) });

	Vertices.push_back({ Vector3(-x, y, -z) });
	Vertices.push_back({ Vector3(x, y, -z) });
	Vertices.push_back({ Vector3(x, y, z) });
	Vertices.push_back({ Vector3(x, y, z) });
	Vertices.push_back({ Vector3(-x, y, z) });
	Vertices.push_back({ Vector3(-x, y, -z) });

	Vertices.push_back({ Vector3(-x, -y, -z) });
	Vertices.push_back({ Vector3(-x, -y, z) });
	Vertices.push_back({ Vector3(x, -y, -z) });
	Vertices.push_back({ Vector3(x, -y, -z) });
	Vertices.push_back({ Vector3(-x, -y, z) });
	Vertices.push_back({ Vector3(x, -y, z) });

	Indices.push_back(0);
	Indices.push_back(1);
	Indices.push_back(2);
	Indices.push_back(3);
	Indices.push_back(4);
	Indices.push_back(5);
	Indices.push_back(6);
	Indices.push_back(7);
	Indices.push_back(8);
	Indices.push_back(0);
	Indices.push_back(10);
	Indices.push_back(11);
	Indices.push_back(12);
	Indices.push_back(13);
	Indices.push_back(14);
	Indices.push_back(15);
	Indices.push_back(16);
	Indices.push_back(17);
	Indices.push_back(18);
	Indices.push_back(19);
	Indices.push_back(20);
	Indices.push_back(21);
	Indices.push_back(22);
	Indices.push_back(23);
	Indices.push_back(24);
	Indices.push_back(25);
	Indices.push_back(26);
	Indices.push_back(27);
	Indices.push_back(28);
	Indices.push_back(29);
	Indices.push_back(30);
	Indices.push_back(31);
	Indices.push_back(32);
	Indices.push_back(33);
	Indices.push_back(34);
	Indices.push_back(35);

	CalculateExtents();
}

void Cube::LoadStandardVerts()
{
	

	float x = Dimensions.x;
	float y = Dimensions.y;
	float z = Dimensions.z;

	// Front
	Vertex f0 = { -x, -y, z, 0.0f, 0.0f }; // 0
	Vertex f1 = { -x, y, z, 0.0f, 1.0f };  // 1 
	Vertex f2 = { x, y, z, 1.0f, 1.0f };   // 2
	Vertex f3 = { x, -y, z, 1.0f, 0.0f };  // 3

	Vertices.push_back(f0);
	Vertices.push_back(f1);
	Vertices.push_back(f2);
	Vertices.push_back(f3);

	Indices.push_back(0);
	Indices.push_back(3);
	Indices.push_back(2);
	Indices.push_back(2);
	Indices.push_back(1);
	Indices.push_back(0);

	// right side
	Vertex r0 = { x, -y, z, 0.0f, 0.0f };
	Vertex r1 = { x, y, z, 0.0f, 1.0f };
	Vertex r2 = { x, y, -z, 1.0f, 1.0f };
	Vertex r3 = { x, -y, -z, 1.0f, 0.0f };

	Vertices.push_back(r0); // 4
	Vertices.push_back(r1); // 5
	Vertices.push_back(r2); // 6
	Vertices.push_back(r3); // 7

	Indices.push_back(4);
	Indices.push_back(7);
	Indices.push_back(6);
	Indices.push_back(6);
	Indices.push_back(5);
	Indices.push_back(4);

	// left side
	Vertex l0 = { -x, -y, -z, 0.0f, 0.0f }; // 8
	Vertex l1 = { -x, -y, z, 1.0f, 0.0f };  // 9
	Vertex l2 = { -x, y, z, 1.0f, 1.0f };   // 10
	Vertex l3 = { -x, y, -z, 0.0f, 1.0f };  // 11

	Vertices.push_back(l0);
	Vertices.push_back(l1);
	Vertices.push_back(l2);
	Vertices.push_back(l3);

	Indices.push_back(8);
	Indices.push_back(9);
	Indices.push_back(10);
	Indices.push_back(10);
	Indices.push_back(11);
	Indices.push_back(8);

	// "Back"
	Vertex b0 = { -x, -y, -z, 1.0f, 0.0f }; // 12
	Vertex b1 = { x, -y, -z, 0.0f, 0.0f };  // 13
	Vertex b2 = { x, y, -z, 0.0f, 1.0f };   // 14
	Vertex b3 = { -x, y, -z, 1.0f, 1.0f };  // 15

	Vertices.push_back(b0);
	Vertices.push_back(b1);
	Vertices.push_back(b2);
	Vertices.push_back(b3);

	Indices.push_back(13);
	Indices.push_back(12);
	Indices.push_back(15);
	Indices.push_back(15);
	Indices.push_back(14);
	Indices.push_back(13);

	// TOP
	Vertex t0 = { -x, y, z, 0.0f, 0.0f };		// 16
	Vertex t1 = { -x, y, -z, 0.0f, 1.0f };	// 17
	Vertex t2 = { x, y, -z, 1.0f, 1.0f };		// 18
	Vertex t3 = { x, y, z, 1.0f, 0.0f };		// 19

	Vertices.push_back(t0);
	Vertices.push_back(t1);
	Vertices.push_back(t2);
	Vertices.push_back(t3);

	Indices.push_back(16);
	Indices.push_back(19);
	Indices.push_back(18);
	Indices.push_back(18);
	Indices.push_back(17);
	Indices.push_back(16);

	// Bottom
	Vertex u0 = { -x, -y, z, 0.0f, 0.0f };	// 20
	Vertex u1 = { -x, -y, -z, 0.0f, 1.0f };	// 21
	Vertex u2 = { x, -y, -z, 1.0f, 1.0f };	// 22
	Vertex u3 = { x, -y, z, 1.0f, 0.0f };	// 23

	Vertices.push_back(u0);
	Vertices.push_back(u1);
	Vertices.push_back(u2);
	Vertices.push_back(u3);

	Indices.push_back(20);
	Indices.push_back(21);
	Indices.push_back(22);
	Indices.push_back(22);
	Indices.push_back(23);
	Indices.push_back(20);

	CalculateFaceNormals();
	CalculateExtents();
}


void Cube::Destroy() {}

}




