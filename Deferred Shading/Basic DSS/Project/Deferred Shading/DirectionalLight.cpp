#include "DirectionalLight.h"
#include "ManualMeshGenerator.h"
#include "Definition.h"


DirectionalLight::DirectionalLight(Ogre::SceneManager *SceneManager, DSSMaterialGenerator *DSSMaterialGenerator) :
	DynamicLight(SceneManager, DSSMaterialGenerator)
{
	m_Light->setType(Ogre::Light::LT_DIRECTIONAL);
}


//DirectionalLight::~DirectionalLight(void)
//{
//}


bool DirectionalLight::IsCameraInside(const Ogre::Camera *Camera)
{
	return false;
}


void DirectionalLight::RebuildGeometry(const float &Radius)
{
	DISABLE_BIT(m_Settings, LightMaterialGenerator::MI_POINT);
	DISABLE_BIT(m_Settings, LightMaterialGenerator::MI_SPOTLIGHT);

	/// XXX this RenderOp should really be re-used between Light objects,
	/// not generated every time
	delete mRenderOp.vertexData; 
	delete mRenderOp.indexData; 

	mRenderOp.vertexData = new Ogre::VertexData();
    mRenderOp.indexData = 0;

	ManualMeshGenerator::CreateQuad(mRenderOp.vertexData);

    mRenderOp.operationType = Ogre::RenderOperation::OT_TRIANGLE_STRIP; 
    mRenderOp.useIndexes = false; 

    setBoundingBox(Ogre::AxisAlignedBox(-10000,-10000,-10000,10000,10000,10000));

	m_Radius = 15000;
	m_IgnoreWorld = true;

	ENABLE_BIT(m_Settings, LightMaterialGenerator::MI_DIRECTIONAL);
}