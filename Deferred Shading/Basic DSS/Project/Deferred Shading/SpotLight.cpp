#include "SpotLight.h"
#include "ManualMeshGenerator.h"
#include "Definition.h"


SpotLight::SpotLight(Ogre::SceneManager *SceneManager, DSSMaterialGenerator *DSSMaterialGenerator) :
	DynamicLight(SceneManager, DSSMaterialGenerator)
{
	m_Light->setType(Ogre::Light::LT_SPOTLIGHT);
}


//SpotLight::~SpotLight(void)
//{
//}


void SpotLight::getWorldTransforms(Ogre::Matrix4 *xform) const
{
	xform->makeTransform(m_Light->getDerivedPosition(), Ogre::Vector3::UNIT_SCALE, Ogre::Vector3::UNIT_Y.getRotationTo(m_Light->getDerivedDirection()));
}


bool SpotLight::IsCameraInside(const Ogre::Camera *Camera)
{
	Ogre::Vector3 lightPos = m_Light->getDerivedPosition();
	Ogre::Vector3 lightDir = m_Light->getDerivedDirection();
	Ogre::Radian attAngle = m_Light->getSpotlightOuterAngle();
		
	//Extend the analytic cone's radius by the near clip range by moving its tip accordingly.
	//Some trigonometry needed here.
	Ogre::Vector3 clipRangeFix = -lightDir * (Camera->getNearClipDistance() / Ogre::Math::Tan(attAngle/2));
	lightPos = lightPos + clipRangeFix;
    
	Ogre::Vector3 lightToCamDir = Camera->getDerivedPosition() - lightPos;
	Ogre::Real distanceFromLight = lightToCamDir.normalise();

	Ogre::Real cosAngle = lightToCamDir.dotProduct(lightDir);
	Ogre::Radian angle = Ogre::Math::ACos(cosAngle);
	//Check whether we will see the cone from our current POV.
	return (distanceFromLight <= (m_Light->getAttenuationRange() + clipRangeFix.length())) && (angle <= attAngle);
}


void SpotLight::RebuildGeometry(const float &Radius)
{
	DISABLE_BIT(m_Settings, LightMaterialGenerator::MI_DIRECTIONAL);
	DISABLE_BIT(m_Settings, LightMaterialGenerator::MI_POINT);

	float height = m_Light->getAttenuationRange();
	float radius = Ogre::Math::Tan(Ogre::Radian(m_Light->getSpotlightOuterAngle() / 2)) * height;

	delete mRenderOp.vertexData;
	delete mRenderOp.indexData;

	mRenderOp.operationType = Ogre::RenderOperation::OT_TRIANGLE_LIST;
	mRenderOp.indexData = new Ogre::IndexData();
	mRenderOp.vertexData = new Ogre::VertexData();
	mRenderOp.useIndexes = true;

	ManualMeshGenerator::CreateCone(mRenderOp.vertexData, mRenderOp.indexData, radius, height, 20);

	setBoundingBox(Ogre::AxisAlignedBox(Ogre::Vector3(-radius, 0, -radius), Ogre::Vector3(radius, height, radius)));

	m_Radius = radius;
	m_IgnoreWorld = false;

	ENABLE_BIT(m_Settings, LightMaterialGenerator::MI_SPOTLIGHT);
}