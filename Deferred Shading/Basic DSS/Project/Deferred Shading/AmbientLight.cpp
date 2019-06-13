#include "AmbientLight.h"
#include "ManualMeshGenerator.h"


AmbientLight::AmbientLight(void) :
	LightBase(15000.f)
{
	setRenderQueueGroup(Ogre::RENDER_QUEUE_2);

	mRenderOp.vertexData = new Ogre::VertexData();
	mRenderOp.indexData = 0;

	ManualMeshGenerator::CreateQuad(mRenderOp.vertexData);

	mRenderOp.operationType = Ogre::RenderOperation::OT_TRIANGLE_STRIP;
	mRenderOp.useIndexes = false;

	setBoundingBox(Ogre::AxisAlignedBox(-10000, -10000, -10000, 10000, 10000, 10000));

	m_Material = Ogre::MaterialManager::getSingleton().getByName("DeferredShading/AmbientLight");
	m_Material->load();

    // This shader needs to be aware if its running under OpenGL or DirectX.
    //float depthFactor = (Ogre::Root::getSingleton().getRenderSystem()->getName() == "OpenGL Rendering Subsystem") ? 2.f : 1.f;
    //m_Material->getTechnique(0)->getPass(0)->getFragmentProgramParameters()->setNamedConstant("depthFactor", depthFactor);
}


//AmbientLight::~AmbientLight(void)
//{
//}


void AmbientLight::Update(Ogre::Camera *Camera/*, const Ogre::LightList *LightList*/)
{
	if (m_IsDirty)
	{
		m_IsDirty = false;
		
		UpdateParameters(Camera);
	}

	LightBase::Update(Camera/*, LightList*/);
}


float AmbientLight::getSquaredViewDepth(const Ogre::Camera *Camera) const
{
	return 0.f;
}


const Ogre::MaterialPtr &AmbientLight::getMaterial(void) const
{
	return m_Material;
}


void AmbientLight::getWorldTransforms(Ogre::Matrix4 *xform) const
{
	*xform = Ogre::Matrix4::IDENTITY;
}


void AmbientLight::UpdateParameters(const Ogre::Camera *Camera)
{
	Ogre::Vector3 farCorner = Camera->getViewMatrix(true) * Camera->getWorldSpaceCorners()[4];

	Ogre::Technique *tech = m_Material->getBestTechnique();
	Ogre::Pass *pass = NULL;
	Ogre::GpuProgramParametersSharedPtr params;
	for (unsigned short i = 0; i < tech->getNumPasses(); i++)
	{
		pass = tech->getPass(i);

		params = pass->getVertexProgramParameters();
		if (params->_findNamedConstantDefinition("farCorner"))
			params->setNamedConstant("farCorner", farCorner);
		
		params = pass->getFragmentProgramParameters();
		if (params->_findNamedConstantDefinition("farCorner"))
			params->setNamedConstant("farCorner", farCorner);
	}
}