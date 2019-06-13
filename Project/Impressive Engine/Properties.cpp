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

#include "Properties.h"
#include "MainWindow.h"


BEGIN_NAMESPACE
static const char *builtInSceneBlendFactorLabel[] = { "One", "Zero", "Destination Color", "Source Color", "One Minus Destination Color", "One Minus Source Color", "Destination Alpha", "Source Alpha", "One Minus Destination Alpha", "One Minus Source Alpha", NULL };
static int builtInSceneBlendFactorValue[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

static const char *builtInSceneBlendOperationLabel[] = { "Add", "Subtract", "Reverse Subtract", "Minimum", "Maximum", NULL };
static int builtInSceneBlendOperationValue[] = { 0, 1, 2, 3, 4 };

static const char *builtInCompareFunctionLabel[] = { "Always Fail", "Always Pass", "Less", "Less Equal", "Equal", "Not Equal", "Greater Equal", "Greater", NULL };
static int builtInCompareFunctionValue[] = { 0, 1, 2, 3, 4, 5, 6, 7 };

static const char *builtInTrackVertexColourTypeLabel[] = { "None", "Ambient", "Diffuse", "Specular", "Emissive", NULL };
static int builtInTrackVertexColourTypeValue[] = { 0, 1, 2, 4, 8 };

static const char *builtInShadeOptionsLabel[] = { "Flat", "Ground", "Phong", NULL };
static int builtInShadeOptionsValue[] = { 0, 1, 2 };

static const char *builtInPolygonModeLabel[] = { "Points", "Wireframe", "Solid", NULL };
static int builtInPolygonModeValue[] = { 1, 2, 3 };

static const char *builtInCullingModeLabel[] = { "None", "Clockwise", "Anti Clockwise", NULL };
static int builtInCullingModeValue[] = { 1, 2, 3 };

static const char *builtInManualCullingModeLabel[] = { "None", "Back", "Front", NULL };
static int builtInManualCullingModeValue[] = { 1, 2, 3 };

static const char *builtInFogModeLabel[] = { "None", "Exponentially", "Square Of Exponentially", "Linear", NULL };
static int builtInFogModeValue[] = { 0, 1, 2, 3 };

static const char *builtInTextureTypeLabel[] = { "1D", "2D", "3D", "Cube Map", NULL };
static int builtInTextureTypeValue[] = { 1, 2, 3, 4 };

static const char *builtInPixelFormatLabel[] = { "Unknown", "BYTE_L", "SHORT_L", "BYTE_A", "A4L4", "BYTE_LA", "R5G6B5", "B5G6R5", "R3G3B2", "A4R4G4B4", "A1R5G5B5", "R8G8B8", "B8G8R8", "A8R8G8B8", "A8B8G8R8", "B8G8R8A8", "R8G8B8A8", "X8R8G8B8", "X8B8G8R8", "A2R10G10B10", "A2B10G10R10", "DXT1", "DXT2", "DXT3", "DXT4", "DXT5", "FLOAT16_R", "FLOAT16_RGB", "FLOAT16_RGBA", "FLOAT32_R", "FLOAT32_RGB", "FLOAT32_RGBA", "FLOAT16_GR", "FLOAT32_GR", "DEPTH", "SHORT_RGBA", "SHORT_GR", "SHORT_RGB", "PVRTC_RGB2", "PVRTC_RGBA2", "PVRTC_RGB4", "PVRTC_RGBA4", NULL };
static int builtInPixelFormatValue[] = { 0, 1, 2, 3, 4, 6, 7, 31, 8, 9, 10, 11, 12, 13, 14, 28, 26, 27, 15, 16, 17, 18, 19, 20, 21, 32, 22, 23, 33, 24, 25, 35, 36, 29, 30, 34, 37, 38, 39, 40, 41, 42 };

static const char *builtInTextureAddressingModeLabel[] = { "Wrap", "Mirror", "Clamp", "Border", NULL };
static int builtInTextureAddressingModeValue[] = { 0, 1, 2, 3 };

static const char *builtInFilterOptionsLabel[] = { "None", "Point", "Linear", "Anisotropic", NULL };
static int builtInFilterOptionsValue[] = { 0, 1, 2, 3 };

static const char *builtInBindingTypeLabel[] = { "Fragment", "Vertex", NULL };
static int builtInBindingTypeValue[] = { 0, 1 };

static const char *builtInLightTypesLabel[] = { "Point", "Directional", "Spot", NULL };
static int builtInLightTypesValue[] = { 0, 1, 2 };

static const char *builtInLightTypeLabel[] = { "Point", "Directional", "Spot", NULL };
static int builtInLightTypeValue[] = { 0, 1, 2 };

//static const char *builtInShadowTechniqueLabel[] = { "None", "Detail Type Additive", "Detail Type Modulative", "Detail Type Integrated", "Detail Type Stencil", "Detail Type Texture", "Stencil Modulative", "Stencil Additive", "Texture Modulative", "Texture Additive", "Texture Additive Integrated", "Texture Modulative Integrated", NULL };
//static int builtInShadowTechniqueValue[] = { 0, 1, 2, 4, 16, 32, 18, 17, 34, 33, 37, 38 };

static const char *builtInShadowTypeLabel[] = { "Texture", "Stencil", "None", NULL };
static int builtInShadowTypeValue[] = { ShadowManager::ST_TEXTURE, ShadowManager::ST_STENCIL, ShadowManager::ST_NONE };

static const char *builtInShadowCameraSetupTypeLabel[] = { "SCST_DEFAULT", "SCST_FOCUSED", "SCST_LISPSM", NULL };
static int builtInShadowCameraSetupTypeValue[] = { ShadowManager::SCST_DEFAULT, ShadowManager::SCST_FOCUSED, ShadowManager::SCST_LISPSM };

static const char *builtInProjectionTypeLabel[] = { "Orthographic", "Perspective", NULL };
static int builtInProjectionTypeValue[] = { 0, 1 };

static const char *builtInShaderModeLabel[] = { "HLSL", "Nvidia Cg", /*"GLSL",*/ NULL };
static int builtInShaderModeValue[] = { 0, 1/*, 2*/ };

static const char *builtInNormalModeLabel[] = { /*"Texture",*/ "Vertext", "RTT", NULL };
static int builtInNormalModeValue[] = { /*0,*/ 1, 2 };

static const char *builtInLightingModeLabel[] = { "Low Dynamic Range (LDR)", "High Dynamic Range (HDR)", NULL };
static int builtInLightingModeValue[] = { 0, 1 };

static const char *builtInSkyModeLabel[] = { "Sky Box", "Sky Dome", "Advanced Sky", NULL };
static int builtInSkyModeValue[] = { 0, 1, 2 };

static const char *builtInModuleTypeLabel[] = { "Simple Grid", "Radial Grdi", "Projected Grid", NULL };
static int builtInModuleTypeValue[] = { 0, 1, 2 };

static const char *builtInNoiseTypeLabel[] = { "Perlin", "FFT", NULL };
static int builtInNoiseTypeValue[] = { 0, 1 };

static const char *builtInMaterialTypeLabel[] = { "Diffuse", "Specular/Diffuse", "Bump/Diffuse", "Bump/Specular/Diffuse", NULL };
static int builtInMaterialTypeValue[] = { 0, 1, 2, 3 };

static const char *builtInRenderModeLabel[] = { "Scene", "Lit", "Depth", "Specular", "Nomal", "Albedo", NULL };
static int builtInRenderModeValue[] = { -1, 0, 1, 2, 3, 4 };


Properties *Singleton<Properties>::m_Instance = NULL;

#include "AdditionProperties.h"


Properties::Properties(void) :
	m_Control(NULL),
	m_LastAddedCategory(NULL)
{
}


//Properties::~Properties(void)
//{
//}


void Properties::Initialize(wxWindow *Parent)
{
	m_Control = new PropertiesControl(Parent);
}


void Properties::Clear(void)
{
	m_Control->SetListener(NULL);

	m_Control->GetPropertyGridPage()->Clear();
	
	m_Control->GetPropertyGridManager()->Refresh();
}


void Properties::BeginNewProperties(const wxString &Caption, IPropertiesListener *Listener)
{
	Clear();

	m_Control->SetListener(Listener);

	MainWindow *main = (MainWindow*)m_Control->GetParent();

	main->GetPane(GetName()).Caption(Caption + " " + GetCaption());

	//main->ShowWindow(GetName());

	main = NULL;
}


void Properties::EndNewProperties(void)
{
	m_Control->GetPropertyGridManager()->Refresh();
}


wxPGProperty *Properties::AddCategory(const wxString &Label, const wxString &Name, wxPGProperty *Parent)
{
	if (Parent)
		m_LastAddedCategory = Parent->AppendChild(new wxPropertyCategory(Label, Name));
	else
		m_LastAddedCategory = m_Control->GetPropertyGridPage()->Append(new wxPropertyCategory(Label, Name));

	return m_LastAddedCategory;
}


void Properties::SetHelp(const wxString &Label, const wxString &Value)
{
	if (Value.size() > 0)
	{
		//m_Control->GetPropertyGridPage()->SetPropertyHelpString(m_Control->GetPropertyGridPage()->GetProperty(Label), Value);
		m_Control->GetPropertyGridPage()->SetPropertyHelpString(m_Control->GetPropertyGridPage()->GetPropertyByLabel(Label), Value);
	}
}


void Properties::SetReadOnly(const wxString &Label, bool Value)
{
	//m_Control->GetPropertyGridPage()->EnableProperty(m_Control->GetPropertyGridPage()->GetProperty(Label), !Value);
	m_Control->GetPropertyGridPage()->EnableProperty(m_Control->GetPropertyGridPage()->GetPropertyByLabel(Label), !Value);
}


void Properties::AddPropertyOfString(const wxString &Label, const wxString &Value, const wxString &HelpText, bool ReadOnly, const wxString &Name)
{
	//m_Control->GetPropertyGridPage()->Append(new wxStringProperty(Label, GetName(Label, Name), Value));
	m_LastAddedCategory->AppendChild(new wxStringProperty(Label, GetName(Label, Name), Value));

	SetReadOnly(Label, ReadOnly);
	SetHelp(Label, HelpText);
}


void Properties::AddPropertyOfFont(const wxString &Label, const wxFont &Value, const wxString &HelpText, const wxString &Name)
{
	//m_Control->GetPropertyGridPage()->Append(new wxFontProperty(Label, GetName(Label, Name), Value));
	m_LastAddedCategory->AppendChild(new wxFontProperty(Label, GetName(Label, Name), Value));

	SetHelp(Label, HelpText);
}


void Properties::AddPropertyOfColor(const wxString &Label, const Ogre::ColourValue &Value/*const wxColor &Value*/, const wxString &HelpText, const wxString &Name)
{
	m_LastAddedCategory->AppendChild(new wxColourProperty(Label, GetName(Label, Name), wxColor(Value.r * 255, Value.g * 255, Value.b * 255, Value.a * 255)));

	//m_Control->GetPropertyGridPage()->Append(new wxSystemColourProperty(Label, GetName(Label, Name), Value));
	//m_LastAddedCategory->AppendChild(new wxSystemColourProperty(Label, GetName(Label, Name), Value));

	SetHelp(Label, HelpText);
}


void Properties::AddPropertyOfBool(const wxString &Label, bool Value, const wxString &HelpText, const wxString &Name)
{
	//m_Control->GetPropertyGridPage()->Append(new wxBoolProperty(Label, GetName(Label, Name), Value));
	m_LastAddedCategory->AppendChild(new wxBoolProperty(Label, GetName(Label, Name), Value));

	m_Control->GetPropertyGridPage()->SetPropertyAttribute(GetName(Label, Name), wxPG_BOOL_USE_CHECKBOX, true);

	SetHelp(Label, HelpText);
}


void Properties::AddPropertyOfInt(const wxString &Label, int Value, const wxString &HelpText, const wxString &Name)
{
	//m_Control->GetPropertyGridPage()->Append(new wxIntProperty(Label, GetName(Label, Name), Value));
	m_LastAddedCategory->AppendChild(new wxIntProperty(Label, GetName(Label, Name), Value));

	SetHelp(Label, HelpText);
}


void Properties::AddPropertyOfFloat(const wxString &Label, double Value, const wxString &HelpText, const wxString &Name)
{
	//m_Control->GetPropertyGridPage()->Append(new wxFloatProperty(Label, GetName(Label, Name), Value));
	m_LastAddedCategory->AppendChild(new wxFloatProperty(Label, GetName(Label, Name), Value));

	SetHelp(Label, HelpText);
}


void Properties::AddPropertyOfVector2(const wxString &Label, Ogre::Vector2 Value, const wxString &HelpText, const wxString &Name)
{
	//m_Control->GetPropertyGridPage()->Append(new VectorProperty(Label, GetName(Label, Name), Value));
	m_LastAddedCategory->AppendChild(new Vector2Property(Label, GetName(Label, Name), Value));

	SetHelp(Label, HelpText);
}


void Properties::AddPropertyOfVector3(const wxString &Label, Ogre::Vector3 Value, const wxString &HelpText, const wxString &Name)
{
	//m_Control->GetPropertyGridPage()->Append(new VectorProperty(Label, GetName(Label, Name), Value));
	m_LastAddedCategory->AppendChild(new Vector3Property(Label, GetName(Label, Name), Value));

	SetHelp(Label, HelpText);
}


void Properties::AddPropertyOfQuaternion(const wxString &Label, Ogre::Quaternion Value, const wxString &HelpText, const wxString &Name)
{
	//m_Control->GetPropertyGridPage()->Append(new QuaternionProperty(Label, GetName(Label, Name), Value));
	m_LastAddedCategory->AppendChild(new QuaternionProperty(Label, GetName(Label, Name), Value));

	SetHelp(Label, HelpText);
}


void Properties::AddPropertyOfEnumerator(const wxString &Label, const char **Labels, const int *Values, int Value, const wxString &HelpText, const wxString &Name)
{
	m_LastAddedCategory->AppendChild(new wxEnumProperty(Label, GetName(Label, Name), Labels, (const long*)Values, (int)Value));

	SetHelp(Label, HelpText);
}


void Properties::AddPropertyOfEnumerator_SceneBlendFactor(const wxString &Label, Ogre::SceneBlendFactor Value, const wxString &HelpText, const wxString &Name)
{
	AddPropertyOfEnumerator(Label, builtInSceneBlendFactorLabel, builtInSceneBlendFactorValue, (int)Value, HelpText, Name);
}


void Properties::AddPropertyOfEnumerator_SceneBlendOperation(const wxString &Label, Ogre::SceneBlendOperation Value, const wxString &HelpText, const wxString &Name)
{
	AddPropertyOfEnumerator(Label, builtInSceneBlendOperationLabel, builtInSceneBlendOperationValue, (int)Value, HelpText, Name);
}


void Properties::AddPropertyOfEnumerator_CompareFunction(const wxString &Label, Ogre::CompareFunction Value, const wxString &HelpText, const wxString &Name)
{
	AddPropertyOfEnumerator(Label, builtInCompareFunctionLabel, builtInCompareFunctionValue, (int)Value, HelpText, Name);
}


void Properties::AddPropertyOfEnumerator_TrackVertexColourType(const wxString &Label, Ogre::TrackVertexColourType Value, const wxString &HelpText, const wxString &Name)
{
	AddPropertyOfEnumerator(Label, builtInTrackVertexColourTypeLabel, builtInTrackVertexColourTypeValue, (int)Value, HelpText, Name);
}


void Properties::AddPropertyOfEnumerator_ShadeOptions(const wxString &Label, Ogre::ShadeOptions Value, const wxString &HelpText, const wxString &Name)
{
	AddPropertyOfEnumerator(Label, builtInShadeOptionsLabel, builtInShadeOptionsValue, (int)Value, HelpText, Name);
}


void Properties::AddPropertyOfEnumerator_PolygonMode(const wxString &Label, Ogre::PolygonMode Value, const wxString &HelpText, const wxString &Name)
{
	AddPropertyOfEnumerator(Label, builtInPolygonModeLabel, builtInPolygonModeValue, (int)Value, HelpText, Name);
}


void Properties::AddPropertyOfEnumerator_CullingMode(const wxString &Label, Ogre::CullingMode Value, const wxString &HelpText, const wxString &Name)
{
	AddPropertyOfEnumerator(Label, builtInCullingModeLabel, builtInCullingModeValue, (int)Value, HelpText, Name);
}


void Properties::AddPropertyOfEnumerator_ManualCullingMode(const wxString &Label, Ogre::ManualCullingMode Value, const wxString &HelpText, const wxString &Name)
{
	AddPropertyOfEnumerator(Label, builtInManualCullingModeLabel, builtInManualCullingModeValue, (int)Value, HelpText, Name);
}


void Properties::AddPropertyOfEnumerator_FogMode(const wxString &Label, Ogre::FogMode Value, const wxString &HelpText, const wxString &Name)
{
	AddPropertyOfEnumerator(Label, builtInFogModeLabel, builtInFogModeValue, (int)Value, HelpText, Name);
}


void Properties::AddPropertyOfEnumerator_TextureType(const wxString &Label, Ogre::TextureType Value, const wxString &HelpText, const wxString &Name)
{
	AddPropertyOfEnumerator(Label, builtInTextureTypeLabel, builtInTextureTypeValue, (int)Value, HelpText, Name);
}


void Properties::AddPropertyOfEnumerator_PixelFormat(const wxString &Label, Ogre::PixelFormat Value, const wxString &HelpText, const wxString &Name)
{
	AddPropertyOfEnumerator(Label, builtInPixelFormatLabel, builtInPixelFormatValue, (int)Value, HelpText, Name);
}


void Properties::AddPropertyOfEnumerator_TextureAddressingMode(const wxString &Label, Ogre::TextureUnitState::TextureAddressingMode Value, const wxString &HelpText, const wxString &Name)
{
	AddPropertyOfEnumerator(Label, builtInTextureAddressingModeLabel, builtInTextureAddressingModeValue, (int)Value, HelpText, Name);
}


void Properties::AddPropertyOfEnumerator_FilterOptions(const wxString &Label, Ogre::FilterOptions Value, const wxString &HelpText, const wxString &Name)
{
	AddPropertyOfEnumerator(Label, builtInFilterOptionsLabel, builtInFilterOptionsValue, (int)Value, HelpText, Name);
}


void Properties::AddPropertyOfEnumerator_BindingType(const wxString &Label, Ogre::TextureUnitState::BindingType Value, const wxString &HelpText, const wxString &Name)
{
	AddPropertyOfEnumerator(Label, builtInBindingTypeLabel, builtInBindingTypeValue, (int)Value, HelpText, Name);
}


void Properties::AddPropertyOfEnumerator_LightTypes(const wxString &Label, Ogre::Light::LightTypes Value, const wxString &HelpText, const wxString &Name)
{
	AddPropertyOfEnumerator(Label, builtInLightTypesLabel, builtInLightTypesValue, (int)Value, HelpText, Name);
}


void Properties::AddPropertyOfEnumerator_LightType(const wxString &Label, Light::LightType Value, const wxString &HelpText, const wxString &Name)
{
	AddPropertyOfEnumerator(Label, builtInLightTypeLabel, builtInLightTypeValue, (int)Value, HelpText, Name);
}


//void Properties::AddPropertyOfEnumerator_ShadowTechnique(const wxString &Label, Ogre::ShadowTechnique Value, const wxString &HelpText, const wxString &Name)
//{
//	AddPropertyOfEnumerator(Label, builtInShadowTechniqueLabel, builtInShadowTechniqueValue, (int)Value, HelpText, Name);
//}


void Properties::AddPropertyOfEnumerator_ShadowType(const wxString &Label, ShadowManager::ShadowType Value, const wxString &HelpText, const wxString &Name)
{
	AddPropertyOfEnumerator(Label, builtInShadowTypeLabel, builtInShadowTypeValue, (int)Value, HelpText, Name);
}


void Properties::AddPropertyOfEnumerator_ShadowCameraSetupType(const wxString &Label, ShadowManager::ShadowCameraSetupType Value, const wxString &HelpText, const wxString &Name)
{
	AddPropertyOfEnumerator(Label, builtInShadowCameraSetupTypeLabel, builtInShadowCameraSetupTypeValue, (int)Value, HelpText, Name);
}


void Properties::AddPropertyOfEnumerator_ProjectionType(const wxString &Label, Ogre::ProjectionType Value, const wxString &HelpText, const wxString &Name)
{
	AddPropertyOfEnumerator(Label, builtInProjectionTypeLabel, builtInProjectionTypeValue, (int)Value, HelpText, Name);
}


void Properties::AddPropertyOfEnumerator_ShaderMode(const wxString &Label, Hydrax::MaterialManager::ShaderMode Value, const wxString &HelpText, const wxString &Name)
{
	AddPropertyOfEnumerator(Label, builtInShaderModeLabel, builtInShaderModeValue, (int)Value, HelpText, Name);
}


void Properties::AddPropertyOfEnumerator_NormalMode(const wxString &Label, Hydrax::MaterialManager::NormalMode Value, const wxString &HelpText, const wxString &Name)
{
	AddPropertyOfEnumerator(Label, builtInNormalModeLabel, builtInNormalModeValue, (int)Value, HelpText, Name);
}


void Properties::AddPropertyOfEnumerator_LightingMode(const wxString &Label, SkyX::SkyX::LightingMode Value, const wxString &HelpText, const wxString &Name)
{
	AddPropertyOfEnumerator(Label, builtInLightingModeLabel, builtInLightingModeValue, (int)Value, HelpText, Name);
}


void Properties::AddPropertyOfEnumerator_SkyMode(const wxString &Label, Scene::SkyMode Value, const wxString &HelpText, const wxString &Name)
{
	AddPropertyOfEnumerator(Label, builtInSkyModeLabel, builtInSkyModeValue, (int)Value, HelpText, Name);
}


void Properties::AddPropertyOfEnumerator_ModuleType(const wxString &Label, OceanGameObject::ModuleType Value, const wxString &HelpText, const wxString &Name)
{
	AddPropertyOfEnumerator(Label, builtInModuleTypeLabel, builtInModuleTypeValue, (int)Value, HelpText, Name);
}


void Properties::AddPropertyOfEnumerator_NoiseType(const wxString &Label, OceanGameObject::NoiseType Value, const wxString &HelpText, const wxString &Name)
{
	AddPropertyOfEnumerator(Label, builtInNoiseTypeLabel, builtInNoiseTypeValue, (int)Value, HelpText, Name);
}


void Properties::AddPropertyOfEnumerator_MaterialType(const wxString &Label, MaterialEditorControl::MaterialType Value, const wxString &HelpText, const wxString &Name)
{
	AddPropertyOfEnumerator(Label, builtInMaterialTypeLabel, builtInMaterialTypeValue, (int)Value, HelpText, Name);
}


void Properties::AddPropertyOfEnumerator_RenderMode(const wxString &Label, const DeferredShadingSystem::RenderMode &Value, const wxString &HelpText, const wxString &Name)
{
	AddPropertyOfEnumerator(Label, builtInRenderModeLabel, builtInRenderModeValue, (int)Value, HelpText, Name);
}

Ogre::ColourValue Properties::GetColorFromValue(const wxVariant &Data)
{
	//wxColourPropertyValue* p =((wxColourPropertyValue*)wxGetVariantCast(Data, wxColourProperty));
	//wxColor color(p->m_colour);

	wxColour color;
	color << Data;

	return Ogre::ColourValue(color.Red() / 255.f, color.Green() / 255.f, color.Blue() / 255.f, color.Alpha() / 255.f);
}


const wxString &Properties::GetName(const wxString &Label, const wxString &Name)
{
	if (Name.size())
		return Name;

	return Label;
}
END_NAMESPACE


#endif