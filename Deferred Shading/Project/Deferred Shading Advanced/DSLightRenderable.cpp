#include "DSLightRenderable.h"
#include "DeferredShadingSystem.h"
#include "ManualMeshGenerator.h"
#include "Definitions.h"
#include "CommonParameters.h"


DSLightRenderable::DSLightRenderable(DeferredShadingSystem *DeferredShadingSystem, const LightType &Type, const float &Radius, Ogre::Light *Light) :
	m_DeferredShadingSystem(DeferredShadingSystem),
	m_Radius(Radius),
	m_OgreLight(Light),
	m_IgnoreWorld(false),
	m_IsDirty(true)
{
	if (m_OgreLight)
	{
		m_OgreLight->setCastShadows(false);
		m_OgreLight->setAttenuation(6, 1, 0, 0);

		SetType(Type);

		m_LightList.push_back(m_OgreLight);
	}
	else
	{
		m_LightType = LT_AMBIENT;

		setRenderQueueGroup(Ogre::RENDER_QUEUE_2);

		mRenderOp.vertexData = new Ogre::VertexData();
		mRenderOp.indexData = 0;

		ManualMeshGenerator::CreateQuad(mRenderOp.vertexData);

		mRenderOp.operationType = Ogre::RenderOperation::OT_TRIANGLE_STRIP;
		mRenderOp.useIndexes = false;

		setBoundingBox(Ogre::AxisAlignedBox(-10000, -10000, -10000, 10000, 10000, 10000));

		m_Material = Ogre::MaterialManager::getSingleton().getByName("AmbientLightMaterial");
		m_Material->load();
	}
}


//DSLightRenderable::~DSLightRenderable(void)
//{
//}


void DSLightRenderable::Render(void)
{
	Ogre::Camera *camera = m_DeferredShadingSystem->GetViewport()->getCamera();

	if (m_IsDirty)
	{
		m_IsDirty = false;

		if (m_LightType != LT_AMBIENT)
			UpdateLightSettings();
	}
		
	UpdateParameters(camera);
	
	if (m_LightType != LT_AMBIENT)
		UpdateMaterial(camera);

	Ogre::Technique *tech = m_Material->getBestTechnique();

	Ogre::Pass *pass = NULL;
	for (unsigned short i = 0; i < tech->getNumPasses(); i++)
	{
		pass = tech->getPass(i);

		//if (m_LightType == LT_AMBIENT)
		//	m_DeferredShadingSystem->GetSceneManager()->injectRenderWithPass(pass, this);
		//else
			//m_DeferredShadingSystem->GetSceneManager()->injectRenderWithPass(pass, this, &m_LightList);

		if (m_LightType == LT_AMBIENT)
			m_DeferredShadingSystem->GetSceneManager()->_injectRenderWithPass(pass, this, false);
		else
			m_DeferredShadingSystem->GetSceneManager()->_injectRenderWithPass(pass, this, false, false, &m_LightList);

		//m_DeferredShadingSystem->GetSceneManager()->_setPass(pass, false, false);

		//Ogre::RenderSystem *system = m_DeferredShadingSystem->GetSceneManager()->getDestinationRenderSystem();
		//system->_render(mRenderOp);
	}
}


void DSLightRenderable::SetType(const LightType &Value)
{
	m_LightType = Value;

	switch (m_LightType)
	{
	case LT_POINT:
		{
			m_OgreLight->setType(Ogre::Light::LT_POINT);
		} break;
	case LT_DIRECTIONAL:
		{
			m_OgreLight->setType(Ogre::Light::LT_DIRECTIONAL);
		} break;
	case LT_SPOTLIGHT:
		{
			m_OgreLight->setType(Ogre::Light::LT_SPOTLIGHT);
		} break;
	}

	m_IsDirty = true;
	m_Settings = 0;
}


float DSLightRenderable::getSquaredViewDepth(const Ogre::Camera *Camera) const
{
	if (m_IgnoreWorld)
		return 0.f;

	return (Camera->getDerivedPosition() - mParentNode->_getDerivedPosition()).squaredLength();
}


void DSLightRenderable::getWorldTransforms(Ogre::Matrix4 *xform) const
{
	switch (m_LightType)
	{
	case LT_AMBIENT:
		{
			*xform = Ogre::Matrix4::IDENTITY;
		} break;

	case LT_POINT:
	case LT_DIRECTIONAL:
		{
			xform->makeTransform(m_OgreLight->getDerivedPosition(), Ogre::Vector3::UNIT_SCALE, Ogre::Quaternion::IDENTITY);
		} break;

	case LT_SPOTLIGHT:
		{
			xform->makeTransform(m_OgreLight->getDerivedPosition(), Ogre::Vector3::UNIT_SCALE, Ogre::Vector3::UNIT_Y.getRotationTo(m_OgreLight->getDerivedDirection()));
		} break;
	}
}


void DSLightRenderable::UpdateLightSettings(void)
{
	if (m_LightType != LT_DIRECTIONAL && (m_OgreLight->getAttenuationConstant() != 1.f || m_OgreLight->getAttenuationLinear() != 0.f || m_OgreLight->getAttenuationQuadric() != 0.f))
		ENABLE_BIT(m_Settings, LightMaterialGenerator::MI_ATTENUATED);
	else
		DISABLE_BIT(m_Settings, LightMaterialGenerator::MI_ATTENUATED);

	if (m_OgreLight->getSpecularColour().r != 0 || m_OgreLight->getSpecularColour().g != 0 || m_OgreLight->getSpecularColour().b != 0)
		ENABLE_BIT(m_Settings, LightMaterialGenerator::MI_SPECULAR);
	else
		DISABLE_BIT(m_Settings, LightMaterialGenerator::MI_SPECULAR);

	//if (GetCastChadows())
	//	ENABLE_BIT(m_Settings, LightMaterialGenerator::MI_SHADOW_CASTER);
	//else
	//	DISABLE_BIT(m_Settings, LightMaterialGenerator::MI_SHADOW_CASTER);

	RebuildGeometry(GetOuterRadius());
	
	m_Material = m_DeferredShadingSystem->GetLightMaterialGenerator()->GetMaterial(m_Settings);

	m_Material->getTechnique(0)->getPass(0)->setVertexProgram("");
	m_Material->getTechnique(0)->getPass(0)->setFragmentProgram("");
	m_Material->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setTextureName("SpotlightPattern.png");
	m_Material->getTechnique(0)->getPass(0)->removeTextureUnitState(1);
	
	if (!m_Material->isLoaded())
		m_Material->load();
}


void DSLightRenderable::UpdateParameters(const Ogre::Camera *Camera)
{
	//const bool castShadows = GetCastChadows();

	//Set shader params
	Ogre::Technique *tech = m_Material->getBestTechnique();

	Ogre::Pass *pass = NULL;
	for (unsigned short i = 0; i < tech->getNumPasses(); i++) 
	{
		pass = tech->getPass(i);

		// get the vertex shader parameters

		// set the camera's far-top-right corner
		//Ogre::GpuProgramParametersSharedPtr params = pass->getVertexProgramParameters();
		//if (params->_findNamedConstantDefinition("cFarCorner"))
		//	params->setNamedConstant("cFarCorner", CommonParameters::GetFarCorner());

		//params = pass->getFragmentProgramParameters();
		//if (params->_findNamedConstantDefinition("cFarCorner"))
		//	params->setNamedConstant("cFarCorner", CommonParameters::GetFarCorner());

		//if (castShadows)
		//{
		//	Ogre::Camera shadowCam("ShadowCameraSetupCam", 0);
		//	shadowCam._notifyViewport(Camera->getViewport());

		//	Camera->getSceneManager()->getShadowCameraSetup()->getShadowCamera(Camera->getSceneManager(), Camera, Camera->getViewport(), m_Light, &shadowCam, 0);

		//	//Get the shadow camera position
		//	if (params->_findNamedConstantDefinition("shadowCamPos")) 
		//		params->setNamedConstant("shadowCamPos", shadowCam.getPosition());
		//	if (params->_findNamedConstantDefinition("shadowFarClip"))
		//		params->setNamedConstant("shadowFarClip", shadowCam.getFarClipDistance());
		//}
	}

	////Update shadow texture
	//if (castShadows)
	//{
	//	Ogre::SceneManager::RenderContext *context = Camera->getSceneManager()->_pauseRendering();
	//	Camera->getSceneManager()->prepareShadowTextures((Ogre::Camera*)Camera, Camera->getViewport(), &m_LightAsList);
	//	Camera->getSceneManager()->_resumeRendering(context);

	//	Ogre::Pass *pass = tech->getPass(0);
	//	Ogre::TextureUnitState *tex = pass->getTextureUnitState("ShadowMap");

	//	assert(tex);

	//	const Ogre::TexturePtr &shadowTex = Camera->getSceneManager()->getShadowTexture(0);

	//	if (tex->_getTexturePtr() != shadowTex)
	//		tex->_setTexturePtr(shadowTex);
	//}
}


void DSLightRenderable::UpdateMaterial(const Ogre::Camera *Camera)
{
	//Set shader params
	Ogre::Technique *tech = m_Material->getBestTechnique();

	//If inside light geometry, render back faces with CMPF_GREATER, otherwise normally
	if (m_LightType == LT_DIRECTIONAL)
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


bool DSLightRenderable::IsCameraInside(const Ogre::Camera *Camera)
{
	switch (m_LightType)
	{
	case LT_AMBIENT:
		{
			OGRE_EXCEPT(Ogre::Exception::ERR_INVALID_STATE, "You must not use this method in ambient-lights", "DSLightRenderable::IsCameraInside");
		} break;
		
	case LT_POINT:
		{
			//Small epsilon fix to account for the fact that we aren't a true sphere.
			return (Camera->getDerivedPosition().squaredDistance(m_OgreLight->getDerivedPosition()) <= Ogre::Math::Sqr(m_Radius + Camera->getNearClipDistance() + 0.1f));
		}
		
	case LT_DIRECTIONAL:
		{
			return false;
		}
		
	case LT_SPOTLIGHT:
		{
			//Ogre::Vector3 lightPos = m_OgreLight->getDerivedPosition();
			//Ogre::Vector3 lightDir = m_OgreLight->getDerivedDirection();
			//Ogre::Radian attAngle = m_OgreLight->getSpotlightOuterAngle();
		
			////Extend the analytic cone's radius by the near clip range by moving its tip accordingly.
			////Some trigonometry needed here.
			//Ogre::Vector3 clipRangeFix = -lightDir * (Camera->getNearClipDistance() / Ogre::Math::Tan(attAngle/2));
			//lightPos = lightPos + clipRangeFix;
   // 
			//Ogre::Vector3 lightToCamDir = Camera->getDerivedPosition() - lightPos;
			//Ogre::Real distanceFromLight = lightToCamDir.normalise();

			//Ogre::Real cosAngle = lightToCamDir.dotProduct(lightDir);
			//Ogre::Radian angle = Ogre::Math::ACos(cosAngle);
			////Check whether we will see the cone from our current POV.
			//return (distanceFromLight <= (m_OgreLight->getAttenuationRange() + clipRangeFix.length())) && (angle <= attAngle);
		}
	}
			return false;
}


void DSLightRenderable::RebuildGeometry(const float &Radius)
{
	DISABLE_BIT(m_Settings, LightMaterialGenerator::MI_POINT);
	DISABLE_BIT(m_Settings, LightMaterialGenerator::MI_DIRECTIONAL);
	DISABLE_BIT(m_Settings, LightMaterialGenerator::MI_SPOTLIGHT);
	
	/// XXX some more intelligent expression for rings and segments
	delete mRenderOp.vertexData; 
	delete mRenderOp.indexData;

	switch (m_LightType)
	{
	case LT_AMBIENT:
		{
			OGRE_EXCEPT(Ogre::Exception::ERR_INVALID_STATE, "You must not use this method in ambient-lights", "DSLightRenderable::RebuildGeometry");
		} break;
		
	case LT_POINT:
		{
			mRenderOp.operationType = Ogre::RenderOperation::OT_TRIANGLE_LIST;
			mRenderOp.indexData = new Ogre::IndexData();
			mRenderOp.vertexData = new Ogre::VertexData();
			mRenderOp.useIndexes = true;

			ManualMeshGenerator::CreateSphere(mRenderOp.vertexData, mRenderOp.indexData, Radius, 3, 3, false, false);

			// Set bounding box and sphere
			setBoundingBox(Ogre::AxisAlignedBox(Ogre::Vector3(-Radius, -Radius, -Radius), Ogre::Vector3(Radius, Radius, Radius)));

			m_Radius = Radius;
			m_IgnoreWorld = false;

			ENABLE_BIT(m_Settings, LightMaterialGenerator::MI_POINT);
		} break;
		
	case LT_DIRECTIONAL:
		{
			mRenderOp.vertexData = new Ogre::VertexData();
			mRenderOp.indexData = 0;

			ManualMeshGenerator::CreateQuad(mRenderOp.vertexData);

			mRenderOp.operationType = Ogre::RenderOperation::OT_TRIANGLE_STRIP; 
			mRenderOp.useIndexes = false; 

			setBoundingBox(Ogre::AxisAlignedBox(-10000,-10000,-10000,10000,10000,10000));

			m_Radius = 15000;
			m_IgnoreWorld = true;

			ENABLE_BIT(m_Settings, LightMaterialGenerator::MI_DIRECTIONAL);
		} break;
		
	case LT_SPOTLIGHT:
		{
			float height = m_OgreLight->getAttenuationRange();
			float radius = Ogre::Math::Tan(Ogre::Radian(m_OgreLight->getSpotlightOuterAngle() / 2)) * height;

			mRenderOp.operationType = Ogre::RenderOperation::OT_TRIANGLE_STRIP;
			mRenderOp.indexData = new Ogre::IndexData();
			mRenderOp.vertexData = new Ogre::VertexData();
			mRenderOp.useIndexes = false;

			//ManualMeshGenerator::CreateCone(mRenderOp.vertexData, mRenderOp.indexData, radius, height, 20);
			ManualMeshGenerator::CreateSquarePyramid(mRenderOp.vertexData, mRenderOp.indexData, radius * 2, height);

			setBoundingBox(Ogre::AxisAlignedBox(Ogre::Vector3(-radius, 0, -radius), Ogre::Vector3(radius, height, radius)));

			m_Radius = radius;
			m_IgnoreWorld = false;

			ENABLE_BIT(m_Settings, LightMaterialGenerator::MI_SPOTLIGHT);
		} break;
	}
}


const float DSLightRenderable::GetOuterRadius(void)
{
	switch (m_LightType)
	{
	case LT_AMBIENT:
		{
			OGRE_EXCEPT(Ogre::Exception::ERR_INVALID_STATE, "You must not use this method in ambient-lights", "DSLightRenderable::IsCameraInside");
		} return 0;
		
	case LT_POINT:
		{
			// Set Attenuation parameter to shader
			//setCustomParameter(3, Ogre::Vector4(m_Light->getAttenuationConstant(), m_Light->getAttenuationLinear(), m_Light->getAttenuationQuadric(), 0));

			float outerRadius = m_OgreLight->getAttenuationRange();

			if (m_OgreLight->getAttenuationConstant() != 1.f || m_OgreLight->getAttenuationLinear() != 0.f || m_OgreLight->getAttenuationQuadric() != 0.f)
			{
				// Calculate radius from Attenuation
				int threshold_level = 10;// difference of 10-15 levels deemed unnoticeable
				float threshold = 1.0f/((float)threshold_level/256.0f); 

				//// Use quadratic formula to determine outer radius
				float c = m_OgreLight->getAttenuationConstant() - threshold;
				float d = sqrt(m_OgreLight->getAttenuationLinear() * m_OgreLight->getAttenuationLinear() - 4 * m_OgreLight->getAttenuationQuadric() * c);
				outerRadius = (-2 * c) / (m_OgreLight->getAttenuationLinear() + d);
				outerRadius *= 1.2f;
			}

			return outerRadius;
		}
		
	case LT_DIRECTIONAL:
	case LT_SPOTLIGHT:
		{
			return m_OgreLight->getAttenuationRange();
		}
	}
}