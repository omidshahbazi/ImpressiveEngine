#include "LightMaterialGenerator.h"
#include "Definitions.h"


Ogre::String GetPPDefines(unsigned int Settings)
{
	Ogre::String strPPD;

	//Get the type of light
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

	//Optional parameters
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
	//{ "worldView",			Ogre::GpuProgramParameters::ACT_WORLDVIEW_MATRIX },
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
}


const Ogre::MaterialPtr LightMaterialGenerator::GetTemplateMaterial(unsigned int Settings)
{
	Ogre::String materialName = m_BaseName;
	
	if (IS_BIT_ENABLED(Settings, MI_DIRECTIONAL))
		materialName += "QuadLightMaterial";
	else
		materialName += "GeometryLightMaterial";

	if(IS_BIT_ENABLED(Settings, MI_SHADOW_CASTER))
		materialName += "Shadow";

	return Ogre::MaterialManager::getSingleton().getByName(materialName);
}


const Ogre::GpuProgramPtr LightMaterialGenerator::GetTemplateVertexProgram(unsigned int Settings)
{
	Ogre::String programName = "";

	if (IS_BIT_ENABLED(Settings, MI_DIRECTIONAL))
		programName += "LightingVertexProgram";
	else
		programName += "GeometryLightVertexProgram";

	return Ogre::HighLevelGpuProgramManager::getSingleton().getByName(programName);
}


const Ogre::GpuProgramPtr LightMaterialGenerator::GetTemplateFragmentProgram(unsigned int Settings)
{
	if (m_MasterSource.empty())
		m_MasterSource = Ogre::ResourceGroupManager::getSingleton().openResource("LightFragmentShader.hlsl", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME)->getAsString();

	Ogre::String name = m_BaseName + Ogre::StringConverter::toString(Settings) + "_FragmentShader";		

	// Create shader object
	Ogre::HighLevelGpuProgramPtr program = Ogre::HighLevelGpuProgramManager::getSingleton().createProgram(name, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, "cg", Ogre::GPT_FRAGMENT_PROGRAM);

	program->setSource(m_MasterSource);
	program->setParameter("entry_point", "Main_FP");
	program->setParameter("profiles", "ps_2_0");

	// Just it has affect on Cg programs
	// Set up the preprocessor defines
	// Important to do this before any call to get parameters, i.e. before the program gets loaded
	program->setParameter("compile_arguments", GetPPDefines(Settings));

	SetupBaseParameters(program->getDefaultParameters());

	return Ogre::GpuProgramPtr(program);
}