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
#ifdef EDIT_MODE

#pragma once

#include "Singleton.h"
#include "PropertiesControl.h"
#include "StdAfxOGRE.h"
#include "StdAfxwxWidgets.h"
#include "Scene.h"
#include "OceanGameObject.h"
#include "MaterialEditorControl.h"
#include "DeferredShadingSystem.h"

BEGIN_NAMESPACE
class Properties : public Singleton<Properties>
{
public:
	Properties(void);
	//~Properties(void);

	void Initialize(wxWindow *Parent);

	void Clear(void);

	void BeginNewProperties(const wxString &Caption, IPropertiesListener *Listener = 0);
	void EndNewProperties(void);

	wxPGProperty *AddCategory(const wxString &Label, const wxString &Name = "", wxPGProperty *Parent = NULL);

	void SetHelp(const wxString &Label, const wxString &Value);
	void SetReadOnly(const wxString &Label, bool Value);

	void AddPropertyOfString(const wxString &Label, const wxString &Value, const wxString &HelpText = "", bool ReadOnly = false, const wxString &Name = "");
	void AddPropertyOfFont(const wxString &Label, const wxFont &Value, const wxString &HelpText = "", const wxString &Name = "");
	void AddPropertyOfColor(const wxString &Label, const Ogre::ColourValue &Value/*const wxColor &Value*/, const wxString &HelpText = "", const wxString &Name = "");
	void AddPropertyOfBool(const wxString &Label, bool Value, const wxString &HelpText = "", const wxString &Name = "");
	void AddPropertyOfInt(const wxString &Label, int Value, const wxString &HelpText = "", const wxString &Name = "");
	void AddPropertyOfFloat(const wxString &Label, double Value, const wxString &HelpText = "", const wxString &Name = "");
	void AddPropertyOfVector2(const wxString &Label, Ogre::Vector2 Value, const wxString &HelpText = "", const wxString &Name = "");
	void AddPropertyOfVector3(const wxString &Label, Ogre::Vector3 Value, const wxString &HelpText = "", const wxString &Name = "");
	void AddPropertyOfQuaternion(const wxString &Label, Ogre::Quaternion Value, const wxString &HelpText = "", const wxString &Name = "");
	void AddPropertyOfEnumerator(const wxString &Label, const char **Labels, const int *Values, int Value, const wxString &HelpText = "", const wxString &Name = "");
	void AddPropertyOfEnumerator_SceneBlendFactor(const wxString &Label, Ogre::SceneBlendFactor Value, const wxString &HelpText = "", const wxString &Name = "");
	void AddPropertyOfEnumerator_SceneBlendOperation(const wxString &Label, Ogre::SceneBlendOperation Value, const wxString &HelpText = "", const wxString &Name = "");
	void AddPropertyOfEnumerator_CompareFunction(const wxString &Label, Ogre::CompareFunction Value, const wxString &HelpText = "", const wxString &Name = "");
	void AddPropertyOfEnumerator_TrackVertexColourType(const wxString &Label, Ogre::TrackVertexColourType Value, const wxString &HelpText = "", const wxString &Name = "");
	void AddPropertyOfEnumerator_ShadeOptions(const wxString &Label, Ogre::ShadeOptions Value, const wxString &HelpText = "", const wxString &Name = "");
	void AddPropertyOfEnumerator_PolygonMode(const wxString &Label, Ogre::PolygonMode Value, const wxString &HelpText = "", const wxString &Name = "");
	void AddPropertyOfEnumerator_CullingMode(const wxString &Label, Ogre::CullingMode Value, const wxString &HelpText = "", const wxString &Name = "");
	void AddPropertyOfEnumerator_ManualCullingMode(const wxString &Label, Ogre::ManualCullingMode Value, const wxString &HelpText = "", const wxString &Name = "");
	void AddPropertyOfEnumerator_FogMode(const wxString &Label, Ogre::FogMode Value, const wxString &HelpText = "", const wxString &Name = "");
	void AddPropertyOfEnumerator_TextureType(const wxString &Label, Ogre::TextureType Value, const wxString &HelpText = "", const wxString &Name = "");
	void AddPropertyOfEnumerator_PixelFormat(const wxString &Label, Ogre::PixelFormat Value, const wxString &HelpText = "", const wxString &Name = "");
	void AddPropertyOfEnumerator_TextureAddressingMode(const wxString &Label, Ogre::TextureUnitState::TextureAddressingMode Value, const wxString &HelpText = "", const wxString &Name = "");
	void AddPropertyOfEnumerator_FilterOptions(const wxString &Label, Ogre::FilterOptions Value, const wxString &HelpText = "", const wxString &Name = "");
	void AddPropertyOfEnumerator_BindingType(const wxString &Label, Ogre::TextureUnitState::BindingType Value, const wxString &HelpText = "", const wxString &Name = "");
	void AddPropertyOfEnumerator_LightTypes(const wxString &Label, Ogre::Light::LightTypes Value, const wxString &HelpText = "", const wxString &Name = "");
	void AddPropertyOfEnumerator_LightType(const wxString &Label, Light::LightType Value, const wxString &HelpText = "", const wxString &Name = "");
	//void AddPropertyOfEnumerator_ShadowTechnique(const wxString &Label, Ogre::ShadowTechnique Value, const wxString &HelpText = "", const wxString &Name = "");
	void AddPropertyOfEnumerator_ShadowType(const wxString &Label, ShadowManager::ShadowType Value, const wxString &HelpText = "", const wxString &Name = "");
	void AddPropertyOfEnumerator_ShadowCameraSetupType(const wxString &Label, ShadowManager::ShadowCameraSetupType Value, const wxString &HelpText = "", const wxString &Name = "");
	void AddPropertyOfEnumerator_ProjectionType(const wxString &Label, Ogre::ProjectionType Value, const wxString &HelpText = "", const wxString &Name = "");
	void AddPropertyOfEnumerator_ShaderMode(const wxString &Label, Hydrax::MaterialManager::ShaderMode Value, const wxString &HelpText = "", const wxString &Name = "");
	void AddPropertyOfEnumerator_NormalMode(const wxString &Label, Hydrax::MaterialManager::NormalMode Value, const wxString &HelpText = "", const wxString &Name = "");
	void AddPropertyOfEnumerator_LightingMode(const wxString &Label, SkyX::SkyX::LightingMode Value, const wxString &HelpText = "", const wxString &Name = "");
	void AddPropertyOfEnumerator_SkyMode(const wxString &Label, Scene::SkyMode Value, const wxString &HelpText = "", const wxString &Name = "");
	void AddPropertyOfEnumerator_ModuleType(const wxString &Label, OceanGameObject::ModuleType Value, const wxString &HelpText = "", const wxString &Name = "");
	void AddPropertyOfEnumerator_NoiseType(const wxString &Label, OceanGameObject::NoiseType Value, const wxString &HelpText = "", const wxString &Name = "");
	void AddPropertyOfEnumerator_MaterialType(const wxString &Label, MaterialEditorControl::MaterialType Value, const wxString &HelpText = "", const wxString &Name = "");
	void AddPropertyOfEnumerator_RenderMode(const wxString &Label, const DeferredShadingSystem::RenderMode &Value, const wxString &HelpText = "", const wxString &Name = "");


	static Ogre::ColourValue GetColorFromValue(const wxVariant &Data);


	const wxString GetName(void)
	{
		return "Properties";
	}
	
	const wxString GetCaption(void)
	{
		return "Properties";
	}

	PropertiesControl *GetControl(void)
	{
		return m_Control;
	}

private:
	const wxString &GetName(const wxString &Label, const wxString &Name);

private:
	PropertiesControl *m_Control;
	wxPGProperty *m_LastAddedCategory;
};


#endif


//#ifdef EDIT_MODE
//
//#pragma once
//
//#include "Singleton.h"
//#include "PropertiesControl.h"
//#include "StdAfxOGRE.h"
//#include "StdAfxwxWidgets.h"
//#include "MaterialEditorControl.h"
//
//class Properties : public Singleton<Properties>
//{
//public:
//	Properties(void);
//	//~Properties(void);
//
//	void Initialize(wxWindow *Parent);
//
//	void Clear(void);
//
//	void BeginNewProperties(const const wxString &&Caption, IPropertiesListener *Listener = 0);
//	void EndNewProperties(void);
//
//	wxPGProperty *AddCategory(const const wxString &&Label, const const wxString &&Name = "", wxPGProperty *Parent = NULL);
//
//	void SetHelp(const const wxString &&Label, const const wxString &&Value);
//	void SetReadOnly(const const wxString &&Label, bool Value);
//
//	void AddPropertyOfString(const const wxString &&Label, const const wxString &&Value, const const wxString &&HelpText = "", bool ReadOnly = false, const const wxString &&Name = "");
//	void AddPropertyOfFont(const const wxString &&Label, const wxFont &Value, const const wxString &&HelpText = "", const const wxString &&Name = "");
//	void AddPropertyOfColor(const const wxString &&Label, const Ogre::ColourValue &Value/*const wxColor &Value*/, const const wxString &&HelpText = "", const const wxString &&Name = "");
//	void AddPropertyOfBool(const const wxString &&Label, bool Value, const const wxString &&HelpText = "", const const wxString &&Name = "");
//	void AddPropertyOfInt(const const wxString &&Label, int Value, const const wxString &&HelpText = "", const const wxString &&Name = "");
//	void AddPropertyOfFloat(const const wxString &&Label, double Value, const const wxString &&HelpText = "", const const wxString &&Name = "");
//	void AddPropertyOfVector3(const const wxString &&Label, Ogre::Vector3 Value, const const wxString &&HelpText = "", const const wxString &&Name = "");
//	void AddPropertyOfQuaternion(const const wxString &&Label, Ogre::Quaternion Value, const const wxString &&HelpText = "", const const wxString &&Name = "");
//	void AddPropertyOfEnumerator(const const wxString &&Label, const char **Labels, const int *Values, int Value, const const wxString &&HelpText = "", const const wxString &&Name = "");
//	void AddPropertyOfEnumerator_SceneBlendFactor(const const wxString &&Label, Ogre::SceneBlendFactor Value, const const wxString &&HelpText = "", const const wxString &&Name = "");
//	void AddPropertyOfEnumerator_SceneBlendOperation(const const wxString &&Label, Ogre::SceneBlendOperation Value, const const wxString &&HelpText = "", const const wxString &&Name = "");
//	void AddPropertyOfEnumerator_CompareFunction(const const wxString &&Label, Ogre::CompareFunction Value, const const wxString &&HelpText = "", const const wxString &&Name = "");
//	void AddPropertyOfEnumerator_TrackVertexColourType(const const wxString &&Label, Ogre::TrackVertexColourType Value, const const wxString &&HelpText = "", const const wxString &&Name = "");
//	void AddPropertyOfEnumerator_ShadeOptions(const const wxString &&Label, Ogre::ShadeOptions Value, const const wxString &&HelpText = "", const const wxString &&Name = "");
//	void AddPropertyOfEnumerator_PolygonMode(const const wxString &&Label, Ogre::PolygonMode Value, const const wxString &&HelpText = "", const const wxString &&Name = "");
//	void AddPropertyOfEnumerator_CullingMode(const const wxString &&Label, Ogre::CullingMode Value, const const wxString &&HelpText = "", const const wxString &&Name = "");
//	void AddPropertyOfEnumerator_ManualCullingMode(const const wxString &&Label, Ogre::ManualCullingMode Value, const const wxString &&HelpText = "", const const wxString &&Name = "");
//	void AddPropertyOfEnumerator_FogMode(const const wxString &&Label, Ogre::FogMode Value, const const wxString &&HelpText = "", const const wxString &&Name = "");
//	void AddPropertyOfEnumerator_TextureType(const const wxString &&Label, Ogre::TextureType Value, const const wxString &&HelpText = "", const const wxString &&Name = "");
//	void AddPropertyOfEnumerator_PixelFormat(const const wxString &&Label, Ogre::PixelFormat Value, const const wxString &&HelpText = "", const const wxString &&Name = "");
//	void AddPropertyOfEnumerator_TextureAddressingMode(const const wxString &&Label, Ogre::TextureUnitState::TextureAddressingMode Value, const const wxString &&HelpText = "", const const wxString &&Name = "");
//	void AddPropertyOfEnumerator_FilterOptions(const const wxString &&Label, Ogre::FilterOptions Value, const const wxString &&HelpText = "", const const wxString &&Name = "");
//	void AddPropertyOfEnumerator_BindingType(const const wxString &&Label, Ogre::TextureUnitState::BindingType Value, const const wxString &&HelpText = "", const const wxString &&Name = "");
//	void AddPropertyOfEnumerator_LightTypes(const const wxString &&Label, Ogre::Light::LightTypes Value, const const wxString &&HelpText = "", const const wxString &&Name = "");
//	void AddPropertyOfEnumerator_MaterialType(const const wxString &&Label, MaterialEditorControl::MaterialType Value, const const wxString &&HelpText = "", const const wxString &&Name = "");
//
//
//	static Ogre::ColourValue GetColorFromValue(const wxVariant &Data);
//
//
//	const wxString &GetName(void)
//	{
//		return "Properties";
//	}
//	
//	const wxString &GetCaption(void)
//	{
//		return "Properties";
//	}
//
//	PropertiesControl *GetControl(void)
//	{
//		return m_Control;
//	}
//
//private:
//	const wxString &GetName(const const wxString &&Label, const const wxString &&Name);
//
//private:
//	PropertiesControl *m_Control;
//	wxPGProperty *m_LastAddedCategory;
//};
//
//
//#endif
END_NAMESPACE