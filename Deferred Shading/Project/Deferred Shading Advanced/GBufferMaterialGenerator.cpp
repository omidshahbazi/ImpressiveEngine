#include "GBufferMaterialGenerator.h"
#include "Definitions.h"


GBufferMaterialGenerator::GBufferMaterialGenerator(void) :
	DSMaterialGenerator("GBufferMaterial")
{
}


const Ogre::MaterialPtr GBufferMaterialGenerator::GetTemplateMaterial(unsigned int Settings)
{
	Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().create(m_BaseName + "GBufferTemplateMaterial_" + Ogre::StringConverter::toString(Settings), Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

	Ogre::Pass* pass = material->getTechnique(0)->getPass(0);

	pass->setName(m_BaseName + "Pass_" + Ogre::StringConverter::toString(Settings));
	pass->setLightingEnabled(false);

	if (IS_BIT_ENABLED(Settings, GBS_NORMAL_MAP))
		pass->createTextureUnitState();

	unsigned short numTextures = Settings & GBufferMaterialGenerator::GBS_TEXTURE_MASK;

	for (unsigned short i = 0; i < numTextures; i++)
		pass->createTextureUnitState();

	return material;
}


const Ogre::GpuProgramPtr GBufferMaterialGenerator::GetTemplateVertexProgram(unsigned int Settings)
{
	Ogre::StringStream ss;
	
	ss << "void Main_VP(" << std::endl;
	ss << "	float4 iPosition : POSITION," << std::endl;
	ss << "	float3 iNormal : NORMAL," << std::endl;

	unsigned short numTexCoords = (Settings & GBufferMaterialGenerator::GBS_TEXCOORD_MASK) >> 8;

	for (unsigned short i = 0; i < numTexCoords; i++)
		ss << "	float2 iUV" << i << " : TEXCOORD" << i << ',' << std::endl;

	if (IS_BIT_ENABLED(Settings, GBS_NORMAL_MAP))
		ss << "	float3 iTangent : TANGENT0," << std::endl;

	ss << std::endl;
	
	ss << "	out float4 oPosition : POSITION," << std::endl;
	ss << "	out float3 oViewPos : TEXCOORD0," << std::endl;
	ss << "	out float3 oNormal : TEXCOORD1," << std::endl;

	unsigned short texCoordNum = 2;
	if (IS_BIT_ENABLED(Settings, GBS_NORMAL_MAP)) 
	{
		ss << "	out float3 oTangent : TEXCOORD" << texCoordNum++ << ',' << std::endl;
		ss << "	out float3 oBiNormal : TEXCOORD" << texCoordNum++ << ',' << std::endl;
	}

	for (unsigned short i = 0; i < numTexCoords; i++)
		ss << "	out float2 oUV" << i << " : TEXCOORD" << texCoordNum++ << ',' << std::endl;

	ss << std::endl;

	ss << "	uniform float4x4 cWorldViewProj," << std::endl;
	ss << "	uniform float4x4 cWorldView" << std::endl;

	ss << "	)" << std::endl;
	
	
	ss << "{" << std::endl;

	ss << "	oPosition = mul(cWorldViewProj, iPosition);" << std::endl;


	ss << "	oNormal = normalize(mul(cWorldView, float4(iNormal, 0)).xyz);" << std::endl;

	if (IS_BIT_ENABLED(Settings, GBS_NORMAL_MAP))
	{
		ss << "	oTangent = mul(cWorldView, float4(iTangent, 0)).xyz;" << std::endl;
		ss << "	oBiNormal = cross(oNormal, oTangent);" << std::endl;
	}

	ss << "	oViewPos = mul(cWorldView, iPosition).xyz;" << std::endl;

	for (unsigned short i = 0; i < numTexCoords; i++)
		ss << "	oUV" << i << " = iUV" << i << ';' << std::endl;

	ss << "}" << std::endl;
	
	Ogre::String programSource = ss.str();
	Ogre::String programName = m_BaseName + "VP_" + Ogre::StringConverter::toString(Settings);

	// Create shader object
	Ogre::HighLevelGpuProgramPtr program = Ogre::HighLevelGpuProgramManager::getSingleton().createProgram(programName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, "hlsl", Ogre::GPT_VERTEX_PROGRAM);
	program->setSource(programSource);
	program->setParameter("entry_point", "Main_VP");
	program->setParameter("target", "vs_1_1");

	const Ogre::GpuProgramParametersSharedPtr &params = program->getDefaultParameters();
	params->setNamedAutoConstant("cWorldViewProj", Ogre::GpuProgramParameters::ACT_WORLDVIEWPROJ_MATRIX);
	params->setNamedAutoConstant("cWorldView", Ogre::GpuProgramParameters::ACT_WORLDVIEW_MATRIX);

	program->load();

	return Ogre::GpuProgramPtr(program);
}


const Ogre::GpuProgramPtr GBufferMaterialGenerator::GetTemplateFragmentProgram(unsigned int Settings)
{
	Ogre::StringStream ss;
	
	ss << "void Main_FP(" << std::endl;

	ss << "	float3 iViewPos : TEXCOORD0," << std::endl;
	ss << "	float3 iNormal : TEXCOORD1," << std::endl;

	unsigned short texCoordNum = 2;
	if (IS_BIT_ENABLED(Settings, GBS_NORMAL_MAP)) 
	{
		ss << "	float3 iTangent : TEXCOORD" << texCoordNum++ << ',' << std::endl;
		ss << "	float3 iBiNormal : TEXCOORD" << texCoordNum++ << ',' << std::endl;
	}

	unsigned short numTexCoords = (Settings & GBufferMaterialGenerator::GBS_TEXCOORD_MASK) >> 8;

	for (unsigned short i = 0; i < numTexCoords; i++)
		ss << "	float2 iUV" << i << " : TEXCOORD" << texCoordNum++ << ',' << std::endl;

	ss << std::endl;

	ss << "	out float4 oColor0 : COLOR0," << std::endl;
	ss << "	out float4 oColor1 : COLOR1," << std::endl;

	ss << std::endl;

	unsigned short samplerNum = 0;
	if (IS_BIT_ENABLED(Settings, GBS_NORMAL_MAP))
		ss << "	uniform sampler sNormalMap : register(s" << samplerNum++ << ")," << std::endl;

	unsigned short numTextures = Settings & GBufferMaterialGenerator::GBS_TEXTURE_MASK;
	for (unsigned short i = 0; i < numTextures; i++)
		ss << "	uniform sampler sTex" << i << " : register(s" << samplerNum++ << ")," << std::endl;

	if (numTextures == 0 || IS_BIT_ENABLED(Settings, GBS_HAS_DIFFUSE_COLOUR))
		ss << "	uniform float4 cDiffuseColour," << std::endl;

	ss << "	uniform float cFarDistance," << std::endl;
	
	ss << "	uniform float cSpecularity" << std::endl;

	ss << "	)" << std::endl;
	
	
	ss << "{" << std::endl;

	ss << "	oColor0.a = -iViewPos.z / cFarDistance;" << std::endl;
	ss << "	oColor1.a = cSpecularity;" << std::endl;
		
	if (IS_BIT_ENABLED(Settings, GBS_NORMAL_MAP))
	{
		ss << " float3 texNormal = (tex2D(sNormalMap, iUV0) - 0.5) * 2;" << std::endl;
		ss << "	float3x3 normalRotation = float3x3(iTangent, iBiNormal, iNormal);" << std::endl;
		ss << "	oColor0.rgb = normalize(mul(texNormal, normalRotation));" << std::endl;
	}
	else
		ss << "	oColor0.rgb = normalize(iNormal);" << std::endl;


	if (numTexCoords > 0 && numTextures > 0) 
	{
		ss << "	oColor1.rgb = tex2D(sTex0, iUV0);" << std::endl;
		if (IS_BIT_ENABLED(Settings, GBS_HAS_DIFFUSE_COLOUR))
			ss << "	oColor1.rgb *= cDiffuseColour.rgb;" << std::endl;
	}
	else
	{
		ss << "	oColor1.rgb = cDiffuseColour.rgb;" << std::endl;
	}

	ss << "}" << std::endl;
	
	Ogre::String programSource = ss.str();
	Ogre::String programName = m_BaseName + "FP_" + Ogre::StringConverter::toString(Settings);

	// Create shader object
	Ogre::HighLevelGpuProgramPtr program = Ogre::HighLevelGpuProgramManager::getSingleton().createProgram(programName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, "hlsl", Ogre::GPT_FRAGMENT_PROGRAM);
	program->setSource(programSource);
	program->setParameter("entry_point", "Main_FP");
	program->setParameter("target", "ps_2_0");

	const Ogre::GpuProgramParametersSharedPtr &params = program->getDefaultParameters();

	params->setNamedAutoConstant("cSpecularity", Ogre::GpuProgramParameters::ACT_SURFACE_SHININESS);

	if (numTextures == 0 || IS_BIT_ENABLED(Settings, GBS_HAS_DIFFUSE_COLOUR))
		params->setNamedAutoConstant("cDiffuseColour", Ogre::GpuProgramParameters::ACT_SURFACE_DIFFUSE_COLOUR);

	params->setNamedAutoConstant("cFarDistance", Ogre::GpuProgramParameters::ACT_FAR_CLIP_DISTANCE);

	program->load();

	return Ogre::GpuProgramPtr(program);
}