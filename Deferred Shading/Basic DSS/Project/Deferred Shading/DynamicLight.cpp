#include "DynamicLight.h"
#include "Definition.h"
#include "ManualMeshGenerator.h"


DynamicLight::DynamicLight(Ogre::SceneManager *SceneManager, DSSMaterialGenerator *DSSMaterialGenerator) :
	m_DSSMaterialGenerator(DSSMaterialGenerator),
	m_Settings(0),
	m_IgnoreWorld(false)
{
	mRenderOp.operationType = Ogre::RenderOperation::OT_TRIANGLE_LIST;
	mRenderOp.indexData = NULL;
	mRenderOp.vertexData = NULL;
	mRenderOp.useIndexes = true;

	m_Light = SceneManager->createLight();
	m_LightAsList.push_back(m_Light);

	m_Light->setCastShadows(false);
}


DynamicLight::~DynamicLight(void)
{
}


void DynamicLight::SetAttenuation(const float &Range, const float &Constant, const float &Linear, const float &Quadratic)
{
	m_Light->setAttenuation(Range, Constant, Linear, Quadratic);

	m_IsDirty = true;
}


void DynamicLight::SetSpecularColor(const Ogre::ColourValue &Color)
{
	m_Light->setSpecularColour(Color);

	m_IsDirty = true;
}


void DynamicLight::Update(Ogre::Camera *Camera/*, const Ogre::LightList *LightList*/)
{
	if (m_IsDirty)
	{
		m_IsDirty = false;

		UpdateLightSettings();

		UpdateParameters(Camera);
	}

	UpdateMaterial(Camera);

	LightBase::Update(Camera, &m_LightAsList);
}


float DynamicLight::getSquaredViewDepth(const Ogre::Camera *Camera) const
{
	if (m_IgnoreWorld)
		return 0.f;

	return (Camera->getDerivedPosition() - mParentNode->_getDerivedPosition()).squaredLength();
}


void DynamicLight::getWorldTransforms(Ogre::Matrix4 *xform) const
{
	xform->makeTransform(m_Light->getDerivedPosition(), Ogre::Vector3::UNIT_SCALE, Ogre::Quaternion::IDENTITY);
}


const bool DynamicLight::GetCastChadows(void) const
{
	return 
		m_Light->_getManager()->isShadowTechniqueInUse() &&
		m_Light->getCastShadows() && 
		(GetLightType() == LT_DIRECTIONAL || GetLightType() == LT_SPOTLIGHT);
}


void DynamicLight::UpdateMaterial(const Ogre::Camera *Camera)
{
	//Set shader params
	Ogre::Technique *tech = m_Material->getBestTechnique();

	//If inside light geometry, render back faces with CMPF_GREATER, otherwise normally
	if (GetLightType() == LT_DIRECTIONAL)
	{
		tech->setCullingMode(Ogre::CULL_CLOCKWISE);
		tech->setDepthCheckEnabled(false);
	}
	else
	{
		tech->setDepthCheckEnabled(true);

		if (IsCameraInside(Camera))
		{
			tech->setCullingMode(Ogre::CULL_ANTICLOCKWISE);
			tech->setDepthFunction(Ogre::CMPF_GREATER_EQUAL);
		}
		else
		{
			tech->setCullingMode(Ogre::CULL_CLOCKWISE);
			tech->setDepthFunction(Ogre::CMPF_LESS_EQUAL);
		}
	}
}


void DynamicLight::UpdateParameters(const Ogre::Camera *Camera)
{
	const bool castShadows = GetCastChadows();

	//Set shader params
	Ogre::Technique *tech = m_Material->getBestTechnique();
	Ogre::Vector3 farCorner = Camera->getViewMatrix(true) * Camera->getWorldSpaceCorners()[4];

	Ogre::Pass *pass = NULL;
	for (unsigned short i = 0; i < tech->getNumPasses(); i++) 
	{
		pass = tech->getPass(i);

		// get the vertex shader parameters

		// set the camera's far-top-right corner
		Ogre::GpuProgramParametersSharedPtr params = pass->getVertexProgramParameters();
		if (params->_findNamedConstantDefinition("farCorner"))
			params->setNamedConstant("farCorner", farCorner);

		params = pass->getFragmentProgramParameters();
		if (params->_findNamedConstantDefinition("farCorner"))
			params->setNamedConstant("farCorner", farCorner);

		if (castShadows)
		{
			Ogre::Camera shadowCam("ShadowCameraSetupCam", 0);
			shadowCam._notifyViewport(Camera->getViewport());

			Camera->getSceneManager()->getShadowCameraSetup()->getShadowCamera(Camera->getSceneManager(), Camera, Camera->getViewport(), m_Light, &shadowCam, 0);

			//Get the shadow camera position
			if (params->_findNamedConstantDefinition("shadowCamPos")) 
				params->setNamedConstant("shadowCamPos", shadowCam.getPosition());
			if (params->_findNamedConstantDefinition("shadowFarClip"))
				params->setNamedConstant("shadowFarClip", shadowCam.getFarClipDistance());
		}
	}

	//Update shadow texture
	if (castShadows)
	{
		Ogre::SceneManager::RenderContext *context = Camera->getSceneManager()->_pauseRendering();
		Camera->getSceneManager()->prepareShadowTextures((Ogre::Camera*)Camera, Camera->getViewport(), &m_LightAsList);
		Camera->getSceneManager()->_resumeRendering(context);

		Ogre::Pass *pass = tech->getPass(0);
		Ogre::TextureUnitState *tex = pass->getTextureUnitState("ShadowMap");

		assert(tex);

		const Ogre::TexturePtr &shadowTex = Camera->getSceneManager()->getShadowTexture(0);

		if (tex->_getTexturePtr() != shadowTex)
			tex->_setTexturePtr(shadowTex);
	}
}


void DynamicLight::UpdateLightSettings(void)
{
	//if (!m_IsDirty)
	//	return;

	//m_IsDirty = false;

	if (m_Light->getAttenuationConstant() != 1.f || m_Light->getAttenuationLinear() != 0.f || m_Light->getAttenuationQuadric() != 0.f)
		ENABLE_BIT(m_Settings, LightMaterialGenerator::MI_ATTENUATED);
	else
		DISABLE_BIT(m_Settings, LightMaterialGenerator::MI_ATTENUATED);

	//if(m_Light->getSpecularColour() != Ogre::ColourValue(0, 0, 0, 1))
	if(m_Light->getSpecularColour().r != 0 || m_Light->getSpecularColour().g != 0 || m_Light->getSpecularColour().b != 0)
		ENABLE_BIT(m_Settings, LightMaterialGenerator::MI_SPECULAR);
	else
		DISABLE_BIT(m_Settings, LightMaterialGenerator::MI_SPECULAR);

	if (GetCastChadows())
		ENABLE_BIT(m_Settings, LightMaterialGenerator::MI_SHADOW_CASTER);
	else
		DISABLE_BIT(m_Settings, LightMaterialGenerator::MI_SHADOW_CASTER);

	RebuildGeometry(GetOuterRadius());

	m_Material = m_DSSMaterialGenerator->GetMaterial(m_Settings);
	
	if (!m_Material->isLoaded())
		m_Material->load();
}