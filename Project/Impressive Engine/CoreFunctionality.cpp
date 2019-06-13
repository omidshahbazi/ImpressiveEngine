/*/////////////////////////////////////////////////////////////////////////////////
///
///  Impressive Engine
///
/// Copyright (c) 2012-2013 Impressive Reality team
//
/// The license is
///
/// Permission is denied, to any person or company
///
/// The above copyright notice and this permission notice shall be included in
/// all copies or substantial portions of the Software.
///
/// Project leader : O.Shahbazi <sh_omid_m@yahoo.com>
/////////////////////////////////////////////////////////////////////////////////*/
#include "CoreFunctionality.h"
#include "ScriptRegister.h"


BEGIN_NAMESPACE
CoreFunctionality *Singleton<CoreFunctionality>::m_Instance = NULL;


//CoreFunctionality::CoreFunctionality(void)
//{
//}


//CoreFunctionality::~CoreFunctionality(void)
//{
//}


BEGIN_DEFINE_SCRIPT_REGISTRATION(CoreFunctionality)
	BEGIN_DEFINE_ENUMERATOR(Ogre::RenderQueue, "RenderQueue")
		DEFINE_ENUMERATOR_VALUE("RQ_BACKGROUND", Ogre::RENDER_QUEUE_BACKGROUND),
		DEFINE_ENUMERATOR_VALUE("RQ_SKIES_EARLY", Ogre::RENDER_QUEUE_SKIES_EARLY),
		DEFINE_ENUMERATOR_VALUE("RQ_QUEUE_1", Ogre::RENDER_QUEUE_1),
		DEFINE_ENUMERATOR_VALUE("RQ_QUEUE_2", Ogre::RENDER_QUEUE_2),
		DEFINE_ENUMERATOR_VALUE("RQ_WORLD_GEOMETRY_1", Ogre::RENDER_QUEUE_WORLD_GEOMETRY_1),
		DEFINE_ENUMERATOR_VALUE("RQ_QUEUE_3", Ogre::RENDER_QUEUE_3),
		DEFINE_ENUMERATOR_VALUE("RQ_QUEUE_4", Ogre::RENDER_QUEUE_4),
		DEFINE_ENUMERATOR_VALUE("RQ_MAIN", Ogre::RENDER_QUEUE_MAIN),
		DEFINE_ENUMERATOR_VALUE("RQ_QUEUE_6", Ogre::RENDER_QUEUE_6),
		DEFINE_ENUMERATOR_VALUE("RQ_QUEUE_7", Ogre::RENDER_QUEUE_7),
		DEFINE_ENUMERATOR_VALUE("RQ_WORLD_GEOMETRY_2", Ogre::RENDER_QUEUE_WORLD_GEOMETRY_2),
		DEFINE_ENUMERATOR_VALUE("RQ_QUEUE_8", Ogre::RENDER_QUEUE_8),
		DEFINE_ENUMERATOR_VALUE("RQ_QUEUE_9", Ogre::RENDER_QUEUE_9),
		DEFINE_ENUMERATOR_VALUE("RQ_SKIES_LATE", Ogre::RENDER_QUEUE_SKIES_LATE),
		DEFINE_ENUMERATOR_VALUE("RQ_OVERLAY", Ogre::RENDER_QUEUE_OVERLAY),
		DEFINE_ENUMERATOR_VALUE("RQ_MAX", Ogre::RENDER_QUEUE_MAX)
	END_DEFINE_ENUMERATOR()

	BEGIN_DEFINE_ENUMERATOR(Ogre::ShadeOptions, "ShadeOption")
		DEFINE_ENUMERATOR_VALUE("SO_FLAT", Ogre::SO_FLAT),
		DEFINE_ENUMERATOR_VALUE("SO_GOURAUD", Ogre::SO_GOURAUD),
		DEFINE_ENUMERATOR_VALUE("SO_PHONG", Ogre::SO_PHONG)
	END_DEFINE_ENUMERATOR()
	
	BEGIN_DEFINE_ENUMERATOR(Ogre::PolygonMode, "PolygonMode")
		DEFINE_ENUMERATOR_VALUE("PM_POINTS", Ogre::PM_POINTS),
		DEFINE_ENUMERATOR_VALUE("PM_SOLID", Ogre::PM_SOLID),
		DEFINE_ENUMERATOR_VALUE("PM_WIREFRAME", Ogre::PM_WIREFRAME)
	END_DEFINE_ENUMERATOR()
	
	BEGIN_DEFINE_ENUMERATOR(Ogre::ProjectionType, "ProjectionType")
		DEFINE_ENUMERATOR_VALUE("PT_ORTHOGRAPHIC", Ogre::PT_ORTHOGRAPHIC),
		DEFINE_ENUMERATOR_VALUE("PT_PERSPECTIVE", Ogre::PT_PERSPECTIVE)
	END_DEFINE_ENUMERATOR()

	BEGIN_DEFINE_ENUMERATOR(Ogre::SceneBlendFactor, "SceneBlendFactor")
		DEFINE_ENUMERATOR_VALUE("SBF_ONE", Ogre::SBF_ONE),
		DEFINE_ENUMERATOR_VALUE("SBF_ZERO", Ogre::SBF_ZERO),
		DEFINE_ENUMERATOR_VALUE("SBF_DEST_COLOUR", Ogre::SBF_DEST_COLOUR),
		DEFINE_ENUMERATOR_VALUE("SBF_SOURCE_COLOUR", Ogre::SBF_SOURCE_COLOUR),
		DEFINE_ENUMERATOR_VALUE("SBF_ONE_MINUS_DEST_COLOUR", Ogre::SBF_ONE_MINUS_DEST_COLOUR),
		DEFINE_ENUMERATOR_VALUE("SBF_ONE_MINUS_SOURCE_COLOUR", Ogre::SBF_ONE_MINUS_SOURCE_COLOUR),
		DEFINE_ENUMERATOR_VALUE("SBF_DEST_ALPHA", Ogre::SBF_DEST_ALPHA),
		DEFINE_ENUMERATOR_VALUE("SBF_SOURCE_ALPHA", Ogre::SBF_SOURCE_ALPHA),
		DEFINE_ENUMERATOR_VALUE("SBF_ONE_MINUS_DEST_ALPHA", Ogre::SBF_ONE_MINUS_DEST_ALPHA),
		DEFINE_ENUMERATOR_VALUE("SBF_ONE_MINUS_SOURCE_ALPHA", Ogre::SBF_ONE_MINUS_SOURCE_ALPHA)
	END_DEFINE_ENUMERATOR()
        
	BEGIN_DEFINE_ENUMERATOR(Ogre::SceneBlendType, "SceneBlendType")
		DEFINE_ENUMERATOR_VALUE("SBT_TRANSPARENT_ALPHA", Ogre::SBT_TRANSPARENT_ALPHA),
		DEFINE_ENUMERATOR_VALUE("SBT_TRANSPARENT_COLOUR", Ogre::SBT_TRANSPARENT_COLOUR),
		DEFINE_ENUMERATOR_VALUE("SBT_ADD", Ogre::SBT_ADD),
		DEFINE_ENUMERATOR_VALUE("SBT_MODULATE", Ogre::SBT_MODULATE),
		DEFINE_ENUMERATOR_VALUE("SBT_REPLACE", Ogre::SBT_REPLACE)
	END_DEFINE_ENUMERATOR()
        
	BEGIN_DEFINE_ENUMERATOR(Ogre::CompareFunction, "CompareFunction")
		DEFINE_ENUMERATOR_VALUE("CMPF_ALWAYS_FAIL", Ogre::CMPF_ALWAYS_FAIL),
		DEFINE_ENUMERATOR_VALUE("CMPF_ALWAYS_PASS", Ogre::CMPF_ALWAYS_PASS),
		DEFINE_ENUMERATOR_VALUE("CMPF_LESS", Ogre::CMPF_LESS),
		DEFINE_ENUMERATOR_VALUE("CMPF_LESS_EQUAL", Ogre::CMPF_LESS_EQUAL),
		DEFINE_ENUMERATOR_VALUE("CMPF_EQUAL", Ogre::CMPF_EQUAL),
		DEFINE_ENUMERATOR_VALUE("CMPF_NOT_EQUAL", Ogre::CMPF_NOT_EQUAL),
		DEFINE_ENUMERATOR_VALUE("CMPF_GREATER_EQUAL", Ogre::CMPF_GREATER_EQUAL),
		DEFINE_ENUMERATOR_VALUE("CMPF_GREATER", Ogre::CMPF_GREATER)
	END_DEFINE_ENUMERATOR()
        
	BEGIN_DEFINE_ENUMERATOR(Ogre::CullingMode, "CullingMode")
		DEFINE_ENUMERATOR_VALUE("CULL_NONE", Ogre::CULL_NONE),
		DEFINE_ENUMERATOR_VALUE("CULL_CLOCKWISE", Ogre::CULL_CLOCKWISE),
		DEFINE_ENUMERATOR_VALUE("CULL_ANTICLOCKWISE", Ogre::CULL_ANTICLOCKWISE)
	END_DEFINE_ENUMERATOR()
        
	BEGIN_DEFINE_ENUMERATOR(Ogre::ManualCullingMode, "ManualCullingMode")
		DEFINE_ENUMERATOR_VALUE("MANUAL_CULL_NONE", Ogre::MANUAL_CULL_NONE),
		DEFINE_ENUMERATOR_VALUE("MANUAL_CULL_BACK", Ogre::MANUAL_CULL_BACK),
		DEFINE_ENUMERATOR_VALUE("MANUAL_CULL_FRONT", Ogre::MANUAL_CULL_FRONT)
	END_DEFINE_ENUMERATOR()

	BEGIN_DEFINE_ENUMERATOR(Ogre::FogMode, "FogMode")
		DEFINE_ENUMERATOR_VALUE("FOG_NONE", Ogre::FOG_NONE),
		DEFINE_ENUMERATOR_VALUE("FOG_EXP", Ogre::FOG_EXP),
		DEFINE_ENUMERATOR_VALUE("FOG_EXP2", Ogre::FOG_EXP2),
		DEFINE_ENUMERATOR_VALUE("FOG_LINEAR", Ogre::FOG_LINEAR)
	END_DEFINE_ENUMERATOR()
	
	BEGIN_DEFINE_ENUMERATOR(Ogre::Light::LightTypes, "LightType")
		DEFINE_ENUMERATOR_VALUE("LT_DIRECTIONAL", Ogre::Light::LT_DIRECTIONAL),
		DEFINE_ENUMERATOR_VALUE("LT_POINT", Ogre::Light::LT_POINT),
		DEFINE_ENUMERATOR_VALUE("LT_SPOTLIGHT", Ogre::Light::LT_SPOTLIGHT)
	END_DEFINE_ENUMERATOR()
	
	BEGIN_DEFINE_ENUMERATOR(Ogre::Animation::InterpolationMode, "InterpolationMode")
		DEFINE_ENUMERATOR_VALUE("IM_LINEAR", Ogre::Animation::IM_LINEAR),
		DEFINE_ENUMERATOR_VALUE("IM_SPLINE", Ogre::Animation::IM_SPLINE)
	END_DEFINE_ENUMERATOR()
	
	BEGIN_DEFINE_ENUMERATOR(Ogre::Animation::RotationInterpolationMode, "RotationInterpolationMode")
		DEFINE_ENUMERATOR_VALUE("RIM_LINEAR", Ogre::Animation::RIM_LINEAR),
		DEFINE_ENUMERATOR_VALUE("RIM_SPHERICAL", Ogre::Animation::RIM_SPHERICAL)
	END_DEFINE_ENUMERATOR()

	//BEGIN_DEFINE_TYPE(Ogre::String, "String")
	//END_DEFINE_TYPE()

	BEGIN_DEFINE_TYPE(Ogre::Math, "Math")
		//DEFINE_FUNCTION("Abs", (Ogre::Real(Ogre::Math::*)(Ogre::Real))&Ogre::Math::Abs)
	END_DEFINE_TYPE()

	BEGIN_DEFINE_TYPE(Ogre::Vector2, "Vector2")
		.def(constructor<>())
		.def(constructor<float>())
		.def(constructor<float, float>())
		
		DEFINE_FUNCTION("GetLength", &Ogre::Vector2::length)
		DEFINE_FUNCTION("GetSquaredLength", &Ogre::Vector2::squaredLength)
		DEFINE_FUNCTION("GetDistance", &Ogre::Vector2::distance)
		DEFINE_FUNCTION("GetSquaredDistance", &Ogre::Vector2::squaredDistance)
		DEFINE_FUNCTION("GetDotProduct", &Ogre::Vector2::dotProduct)
		DEFINE_FUNCTION("GetNormalize", &Ogre::Vector2::normalise)

		DEFINE_PROPERTY("X", &Ogre::Vector2::x)
		DEFINE_PROPERTY("Y", &Ogre::Vector2::y)

		DEFINE_OPERATOR(const_self - Ogre::Vector2())
		DEFINE_OPERATOR(const_self + Ogre::Vector2())
		DEFINE_OPERATOR(const_self * float())
		DEFINE_OPERATOR(const_self * Ogre::Vector2())
		DEFINE_OPERATOR(const_self / float())
		DEFINE_OPERATOR(const_self / Ogre::Vector2())
	END_DEFINE_TYPE()

	BEGIN_DEFINE_TYPE(Ogre::Vector3, "Vector3")
		.def(constructor<>())
		.def(constructor<float>())
		.def(constructor<float, float, float>())
		
		DEFINE_FUNCTION("GetLength", &Ogre::Vector3::length)
		DEFINE_FUNCTION("GetSquaredLength", &Ogre::Vector3::squaredLength)
		DEFINE_FUNCTION("GetDistance", &Ogre::Vector3::distance)
		DEFINE_FUNCTION("GetSquaredDistance", &Ogre::Vector3::squaredDistance)
		DEFINE_FUNCTION("GetDotProduct", &Ogre::Vector3::dotProduct)
		DEFINE_FUNCTION("GetAbsDotProduct", &Ogre::Vector3::absDotProduct)
		DEFINE_FUNCTION("GetNormalize", &Ogre::Vector3::normalise)
		DEFINE_FUNCTION("GetCrossProduct", &Ogre::Vector3::crossProduct)
		DEFINE_FUNCTION("GetReflect", &Ogre::Vector3::reflect)

		DEFINE_PROPERTY("X", &Ogre::Vector3::x)
		DEFINE_PROPERTY("Y", &Ogre::Vector3::y)
		DEFINE_PROPERTY("Z", &Ogre::Vector3::z)
		
		DEFINE_OPERATOR(const_self - Ogre::Vector3())
		//DEFINE_OPERATOR(const_self - Ogre::Vector3())
		DEFINE_OPERATOR(const_self + Ogre::Vector3())
		DEFINE_OPERATOR(const_self * float())
		DEFINE_OPERATOR(const_self * Ogre::Vector3())
		DEFINE_OPERATOR(const_self / float())
		DEFINE_OPERATOR(const_self / Ogre::Vector3())
	END_DEFINE_TYPE()

	BEGIN_DEFINE_TYPE(Ogre::AxisAlignedBox, "AxisAlignedBox")
	END_DEFINE_TYPE()

	BEGIN_DEFINE_TYPE(Ogre::Quaternion, "Quaternion")
		DEFINE_OPERATOR(const_self * Ogre::Vector3())
	END_DEFINE_TYPE()
	
	BEGIN_DEFINE_TYPE(Ogre::Plane, "Plane")
		.def(constructor<Ogre::Vector3, Ogre::Vector3>())

		DEFINE_FUNCTION("GetDistance", &Ogre::Plane::getDistance)
	END_DEFINE_TYPE()

	BEGIN_DEFINE_TYPE(Ogre::CompositorInstance, "CompositorInstance")
		DEFINE_FUNCTION("SetEnabled", &Ogre::CompositorInstance::setEnabled)
	END_DEFINE_TYPE()
	
	BEGIN_DEFINE_TYPE(Ogre::ColourValue, "Color")
		.def(constructor<float, float, float>())
		.def(constructor<float, float, float, float>())

		DEFINE_PROPERTY("R", &Ogre::ColourValue::r)
		DEFINE_PROPERTY("G", &Ogre::ColourValue::g)
		DEFINE_PROPERTY("B", &Ogre::ColourValue::b)
		DEFINE_PROPERTY("A", &Ogre::ColourValue::a)
	END_DEFINE_TYPE()
	
	BEGIN_DEFINE_TYPE(Ogre::Pass, "Pass")
		DEFINE_FUNCTION("SetLightingEnabled", &Ogre::Pass::setLightingEnabled)
		DEFINE_FUNCTION("GetLightingEnabled", &Ogre::Pass::getLightingEnabled)

		DEFINE_FUNCTION("SetMaxSimultaneousLights", &Ogre::Pass::setMaxSimultaneousLights)
		DEFINE_FUNCTION("GetMaxSimultaneousLights", &Ogre::Pass::getMaxSimultaneousLights)
		
		DEFINE_FUNCTION("SetIteratePerLight", &Ogre::Pass::setIteratePerLight)
		DEFINE_FUNCTION("GetIteratePerLight", &Ogre::Pass::getIteratePerLight)
		DEFINE_FUNCTION("GetRunOnlyForOneLightType", &Ogre::Pass::getRunOnlyForOneLightType)
		DEFINE_FUNCTION("GetOnlyLightType", &Ogre::Pass::getOnlyLightType)
		
		DEFINE_FUNCTION("SetLightCountPerIteration", &Ogre::Pass::setLightCountPerIteration)
		DEFINE_FUNCTION("GetLightCountPerIteration", &Ogre::Pass::getLightCountPerIteration)

		DEFINE_FUNCTION("SetShadingMode", &Ogre::Pass::setShadingMode)
		DEFINE_FUNCTION("GetShadingMode", &Ogre::Pass::getShadingMode)
		
		DEFINE_FUNCTION("SetPolygonMode", &Ogre::Pass::setPolygonMode)
		DEFINE_FUNCTION("GetPolygonMode", &Ogre::Pass::getPolygonMode)
		
		DEFINE_FUNCTION("SetNormaliseNormals", &Ogre::Pass::setNormaliseNormals)
		DEFINE_FUNCTION("GetNormaliseNormals", &Ogre::Pass::getNormaliseNormals)
		
		DEFINE_FUNCTION("SetPolygonModeOverrideable", &Ogre::Pass::setPolygonModeOverrideable)
		DEFINE_FUNCTION("GetPolygonModeOverrideable", &Ogre::Pass::getPolygonModeOverrideable)
		
		DEFINE_FUNCTION("SetAmbient", (void(Ogre::Pass::*)(float, float, float))&Ogre::Pass::setAmbient)
		DEFINE_FUNCTION("SetAmbient", (void(Ogre::Pass::*)(const Ogre::ColourValue&))&Ogre::Pass::setAmbient)
		DEFINE_FUNCTION("GetAmbient", &Ogre::Pass::getAmbient)
		
		DEFINE_FUNCTION("SetDiffuse", (void(Ogre::Pass::*)(float, float, float, float))&Ogre::Pass::setDiffuse)
		DEFINE_FUNCTION("SetDiffuse", (void(Ogre::Pass::*)(const Ogre::ColourValue&))&Ogre::Pass::setDiffuse)
		DEFINE_FUNCTION("GetDiffuse", &Ogre::Pass::getDiffuse)
		
		DEFINE_FUNCTION("SetSpecular", (void(Ogre::Pass::*)(float, float, float, float))&Ogre::Pass::setSpecular)
		DEFINE_FUNCTION("SetSpecular", (void(Ogre::Pass::*)(const Ogre::ColourValue&))&Ogre::Pass::setSpecular)
		DEFINE_FUNCTION("GetSpecular", &Ogre::Pass::getSpecular)
		
		DEFINE_FUNCTION("SetSelfIllumination", (void(Ogre::Pass::*)(float, float, float))&Ogre::Pass::setSelfIllumination)
		DEFINE_FUNCTION("SetSelfIllumination", (void(Ogre::Pass::*)(const Ogre::ColourValue&))&Ogre::Pass::setSelfIllumination)
		DEFINE_FUNCTION("GetSelfIllumination", &Ogre::Pass::getSelfIllumination)
		
		DEFINE_FUNCTION("SetShininess", &Ogre::Pass::setShininess)
		DEFINE_FUNCTION("GetShininess", &Ogre::Pass::getShininess)
		
		DEFINE_FUNCTION("SetVertexColourTracking", &Ogre::Pass::setVertexColourTracking)
		DEFINE_FUNCTION("GetVertexColourTracking", &Ogre::Pass::getVertexColourTracking)
		
		DEFINE_FUNCTION("SetSceneBlending", (void(Ogre::Pass::*)(const Ogre::SceneBlendFactor, const Ogre::SceneBlendFactor))&Ogre::Pass::setSceneBlending)
		DEFINE_FUNCTION("SetSceneBlending", (void(Ogre::Pass::*)(const Ogre::SceneBlendType))&Ogre::Pass::setSceneBlending)
		DEFINE_FUNCTION("setSeparateSceneBlending", (void(Ogre::Pass::*)(const Ogre::SceneBlendType, const Ogre::SceneBlendType))&Ogre::Pass::setSeparateSceneBlending)
		DEFINE_FUNCTION("setSeparateSceneBlending", (void(Ogre::Pass::*)(const Ogre::SceneBlendFactor, const Ogre::SceneBlendFactor, const Ogre::SceneBlendFactor, const Ogre::SceneBlendFactor))&Ogre::Pass::setSeparateSceneBlending)
		
		DEFINE_FUNCTION("GetSourceBlendFactor", &Ogre::Pass::getSourceBlendFactor)
		DEFINE_FUNCTION("GetDestBlendFactor", &Ogre::Pass::getDestBlendFactor)
		DEFINE_FUNCTION("GetSourceBlendFactorAlpha", &Ogre::Pass::getSourceBlendFactorAlpha)
		DEFINE_FUNCTION("GetDestBlendFactorAlpha", &Ogre::Pass::getDestBlendFactorAlpha)

		DEFINE_FUNCTION("SetSeparateSceneBlendingOperation", (void(Ogre::Pass::*)(const Ogre::SceneBlendOperation))&Ogre::Pass::setSeparateSceneBlendingOperation)
		DEFINE_FUNCTION("SetSeparateSceneBlendingOperation", (void(Ogre::Pass::*)(const Ogre::SceneBlendOperation, const Ogre::SceneBlendOperation))&Ogre::Pass::setSeparateSceneBlendingOperation)
		DEFINE_FUNCTION("SetSceneBlendingOperation", &Ogre::Pass::getSceneBlendingOperation)
		DEFINE_FUNCTION("GetSceneBlendingOperationAlpha", &Ogre::Pass::getSceneBlendingOperationAlpha)

		DEFINE_FUNCTION("SetDepthCheckEnabled", &Ogre::Pass::setDepthCheckEnabled)
		DEFINE_FUNCTION("GetDepthCheckEnabled", &Ogre::Pass::getDepthCheckEnabled)

		DEFINE_FUNCTION("SetDepthWriteEnabled", &Ogre::Pass::setDepthWriteEnabled)
		DEFINE_FUNCTION("GetDepthWriteEnabled", &Ogre::Pass::getDepthWriteEnabled)

		DEFINE_FUNCTION("SetDepthFunction", &Ogre::Pass::setDepthFunction)
		DEFINE_FUNCTION("GetDepthFunction", &Ogre::Pass::getDepthFunction)
		
		DEFINE_FUNCTION("SetDepthBias", &Ogre::Pass::setDepthBias)
		DEFINE_FUNCTION("GetDepthBiasConstant", &Ogre::Pass::getDepthBiasConstant)
		DEFINE_FUNCTION("GetDepthBiasSlopeScale", &Ogre::Pass::getDepthBiasSlopeScale)

		DEFINE_FUNCTION("SetIterationDepthBias", &Ogre::Pass::setIterationDepthBias)
		DEFINE_FUNCTION("GetIterationDepthBias", &Ogre::Pass::getIterationDepthBias)

		DEFINE_FUNCTION("SetColourWriteEnabled", &Ogre::Pass::setColourWriteEnabled)
		DEFINE_FUNCTION("GetColourWriteEnabled", &Ogre::Pass::getColourWriteEnabled)

		DEFINE_FUNCTION("SetAlphaRejectFunction", &Ogre::Pass::setAlphaRejectFunction)
		DEFINE_FUNCTION("GetAlphaRejectFunction", &Ogre::Pass::getAlphaRejectFunction)
		
		DEFINE_FUNCTION("SetAlphaRejectValue", &Ogre::Pass::setAlphaRejectValue)
		DEFINE_FUNCTION("GetAlphaRejectValue", &Ogre::Pass::getAlphaRejectValue)
		
		DEFINE_FUNCTION("SetAlphaToCoverageEnabled", &Ogre::Pass::setAlphaToCoverageEnabled)
		DEFINE_FUNCTION("IsAlphaToCoverageEnabled", &Ogre::Pass::isAlphaToCoverageEnabled)
		
		DEFINE_FUNCTION("SetTransparentSortingEnabled", &Ogre::Pass::setTransparentSortingEnabled)
		DEFINE_FUNCTION("GetTransparentSortingEnabled", &Ogre::Pass::getTransparentSortingEnabled)
		
		DEFINE_FUNCTION("SetTransparentSortingForced", &Ogre::Pass::setTransparentSortingForced)
		DEFINE_FUNCTION("GetTransparentSortingForced", &Ogre::Pass::getTransparentSortingForced)
	
		DEFINE_FUNCTION("SetCullingMode", &Ogre::Pass::setCullingMode)
		DEFINE_FUNCTION("GetCullingMode", &Ogre::Pass::getCullingMode)
		
		DEFINE_FUNCTION("SetManualCullingMode", &Ogre::Pass::setManualCullingMode)
		DEFINE_FUNCTION("GetManualCullingMode", &Ogre::Pass::getManualCullingMode)
		
		DEFINE_FUNCTION("SetFog", &Ogre::Pass::setFog)
		DEFINE_FUNCTION("GetFogOverride", &Ogre::Pass::getFogOverride)
		DEFINE_FUNCTION("GetFogMode", &Ogre::Pass::getFogMode)
		DEFINE_FUNCTION("GetFogColour", &Ogre::Pass::getFogColour)
		DEFINE_FUNCTION("GetFogStart", &Ogre::Pass::getFogStart)
		DEFINE_FUNCTION("GetFogEnd", &Ogre::Pass::getFogEnd)
		DEFINE_FUNCTION("GetFogDensity", &Ogre::Pass::getFogDensity)
	END_DEFINE_TYPE()
	
	BEGIN_DEFINE_TYPE(Ogre::Technique, "Technique")
		DEFINE_FUNCTION("GetNumPasses", &Ogre::Technique::getNumPasses)

		DEFINE_FUNCTION("GetPass", (Ogre::Pass*(Ogre::Technique::*)(unsigned short))&Ogre::Technique::getPass)
		DEFINE_FUNCTION("GetPass", (Ogre::Pass*(Ogre::Technique::*)(const Ogre::String&))&Ogre::Technique::getPass)
		
		DEFINE_FUNCTION("MovePass", &Ogre::Technique::movePass)
	END_DEFINE_TYPE()
	
	BEGIN_DEFINE_TYPE(Ogre::Material, "Material")
		DEFINE_FUNCTION("GetNumTechniques", &Ogre::Material::getNumTechniques)

		DEFINE_FUNCTION("GetTechnique", (Ogre::Technique*(Ogre::Material::*)(unsigned short))&Ogre::Material::getTechnique)
		DEFINE_FUNCTION("GetTechnique", (Ogre::Technique*(Ogre::Material::*)(const Ogre::String&))&Ogre::Material::getTechnique)
	END_DEFINE_TYPE()

	BEGIN_DEFINE_TYPE(Ogre::AnimationState, "AnimationState")
		//
		// There's some blending options in this class, so if we required them, we can add them here!
		//
		DEFINE_FUNCTION("GetTimePosition", &Ogre::AnimationState::getTimePosition)
		DEFINE_FUNCTION("SetTimePosition", &Ogre::AnimationState::setTimePosition)

		DEFINE_FUNCTION("GetLength", &Ogre::AnimationState::getLength)
		DEFINE_FUNCTION("SetLength", &Ogre::AnimationState::setLength)

		DEFINE_FUNCTION("GetWeight", &Ogre::AnimationState::getWeight)
		DEFINE_FUNCTION("SetWeight", &Ogre::AnimationState::setWeight)

		DEFINE_FUNCTION("HasEnded", &Ogre::AnimationState::hasEnded)

		DEFINE_FUNCTION("GetEnabled", &Ogre::AnimationState::getEnabled)
		DEFINE_FUNCTION("SetEnabled", &Ogre::AnimationState::setEnabled)
		
		DEFINE_FUNCTION("GetLoop", &Ogre::AnimationState::getLoop)
		DEFINE_FUNCTION("SetLoop", &Ogre::AnimationState::setLoop)
	END_DEFINE_TYPE()
	
	BEGIN_DEFINE_TYPE(Ogre::TransformKeyFrame, "KeyFrame")
		DEFINE_FUNCTION("SetTranslate", &Ogre::TransformKeyFrame::setTranslate)
		DEFINE_FUNCTION("GetTranslate", &Ogre::TransformKeyFrame::getTranslate)
		
		DEFINE_FUNCTION("SetScale", &Ogre::TransformKeyFrame::setScale)
		DEFINE_FUNCTION("GetScale", &Ogre::TransformKeyFrame::getScale)
		
		DEFINE_FUNCTION("SetRotation", &Ogre::TransformKeyFrame::setRotation)
		DEFINE_FUNCTION("GetRotation", &Ogre::TransformKeyFrame::getRotation)
	END_DEFINE_TYPE()

	BEGIN_DEFINE_TYPE(CoreFunctionality, "Core")
		DEFINE_FUNCTION("GetInEditMode", &CoreFunctionality::GetInEditMode)

		DEFINE_FUNCTION("GetEngineName", &CoreFunctionality::GetEngineName)

		DEFINE_FUNCTION("GetPathData", &CoreFunctionality::GetPathData)
		DEFINE_FUNCTION("GetPathLevel", &CoreFunctionality::GetPathLevel)
		DEFINE_FUNCTION("GetPathMaterial", &CoreFunctionality::GetPathMaterial)
		DEFINE_FUNCTION("GetPathModel", &CoreFunctionality::GetPathModel)
		DEFINE_FUNCTION("GetPathPlugIns", &CoreFunctionality::GetPathPlugIns)
		DEFINE_FUNCTION("GetPathScene", &CoreFunctionality::GetPathScene)
		DEFINE_FUNCTION("GetPathScript", &CoreFunctionality::GetPathScript)
		DEFINE_FUNCTION("GetPathTexture", &CoreFunctionality::GetPathTexture)

		DEFINE_FUNCTION("LoadScene", &CoreFunctionality::LoadScene)
		DEFINE_FUNCTION("GetScene", (Scene*(CoreFunctionality::*)(const Ogre::String&))&CoreFunctionality::GetScene)
		DEFINE_FUNCTION("GetScene", (Scene*(CoreFunctionality::*)(unsigned int))&CoreFunctionality::GetScene)
		DEFINE_FUNCTION("SetCurrentScene", &CoreFunctionality::SetCurrentScene)
		DEFINE_FUNCTION("GetCurrentScene", &CoreFunctionality::GetCurrentScene)
		DEFINE_FUNCTION("DestroyScene", (void(CoreFunctionality::*)(const Ogre::String&))&CoreFunctionality::DestroyScene)
		DEFINE_FUNCTION("DestroyScene", (void(CoreFunctionality::*)(unsigned int))&CoreFunctionality::DestroyScene)
		DEFINE_FUNCTION("DestroyScene", (void(CoreFunctionality::*)(Scene*))&CoreFunctionality::DestroyScene)

		DEFINE_FUNCTION("SetCamera", &CoreFunctionality::SetCamera)
		DEFINE_FUNCTION("GetCamera", &CoreFunctionality::GetCamera)
		
		DEFINE_FUNCTION("AddScriptLocation", &CoreFunctionality::AddScriptLocation)
		DEFINE_FUNCTION("DefineFunction", &CoreFunctionality::DefineFunction)
		DEFINE_FUNCTION("ExecuteScript", &CoreFunctionality::ExecuteScript)
		DEFINE_FUNCTION("ExecuteCommand", &CoreFunctionality::ExecuteCommand)
		
		DEFINE_FUNCTION("SetSize", (void(CoreFunctionality::*)(unsigned int, unsigned int))&CoreFunctionality::SetSize)
		DEFINE_FUNCTION("SetSize", (void(CoreFunctionality::*)(unsigned int, unsigned int, bool))&CoreFunctionality::SetSize)
		DEFINE_FUNCTION("SetPosition", &CoreFunctionality::SetPosition)
		DEFINE_FUNCTION("GetWidth", &CoreFunctionality::GetWidth)
		DEFINE_FUNCTION("GetHeight", &CoreFunctionality::GetHeight)
		DEFINE_FUNCTION("SetText", &CoreFunctionality::SetText)
		
		DEFINE_FUNCTION("TakeScreenshot", &CoreFunctionality::TakeScreenshot)

		DEFINE_FUNCTION("Close", &CoreFunctionality::Close)
	END_DEFINE_TYPE()
END_DEFINE_SCRIPT_REGISTRATION()

END_NAMESPACE