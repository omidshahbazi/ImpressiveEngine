#include "PointLight.h"
#include "ManualMeshGenerator.h"
#include "Definition.h"


PointLight::PointLight(Ogre::SceneManager *SceneManager, DSSMaterialGenerator *DSSMaterialGenerator) :
	DynamicLight(SceneManager, DSSMaterialGenerator)
{
	m_Light->setType(Ogre::Light::LT_POINT);

	m_Light->setAttenuation(5, 1.f, 0.f, 0.f);
}


//PointLight::~PointLight(void)
//{
//}


const float PointLight::GetOuterRadius(void)
{
	// Set Attenuation parameter to shader
	//setCustomParameter(3, Ogre::Vector4(m_Light->getAttenuationConstant(), m_Light->getAttenuationLinear(), m_Light->getAttenuationQuadric(), 0));

	float outerRadius = m_Light->getAttenuationRange();

	if (m_Light->getAttenuationConstant() != 1.f || m_Light->getAttenuationLinear() != 0.f || m_Light->getAttenuationQuadric() != 0.f)
	{
		// Calculate radius from Attenuation
		int threshold_level = 10;// difference of 10-15 levels deemed unnoticeable
		float threshold = 1.0f/((float)threshold_level/256.0f); 

		//// Use quadratic formula to determine outer radius
		float c = m_Light->getAttenuationConstant() - threshold;
		float d = sqrt(m_Light->getAttenuationLinear() * m_Light->getAttenuationLinear() - 4 * m_Light->getAttenuationQuadric() * c);
		outerRadius = (-2 * c) / (m_Light->getAttenuationLinear() + d);
		outerRadius *= 1.2f;
	}

	return outerRadius;
}


bool PointLight::IsCameraInside(const Ogre::Camera *Camera)
{
	//Small epsilon fix to account for the fact that we aren't a true sphere.
	return (Camera->getDerivedPosition().squaredDistance(m_Light->getDerivedPosition()) <= Ogre::Math::Sqr(m_Radius + Camera->getNearClipDistance() + 0.1f));
}


void PointLight::RebuildGeometry(const float &Radius)
{
	DISABLE_BIT(m_Settings, LightMaterialGenerator::MI_DIRECTIONAL);
	DISABLE_BIT(m_Settings, LightMaterialGenerator::MI_SPOTLIGHT);

	/// XXX some more intelligent expression for rings and segments
	delete mRenderOp.vertexData; 
	delete mRenderOp.indexData;
	mRenderOp.operationType = Ogre::RenderOperation::OT_TRIANGLE_LIST;
	mRenderOp.indexData = new Ogre::IndexData();
	mRenderOp.vertexData = new Ogre::VertexData();
	mRenderOp.useIndexes = true;

	ManualMeshGenerator::CreateSphere(mRenderOp.vertexData, mRenderOp.indexData, Radius, 10, 10, false, false);

	// Set bounding box and sphere
	setBoundingBox(Ogre::AxisAlignedBox(Ogre::Vector3(-Radius, -Radius, -Radius), Ogre::Vector3(Radius, Radius, Radius)));

	m_Radius = Radius;
	m_IgnoreWorld = false;

	ENABLE_BIT(m_Settings, LightMaterialGenerator::MI_POINT);
}