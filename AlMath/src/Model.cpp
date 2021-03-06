#include <Jasper\Model.h>

#include <assimp\scene.h>
#include <assimp\Importer.hpp>
#include <assimp\postprocess.h>

#include <Jasper\Common.h>
#include <Jasper\Mesh.h>
#include <Jasper\MeshRenderer.h>
#include <Jasper\Material.h>
#include <Jasper\BoxCollider.h>
#include <Jasper\CapsuleCollider.h>
#include <Jasper\ConvexHullCollider.h>
#include <Jasper\CylinderCollider.h>
#include <Jasper\SphereCollider.h>
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
	const aiScene* scene = importer.ReadFile(m_filename, aiProcessPreset_TargetRealtime_Quality | aiProcess_FlipUVs);

	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		printf("aiScene was corrupt in model load.");
		return;
	}

	m_directory = m_filename.substr(0, m_filename.find_last_of("/"));

	ProcessAiSceneNode(scene, scene->mRootNode);

	auto meshes = this->GetComponentsByType<Mesh>();
	int sz = meshes.size();
	printf("\nLoaded %d meshes in model: %s", sz, this->GetName().c_str());
	uint numTris = 0;
	uint numVerts = 0;
	MaxExtents = { -100000.0f, -1000000.0f, -1000000.0f };
	MinExtents = { 1000000.0f, 1000000.0f, 1000000.0f };

	for (auto& m : meshes) {
		this->TriCount += m->Indices.size() / 3;
		this->VertCount += m->Vertices.size();
		if (m->GetMaxExtents().x > MaxExtents.x) MaxExtents.x = m->GetMaxExtents().x;
		if (m->GetMaxExtents().y > MaxExtents.y) MaxExtents.y = m->GetMaxExtents().y;
		if (m->GetMaxExtents().z > MaxExtents.z) MaxExtents.z = m->GetMaxExtents().z;

		if (m->GetMinExtents().x < MinExtents.x) MinExtents.x = m->GetMinExtents().x;
		if (m->GetMinExtents().y < MinExtents.y) MinExtents.y = m->GetMinExtents().y;
		if (m->GetMinExtents().z < MinExtents.z) MinExtents.z = m->GetMinExtents().z;
	}
	Vector3 localOrigin = { (MinExtents.x + MaxExtents.x) / 2.f, (MinExtents.y + MaxExtents.y) / 2.f , (MinExtents.z + MaxExtents.z) / 2.f };
	float epsilon = 0.000001f;
	if (fabs(localOrigin.x) > epsilon || fabs(localOrigin.y) > epsilon || fabs(localOrigin.z) > epsilon) {
		for (auto& m : meshes) {
			for (auto& v : m->Vertices) {
				v.Position -= localOrigin;
			}
			m->CalculateExtents();
		}
	}

	if (m_enablePhysics) {
		Vector3 hes = { (MaxExtents.x - MinExtents.x) / 2, (MaxExtents.y - MinExtents.y) / 2, (MaxExtents.z - MinExtents.z) / 2 };
		PhysicsCollider* collider = nullptr;
		switch (this->ColliderType) {
		case PHYSICS_COLLIDER_TYPE::Box:
			collider = AttachNewComponent<BoxCollider>(this->GetName() + "_Collider_", hes, m_physicsWorld);			
			break;
		case PHYSICS_COLLIDER_TYPE::Capsule:
			collider = AttachNewComponent<CapsuleCollider>(this->GetName() + "_Collider_", hes, m_physicsWorld);			
			break;
		case PHYSICS_COLLIDER_TYPE::Sphere:
			collider = AttachNewComponent<SphereCollider>(this->GetName() + "_Collider_", hes, m_physicsWorld);			
			break;
		case PHYSICS_COLLIDER_TYPE::Cylinder:
			collider = AttachNewComponent<CylinderCollider>(this->GetName() + "_Collider_", hes, m_physicsWorld);
			break;
		case PHYSICS_COLLIDER_TYPE::ConvexHull:
			collider = AttachNewComponent<ConvexHullCollider>(this->GetName() + "_Collider_", hes, m_physicsWorld);
			break;
		}
		if (collider) {
			collider->Mass = this->Mass;
			collider->Restitution = this->Restitution;
			collider->Friction = this->Friction;
		}
	}


	int i = 0;
	for (auto mesh : meshes) {
		//Vector3 meshOrigin = { (mesh->GetMinExtents().x + mesh->GetMaxExtents().x) / 2.f, (mesh->GetMinExtents().y + mesh->GetMaxExtents().y) / 2.f , (mesh->GetMinExtents().z + mesh->GetMaxExtents().z) / 2.f };
		//auto child = make_unique<GameObject>("child_" + std::to_string(i));
		//child->GetLocalTransform().Position = meshOrigin;
		AttachNewComponent<MeshRenderer>(mesh, mesh->m_material);		
		//this->AttachChild(move(child));
		i++;
	}
	printf("\nModel Contains %d Vertices and %d Triangles and %d Materials.", VertCount, TriCount, m_materialManager.GetCache().size());


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

	auto m = this->AttachNewComponent<Mesh>();
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
		if (aiMesh->HasTangentsAndBitangents()) {
			auto tang = aiMesh->mTangents[i];
			v.Tangent = { tang.x, tang.y, tang.z, 1.f };
			auto bitang = aiMesh->mBitangents[i];
			v.Bitangent = { bitang.x, bitang.y, bitang.z };

			//v.Tangent.w = (Dot(Cross(v.Normal, v.Tangent.AsVector3()), v.Bitangent) > 0.0f) ? -1.0f : 1.0f;

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

	m->CalculateExtents();
	Material* myMaterial = nullptr;
	if (aiMesh->mMaterialIndex >= 0) {
		auto mat = scene->mMaterials[aiMesh->mMaterialIndex];
		printf("Found Material...\n");
		aiString texString;
		mat->GetTexture(aiTextureType::aiTextureType_DIFFUSE, 0, &texString);
		string textureFileName = string(texString.C_Str());
		if (textureFileName.find(".") == string::npos) {
			textureFileName += "_D.tga";
		}
		string texturePath = m_directory + "/" + textureFileName;
		if (texString.length > 0) {

			auto existingMat = std::find_if(std::begin(m_materialManager.GetCache()), std::end(m_materialManager.GetCache()), [&](const std::unique_ptr<Material>& mm) {
				return mm->DiffuseTextureFilename() == texturePath;
			});
			if (existingMat != std::end(m_materialManager.GetCache())) {
				myMaterial = existingMat->get();
			}
			else {
				myMaterial = m_materialManager.CreateInstance<Material>(m_shader);

				aiColor3D diffuse, ambient, specular;
				float shine;
				mat->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
				mat->Get(AI_MATKEY_COLOR_AMBIENT, ambient);
				mat->Get(AI_MATKEY_COLOR_SPECULAR, specular);
				mat->Get(AI_MATKEY_SHININESS, shine);
				if (shine == 0.0f) {
					shine = 64.0f;
				}
				myMaterial->SetTextureDiffuse(texturePath);
				myMaterial->Ambient = Vector3(ambient.r, ambient.g, ambient.b);
				myMaterial->Diffuse = Vector3(diffuse.r, diffuse.g, diffuse.b);
				myMaterial->Specular = Vector3(specular.r, specular.g, specular.b);
				myMaterial->Shine = shine / 4.0f;
				// try to load a normal map
				texString.Clear();
				mat->GetTexture(aiTextureType::aiTextureType_NORMALS, 0, &texString);
				if (texString.length > 0) {
					myMaterial->SetTextureNormalMap(m_directory + "/" + texString.C_Str());
				}
				else {
					texString.Clear();
					mat->GetTexture(aiTextureType::aiTextureType_HEIGHT, 0, &texString);
					if (texString.length > 0) {
						myMaterial->SetTextureNormalMap(m_directory + "/" + texString.C_Str());
					}
				}
				texString.Clear();
				mat->GetTexture(aiTextureType::aiTextureType_SPECULAR, 0, &texString);
				if (texString.length > 0) {
					myMaterial->SetTextureSpecularMap(m_directory + "/" + texString.C_Str());
				}
			}
		}
	}

	if (!aiMesh->HasNormals()) {
		m->CalculateFaceNormals();
	}

	Material* renderMaterial;
	if (myMaterial) {
		renderMaterial = myMaterial;
	}
	else {
		renderMaterial = m_materialManager.CreateInstance<Material>(m_shader);
		renderMaterial->SetTextureDiffuse("./textures/default.png");
	}
	m->m_material = renderMaterial;
	//auto mr = this->AttachNewComponent<MeshRenderer>(m, renderMaterial);

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
