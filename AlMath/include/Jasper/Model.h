#ifndef _MODEL_H_
#define _MODEL_H_

#include "Common.h"
#include "GameObject.h"
#include "Scene.h"

class aiScene;
class aiNode;
class aiMesh;

namespace Jasper {

class Shader;

class Model : public GameObject
{
public:
	explicit Model(const std::string& name, const std::string& filename, Shader* shader, bool enablePhysics = false, PhysicsWorld* physicsWorld = nullptr);
	~Model();

	virtual void Initialize() override;
	virtual void Destroy() override;
	virtual void Awake() override;
	virtual void Update(float dt) override;

	Vector3 HalfExtents;

	uint TriCount = 0;
	uint VertCount = 0;

private:
	std::string m_filename;
	std::string m_directory;
	Shader* m_shader;
	bool m_enablePhysics = false;
	void ProcessAiSceneNode(const aiScene* aiscene, aiNode* ainode);
	void ProcessAiMesh(const aiMesh* aimesh, const aiScene* aiscene);
	PhysicsWorld* m_physicsWorld = nullptr;
	ResourceManager<Mesh> m_meshManager;
	ResourceManager<Material> m_materialManager;

	NON_COPYABLE(Model);

protected:

	void CalculateHalfExtents();
};
}
#endif // _MODEL_H_