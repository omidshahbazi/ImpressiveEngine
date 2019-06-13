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
#include "LightMaterialGenerator.h"
#include "DSCommon.h"
#include "Utility.h"


BEGIN_NAMESPACE
Ogre::String GetPPDefines(unsigned int Settings)
{
	Ogre::String strPPD;

	// Get the type of light
	Ogre::String lightType;
	if (IS_BIT_ENABLED(Settings, LightMaterialGenerator::MI_POINT))
		lightType = "POINT";
	else if (IS_BIT_ENABLED(Settings, LightMaterialGenerator::MI_SPOTLIGHT))
		lightType = "SPOT";
	else if (IS_BIT_ENABLED(Settings, LightMaterialGenerator::MI_DIRECTIONAL))
		lightType = "DIRECTIONAL";
	else
		OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "Settings must have a light type", "LightMaterialGenerator::GetPPDefines");

	strPPD += "-DLIGHT_TYPE=LIGHT_" + lightType + " ";

	// Optional parameters
	if (IS_BIT_ENABLED(Settings, LightMaterialGenerator::MI_ATTENUATED))
		strPPD += "-DIS_ATTENUATED ";
	if (IS_BIT_ENABLED(Settings, LightMaterialGenerator::MI_SPECULAR))
		strPPD += "-DIS_SPECULAR ";
	if (IS_BIT_ENABLED(Settings, LightMaterialGenerator::MI_SHADOW_CASTER))
		strPPD += "-DIS_SHADOW_CASTER ";

	return strPPD;
}

void SetupBaseParameters(const Ogre::GpuProgramParametersSharedPtr &Params)
{
	struct AutoParamPair
	{ 
		Ogre::String Name; 
		Ogre::GpuProgramParameters::AutoConstantType Type; 
	};	

	//A list of auto params that might be present in the shaders generated
	static const AutoParamPair AUTO_PARAMS[] = {
	{ "cVPWidth",			Ogre::GpuProgramParameters::ACT_VIEWPORT_WIDTH },
	{ "cVPHeight",			Ogre::GpuProgramParameters::ACT_VIEWPORT_HEIGHT },
	//{ "worldView",		Ogre::GpuProgramParameters::ACT_WORLDVIEW_MATRIX },
	//{ "invProj",			Ogre::GpuProgramParameters::ACT_INVERSE_PROJECTION_MATRIX },
	{ "invView",			Ogre::GpuProgramParameters::ACT_INVERSE_VIEW_MATRIX },
	{ "cFlip",				Ogre::GpuProgramParameters::ACT_RENDER_TARGET_FLIPPING },
	{ "cLightDiffuseColor",	Ogre::GpuProgramParameters::ACT_LIGHT_DIFFUSE_COLOUR },
	{ "cLightSpecularColor",Ogre::GpuProgramParameters::ACT_LIGHT_SPECULAR_COLOUR },
	{ "cLightFalloff",		Ogre::GpuProgramParameters::ACT_LIGHT_ATTENUATION },
	{ "cLightPosition",		Ogre::GpuProgramParameters::ACT_LIGHT_POSITION_VIEW_SPACE },
	{ "cLightDirection",	Ogre::GpuProgramParameters::ACT_LIGHT_DIRECTION_VIEW_SPACE },
	{ "spotParams",			Ogre::GpuProgramParameters::ACT_SPOTLIGHT_PARAMS },
	{ "cFarClipDistance",	Ogre::GpuProgramParameters::ACT_FAR_CLIP_DISTANCE },
	{ "shadowViewProjMat",	Ogre::GpuProgramParameters::ACT_TEXTURE_VIEWPROJ_MATRIX }
	};

	int numParams = sizeof(AUTO_PARAMS) / sizeof(AutoParamPair);

	for (unsigned short i = 0; i < numParams; i++)
		if (Params->_findNamedConstantDefinition(AUTO_PARAMS[i].Name))
			Params->setNamedAutoConstant(AUTO_PARAMS[i].Name, AUTO_PARAMS[i].Type);
}


LightMaterialGenerator::LightMaterialGenerator(void) :
	DSMaterialGenerator("")
{
	Ogre::StringStream ss;

	ss << "#define LIGHT_POINT 			1" << std::endl;
	ss << "#define LIGHT_SPOT			2" << std::endl;
	ss << "#define LIGHT_DIRECTIONAL	3" << std::endl;

	ss << "float2 FixUV(float2 TexCoord, float2 TexSize)" << std::endl;
	ss << "{" << std::endl;
	//return TexCoord;
	ss << "	return TexCoord - (float2(0.5, 0.5) / TexSize);" << std::endl;
	ss << "}" << std::endl;

	ss << "float4 " << DSCommon::MAIN_FP_NAME << "(" << std::endl;

	ss << "#if LIGHT_TYPE == LIGHT_DIRECTIONAL" << std::endl;
	ss << "	float2 iTexCoord : TEXCOORD0," << std::endl;
	ss << "	float3 iRay : TEXCOORD1," << std::endl;
	ss << "#else" << std::endl;
	ss << "	float4 iProjectionPosition : TEXCOORD0," << std::endl;
	ss << "#endif" << std::endl;

	//ss << "#if LIGHT_TYPE == LIGHT_SPOT" << std::endl;
	//ss << "	float3 iPatternTexCoord : TEXCOORD1," << std::endl;
	//ss << "#endif" << std::endl;

	ss << "	uniform sampler cTex0: register(s0)," << std::endl;
	ss << "	uniform sampler cTex1: register(s1)," << std::endl;

	//ss << "#if LIGHT_TYPE == LIGHT_SPOT" << std::endl;
	//ss << "	uniform sampler cTexPattern: register(s2)," << std::endl;
	//ss << "#endif" << std::endl;

	ss << "#if LIGHT_TYPE != LIGHT_POINT" << std::endl;
	ss << "	uniform float3 cLightDirection," << std::endl;
	ss << "#endif" << std::endl;

	ss << "#if LIGHT_TYPE == LIGHT_SPOT" << std::endl;
	ss << "	uniform float4 spotParams," << std::endl;
	ss << "#endif" << std::endl;

	ss << "#if LIGHT_TYPE != LIGHT_DIRECTIONAL" << std::endl;
	ss << "	uniform float cVPWidth," << std::endl;
	ss << "	uniform float cVPHeight," << std::endl;
	ss << "	uniform float3 cFarCorner," << std::endl;
	ss << "	uniform float cFlip," << std::endl;
	ss << "#endif" << std::endl;
	
	ss << "	uniform float cFarClipDistance," << std::endl;

	// Attributes of light
	ss << "	uniform float4 cLightDiffuseColor," << std::endl;
	ss << "	uniform float4 cLightSpecularColor," << std::endl;
	ss << "	uniform float4 cLightFalloff," << std::endl;
	ss << "	uniform float3 cLightPosition" << std::endl;

	ss << "	) : COLOR " << std::endl;
	ss << "{" << std::endl;
	
	//None directional lights have some calculations to do in the beginning of the pixel shader
	ss << "#if LIGHT_TYPE != LIGHT_DIRECTIONAL" << std::endl;
	ss << "	iProjectionPosition /= iProjectionPosition.w;" << std::endl;
	// -1 is because generally +Y is down for textures but up for the screen
	ss << "	float2 iTexCoord = float2(iProjectionPosition.x, iProjectionPosition.y * -1 * cFlip) * 0.5 + 0.5;" << std::endl;
	// Texture coordinate magic, this compensates for jitter
	ss << "	iTexCoord = FixUV(iTexCoord, float2(cVPWidth, cVPHeight));" << std::endl;

	//ss << "#if LIGHT_TYPE == LIGHT_SPOT" << std::endl;
	//ss << "	clip(tex2D(cTexPattern, iPatternTexCoord).a - 1);" << std::endl;
	//ss << "#endif" << std::endl;
	
	ss << "	float3 iRay = float3(iProjectionPosition.x, iProjectionPosition.y * cFlip, 1) * cFarCorner;" << std::endl;
	ss << "#endif" << std::endl;
	
	// Depth + Normal
	ss << "	float4 a0 = tex2D(cTex0, iTexCoord);" << std::endl;
	// Specular + Diffuse Color
	ss << "	float4 a1 = tex2D(cTex1, iTexCoord);" << std::endl;
	
	// Attributes
	ss << "	float distance = a0.a;" << std::endl;
	ss << "	float3 normal = a0.rgb;" << std::endl;
	ss << "	float specularity = a1.a;" << std::endl;
	ss << "	float3 color = a1.rgb;" << std::endl;

	// Calculate position of texel in view space
	ss << "	half3 viewPos = normalize(iRay) * distance * cFarClipDistance;" << std::endl;

	// Calculate light direction and distance
	ss << "#if LIGHT_TYPE == LIGHT_DIRECTIONAL" << std::endl;
	ss << "	float3 objToLightDir = -cLightDirection.xyz;" << std::endl;
	ss << "#else" << std::endl;
	ss << "	float3 objToLightVec = cLightPosition - viewPos;" << std::endl;
	ss << "	float len_sq = dot(objToLightVec, objToLightVec);" << std::endl;
	ss << "	float len = sqrt(len_sq);" << std::endl;
	ss << "	float3 objToLightDir = objToLightVec / len;" << std::endl;
	ss << "#endif" << std::endl;

	// Calculate diffuse color
	ss << "	float3 total_light_contrib = max(0, dot(objToLightDir, normal)) * cLightDiffuseColor.rgb;" << std::endl;
	
	ss << "#if IS_SPECULAR" << std::endl;
	// Calculate specular component
	ss << "	half3 viewDir = -normalize(viewPos);" << std::endl;
	ss << "	half3 h = normalize(viewDir + objToLightDir);" << std::endl;
	ss << "	float3 light_specular = pow(dot(normal, h), 32) * cLightSpecularColor.rgb;" << std::endl;
	ss << "	total_light_contrib += specularity * light_specular;" << std::endl;
	ss << "#endif" << std::endl;
	
	ss << "#if IS_ATTENUATED" << std::endl;
	ss << "	clip(cLightFalloff.x - len);" << std::endl;
	// Calculate attenuation
	ss << "	float attenuation = dot(cLightFalloff.yzw, float3(1.0, len, len_sq));" << std::endl;
	ss << "	total_light_contrib /= attenuation;" << std::endl;
	ss << "#endif" << std::endl;
	
	ss << "#if LIGHT_TYPE == LIGHT_SPOT" << std::endl;
	ss << "	float spotlightAngle = saturate(dot(cLightDirection.xyz, -objToLightDir));" << std::endl;
	ss << "	float spotFalloff = saturate((spotlightAngle - spotParams.x) / (spotParams.y - spotParams.x));" << std::endl;
	ss << "	total_light_contrib *= (1 - spotFalloff);" << std::endl;
	ss << "#endif" << std::endl;

	ss << "	return float4(total_light_contrib * color, 0);" << std::endl;
	ss << "}" << std::endl;

	m_MasterSource = ss.str();
}


//LightMaterialGenerator::~LightMaterialGenerator(void)
//{
//}


const Ogre::MaterialPtr LightMaterialGenerator::GetTemplateMaterial(unsigned int Settings)
{
	Ogre::String materialName = m_BaseName;
	
	if (IS_BIT_ENABLED(Settings, MI_DIRECTIONAL))
		materialName += DSCommon::QUAD_LIGHT_MATERIAL_NAME;
	else
		materialName += DSCommon::GEOMETRY_LIGHT_MATERIAL_NAME;

	if(IS_BIT_ENABLED(Settings, MI_SHADOW_CASTER))
		materialName += "Shadow";

	return Ogre::MaterialManager::getSingleton().getByName(materialName);
}


const Ogre::GpuProgramPtr LightMaterialGenerator::GetTemplateVertexProgram(unsigned int Settings)
{
	Ogre::String programName = "";

	if (IS_BIT_ENABLED(Settings, MI_DIRECTIONAL))
		programName += DSCommon::LIGHTING_VP_NAME;
	else
		programName += DSCommon::GEOMETRY_LIGHT_VP_NAME;

	return Ogre::HighLevelGpuProgramManager::getSingleton().getByName(programName);
}


const Ogre::GpuProgramPtr LightMaterialGenerator::GetTemplateFragmentProgram(unsigned int Settings)
{
	// Create shader object
	Ogre::HighLevelGpuProgramPtr program = Ogre::HighLevelGpuProgramManager::getSingleton().createProgram(
		m_BaseName + Ogre::StringConverter::toString(Settings) + "_FragmentShader", 
		Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, "cg", Ogre::GPT_FRAGMENT_PROGRAM);

	program->setSource(m_MasterSource);
	program->setParameter("entry_point", DSCommon::MAIN_FP_NAME);
	program->setParameter("profiles", "ps_3_0");

	// Just it has affect on Cg programs
	// Set up the preprocessor defines
	// Important to do this before any call to get parameters, i.e. before the program gets loaded
	program->setParameter("compile_arguments", GetPPDefines(Settings));

	SetupBaseParameters(program->getDefaultParameters());

	return Ogre::GpuProgramPtr(program);
}
END_NAMESPACE