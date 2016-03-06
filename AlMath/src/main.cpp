#include <Jasper\vector3.h>
#include <Jasper\matrix.h>
#include <Jasper\GameObject.h>
#include <Jasper\GLWindow.h>
#include <Jasper\BasicShader.h>
#include <Jasper\Cube.h>
#include <Jasper\Material.h>
#include <iostream>

using namespace std;
using namespace Jasper;

int main(int argc, char** argv) {

	/*Matrix4 testFromTransform = Matrix4::FromTransform(*trans);
	std::cout << "Matrix from transform: " << std::endl;
	std::cout << testFromTransform.ToString() << std::endl;*/
	Matrix4 mm;
	float* mmdat = &(mm.mat[0].x);
	cout << "Matrix4 as float array:" << endl;
	for (int i = 0; i < 16; i++) {
		cout << mmdat[i] << endl;
	}




	std::unique_ptr<GLWindow> window = std::make_unique<GLWindow>(800, 600, "My Window");
	window->Init();
	window->InitializeScene();

	window->RunLoop();

	Vector3 vecA = Vector3(23.4f, 34.5, 645.6);
	Vector3 vecB = Vector3(12.4, 56.8, 72.8);

	float alen = vecA.Length();
	float blen = vecB.Length();

	auto crossResultMember = vecA.Cross(vecB);
	auto crossResultNonMember = Cross(vecA, vecB);

	std::cout << "Member Cross Product: " << crossResultMember.ToString() << std::endl;
	std::cout << "Non Member Cross Product: " << crossResultNonMember.ToString() << std::endl;

	float dotResultMember = vecA.Dot(vecB);

	std::cout << "Member Dot Product: " << dotResultMember << std::endl;

	Matrix3 mat;
	std::cout << "Matrix 1:" << std::endl;
	std::cout << mat.ToString();

	Vector3 r0 = { 1.2f, 3.4f, 5.8f };
	Vector3 r1 = { 1.9f, 4.4f, 8.8f };
	Vector3 r2 = { 5.2f, 1.4f, 2.8f };

	Matrix3 mat2 = { r0, r1, r2 };
	std::cout << "Matrix 2:" << std::endl;
	std::cout << mat2.ToString();

	Matrix3 matProd = mat * mat2;
	std::cout << "Matrix Product:" << std::endl;
	std::cout << matProd.ToString() << std::endl << std::endl;

	Matrix3 matToDet = { Vector3(-1, -2, 2), Vector3(2, 1, 1), Vector3(3, 4, 5) };
	float det = matToDet.Determinant();

	std::cout << "Determinant is: " << std::to_string(det) << std::endl;


	Matrix4 mat4a = { Vector4(1, 2, 3, 0), Vector4(4, 5, 6, 0), Vector4(7, 8, 9, 0), Vector4(0,0,0,1) };
	Matrix4 mat4b = { Vector4(9, 8, 7, 0), Vector4(6, 5, 4, 0), Vector4(3, 2, 1, 0), Vector4(0,0,0,1) };
	//mat4.CreatePerspectiveProjection(90.0f, 800.0f / 600.0f, 0.1f, 100.0f);
	Matrix4 mat4c = mat4a * mat4b;
	std::cout << mat4c.ToString() << std::endl;

	Matrix3 mt = { Vector3(1, 2, 3), Vector3(4, 5, 6), Vector3(7,8,9) };
	Matrix3 mTransposed = mt.Transposed();

	std::cout << "3x3 Transpose:" << std::endl;
	std::cout << mTransposed.ToString() << std::endl;

	Matrix4 mtoInvert = { Vector4(-1, 2, 6, 0), Vector4(-4, 7, 8, 0), Vector4(-4, -2, 9, 0), Vector4(0,0,0,1) };
	float m4invdet = mtoInvert.Determinant();
	std::cout << "Determinant of the 4x4 is: " << m4invdet << std::endl;
	Matrix4 inverse = mtoInvert.Inverted();
	std::cout << "Inverse is: " << std::endl;
	std::cout << inverse.ToString();

	auto normal = mtoInvert.NormalMatrix();
	std::cout << "Normal Matrix is: " << std::endl;
	std::cout << normal.ToString() << std::endl;
	//int in;
	//std::cin >> in;
}

void printGLInfo() {
	printf("Using OpenGL Version: %s\n", glGetString(GL_VERSION));
}
