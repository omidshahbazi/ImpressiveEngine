#include "GBufferMaterialGenerator.h"
#include "Definition.h"

//Use this directive to control whether you are writing projective (regular) or linear depth.
#define WRITE_LINEAR_DEPTH

GBufferMaterialGenerator::GBufferMaterialGenerator(void) :
	DSSMaterialGenerator("DeferredShading/GBuffer/", 0xFF00FFFF, 0x00FFFF00, 0x0000FFFF)
{
}


const Ogre::MaterialPtr GBufferMaterialGenerator::GenerateTemplateMaterial(unsigned int Settings)
{
	Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().create(m_BaseName + "Mat_" + Ogre::StringConverter::toString(Settings), Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

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


const Ogre::GpuProgramPtr GBufferMaterialGenerator::GenerateVertexProgram(unsigned int Settings)
{
	Ogre::StringStream ss;
	
	ss << "void ToGBufferVP(" << std::endl;
	ss << "	float4 iPosition : POSITION," << std::endl;
	ss << "	float3 iNormal   : NORMAL," << std::endl;

	Ogre::uint32 numTexCoords = (Settings & GBufferMaterialGenerator::GBS_TEXCOORD_MASK) >> 8;

	for (Ogre::uint32 i=0; i<numTexCoords; i++)
		ss << "	float2 iUV" << i << " : TEXCOORD" << i << ',' << std::endl;

	if (IS_BIT_ENABLED(Settings, GBS_NORMAL_MAP))
		ss << "	float3 iTangent : TANGENT0," << std::endl;

	//TODO : Skinning inputs
	ss << std::endl;
	


	ss << "	out float4 oPosition : POSITION," << std::endl;
#ifdef WRITE_LINEAR_DEPTH
    ss << "	out float3 oViewPos : TEXCOORD0," << std::endl;
#else
	ss << "	out float oDepth : TEXCOORD0," << std::endl;
#endif
	ss << "	out float3 oNormal : TEXCOORD1," << std::endl;

	int texCoordNum = 2;
	if (IS_BIT_ENABLED(Settings, GBS_NORMAL_MAP)) 
	{
		ss << "	out float3 oTangent : TEXCOORD" << texCoordNum++ << ',' << std::endl;
		ss << "	out float3 oBiNormal : TEXCOORD" << texCoordNum++ << ',' << std::endl;
	}

	for (Ogre::uint32 i=0; i<numTexCoords; i++)
		ss << "	out float2 oUV" << i << " : TEXCOORD" << texCoordNum++ << ',' << std::endl;

	ss << std::endl;

	ss << "	uniform float4x4 cWorldViewProj," << std::endl;
	ss << "	uniform float4x4 cWorldView" << std::endl;

	ss << "	)" << std::endl;
	
	
	ss << "{" << std::endl;
	ss << "	oPosition = mul(cWorldViewProj, iPosition);" << std::endl;
	ss << "	oNormal = mul(cWorldView, float4(iNormal,0)).xyz;" << std::endl;

	if (IS_BIT_ENABLED(Settings, GBS_NORMAL_MAP))
	{
		ss << "	oTangent = mul(cWorldView, float4(iTangent,0)).xyz;" << std::endl;
		ss << "	oBiNormal = cross(oNormal, oTangent);" << std::endl;
	}

#ifdef WRITE_LINEAR_DEPTH
    ss << "	oViewPos = mul(cWorldView, iPosition).xyz;" << std::endl;
#else
	ss << "	oDepth = oPosition.w;" << std::endl;
#endif

	for (Ogre::uint32 i=0; i<numTexCoords; i++)
		ss << "	oUV" << i << " = iUV" << i << ';' << std::endl;

	ss << "}" << std::endl;
	
	Ogre::String programSource = ss.str();
	Ogre::String programName = m_BaseName + "VP_" + Ogre::StringConverter::toString(Settings);

	// Create shader object
	Ogre::HighLevelGpuProgramPtr program = Ogre::HighLevelGpuProgramManager::getSingleton().createProgram(programName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, "cg", Ogre::GPT_VERTEX_PROGRAM);
	program->setSource(programSource);
	program->setParameter("entry_point", "ToGBufferVP");
	program->setParameter("profiles", "vs_1_1 arbvp1");

	const Ogre::GpuProgramParametersSharedPtr &params = program->getDefaultParameters();
	params->setNamedAutoConstant("cWorldViewProj", Ogre::GpuProgramParameters::ACT_WORLDVIEWPROJ_MATRIX);
	params->setNamedAutoConstant("cWorldView", Ogre::GpuProgramParameters::ACT_WORLDVIEW_MATRIX);

	program->load();

	return Ogre::GpuProgramPtr(program);
}


const Ogre::GpuProgramPtr GBufferMaterialGenerator::GenerateFragmentProgram(unsigned int Settings)
{
	Ogre::StringStream ss;
	
	ss << "void ToGBufferFP(" << std::endl;
#ifdef WRITE_LINEAR_DEPTH
    ss << "	float3 iViewPos : TEXCOORD0," << std::endl;
#else
    ss << "	float1 iDepth : TEXCOORD0," << std::endl;
#endif
	ss << "	float3 iNormal   : TEXCOORD1," << std::endl;

	int texCoordNum = 2;
	if (IS_BIT_ENABLED(Settings, GBS_NORMAL_MAP)) 
	{
		ss << "	float3 iTangent : TEXCOORD" << texCoordNum++ << ',' << std::endl;
		ss << "	float3 iBiNormal : TEXCOORD" << texCoordNum++ << ',' << std::endl;
	}

	Ogre::uint32 numTexCoords = (Settings & GBufferMaterialGenerator::GBS_TEXCOORD_MASK) >> 8;

	for (Ogre::uint32 i=0; i<numTexCoords; i++)
		ss << "	float2 iUV" << i << " : TEXCOORD" << texCoordNum++ << ',' << std::endl;

	ss << std::endl;

	ss << "	out float4 oColor0 : COLOR0," << std::endl;
	ss << "	out float4 oColor1 : COLOR1," << std::endl;

	ss << std::endl;

	int samplerNum = 0;
	if (IS_BIT_ENABLED(Settings, GBS_NORMAL_MAP))
		ss << "	uniform sampler sNormalMap : register(s" << samplerNum++ << ")," << std::endl;

	Ogre::uint32 numTextures = Settings & GBufferMaterialGenerator::GBS_TEXTURE_MASK;
	for (Ogre::uint32 i = 0; i < numTextures; i++)
		ss << "	uniform sampler sTex" << i << " : register(s" << samplerNum++ << ")," << std::endl;

    if (numTextures == 0 || IS_BIT_ENABLED(Settings, GBS_HAS_DIFFUSE_COLOUR))
		ss << "	uniform float4 cDiffuseColour," << std::endl;

#ifdef WRITE_LINEAR_DEPTH
    ss << "	uniform float cFarDistance," << std::endl;
#endif
	
	ss << "	uniform float cSpecularity" << std::endl;

	ss << "	)" << std::endl;
	
	
	ss << "{" << std::endl;

	if (numTexCoords > 0 && numTextures > 0) 
	{
		ss << "	oColor0.rgb = tex2D(sTex0, iUV0);" << std::endl;
        if (IS_BIT_ENABLED(Settings, GBS_HAS_DIFFUSE_COLOUR))
            ss << "	oColor0.rgb *= cDiffuseColour.rgb;" << std::endl;
	}
    else
	{
		ss << "	oColor0.rgb = cDiffuseColour.rgb;" << std::endl;
	}
    
	
	ss << "	oColor0.a = cSpecularity;" << std::endl;
	if (IS_BIT_ENABLED(Settings, GBS_NORMAL_MAP))
	{
		ss << "	float3 texNormal = (tex2D(sNormalMap, iUV0)-0.5)*2;" << std::endl;
		ss << "	float3x3 normalRotation = float3x3(iTangent, iBiNormal, iNormal);" << std::endl;
		ss << "	oColor1.rgb = normalize(mul(texNormal, normalRotation));" << std::endl;
	} 
	else
		ss << "	oColor1.rgb = normalize(iNormal);" << std::endl;

#ifdef WRITE_LINEAR_DEPTH
    ss << "	oColor1.a = length(iViewPos) / cFarDistance;" << std::endl;
#else
    ss << "	oColor1.a = iDepth;" << std::endl;
#endif

	ss << "}" << std::endl;
	
	Ogre::String programSource = ss.str();
	Ogre::String programName = m_BaseName + "FP_" + Ogre::StringConverter::toString(Settings);

	// Create shader object
	Ogre::HighLevelGpuProgramPtr program = Ogre::HighLevelGpuProgramManager::getSingleton().createProgram(programName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, "cg", Ogre::GPT_FRAGMENT_PROGRAM);
	program->setSource(programSource);
	program->setParameter("entry_point", "ToGBufferFP");
	program->setParameter("profiles", "ps_2_0 arbfp1");

	const Ogre::GpuProgramParametersSharedPtr &params = program->getDefaultParameters();

	params->setNamedAutoConstant("cSpecularity", Ogre::GpuProgramParameters::ACT_SURFACE_SHININESS);

	if (numTextures == 0 || IS_BIT_ENABLED(Settings, GBS_HAS_DIFFUSE_COLOUR))
		params->setNamedAutoConstant("cDiffuseColour", Ogre::GpuProgramParameters::ACT_SURFACE_DIFFUSE_COLOUR);

#ifdef WRITE_LINEAR_DEPTH
    //TODO : Should this be the distance to the far corner, not the far clip distance?
    params->setNamedAutoConstant("cFarDistance", Ogre::GpuProgramParameters::ACT_FAR_CLIP_DISTANCE);
#endif

	program->load();

	return Ogre::GpuProgramPtr(program);
}