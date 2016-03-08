#include <Jasper\Model.h>

#include <assimp\scene.h>
#include <assimp\Importer.hpp>
#include <assimp\postprocess.h>

#include <Jasper\Mesh.h>
#include <Jasper\MeshRenderer.h>
#include <Jasper\Material.h>
#include <Jasper\BoxCollider.h>
#include <string>

namespace Jasper {

using namespace std;

Model::Model(const std::string& name, const std::string& filename, Shader* shader, bool enablePhysics, PhysicsWorld* physicsWorld)
	:GameObject(name), m_filename(filename), m_shader(shader), m_enablePhysics(enablePhysics), m_physicsWorld(physicsWorld)
{

}


Model::~Model()
{
}

void Model::Initialize()
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(m_filename, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals | aiProcess_OptimizeMeshes);


	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		printf("aiScene was corrupt in model load.");
		return;
	}

	m_directory = m_filename.substr(0, m_filename.find_last_of("/"));

	ProcessAiSceneNode(scene, scene->mRootNode);

	size_t sz = m_meshManager.GetSize();
	printf("\nLoaded %d meshes in model: %s", sz, this->GetName().c_str());
	uint numTris = 0;
	uint numVerts = 0;
	
	for (auto& m : m_meshManager.GetCache()) {
		numTris += m->Indices.size() / 3;
		numVerts += m->Vertices.size();
	}
	printf("\nModel Contains %d Vertices and %d Triangles.", numTris, numVerts);

}

void Model::ProcessAiSceneNode(const aiScene* scene, aiNode* node)
{
	for (uint i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		ProcessAiMesh(mesh, scene);
	}

	for (uint i = 0; i < node->mNumChildren; i++) {
		ProcessAiSceneNode(scene, node->mChildren[i]);
	}
}

void Model::ProcessAiMesh(const aiMesh* aiMesh, const aiScene* scene)
{
	auto m = m_meshManager.CreateInstance<Mesh>();
	m->Vertices.reserve(aiMesh->mNumVertices);
	m->Indices.reserve(aiMesh->mNumFaces * 3);

	for (uint i = 0; i < aiMesh->mNumVertices; i++) {
		Vertex v;
		auto avert = aiMesh->mVertices[i];
		v.Position = { avert.x, avert.y, avert.z };
		if (aiMesh->HasNormals()) {
			auto norm = aiMesh->mNormals[i];
			v.Normal = { norm.x, norm.y, norm.z };
		}
		if (aiMesh->HasTextureCoords(0)) {
			auto tc = aiMesh->mTextureCoords[0][i];
			v.TexCoords = { tc.x, tc.y };
		}
		m->AddVertex(v);
	}

	if (aiMesh->HasFaces()) {
		for (uint i = 0; i < aiMesh->mNumFaces; i++) {
			auto face = aiMesh->mFaces[i];
			for (unsigned j = 0; j < face.mNumIndices; j++) {
				m->Indices.push_back(face.mIndices[j]);
			}
		}

	}

	if (aiMesh->mMaterialIndex >= 0) {
		auto mat = scene->mMaterials[aiMesh->mMaterialIndex];
		printf("Found Material...\n");
		aiString texString;
		mat->GetTexture(aiTextureType::aiTextureType_DIFFUSE, 0, &texString);
		auto myMaterial = m_materialManager.CreateInstance<Material>(m_shader);
		string textureFileName = string(texString.C_Str());
		if (textureFileName.find(".") == string::npos) {
			textureFileName += "_D.tga";
		}
		myMaterial->SetTexture2D(m_directory + "/" + textureFileName);
	}

	if (aiMesh->HasBones()) {
		for (uint i = 0; i < aiMesh->mNumBones; ++i) {
			auto bone = aiMesh->mBones[i];
			string boneName = string(bone->mName.C_Str());
			printf("Found bone named: %s\n", boneName.c_str());
		}

	}

	m->CalculateHalfExtents();
	if (!aiMesh->HasNormals()) {
		m->CalculateFaceNormals();
	}

	if (m_enablePhysics) {
		auto bc = AttachNewComponent<BoxCollider>(this->GetName() + "Collider", m, m_physicsWorld);
		bc->Mass = 2.0f;
	}
	Material* renderMaterial;
	if (m_materialManager.GetSize() > 0) {
		renderMaterial = m_materialManager.GetLatestInstanceAdded();
	}
	else {
		renderMaterial = m_materialManager.CreateInstance<Material>(m_shader);
		renderMaterial->SetTexture2D("./textures/default.png");
	}
	auto mr = this->AttachNewComponent<MeshRenderer>(m, renderMaterial);
	printf("Loaded Model Mesh\n");

}

void Model::CalculateHalfExtents()
{
	float minx, miny, minz;
	minx = miny = minz = 1000000.f;
	float maxx, maxy, maxz;
	maxx = maxy = maxz = -100000.f;
	for (auto& mesh : m_meshManager.GetCache()) {
		auto he = mesh->GetHalfExtents();
		if (he.x > maxx) maxx = he.x;
		if (he.y > maxy) maxy = he.y;
		if (he.z > maxz) maxz = he.z;

		if (he.x < minx) minx = he.x;
		if (he.y < miny) miny = he.y;
		if (he.z < minz) minz = he.z;
	}
	this->HalfExtents = Vector3();
}

void Model::Destroy()
{
	m_materialManager.Clear();
	m_meshManager.Clear();
	GameObject::Destroy();
}

void Model::Awake()
{
	Initialize();
	GameObject::Awake();
}

void Model::Update(float dt)
{
	GameObject::Update(dt);
}

}
