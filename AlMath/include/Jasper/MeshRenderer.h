#ifndef _MESH_RENDERER_H_
#define _MESH_RENDERER_H_

#include "Component.h"
#include "Transform.h"
#include "GLBuffer.h"
#include "Mesh.h"
#include <memory>
#include <vector>



namespace Jasper {

class Material;
class Shader;

class MeshRenderer : public Component
{
private:

	GLBuffer m_vertexBuffer;
	GLBuffer m_indexBuffer;

protected:

	Mesh* m_mesh;
	unsigned m_vaoID; 
	int m_elementCount = 0;
	Material* m_material;

public:

	Material* GetMaterial() const {
		return m_material;
	}

	NON_COPYABLE(MeshRenderer);	

	explicit MeshRenderer(Mesh* mesh, Material* material);
	~MeshRenderer();

	virtual void Render();

	virtual void Destroy() override;
	virtual void Awake() override;
	virtual void Start() override;
	virtual void FixedUpdate() override;
	virtual void Update() override;
	virtual void LateUpdate() override;

};

} // namespace Jasper
#endif // _MESH_RENDERER_H_

