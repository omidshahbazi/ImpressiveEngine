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


#include "Engine.h"
#include "SceneControl.h"
#include "MeshGameObject.h"
#include "CameraGameObject.h"
#include "LightGameObject.h"
#include "TerrainGameObject.h"
#include "OceanGameObject.h"
#include "ParticleGameObject.h"
#include "ScenesHolder.h"
#include "AtmosphereManager.h"
#include "Utility.h"
#include "AdditionProperties.h"
#include "ViewPortsManager.h"
#include "RenderControl.h"
#include "Script.h"
#include "ManualFunctionality.h"
#include "Editor.h"


BEGIN_NAMESPACE
BEGIN_EVENT_TABLE(SceneControl, wxWindow)
	EVT_SIZE(SceneControl::OnSize)
	END_EVENT_TABLE()



	SceneControl::SceneControl(wxWindow *Parent) :
	wxWindow(Parent, -1),
	m_LastSelectedGameObject(NULL),
	m_CutItem(false)
{
	wxBoxSizer *fgs = new wxBoxSizer(wxVERTICAL);
	//fgs->SetFlexibleDirection(wxBOTH);
	//fgs->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);


	m_ToolBar = new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL | wxTB_FLAT);
	m_ToolBar->AddTool(ID_ADD_GAMEOBJECT, "", wxBitmap(Engine::PATH_EDITOR_IMAGE + "New.png", wxBITMAP_TYPE_ANY), wxNullBitmap, wxITEM_NORMAL, "Add EmptyGameObject");
	m_ToolBar->AddSeparator();
	m_ToolBar->AddTool(ID_DELETE, "", wxBitmap(Engine::PATH_EDITOR_IMAGE + "Delete.png", wxBITMAP_TYPE_ANY), wxNullBitmap, wxITEM_NORMAL, "Delete");
	m_ToolBar->Realize();

	fgs->Add(m_ToolBar, 0, wxALL);

	m_Tree = new wxTreeCtrl(this);
	fgs->Add(m_Tree, 0, wxALL);

	SetSizer(fgs);
	Layout();


	m_Tree->Connect(wxEVT_COMMAND_TREE_KEY_DOWN, wxTreeEventHandler(SceneControl::OnKey), NULL, this);
	m_Tree->Connect(wxEVT_COMMAND_TREE_SEL_CHANGED, wxTreeEventHandler(SceneControl::OnSelectedItem), NULL, this);
	m_Tree->Connect(wxEVT_COMMAND_TREE_ITEM_ACTIVATED,wxTreeEventHandler(SceneControl::OnDoubleClick));
	//m_Tree->Connect(wxEVT_COMMAND_TREE_BEGIN_DRAG, wxTreeEventHandler(SceneControl::OnBeginDrag), NULL, this);
	//m_Tree->Connect(wxEVT_COMMAND_TREE_END_DRAG, wxTreeEventHandler(SceneControl::OnEngDrag), NULL, this);

	Connect(ID_DELETE, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(SceneControl::OnDeleteGameObject));
	Connect(ID_ADD_GAMEOBJECT, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(SceneControl::OnAddGameObject));

	ReloadContent();
}


//SceneControl::~SceneControl(void)
//{
//}


void SceneControl::SelectItem(Ogre::String Name)
{
	wxTreeItemId item = GetItem(Name.c_str(), m_Tree->GetRootItem());

	if (item.IsOk())
		m_Tree->SelectItem(item);
	else
		m_Tree->SelectItem(m_Tree->GetRootItem());
}


void SceneControl::ReloadContent(void)
{
	m_Tree->DeleteAllItems();

	m_RootID = m_Tree->AddRoot("Scene");

	if (ScenesHolder::GetInstance().GetCurrentScene())
	{
		BEGIN_LIST_CONST_ITERATOR(ScenesHolder::GetInstance().GetCurrentScene()->GetChildren(), GameObject)

			AddChildSceneContent(m_RootID, current);

		END_LIST_CONST_ITERATOR

			m_Tree->ExpandAll();
	}
}


void SceneControl::RemoveSelectedItem(void)
{
	wxTreeItemId selection = m_Tree->GetSelection();

	GameObject *obj = ScenesHolder::GetInstance().GetCurrentScene()->GetGameObject(GetGameObjectID(selection));

	if (obj)
		ScenesHolder::GetInstance().GetCurrentScene()->RemoveGameObject(obj);
	//obj->GetParent()->RemoveChild(obj);
}


//void SceneControl::AddGPUParameters(const Ogre::String &Category, Ogre::Pass *Pass, const Ogre::GpuNamedConstants &Params)
//{
//	Properties &prop = Properties::GetInstance();
//
//	prop.AddCategory(Category);
//
//	Ogre::GpuConstantDefinitionIterator::const_iterator it = Params.map.begin();
//	while (true)
//	{
//		try
//		{
//			it++;
//		}
//		catch (...)
//		{
//			continue;
//		}
//
//		if (!it->first.size())
//			break;
//
//		if (!Ogre::StringUtil::endsWith(it->first, "[0]", false))
//		{
//			Ogre::Vector4 value;
//			Pass->getFragmentProgramParameters()->_readRawConstants(it->second.physicalIndex, it->second.elementSize, value.ptr());
//
//			switch (it->second.constType)
//			{
//			case Ogre::GCT_FLOAT1:
//				{
//					prop.AddPropertyOfFloat(it->first, value.x);
//				} break;
//			case Ogre::GCT_FLOAT2:
//				{
//					prop.AddPropertyOfVector2(it->first, Ogre::Vector2(value.x, value.y));
//				} break;
//			case Ogre::GCT_FLOAT4:
//				{
//					prop.AddPropertyOfColor(it->first, Ogre::ColourValue(value.x, value.y, value.z, value.w));
//				} break;
//			}
//		}
//	}
//}


void SceneControl::AddChildSceneContent(wxTreeItemId Parent, GameObject *Object)
{
	if (Object->GetWasRemove())
		return;

	wxTreeItemId item = m_Tree->AppendItem(Parent, STRING_OGRE_TO_WX(Object->GetEditorText()));
	wxTreeItemId subID;

	BEGIN_LIST_CONST_ITERATOR(Object->GetChildren(), GameObject)
		AddChildSceneContent(item, current);
	END_LIST_CONST_ITERATOR

		//if (m_ShowEntites->GetValue())
		//{
		//	BEGIN_LIST_ITERATOR(Object->GetAttachedObjects(), Ogre::Entity*)
		//		subID = m_Tree->AppendItem(item, current->getName());

		//		for (unsigned int i = 0; i < current->getNumSubEntities(); i++)
		//		{
		//			Ogre::SubEntity *sub = current->getSubEntity(i);
		//			m_Tree->AppendItem(subID, (Ogre::String("Sub-Mesh-") + Ogre::StringConverter::toString(i)));
		//		}
		//	END_LIST_ITERATOR(NULL)
		//}
}


wxTreeItemId SceneControl::GetItem(const char *Text, wxTreeItemId &Parent)
{
	wxTreeItemIdValue cookie;
	wxTreeItemId current = m_Tree->GetFirstChild(Parent, cookie);
	wxTreeItemId child;

	while (current.IsOk())
	{
		if (Text == m_Tree->GetItemText(current))
			return current;

		if (m_Tree->ItemHasChildren(current))
		{
			wxTreeItemId search = GetItem(Text, current);

			if (search.IsOk())
				return search;
		}

		current = m_Tree->GetNextChild(Parent, cookie);
	}

	return wxTreeItemId();
}


void SceneControl::RefreshPropertyGrid(void)
{
	std::vector<Script*> scripts;

	Properties &prop = Properties::GetInstance();
	prop.Clear();

	if (m_LastSelectedGameObject)
	{
		prop.BeginNewProperties(ScenesHolder::GetInstance().GetCaption(), this);

		prop.AddCategory("General");

		prop.AddPropertyOfString("Name", m_LastSelectedGameObject->GetName(), "Name of game object");
		prop.AddPropertyOfInt("ID", m_LastSelectedGameObject->GetID(), "ID of game object\nWarning : This is a automatic value generated by engine, don't use it in scripts!");
		prop.SetReadOnly("ID", true);

		if (m_LastSelectedGameObject->GetParent() != ScenesHolder::GetInstance().GetCurrentScene()->GetRootGameObject())
		{
			prop.AddPropertyOfString("Parent", m_LastSelectedGameObject->GetParent()->GetName(), "Name of game object's parent", true);
		}

		prop.AddCategory("Transform");

		prop.AddPropertyOfBool("Locked", m_LastSelectedGameObject->GetLocked(), "Locking state of game object");
		prop.AddPropertyOfVector3("Position", m_LastSelectedGameObject->GetPosition(), "Position of game object");
		prop.AddPropertyOfVector3("Rotation", m_LastSelectedGameObject->GetRotation(), "Rotation of game object");
		prop.AddPropertyOfVector3("Scale", m_LastSelectedGameObject->GetScale(), "Scale of game object");
		//prop.AddPropertyOfQuaternion("Orientation", m_LastSelectedGameObject->GetOrientation(), "Orientation of game object");

		prop.AddCategory("Editing");

		GameObject::GameObjectType goType = m_LastSelectedGameObject->GetType();
		if (goType == GameObject::GOT_CAMERA)
		{
			CameraGameObject *camera = (CameraGameObject*)m_LastSelectedGameObject;

			prop.AddPropertyOfBool("Is Main", camera->GetIsMain());

			prop.AddPropertyOfEnumerator_PolygonMode("Polygon Mode", camera->GetPolygonMode());

			prop.AddPropertyOfFloat("LOD Bias", camera->GetLODBias(), "The level-of-detail bias factor currently applied to this camera and must be greather than 0");

			prop.AddPropertyOfFloat("FOV Y", camera->GetFOVY(), "Field Of View (FOV) is the angle made between the frustum's position, and the edges\nof the 'screen' onto which the scene is projected. High values (90+ degrees) result in a wide-angle,\nfish-eye kind of view, low values (30- degrees) in a stretched, telescopic kind of view. Typical values\nare between 45 and 60 degrees.\nThis value represents the VERTICAL field-of-view. The horizontal field of view is calculated from\nthis depending on the dimensions of the viewport (they will only be the same if the viewport is square).");

			prop.AddPropertyOfFloat("Far Clip Distance", camera->GetFarClipDistance(), "The distance from the frustum to the far clipping plane");
			prop.AddPropertyOfFloat("Near Clip Distance", camera->GetNearClipDistance(), "The position of the near clipping plane");

			//prop.AddPropertyOfFloat("Aspect Ratio", camera->GetAspectRatio());

			prop.AddPropertyOfEnumerator_ProjectionType("Projection Type", camera->GetProjectionType());

			camera = NULL;
		}
		else if (goType == GameObject::GOT_LIGHT)
		{
			LightGameObject *light = (LightGameObject*)m_LastSelectedGameObject;
			Light::LightType lType = light->GetLightType();

			prop.AddPropertyOfBool("Enabled", light->GetEnabled());

			prop.AddPropertyOfEnumerator_LightType("Type", lType);

			//prop.AddPropertyOfBool("Cast Shadows", light->GetCastShadows(), "", "LightCastShadows");

			prop.AddPropertyOfColor("Diffuse", light->GetDiffuseColour());
			prop.AddPropertyOfColor("Specular", light->GetSpecularColour());

			if (lType == Light::LT_DIRECTIONAL || lType == Light::LT_SPOTLIGHT)
			{
				prop.AddPropertyOfVector3("Direction", light->GetDirection());
			}
			
			if (lType == Light::LT_SPOTLIGHT)
			{
				prop.AddPropertyOfFloat("Inner Angle", light->GetSpotlightInnerAngle());
				prop.AddPropertyOfFloat("Outer Angle", light->GetSpotlightOuterAngle());
				prop.AddPropertyOfFloat("Falloff", light->GetSpotlightFalloff());
			}

			if (lType != Light::LT_DIRECTIONAL)
			{
				prop.AddPropertyOfFloat("Attenuation Range", light->GetAttenuationRange());
				prop.AddPropertyOfFloat("Attenuation Constant", light->GetAttenuationConstant());
				prop.AddPropertyOfFloat("Attenuation Linear", light->GetAttenuationLinear());
				prop.AddPropertyOfFloat("Attenuation Quadric", light->GetAttenuationQuadric());
			}

			//prop.AddPropertyOfFloat("Power Scale", light->GetPowerScale());

			light = NULL;
		}
		else if (goType == GameObject::GOT_EMPTY || goType == GameObject::GOT_MESH/* || goType == GameObject::GOT_TERRAIN*/ || goType == GameObject::GOT_OCEAN || goType == GameObject::GOT_PARTICLE)
		{
			prop.AddPropertyOfBool("Visible", m_LastSelectedGameObject->GetVisible(), "Visiblity of game object");
		}


		if (goType == GameObject::GOT_MESH)
		{
			prop.AddPropertyOfBool("Cast Shadows", ((MeshGameObject*)m_LastSelectedGameObject)->GetCastShadows(), "Whether shadow casting is enabled for this object", "MeshCastShadows");
		}
		else if (goType == GameObject::GOT_TERRAIN)
		{
			prop.AddPropertyOfString("Navigation Mesh File Name", ((TerrainGameObject*)m_LastSelectedGameObject)->GetNavMeshFileName());
		}
		else if (goType == GameObject::GOT_OCEAN)
		{
			OceanGameObject *ocean = (OceanGameObject*)m_LastSelectedGameObject;

			prop.AddCategory("Module");
			prop.AddPropertyOfEnumerator_ModuleType("Module Type", ocean->GetModuleType());

			switch (ocean->GetModuleType())
			{
			case OceanGameObject::MT_SIMPLE_GRID:
				{
					prop.AddPropertyOfInt("Complexity", ocean->GetSimpleGridOption().Complexity);
					prop.AddPropertyOfVector2("Mesh Size", Ogre::Vector2(ocean->GetSimpleGridOption().MeshSize.Width, ocean->GetSimpleGridOption().MeshSize.Height));
					prop.AddPropertyOfInt("Strength", ocean->GetSimpleGridOption().Strength);
					prop.AddPropertyOfBool("Smooth", ocean->GetSimpleGridOption().Smooth);
					prop.AddPropertyOfBool("Choppy Waves", ocean->GetSimpleGridOption().ChoppyWaves);
					prop.AddPropertyOfFloat("Choppy Strength", ocean->GetSimpleGridOption().ChoppyStrength);
				} break;

			case OceanGameObject::MT_RADIAL_GRID:
				{
					prop.AddPropertyOfInt("Steps", ocean->GetRadialGridOption().Steps);
					prop.AddPropertyOfInt("Circles", ocean->GetRadialGridOption().Circles);
					prop.AddPropertyOfFloat("Radius", ocean->GetRadialGridOption().Radius);
					prop.AddPropertyOfBool("Smooth", ocean->GetRadialGridOption().Smooth);
					prop.AddPropertyOfBool("Choppy Waves", ocean->GetRadialGridOption().ChoppyWaves);
					prop.AddPropertyOfFloat("Choppy Strength", ocean->GetRadialGridOption().ChoppyStrength);
					prop.AddPropertyOfFloat("Step Cube Size", ocean->GetRadialGridOption().StepSizeCube);
					prop.AddPropertyOfFloat("Step Size Five", ocean->GetRadialGridOption().StepSizeFive);
					prop.AddPropertyOfFloat("Step Lin Size", ocean->GetRadialGridOption().StepSizeLin);
					prop.AddPropertyOfFloat("Strength", ocean->GetRadialGridOption().Strength);
				} break;

			case OceanGameObject::MT_PROJECTED_GRID:
				{
					prop.AddPropertyOfInt("Complexity", ocean->GetProjectedGridOption().Complexity);
					prop.AddPropertyOfFloat("Strength", ocean->GetProjectedGridOption().Strength);
					prop.AddPropertyOfFloat("Elevation", ocean->GetProjectedGridOption().Elevation);
					prop.AddPropertyOfBool("Smooth", ocean->GetProjectedGridOption().Smooth);
					prop.AddPropertyOfBool("Force Recalculate Geometry", ocean->GetProjectedGridOption().ForceRecalculateGeometry);
					prop.AddPropertyOfBool("ChoppyWaves", ocean->GetProjectedGridOption().ChoppyWaves);
					prop.AddPropertyOfFloat("Choppy Strength", ocean->GetProjectedGridOption().ChoppyStrength);
				} break;
			}

			prop.AddCategory("Noise");
			prop.AddPropertyOfEnumerator_NoiseType("Noise Type", ocean->GetNoiseType());

			switch (ocean->GetNoiseType())
			{
			case OceanGameObject::NT_PERLIN:
				{
					prop.AddPropertyOfInt("Octaves", ocean->GetPerlinOption().Octaves);
					prop.AddPropertyOfFloat("Scale", ocean->GetPerlinOption().Scale, "", "NoiseScale");
					prop.AddPropertyOfFloat("Falloff", ocean->GetPerlinOption().Falloff);
					prop.AddPropertyOfFloat("Animation Speed", ocean->GetPerlinOption().Animspeed);
					prop.AddPropertyOfFloat("Time Multi", ocean->GetPerlinOption().Timemulti);
				} break;

			case OceanGameObject::NT_FFT:
				{
					prop.AddPropertyOfInt("Resolution", ocean->GetFFTOption().Resolution, "2^n");
					prop.AddPropertyOfFloat("Physical Resolution", ocean->GetFFTOption().PhysicalResolution);
					prop.AddPropertyOfFloat("Scale", ocean->GetFFTOption().Scale, "", "NoiseScale");
					prop.AddPropertyOfVector2("Wind Direction", ocean->GetFFTOption().WindDirection);
					prop.AddPropertyOfFloat("Animation Speed", ocean->GetFFTOption().AnimationSpeed);
					prop.AddPropertyOfFloat("Power", ocean->GetFFTOption().KwPower);
					prop.AddPropertyOfFloat("Amplitude", ocean->GetFFTOption().Amplitude);
				} break;
			}

			prop.AddCategory("Global");
			prop.AddPropertyOfEnumerator_NormalMode("Normal Mode", ocean->GetNormalMode());
			prop.AddPropertyOfEnumerator_PolygonMode("Polygon Mode", ocean->GetPolygonMode());
			prop.AddPropertyOfEnumerator_ShaderMode("Shader Mode", ocean->GetShaderMode());
			prop.AddPropertyOfFloat("Full Reflection Distance", ocean->GetFullReflectionDistance());
			prop.AddPropertyOfFloat("Global Transparency", ocean->GetGlobalTransparency());
			prop.AddPropertyOfColor("Water Color", ocean->GetWaterColor());
			prop.AddPropertyOfFloat("Normal Distortion", ocean->GetNormalDistortion());
			prop.AddPropertyOfFloat("Depth Limit", ocean->GetDepthLimit());

			prop.AddCategory("Components");
			prop.AddPropertyOfBool("Sun", ocean->IsComponentEnabled(Hydrax::HYDRAX_COMPONENT_SUN));
			prop.AddPropertyOfBool("Foam", ocean->IsComponentEnabled(Hydrax::HYDRAX_COMPONENT_FOAM));
			prop.AddPropertyOfBool("Depth", ocean->IsComponentEnabled(Hydrax::HYDRAX_COMPONENT_DEPTH));
			prop.AddPropertyOfBool("Smooth", ocean->IsComponentEnabled(Hydrax::HYDRAX_COMPONENT_SMOOTH), "", "ComponentSmooth");
			prop.AddPropertyOfBool("Caustics", ocean->IsComponentEnabled(Hydrax::HYDRAX_COMPONENT_CAUSTICS));
			prop.AddPropertyOfBool("Under Water", ocean->IsComponentEnabled(Hydrax::HYDRAX_COMPONENT_UNDERWATER));
			prop.AddPropertyOfBool("Under Water reflections", ocean->IsComponentEnabled(Hydrax::HYDRAX_COMPONENT_UNDERWATER_REFLECTIONS));
			prop.AddPropertyOfBool("Under Water God Rays", ocean->IsComponentEnabled(Hydrax::HYDRAX_COMPONENT_UNDERWATER_GODRAYS));

			prop.AddCategory("Foam");
			prop.AddPropertyOfFloat("Foam Max Distance", ocean->GetFoamMaxDistance());
			prop.AddPropertyOfFloat("Foam Scale", ocean->GetFoamScale());
			prop.AddPropertyOfFloat("Foam Start", ocean->GetFoamStart());
			prop.AddPropertyOfFloat("Foam Transparency", ocean->GetFoamTransparency());

			prop.AddCategory("Caustics");
			prop.AddPropertyOfFloat("Smooth Power", ocean->GetSmoothPower());
			prop.AddPropertyOfFloat("Caustics Scale", ocean->GetCausticsScale());
			prop.AddPropertyOfFloat("Caustics Power", ocean->GetCausticsPower());
			prop.AddPropertyOfFloat("Caustics End", ocean->GetCausticsEnd());

			prop.AddCategory("Under Water");
			prop.AddPropertyOfColor("God Rays Exposure", ocean->GetGodRaysExposure());
			prop.AddPropertyOfFloat("God Rays Intensity", ocean->GetGodRaysIntensity());
			prop.AddPropertyOfFloat("Underwater Camera Switch Delta", ocean->GetUnderwaterCameraSwitchDelta());
		}
		else if (goType == GameObject::GOT_PARTICLE)
		{
			prop.AddPropertyOfBool("Enabled", ((ParticleGameObject*)m_LastSelectedGameObject)->GetEnabled());
			prop.AddPropertyOfString("Template", ((ParticleGameObject*)m_LastSelectedGameObject)->GetParticleTemplate());
		}

		scripts = m_LastSelectedGameObject->GetScripts();

		if (goType == GameObject::GOT_MESH)
		{
			prop.AddCategory("Animations");

			Ogre::AnimationStateSet *set = ((MeshGameObject*)m_LastSelectedGameObject)->GetAllAnimationStates();

			if (set)
			{
				Ogre::AnimationStateIterator it = set->getAnimationStateIterator();
				Ogre::AnimationState *state = NULL;

				while (it.hasMoreElements())
				{
					state = it.getNext();
					prop.AddPropertyOfBool(state->getAnimationName(), state->getEnabled(), "", "Animations|" + state->getAnimationName());
					//prop.AddPropertyOfBool(state->getAnimationName(), state->get(), "", "Animations|" + state->getAnimationName());
				}

				state = NULL;
				set = NULL;
			}
		}
	}
	else
	{
		EditorScene *scene = ScenesHolder::GetInstance().GetCurrentScene();

		prop.BeginNewProperties("Scene", this);

		prop.AddCategory("General");

		prop.AddPropertyOfString("Name", scene->GetName());
		prop.AddPropertyOfColor("Clear Color", ViewPortsManager::GetInstance().GetEditRenderControl()->GetViewport()->getBackgroundColour());

		prop.AddCategory("Lighting");

		prop.AddPropertyOfEnumerator_RenderMode("Render Mode", scene->GetDeferredShadingSystem()->GetRenderMode());

		prop.AddPropertyOfColor("Ambient Light", scene->GetSceneManager()->getAmbientLight());

		prop.AddCategory("Shadow");
		//prop.AddPropertyOfBool("Show Debug Shadows", scene->GetShadowManager()->);--------------------------------------------------------------------
		prop.AddPropertyOfEnumerator_ShadowType("Shadow Type", scene->GetShadowManager()->GetShadowType());

		switch(scene->GetShadowManager()->GetShadowType())
		{
		case ShadowManager::ST_STENCIL:
			{
				prop.AddPropertyOfColor("Shadow Color", scene->GetShadowManager()->GetShadowColor());
				prop.AddPropertyOfFloat("Shadow Directional Light Extrusion Distance", scene->GetShadowManager()->GetShadowDirectionalLightExtrusionDistance());
				prop.AddPropertyOfFloat("Shadow Far Distance",  scene->GetShadowManager()->GetShadowFarDistance());
				prop.AddPropertyOfInt("Shadow Index Buffer Size", scene->GetShadowManager()->GetShadowIndexBufferSize());
				prop.AddPropertyOfBool("Shadow Use Infinite Far Plane", scene->GetShadowManager()->GetShadowUseInfiniteFarPlane());
				prop.AddPropertyOfBool("Shadow Use Light Clip Planes", scene->GetShadowManager()->GetShadowUseLightClipPlanes());
			} break;

		case ShadowManager::ST_TEXTURE:
			{
				prop.AddPropertyOfEnumerator_ShadowCameraSetupType("Shadow Camera Setup Type", scene->GetShadowManager()->GetShadowCameraSetupType());
				prop.AddPropertyOfColor("Shadow Color", scene->GetShadowManager()->GetShadowColor());
				prop.AddPropertyOfFloat("Shadow Directional Light Extrusion Distance", scene->GetShadowManager()->GetShadowDirectionalLightExtrusionDistance());
				prop.AddPropertyOfFloat("Shadow Far Distance",  scene->GetShadowManager()->GetShadowFarDistance());
				prop.AddPropertyOfInt("Shadow Index Buffer Size", scene->GetShadowManager()->GetShadowIndexBufferSize());
				prop.AddPropertyOfInt("Shadow Texture Size", scene->GetShadowManager()->GetShadowTextureSize());
				prop.AddPropertyOfFloat("Shadow Directional Light Texture Offset", scene->GetShadowManager()->GetShadowDirLightTextureOffset());
				prop.AddPropertyOfBool("Shadow Texture Self Shadow", scene->GetShadowManager()->GetShadowTextureSelfShadow());
				prop.AddPropertyOfBool("Shadow Caster Render Back Faces", scene->GetShadowManager()->GetShadowCasterRenderBackFaces());
				prop.AddPropertyOfInt("Shadow Texture Count", scene->GetShadowManager()->GetShadowTextureCount());
				prop.AddPropertyOfFloat("Shadow Texture FadeEnd", scene->GetShadowManager()->GetShadowTextureFadeEnd());
				prop.AddPropertyOfFloat("Shadow Texture FadeStart", scene->GetShadowManager()->GetShadowTextureFadeStart());
				prop.AddPropertyOfBool("Shadow Use Light Clip Planes", scene->GetShadowManager()->GetShadowUseLightClipPlanes());
			} break;
		}


		prop.AddCategory("Sky");
		prop.AddPropertyOfEnumerator_SkyMode("Sky Mode", scene->GetSkyMode());

		switch (scene->GetSkyMode())
		{
		case Scene::SM_SKY_BOX:
			{
				prop.AddPropertyOfBool("Sky Box Enabled", scene->GetSkyBoxEnabled());
				prop.AddPropertyOfString("Sky Box Material", scene->GetSkyBoxMaterialName());
				prop.AddPropertyOfFloat("Sky Box Distance", scene->GetSkyBoxDistance());
			} break;

		case Scene::SM_SKY_DOME:
			{
				prop.AddPropertyOfBool("Sky Dome Enabled", scene->GetSkyDomeEnabled());
				prop.AddPropertyOfString("Sky Dome Material", scene->GetSkyDomeMaterialName());
				prop.AddPropertyOfFloat("Sky Dome Curvature", scene->GetSkyDomeCurvature());
				prop.AddPropertyOfFloat("Sky Dome Tiling", scene->GetSkyDomeTiling());
				prop.AddPropertyOfFloat("Sky Dome Distance", scene->GetSkyDomeDistance());
			} break;

		case Scene::SM_ADVANCED_SKY:
			{
				AtmosphereManager *am =  scene->GetAtmosphereManager();

				prop.AddPropertyOfVector2("Time Of Day", am->GetTimeOfDay());
				prop.AddPropertyOfVector2("Time Of Sunrise", am->GetTimeOfSunrise());
				prop.AddPropertyOfVector2("Time Of Sunset", am->GetTimeOfSunset());
				prop.AddPropertyOfVector2("East Direction", am->GetEastDirection());

				prop.AddPropertyOfVector3("Sun Direction", am->GetSunDirection());
				prop.SetReadOnly("Sun Direction", true);
				prop.AddPropertyOfVector3("Sun Position", am->GetSunPosition(), "Sunp");
				prop.SetReadOnly("Sun Position", true);
				prop.AddPropertyOfVector3("Moon Direction", am->GetMoonDirection());
				prop.SetReadOnly("Moon Direction", true);

				prop.AddPropertyOfFloat("Moon Phase", am->GetMoonPhase());
				prop.AddPropertyOfFloat("Time Multiplier", am->GetTimeMultiplier());
				prop.AddPropertyOfEnumerator_LightingMode("Lighting Mode", am->GetLightingMode());
				prop.AddPropertyOfBool("Starfield Enabled", am->GetStarfieldEnabled());
				prop.AddPropertyOfFloat("Max Far Clip Distance", am->GetMaxFarClipDistance());

				prop.AddPropertyOfFloat("Inner Radius", am->GetAtmosphereManagerOption().InnerRadius);
				prop.AddPropertyOfFloat("Outer Radius", am->GetAtmosphereManagerOption().OuterRadius);
				prop.AddPropertyOfFloat("Height Position", am->GetAtmosphereManagerOption().HeightPosition, "Height position must be in [0, 1] range, 0 = Inner Radius, 1 = Outer Radius");
				prop.AddPropertyOfFloat("Rayleigh Multiplier", am->GetAtmosphereManagerOption().RayleighMultiplier);
				prop.AddPropertyOfFloat("Mie Multiplier", am->GetAtmosphereManagerOption().MieMultiplier);
				prop.AddPropertyOfFloat("Sun Intensity", am->GetAtmosphereManagerOption().SunIntensity);
				prop.AddPropertyOfColor("Wave Length", Ogre::ColourValue(am->GetAtmosphereManagerOption().WaveLength.x, am->GetAtmosphereManagerOption().WaveLength.y, am->GetAtmosphereManagerOption().WaveLength.z));
				prop.AddPropertyOfFloat("Phase Function", am->GetAtmosphereManagerOption().G);
				prop.AddPropertyOfFloat("Exposure", am->GetAtmosphereManagerOption().Exposure);
				prop.AddPropertyOfInt("NumberOfSamples", am->GetAtmosphereManagerOption().NumberOfSamples);
			} break;
		}

		//prop.AddCategory("Sun");
		//prop.AddPropertyOfVector3("Sun Position", scene->GetSunPosition());
		//prop.AddPropertyOfFloat("Sun Strength", scene->GetSunStrength());
		//prop.AddPropertyOfFloat("Sun Area", scene->GetSunArea());
		//prop.AddPropertyOfColor("Sun Color", scene->GetSunColor());

		prop.AddCategory("Post-Process Effects");

		const std::vector<Ogre::String> &list = scene->GetPostProcessEffects();
		Ogre::CompositorManager::ResourceMapIterator it = Ogre::CompositorManager::getSingleton().getResourceIterator();
		bool hasIt = false;
		while (it.hasMoreElements())
		{
			hasIt = false;
			Ogre::ResourcePtr res = it.getNext();

			if (res->getName() == "Ogre/Scene")
				continue;

			BEGIN_VECTOR_ITERATOR(list, Ogre::String)
				if (current == res->getName())
				{
					hasIt = true;
					break;
				}
				END_VECTOR_ITERATOR("")

					prop.AddPropertyOfBool(res->getName(), hasIt, "", "PPE|" + res->getName());
		}

		scripts = scene->GetScripts();
	}

	prop.AddCategory("Scripts");

	BEGIN_VECTOR_ITERATOR(scripts, Script*)
		prop.AddPropertyOfString("Script " + Ogre::StringConverter::toString(i + 1), current->GetFilePath(), "", false, "Script|" + Ogre::StringConverter::toString(i));
	prop.AddPropertyOfBool("Enabled " + Ogre::StringConverter::toString(i + 1), current->GetEnabled(), "", "ScriptEnabled|" + Ogre::StringConverter::toString(i));
	END_VECTOR_ITERATOR(NULL)
		scripts.clear();

	prop.AddPropertyOfString("Add New", "", "", false, "NewScript");

	prop.EndNewProperties();
}


bool SceneControl::IsAnGameObject(const wxTreeItemId &ID)
{
	return !(ID == m_RootID || m_Tree->GetItemText(ID).Find(".mesh") != wxString::npos);
}


/*unsigned */int SceneControl::GetGameObjectID(const wxTreeItemId &ID)
{
	return Ogre::StringConverter::parseInt(Ogre::StringUtil::split(STRING_WX_TO_OGRE(m_Tree->GetItemText(ID)), "-")[0], -1);
}


GameObject *SceneControl::GetSelectedObject(void)
{
	const wxTreeItemId &selected = m_Tree->GetSelection();

	if (selected.IsOk())
	{
		int id = GetGameObjectID(selected);
		if (id == -1)
			return NULL;

		return ScenesHolder::GetInstance().GetCurrentScene()->GetGameObject(id);
	}

	return NULL;
}


void SceneControl::OnSize(wxSizeEvent &event)
{
	m_ToolBar->SetSize(event.GetSize().x, m_ToolBar->GetSize().y);
	m_Tree->SetSize(event.GetSize().x, event.GetSize().y - m_ToolBar->GetSize().y + 4);
}

void SceneControl::OnDoubleClick(wxTreeEvent &event)
{
	Editor::GetInstance().FocusGameObject();
}

void SceneControl::OnKey(wxTreeEvent &event)
{
	if (event.GetKeyCode() == WXK_DELETE)
	{
		RemoveSelectedItem();
	}
	if (event.GetKeyEvent().ControlDown())
	{
		if (IsAnGameObject(m_Tree->GetSelection()))
		{
			if (event.GetKeyCode() == 'X')
			{
				m_ItemToMove = m_Tree->GetSelection();
				m_CutItem = true;
			}
			else if (event.GetKeyCode() == 'C')
			{
				m_ItemToMove = m_Tree->GetSelection();
				m_CutItem = false;
			}
		}

		if (event.GetKeyCode() == 'V' && m_ItemToMove.IsOk())
		{
			wxTreeItemId &current = m_Tree->GetSelection();

			if (current.IsOk())
			{
				GameObject *toMove = ScenesHolder::GetInstance().GetCurrentScene()->GetGameObject(GetGameObjectID(m_ItemToMove));

				if (toMove)
				{
					GameObject *parent = GetSelectedObject();

					if (!parent)
						parent = ScenesHolder::GetInstance().GetCurrentScene()->GetRootGameObject();

					if (m_CutItem)
					{
						if (current != m_ItemToMove)
						{
							toMove->GetParent()->RemoveChild(toMove);
							toMove->SetParent(NULL);
							parent->AddChild(toMove);

							m_ItemToMove = NULL;
						}
					}
					else
					{
						toMove->Clone(toMove->GetName() + " Copy", parent);
					}

					m_CutItem = false;
				}
			}

			ReloadContent();
		}
	}
}


void SceneControl::OnSelectedItem(wxTreeEvent &event)
{
	const wxTreeItemId &id = m_Tree->GetSelection();

	m_LastSelectedGameObject = NULL;

	if (IsAnGameObject(id))
	{
		m_LastSelectedGameObject = GetSelectedObject();

		ScenesHolder::GetInstance().GetCurrentScene()->SelectObject(m_LastSelectedGameObject->GetID());
	}

	RefreshPropertyGrid();
}


//void SceneControl::OnBeginDrag(wxTreeEvent &event)
//{
//	if (!IsAnGameObject(event.GetItem()))
//	{
//		return;
//	}
//
//	m_DragItem = event.GetItem();
//	event.Allow();
//}


//void SceneControl::OnEngDrag(wxTreeEvent &event)
//{
//	event.Skip();
//}


void SceneControl::OnPropertyChanged(/*wxPropertyGridManager *Manager, */const wxPropertyGridEvent &event)
{
	Scriptable *scriptHolder = NULL;
	const wxString &name = event.GetPropertyName();

#pragma region GameObject
	if (m_LastSelectedGameObject)
	{
		if (name == "Name")
		{
			m_LastSelectedGameObject->SetName(STRING_WX_TO_OGRE(event.GetProperty()->GetValueAsString()));

			ReloadContent();
		}
		else if (name == "Position")
		{
			m_LastSelectedGameObject->SetPosition(Vector3FromVariant(event.GetProperty()->GetValue()));
		}
		else if (name == "Rotation")
		{
			m_LastSelectedGameObject->SetRotation(Vector3FromVariant(event.GetProperty()->GetValue()));
		}
		else if (name == "Scale")
		{
			m_LastSelectedGameObject->SetScale(Vector3FromVariant(event.GetProperty()->GetValue()));
		}
		//else if (name == "Orientation")
		//{
		//	m_LastSelectedGameObject->SetOrientation(QuaternionFromVariant(event.GetProperty()->GetValue()));
		//}
		else if (name == "Visible")
		{
			//m_LastSelectedGameObject->SetVisible((bool)((wxBoolProperty*)event.GetProperty())->GetCurrentChoice()->GetValue());
			m_LastSelectedGameObject->SetVisible(event.GetPropertyValue().GetBool());
		}
		else if (name == "Locked")
		{
			//m_LastSelectedGameObject->SetLocked((bool)((wxBoolProperty*)event.GetProperty())->GetCurrentChoice()->GetValue());
			m_LastSelectedGameObject->SetLocked(event.GetPropertyValue().GetBool());
		}

		GameObject::GameObjectType goType = m_LastSelectedGameObject->GetType();
		if (goType == GameObject::GOT_MESH)
		{
			MeshGameObject *mesh = (MeshGameObject*)m_LastSelectedGameObject;
			if (name == "MeshCastShadows")
			{
				mesh->SetCastShadows(event.GetPropertyValue().GetBool());
			}
			if (name.StartsWith("Animation"))
			{
				mesh->GetAnimationState(Ogre::StringUtil::split(STRING_WX_TO_OGRE(name), "|")[1])->setEnabled(event.GetPropertyValue().GetBool());
			} 
		}
		else if (goType == GameObject::GOT_CAMERA)
		{
			CameraGameObject *camera = (CameraGameObject*)m_LastSelectedGameObject;
			camera->BeginChangeProperties();

			if (name == "Is Main")
			{
				bool value = event.GetPropertyValue().GetBool();

				if (value)
				{
					BEGIN_LIST_ITERATOR(ScenesHolder::GetInstance().GetCurrentScene()->GetCameras(), CameraGameObject*)
						if (current != camera)
							current->SetIsMain(false);
					END_LIST_ITERATOR(NULL)

						camera->SetIsMain(true);
				}
			}
			else if (name == "Polygon Mode")
			{
				camera->SetPolygonMode((Ogre::PolygonMode)event.GetPropertyValue().GetInteger());
			}
			else if (name == "LOD Bias")
			{
				camera->SetLODBias(event.GetPropertyValue().GetDouble());
			}
			else if (name == "FOV Y")
			{
				camera->SetFOVY(event.GetPropertyValue().GetDouble());
			}
			else if (name == "Far Clip Distance")
			{
				camera->SetFarClipDistance(event.GetPropertyValue().GetDouble());
			}
			else if (name == "Near Clip Distance")
			{
				camera->SetNearClipDistance(event.GetPropertyValue().GetDouble());
			}
			//else if (name == "Aspect Ratio")
			//{
			//	camera->SetAspectRatio(event.GetPropertyValue().GetDouble());
			//}
			else if (name == "Projection Type")
			{
				camera->SetProjectionType((Ogre::ProjectionType)event.GetPropertyValue().GetInteger());
			}

			camera->EndChangeProperties();
			camera = NULL;
		}
		else if (goType == GameObject::GOT_LIGHT)
		{
			LightGameObject *light = (LightGameObject*)m_LastSelectedGameObject;

			if (name == "Enabled")
			{
				light->SetEnabled(event.GetPropertyValue().GetBool());
			}
			else if (name == "Type")
			{
				light->SetLightType((Light::LightType)event.GetPropertyValue().GetInteger());
				RefreshPropertyGrid();
			}
			else if (name == "LightCastShadows")
			{
				//light->SetCastShadows(event.GetPropertyValue().GetBool());
			}
			else if (name == "Direction")
			{
				light->SetDirection(Vector3FromVariant(event.GetProperty()->GetValue()));
			}
			else if (name == "Inner Angle")
			{
				light->SetSpotlightInnerAngle(event.GetPropertyValue().GetDouble());
			}
			else if (name == "Outer Angle")
			{
				light->SetSpotlightOuterAngle(event.GetPropertyValue().GetDouble());
			}
			else if (name == "Falloff")
			{
				light->SetSpotlightFalloff(event.GetPropertyValue().GetDouble());
			}
			else if (name == "Diffuse")
			{
				light->SetDiffuseColour(Properties::GetColorFromValue(event.GetPropertyValue()));
			}
			else if (name == "Specular")
			{
				light->SetSpecularColour(Properties::GetColorFromValue(event.GetPropertyValue()));
			}
			else if (name == "Attenuation Range")
			{
				light->SetAttenuationRange(event.GetPropertyValue().GetDouble());
			}
			else if (name == "Attenuation Constant")
			{
				light->SetAttenuationConstant(event.GetPropertyValue().GetDouble());
			}
			else if (name == "Attenuation Linear")
			{
				light->SetAttenuationLinear(event.GetPropertyValue().GetDouble());
			}
			else if (name == "Attenuation Quadric")
			{
				light->SetAttenuationQuadric(event.GetPropertyValue().GetDouble());
			}
			else if (name == "Power Scale")
			{
				//light->SetPowerScale(event.GetPropertyValue().GetDouble());
			}

			light = NULL;
		}
		else if (goType == GameObject::GOT_TERRAIN)
		{
			if (name == "Navigation Mesh File Name")
			{
				((TerrainGameObject*)m_LastSelectedGameObject)->SetNavMeshFileName(STRING_WX_TO_OGRE(event.GetPropertyValue().GetString()));
			}
		}
		else if (goType == GameObject::GOT_OCEAN)
		{
			OceanGameObject *ocean = (OceanGameObject*)m_LastSelectedGameObject;

			bool updateModule = true;
			bool updateNoise = true;

			switch (ocean->GetModuleType())
			{
			case OceanGameObject::MT_SIMPLE_GRID:
				{
					if (name == "Complexity")
					{
						ocean->GetSimpleGridOption().Complexity = event.GetPropertyValue().GetInteger();
					}
					else if (name == "Mesh Size")
					{
						ocean->GetSimpleGridOption().MeshSize.setSize(Vector2FromVariant(event.GetProperty()->GetValue()).x, Vector3FromVariant(event.GetProperty()->GetValue()).y);
					}
					else if (name == "Strength")
					{
						ocean->GetSimpleGridOption().Strength = event.GetPropertyValue().GetInteger();
					}
					else if (name == "Smooth")
					{
						ocean->GetSimpleGridOption().Smooth = event.GetPropertyValue().GetBool();
					}
					else if (name == "Choppy Waves")
					{
						ocean->GetSimpleGridOption().ChoppyWaves = event.GetPropertyValue().GetBool();
					}
					else if (name == "Choppy Strength")
					{
						ocean->GetSimpleGridOption().ChoppyStrength = event.GetPropertyValue().GetDouble();
					}
					else
					{
						updateModule = false;
					}
				} break;

			case OceanGameObject::MT_RADIAL_GRID:
				{
					if (name == "Steps")
					{
						ocean->GetRadialGridOption().Steps = event.GetPropertyValue().GetInteger();
					}
					else if (name == "Circles")
					{
						ocean->GetRadialGridOption().Circles = event.GetPropertyValue().GetInteger();
					}
					else if (name == "Radius")
					{
						ocean->GetRadialGridOption().Radius = event.GetPropertyValue().GetDouble();
					}
					else if (name == "Smooth")
					{
						ocean->GetRadialGridOption().Smooth = event.GetPropertyValue().GetBool();
					}
					else if (name == "Choppy Waves")
					{
						ocean->GetRadialGridOption().ChoppyWaves = event.GetPropertyValue().GetBool();
					}
					else if (name == "Choppy Strength")
					{
						ocean->GetRadialGridOption().ChoppyStrength = event.GetPropertyValue().GetDouble();
					}
					else if (name == "Step Cube Size")
					{
						ocean->GetRadialGridOption().StepSizeCube = event.GetPropertyValue().GetDouble();
					}
					else if (name == "Step Size Five")
					{
						ocean->GetRadialGridOption().StepSizeFive = event.GetPropertyValue().GetDouble();
					}
					else if (name == "Step Lin Size")
					{
						ocean->GetRadialGridOption().StepSizeLin = event.GetPropertyValue().GetDouble();
					}
					else if (name == "Strength")
					{
						ocean->GetRadialGridOption().Strength = event.GetPropertyValue().GetDouble();
					}
					else
					{
						updateModule = false;
					}
				} break;

			case OceanGameObject::MT_PROJECTED_GRID:
				{
					if (name == "Complexity")
					{
						ocean->GetProjectedGridOption().Complexity = event.GetPropertyValue().GetInteger();
					}
					else if (name == "Strength")
					{
						ocean->GetProjectedGridOption().Strength = event.GetPropertyValue().GetDouble();
					}
					else if (name == "Elevation")
					{
						ocean->GetProjectedGridOption().Elevation = event.GetPropertyValue().GetDouble();
					}
					else if (name == "Smooth")
					{
						ocean->GetProjectedGridOption().Smooth = event.GetPropertyValue().GetBool();
					}
					else if (name == "Force Recalculate Geometry")
					{
						ocean->GetProjectedGridOption().ForceRecalculateGeometry = event.GetPropertyValue().GetBool();
					}
					else if (name == "Choppy Waves")
					{
						ocean->GetProjectedGridOption().ChoppyWaves = event.GetPropertyValue().GetBool();
					}
					else if (name == "Choppy Strength")
					{
						ocean->GetProjectedGridOption().ChoppyStrength = event.GetPropertyValue().GetDouble();
					}
					else
					{
						updateModule = false;
					}
				} break;
			}

			switch (ocean->GetNoiseType())
			{
			case OceanGameObject::NT_PERLIN:
				{
					if (name == "Octaves")
					{
						ocean->GetPerlinOption().Octaves = event.GetPropertyValue().GetInteger();
					}
					else if (name == "NoiseScale")
					{
						ocean->GetPerlinOption().Scale = event.GetPropertyValue().GetDouble();
					}
					else if (name == "Falloff")
					{
						ocean->GetPerlinOption().Falloff = event.GetPropertyValue().GetDouble();
					}
					else if (name == "Animation Speed")
					{
						ocean->GetPerlinOption().Animspeed = event.GetPropertyValue().GetDouble();
					}
					else if (name == "Time Multi")
					{
						ocean->GetPerlinOption().Timemulti = event.GetPropertyValue().GetDouble();
					}
					else
					{
						updateNoise = false;
					}
				} break;

			case OceanGameObject::NT_FFT:
				{
					if (name == "Resolution")
					{
						ocean->GetFFTOption().Resolution = event.GetPropertyValue().GetInteger();
					}
					else if (name == "Physical Resolution")
					{
						ocean->GetFFTOption().PhysicalResolution = event.GetPropertyValue().GetDouble();
					}
					else if (name == "Noise Scale")
					{
						ocean->GetFFTOption().Scale = event.GetPropertyValue().GetDouble();
					}
					else if (name == "Wind Direction")
					{
						ocean->GetFFTOption().WindDirection = Vector2FromVariant(event.GetProperty()->GetValue());
					}
					else if (name == "Animation Speed")
					{
						ocean->GetFFTOption().AnimationSpeed = event.GetPropertyValue().GetDouble();
					}
					else if (name == "Power")
					{
						ocean->GetFFTOption().KwPower = event.GetPropertyValue().GetDouble();
					}
					else if (name == "Amplitude")
					{
						ocean->GetFFTOption().Amplitude = event.GetPropertyValue().GetDouble();
					}
					else
					{
						updateNoise = false;
					}
				} break;
			}

			if (updateModule)
			{
				ocean->UpdateModuleOptionChanges();
				return;
			}
			if (updateNoise)
			{
				ocean->UpdateNoiseOptionChanges();
				return;
			}

			else if (name == "Module Type")
			{
				ocean->SetModuleType((OceanGameObject::ModuleType)event.GetPropertyValue().GetInteger());
				RefreshPropertyGrid();
			}
			else if (name == "Noise Type")
			{
				ocean->SetNoiseType((OceanGameObject::NoiseType)event.GetPropertyValue().GetInteger());
				RefreshPropertyGrid();
			}
			else if (name == "Normal Mode")
			{
				ocean->SetNormalMode((Hydrax::MaterialManager::NormalMode)event.GetPropertyValue().GetInteger());
			}
			else if (name == "Polygon Mode")
			{
				ocean->SetPolygonMode((Ogre::PolygonMode)event.GetPropertyValue().GetInteger());
			}
			else if (name == "Shader Mode")
			{
				ocean->SetShaderMode((Hydrax::MaterialManager::ShaderMode)event.GetPropertyValue().GetInteger());
			}
			else if (name == "Full Reflection Distance")
			{
				ocean->SetFullReflectionDistance(event.GetPropertyValue().GetDouble());
			}
			else if (name == "Global Transparency")
			{
				ocean->SetGlobalTransparency(event.GetPropertyValue().GetDouble());
			}
			else if (name == "Water Color")
			{
				ocean->SetWaterColor(Properties::GetColorFromValue(event.GetPropertyValue()));
			}
			else if (name == "Normal Distortion")
			{
				ocean->SetNormalDistortion(event.GetPropertyValue().GetDouble());
			}
			else if (name == "Depth Limit")
			{
				ocean->SetDepthLimit(event.GetPropertyValue().GetDouble());
			}
			else if (name == "Sun")
			{
				ocean->SetComponentEnabled(Hydrax::HYDRAX_COMPONENT_SUN, event.GetPropertyValue().GetBool());
			}
			else if (name == "Foam")
			{
				ocean->SetComponentEnabled(Hydrax::HYDRAX_COMPONENT_FOAM, event.GetPropertyValue().GetBool());
			}
			else if (name == "Depth")
			{
				ocean->SetComponentEnabled(Hydrax::HYDRAX_COMPONENT_DEPTH, event.GetPropertyValue().GetBool());
			}
			else if (name == "ComponentSmooth")
			{
				ocean->SetComponentEnabled(Hydrax::HYDRAX_COMPONENT_SMOOTH, event.GetPropertyValue().GetBool());
			}
			else if (name == "Caustics")
			{
				ocean->SetComponentEnabled(Hydrax::HYDRAX_COMPONENT_CAUSTICS, event.GetPropertyValue().GetBool());
			}
			else if (name == "Under Water")
			{
				ocean->SetComponentEnabled(Hydrax::HYDRAX_COMPONENT_UNDERWATER, event.GetPropertyValue().GetBool());
			}
			else if (name == "Under Water reflections")
			{
				ocean->SetComponentEnabled(Hydrax::HYDRAX_COMPONENT_UNDERWATER_REFLECTIONS, event.GetPropertyValue().GetBool());
			}
			else if (name == "Under Water God Rays")
			{
				ocean->SetComponentEnabled(Hydrax::HYDRAX_COMPONENT_UNDERWATER_GODRAYS, event.GetPropertyValue().GetBool());
			}
			else if (name == "Foam Max Distance")
			{
				ocean->SetFoamMaxDistance(event.GetPropertyValue().GetDouble());
			}
			else if (name == "Foam Scale")
			{
				ocean->SetFoamScale(event.GetPropertyValue().GetDouble());
			}
			else if (name == "Foam Start")
			{
				ocean->SetFoamStart(event.GetPropertyValue().GetDouble());
			}
			else if (name == "Foam Transparency")
			{
				ocean->SetFoamTransparency(event.GetPropertyValue().GetDouble());
			}
			else if (name == "Smooth Power")
			{
				ocean->SetSmoothPower(event.GetPropertyValue().GetDouble());
			}
			else if (name == "Caustics Scale")
			{
				ocean->SetCausticsScale(event.GetPropertyValue().GetDouble());
			}
			else if (name == "Caustics Power")
			{
				ocean->SetCausticsPower(event.GetPropertyValue().GetDouble());
			}
			else if (name == "Caustics End")
			{
				ocean->SetCausticsEnd(event.GetPropertyValue().GetDouble());
			}
			else if (name == "God Rays Exposure")
			{
				ocean->SetGodRaysExposure(Properties::GetColorFromValue(event.GetPropertyValue()));
			}
			else if (name == "God Rays Intensity")
			{
				ocean->SetGodRaysIntensity(event.GetPropertyValue().GetDouble());
			}
			else if (name == "Underwater Camera Switch Delta")
			{
				ocean->SetUnderwaterCameraSwitchDelta(event.GetPropertyValue().GetDouble());
			}
		}
		else if (goType == GameObject::GOT_PARTICLE)
		{
			if (name == "Enabled")
			{
				((ParticleGameObject*)m_LastSelectedGameObject)->SetEnabled(event.GetPropertyValue().GetBool());
			}
			else if (name == "Template")
			{
				((ParticleGameObject*)m_LastSelectedGameObject)->SetParticleTemplate(STRING_WX_TO_OGRE(event.GetPropertyValue().GetString()));
			}
		}

		scriptHolder = m_LastSelectedGameObject;
	}
#pragma endregion
#pragma region Scene
	else
	{
		EditorScene *scene = ScenesHolder::GetInstance().GetCurrentScene();
		AtmosphereManager *atmosphere = ScenesHolder::GetInstance().GetCurrentScene()->GetAtmosphereManager();

		if (name == "Name")
		{
			scene->SetName(STRING_WX_TO_OGRE(event.GetPropertyValue().GetString()));
		}
		else if (name == "Clear Color")
		{
			ViewPortsManager::GetInstance().GetEditRenderControl()->GetViewport()->setBackgroundColour(Properties::GetColorFromValue(event.GetPropertyValue()));
		}
		if (name == "Render Mode")
		{
			scene->GetDeferredShadingSystem()->SetRenderMode((DeferredShadingSystem::RenderMode)event.GetPropertyValue().GetInteger());
		}
		if (name == "Ambient Light")
		{
			scene->GetSceneManager()->setAmbientLight(Properties::GetColorFromValue(event.GetPropertyValue()));
		}
		else if (name == "Show Debug Shadows")
		{
			//scene->setShowDebugShadows(event.GetPropertyValue().GetBool());
		}
		else if (name == "Shadow Type")
		{
			scene->GetShadowManager()->SetShadowType((ShadowManager::ShadowType)event.GetPropertyValue().GetInteger());
			RefreshPropertyGrid();
		}
		else if (name == "Shadow Color")
		{
			scene->GetShadowManager()->SetShadowColor(Properties::GetColorFromValue(event.GetPropertyValue()));
		}
		else if (name == "Shadow Camera Setup Type")
		{
			scene->GetShadowManager()->SetShadowCameraSetupType((ShadowManager::ShadowCameraSetupType)event.GetPropertyValue().GetInteger());
		}
		else if (name == "Shadow Directional Light Extrusion Distance")
		{
			scene->GetShadowManager()->SetShadowDirectionalLightExtrusionDistance(event.GetPropertyValue().GetDouble());
		}
		else if (name == "Shadow Far Distance")
		{
			scene->GetShadowManager()->SetShadowFarDistance(event.GetPropertyValue().GetDouble());
		}
		else if (name == "Shadow Index Buffer Size")
		{
			scene->GetShadowManager()->SetShadowIndexBufferSize(event.GetPropertyValue().GetInteger());
		}
		else if (name == "Shadow Texture Size")
		{
			scene->GetShadowManager()->SetShadowTextureSize(event.GetPropertyValue().GetInteger());
		}
		else if (name == "Shadow Directional Light Texture Offset")
		{
			scene->GetShadowManager()->SetShadowDirLightTextureOffset(event.GetPropertyValue().GetDouble());
		}
		else if (name == "Shadow Texture Self Shadow")
		{
			scene->GetShadowManager()->SetShadowTextureSelfShadow(event.GetPropertyValue().GetBool());
		}
		else if (name == "Shadow Caster Render Back Faces")
		{
			scene->GetShadowManager()->SetShadowCasterRenderBackFaces(event.GetPropertyValue().GetBool());
		}
		else if (name == "Shadow Texture Count")
		{
			scene->GetShadowManager()->SetShadowTextureCount(event.GetPropertyValue().GetInteger());
		}
		else if (name == "Shadow Texture FadeEnd")
		{
			scene->GetShadowManager()->SetShadowTextureFadeEnd(event.GetPropertyValue().GetDouble());
		}
		else if (name == "Shadow Texture FadeStart")
		{
			scene->GetShadowManager()->SetShadowTextureFadeStart(event.GetPropertyValue().GetDouble());
		}
		else if (name == "Shadow Use Infinite Far Plane")
		{
			scene->GetShadowManager()->SetShadowUseInfiniteFarPlane(event.GetPropertyValue().GetBool());
		}
		else if (name == "Shadow Use Light Clip Planes")
		{
			scene->GetShadowManager()->SetShadowUseLightClipPlanes(event.GetPropertyValue().GetBool());
		}
		
		else if (name == "Sky Mode")
		{
			scene->SetSkyMode((Scene::SkyMode)event.GetPropertyValue().GetInteger());
			RefreshPropertyGrid();
		}

		else if (name == "Sky Box Enabled")
		{
			scene->SetSkyBoxEnabled(event.GetPropertyValue().GetBool());
		}
		else if (name == "Sky Box Material")
		{
			scene->SetSkyBoxMaterialName(STRING_WX_TO_OGRE(event.GetPropertyValue().GetString()));
		}
		else if (name == "Sky Box Distance")
		{
			scene->SetSkyBoxDistance(event.GetPropertyValue().GetDouble());
		}

		else if (name == "Sky Dome Enabled")
		{
			scene->SetSkyDomeEnabled(event.GetPropertyValue().GetBool());
		}
		else if (name == "Sky Dome Material")
		{
			scene->SetSkyDomeMaterialName(STRING_WX_TO_OGRE(event.GetPropertyValue().GetString()));
		}
		else if (name == "Sky Dome Curvature")
		{
			scene->SetSkyDomeCurvature(event.GetPropertyValue().GetDouble());
		}
		else if (name == "Sky Dome Tiling")
		{
			scene->SetSkyDomeTiling(event.GetPropertyValue().GetDouble());
		}
		else if (name == "Sky Dome Distance")
		{
			scene->SetSkyDomeDistance(event.GetPropertyValue().GetDouble());
		}

		else if (name == "Time Of Day")
		{
			atmosphere->SetTimeOfDay(Vector2FromVariant(event.GetProperty()->GetValue()));
		}
		else if (name == "Time Of Sunrise")
		{
			atmosphere->SetTimeOfSunrise(Vector2FromVariant(event.GetProperty()->GetValue()));
		}
		else if (name == "Time Of Sunset")
		{
			atmosphere->SetTimeOfSunset(Vector2FromVariant(event.GetProperty()->GetValue()));
		}
		else if (name == "East Direction")
		{
			atmosphere->SetEastDirection(Vector2FromVariant(event.GetProperty()->GetValue()));
		}
		else if (name == "Moon Phase")
		{
			atmosphere->SetMoonPhase(event.GetPropertyValue().GetDouble());
		}
		else if (name == "Time Multiplier")
		{
			atmosphere->SetTimeMultiplier(event.GetPropertyValue().GetDouble());
		}
		else if (name == "Lighting Mode")
		{
			atmosphere->SetLightingMode((SkyX::SkyX::LightingMode)event.GetPropertyValue().GetInteger());
		}
		else if (name == "Starfield Enabled")
		{
			atmosphere->SetStarfieldEnabled(event.GetPropertyValue().GetBool());
		}
		else if (name == "Max Far Clip Distance")
		{
			atmosphere->SetMaxFarClipDistance(event.GetPropertyValue().GetDouble());
		}

		else if (name == "Inner Radius")
		{
			atmosphere->GetAtmosphereManagerOption().InnerRadius = event.GetPropertyValue().GetDouble();
			atmosphere->UpdateAtmosphereManagerOptionChanges();
		}
		else if (name == "Outer Radius")
		{
			atmosphere->GetAtmosphereManagerOption().OuterRadius = event.GetPropertyValue().GetDouble();
			atmosphere->UpdateAtmosphereManagerOptionChanges();
		}
		else if (name == "Height Position")
		{
			atmosphere->GetAtmosphereManagerOption().HeightPosition = event.GetPropertyValue().GetDouble();
			atmosphere->UpdateAtmosphereManagerOptionChanges();
		}
		else if (name == "Rayleigh Multiplier")
		{
			atmosphere->GetAtmosphereManagerOption().RayleighMultiplier = event.GetPropertyValue().GetDouble();
			atmosphere->UpdateAtmosphereManagerOptionChanges();
		}
		else if (name == "Mie Multiplier")
		{
			atmosphere->GetAtmosphereManagerOption().MieMultiplier = event.GetPropertyValue().GetDouble();
			atmosphere->UpdateAtmosphereManagerOptionChanges();
		}
		else if (name == "Sun Intensity")
		{
			atmosphere->GetAtmosphereManagerOption().SunIntensity = event.GetPropertyValue().GetDouble();
			atmosphere->UpdateAtmosphereManagerOptionChanges();
		}
		else if (name == "Wave Length")
		{
			Ogre::ColourValue color = Properties::GetColorFromValue(event.GetProperty()->GetValue());
			atmosphere->GetAtmosphereManagerOption().WaveLength = Ogre::Vector3(color.r, color.g, color.b);
			atmosphere->UpdateAtmosphereManagerOptionChanges();
		}
		else if (name == "Phase Function")
		{
			atmosphere->GetAtmosphereManagerOption().G = event.GetPropertyValue().GetDouble();
			atmosphere->UpdateAtmosphereManagerOptionChanges();
		}
		else if (name == "Exposure")
		{
			atmosphere->GetAtmosphereManagerOption().Exposure = event.GetPropertyValue().GetDouble();
			atmosphere->UpdateAtmosphereManagerOptionChanges();
		}
		else if (name == "NumberOfSamples")
		{
			atmosphere->GetAtmosphereManagerOption().NumberOfSamples = event.GetPropertyValue().GetInteger();
			atmosphere->UpdateAtmosphereManagerOptionChanges();
		}

		else if (name == "Sun Position")
		{
			scene->SetSunPosition(Vector3FromVariant(event.GetProperty()->GetValue()));
		}
		else if (name == "Sun Strength")
		{
			scene->SetSunStrength(event.GetPropertyValue().GetDouble());
		}
		else if (name == "Sun Area")
		{
			scene->SetSunArea(event.GetPropertyValue().GetDouble());
		}
		else if (name == "Sun Color")
		{
			scene->SetSunColor(Properties::GetColorFromValue(event.GetPropertyValue()));
		}

		if (name.StartsWith("PPE"))
		{
			Ogre::String value = Ogre::StringUtil::split(STRING_WX_TO_OGRE(name), "|")[1];

			if (event.GetPropertyValue().GetBool())
				scene->AddPostProcessEffect(value);
			else
				scene->RemovePostProcessEffect(value);
		}

		scene = NULL;
		scriptHolder = ScenesHolder::GetInstance().GetCurrentScene();
	}
#pragma endregion

	if (name.StartsWith("ScriptEnabled"))
	{
		unsigned int i = Ogre::StringConverter::parseUnsignedInt(Ogre::StringUtil::split(STRING_WX_TO_OGRE(name), "|")[1]);

		scriptHolder->GetScript(i)->SetEnabled(event.GetPropertyValue().GetBool());
	}
	else if (name.StartsWith("Script"))
	{
		unsigned int i = Ogre::StringConverter::parseUnsignedInt(Ogre::StringUtil::split(STRING_WX_TO_OGRE(name), "|")[1]);

		scriptHolder->SetScript(i, STRING_WX_TO_OGRE(event.GetPropertyValue().GetString()));
	} 
	else if (name == "NewScript")
	{
		scriptHolder->AddScript(STRING_WX_TO_OGRE(event.GetPropertyValue().GetString()));

		//RefreshPropertyGrid();
	}

	scriptHolder = NULL;
}


void SceneControl::OnDeleteGameObject(wxCommandEvent &event)
{
	GameObject *obj = GetSelectedObject();

	if (obj)
		ScenesHolder::GetInstance().GetCurrentScene()->RemoveGameObject(obj);
	//obj->GetParent()->RemoveChild(obj);

	ReloadContent();
}


void SceneControl::OnAddGameObject(wxCommandEvent &event)
{
	GameObject *parent = GetSelectedObject();

	if (!parent)
		parent = ScenesHolder::GetInstance().GetCurrentScene()->GetRootGameObject();

	parent->CreateChild("EmptyGameObject");

	ReloadContent();
}
END_NAMESPACE

#endif