#include "SSAOLogic.h"
#include <Ogre.h>

const Ogre::Matrix4 CLIP_SPACE_TO_IMAGE_SPACE(
					0.5,	0,		0,		0.5,
					0,		-0.5,	0,		0.5,
					0,		0,		1,		0,
					0,		0,		0,		1);

class SSAOListener : public Ogre::CompositorInstance::Listener
{
public:
	SSAOListener(Ogre::CompositorInstance *Instance) :
		m_Instance(Instance)
	{
	}

private:
	void notifyMaterialRender(unsigned int PassID, Ogre::MaterialPtr &Material)
	{
		// Not SSAO, return
		if (PassID != 42)
			return;

		Ogre::Camera *camera = m_Instance->getChain()->getViewport()->getCamera();

        // calculate the far-top-right corner in view-space
        Ogre::Vector3 farCorner = camera->getViewMatrix(true) * camera->getWorldSpaceCorners()[4];

		Ogre::Pass *pass = Material->getBestTechnique()->getPass(0);

		Ogre::GpuProgramParametersSharedPtr params = pass->getVertexProgramParameters();
		
		// set the camera's far-top-right corner
		if (params->_findNamedConstantDefinition("farCorner"))
			params->setNamedConstant("farCorner", farCorner);

		params = pass->getFragmentProgramParameters();

		// set the projection matrix we need

		if (params->_findNamedConstantDefinition("ptMat"))
			params->setNamedConstant("ptSpaceToImageMat", CLIP_SPACE_TO_IMAGE_SPACE);

		if (params->_findNamedConstantDefinition("ptMat"))
			params->setNamedConstant("ptMat", camera->getProjectionMatrixWithRSDepth());

		if (params->_findNamedConstantDefinition("far"))
			params->setNamedConstant("far", camera->getFarClipDistance());
	}

private:
	Ogre::CompositorInstance *m_Instance;
};


Ogre::CompositorInstance::Listener *SSAOLogic::CreateListener(Ogre::CompositorInstance *Instance)
{
	return new SSAOListener(Instance);
}
