#ifndef _JASPER_SKYBOX_RENDERER_H_
#define _JASPER_SKYBOX_RENDERER_H_


#include "MeshRenderer.h"

namespace Jasper {

class SkyboxRenderer :
	public MeshRenderer
{
public:
	explicit SkyboxRenderer(Mesh* mesh, Material* mat);
	SkyboxRenderer(const SkyboxRenderer& o) = delete;
	SkyboxRenderer& operator=(const SkyboxRenderer& o) = delete;
	~SkyboxRenderer();
	virtual void Update() override;
	virtual void Render() override;
	

};

}

#endif // _JASPER_SKYBOX_RENDERER_H_