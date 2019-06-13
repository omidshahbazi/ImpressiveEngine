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
#include "DSCommon.h"
#include "Engine.h"
#include "DeferredShadingSystem.h"


BEGIN_NAMESPACE
const char *DSCommon::MAIN_FP_NAME("Main_FP");
const char *DSCommon::MAIN_VP_NAME("Main_VP");

const char *DSCommon::LIGHTING_VP_NAME("LightingVertexProgram");
const char *DSCommon::GEOMETRY_LIGHT_VP_NAME("GeometryLightVertexProgram");

const char *DSCommon::AMBIENT_LIGHT_MATERIAL_NAME("AmbientLightMaterial");
const char *DSCommon::GEOMETRY_LIGHT_MATERIAL_NAME("GeometryLightMaterial");
const char *DSCommon::QUAD_LIGHT_MATERIAL_NAME("QuadLightMaterial");

const unsigned char DSCommon::FIRST_RENDER_QUEUE(Ogre::RENDER_QUEUE_1);
const unsigned char DSCommon::LAST_RENDER_QUEUE(Ogre::RENDER_QUEUE_8 - 1);

const char *DSCommon::GBUFFER_NAME("GBufferCompositor_DSS");
const char *DSCommon::SHOW_LIT_NAME("ShowLitCompositor_DSS");
const char *DSCommon::SHOW_DEPTH_NAME("ShowDepthCompositor_DSS");
const char *DSCommon::SHOW_SPECULAR_NAME("ShowSpecularCompositor_DSS");
const char *DSCommon::SHOW_NORMAL_NAME("ShowNormalCompositor_DSS");
const char *DSCommon::SHOW_ALBEDO_NAME("ShowAlbedoCompositor_DSS");

const char *DSCommon::SCHEME_GBUFFER("GBufferScheme");
const char *DSCommon::SCHEME_NO_GBUFFER("NoGBufferScheme");

const char *DSCommon::DEFERRED_LIGHTING_PASS("DeferredLighting");

Ogre::Vector3 DSCommon::m_FarCorner = Ogre::Vector3::ZERO;


const char *GBBUFFER_TEXTURE_NAME("GBuffer_RT");

const char *SHOW_ALBEDO_FP_NAME("ShowAlbedo_FP");
const char *SHOW_DEPTH_FP_NAME("ShowDepth_FP");
const char *SHOW_NORMAL_FP_NAME("ShowNormal_FP");
const char *SHOW_SPECULAR_FP_NAME("ShowSpecular_FP");

const char *AMBIENT_LIGHT_FP_NAME("AmbientLightFragmetProgram");


const char *SHOW_ALBEDO_MATERIAL_NAME("ShowAlbedoMaterial");
const char *SHOW_DEPTH_MATERIAL_NAME("ShowDepthMaterial");
const char *SHOW_NORMAL_MATERIAL_NAME("ShowNormalMaterial");
const char *SHOW_SPECULAR_MATERIAL_NAME("ShowSpecularMaterial");


Ogre::HighLevelGpuProgramPtr CreateHighLevelGpuProgram(const Ogre::String &Name, const Ogre::StringStream &Source, const Ogre::String &EntryPoint, const Ogre::GpuProgramType &Type, const Ogre::String &Profile = "3_0", const Ogre::String &Language = "hlsl")
{
	Ogre::HighLevelGpuProgramPtr program = Ogre::HighLevelGpuProgramManager::getSingleton().createProgram(Name, Engine::RESOURCES_DEFERREDSHADING, Language, Type);
	program->setSource(Source.str());
	program->setParameter("entry_point", EntryPoint);

	if (Type == Ogre::GPT_FRAGMENT_PROGRAM)
		program->setParameter("target", "ps_" + Profile);
	else if (Type == Ogre::GPT_VERTEX_PROGRAM)
		program->setParameter("target", "vs_" + Profile);

	return program;
}


void CreateHighLevelGpuPrograms(void)
{
	Ogre::StringStream ss;

	ss << "void " << DSCommon::MAIN_FP_NAME << "(" << std::endl;
	ss << "	float2 iTexCoord : TEXCOORD0," << std::endl;
	ss << "	out float4 oColor : COLOR," << std::endl;
	ss << "	uniform sampler2D cRT : register(s0)" << std::endl;
	ss << "	)" << std::endl;

	ss << "{" << std::endl;
	ss << "	oColor = float4(tex2D(cRT, iTexCoord).rgb, 0);" << std::endl;
	ss << "}" << std::endl;

	// These kind of programs, don't work with profiler 3_0
	CreateHighLevelGpuProgram(SHOW_ALBEDO_FP_NAME, ss, DSCommon::MAIN_FP_NAME, Ogre::GPT_FRAGMENT_PROGRAM, "2_0");
	
	ss.str("");
	ss << "void " << DSCommon::MAIN_FP_NAME << "(" << std::endl;
	ss << "	float2 iTexCoord : TEXCOORD0," << std::endl;
	ss << "	out float4 oColor : COLOR," << std::endl;
	ss << "	uniform sampler2D cRT : register(s0)" << std::endl;
	ss << "	)" << std::endl;

	ss << "{" << std::endl;
	ss << "	oColor = tex2D(cRT, iTexCoord).a;" << std::endl;
	ss << "}" << std::endl;

	CreateHighLevelGpuProgram(SHOW_DEPTH_FP_NAME, ss, DSCommon::MAIN_FP_NAME, Ogre::GPT_FRAGMENT_PROGRAM, "2_0");
	
	ss.str("");
	ss << "void " << DSCommon::MAIN_FP_NAME << "(" << std::endl;
	ss << "	float2 iTexCoord : TEXCOORD0," << std::endl;
	ss << "	out float4 oColor : COLOR," << std::endl;
	ss << "	uniform sampler2D cRT : register(s0)" << std::endl;
	ss << "	)" << std::endl;

	ss << "{" << std::endl;
	ss << "	oColor = float4(tex2D(cRT, iTexCoord).rgb, 0);" << std::endl;
	ss << "}" << std::endl;

	CreateHighLevelGpuProgram(SHOW_NORMAL_FP_NAME, ss, DSCommon::MAIN_FP_NAME, Ogre::GPT_FRAGMENT_PROGRAM, "2_0");
	
	ss.str("");
	ss << "void " << DSCommon::MAIN_FP_NAME << "(" << std::endl;
	ss << "	float2 iTexCoord : TEXCOORD0," << std::endl;
	ss << "	out float4 oColor : COLOR," << std::endl;
	ss << "	uniform sampler2D cRT : register(s0)" << std::endl;
	ss << "	)" << std::endl;

	ss << "{" << std::endl;
	ss << "	oColor = tex2D(cRT, iTexCoord).a;" << std::endl;
	ss << "}" << std::endl;

	CreateHighLevelGpuProgram(SHOW_SPECULAR_FP_NAME, ss, DSCommon::MAIN_FP_NAME, Ogre::GPT_FRAGMENT_PROGRAM, "2_0");
	
	ss.str("");
	ss << "void " << DSCommon::MAIN_VP_NAME << "(" << std::endl;
	ss << "	float4 iPosition : POSITION," << std::endl;
	ss << "	out float4 oPosition : POSITION," << std::endl;
	ss << "	out float2 oTexCoord : TEXCOORD0," << std::endl;
	ss << "	out float3 oRay : TEXCOORD1," << std::endl;
	ss << "	uniform float3 cFarCorner," << std::endl;
	ss << "	uniform float cFlip" << std::endl;
	ss << "	)" << std::endl;

	ss << "{" << std::endl;
	
	// Clean up inaccuracies
	ss << "	iPosition.xy = sign(iPosition.xy);" << std::endl;
	
	ss << "	oPosition = float4(iPosition.xy, 0, 1);" << std::endl;
	ss << "	oPosition.y *= cFlip;" << std::endl;

	// Image-space
	ss << "	oTexCoord.x = 0.5 * (1 + iPosition.x);" << std::endl;
	ss << "	oTexCoord.y = 0.5 * (1 - iPosition.y);" << std::endl;
	
	// This ray will be interpolated and will be the ray from the camera to the far clip plane, per pixel !!!
	ss << "	oRay = cFarCorner * float3(iPosition.xy, 1);" << std::endl;
	ss << "}" << std::endl;

	Ogre::HighLevelGpuProgramPtr program = CreateHighLevelGpuProgram(DSCommon::LIGHTING_VP_NAME, ss, DSCommon::MAIN_VP_NAME, Ogre::GPT_VERTEX_PROGRAM);
	Ogre::GpuProgramParametersSharedPtr params = program->getDefaultParameters();
	//params->setNamedAutoConstant("invProj", Ogre::GpuProgramParameters::ACT_INVERSE_PROJECTION_MATRIX);
	params->setNamedAutoConstant("cFlip", Ogre::GpuProgramParameters::ACT_RENDER_TARGET_FLIPPING);
	params->setNamedConstant("cFarCorner", Ogre::Vector3(1, 1, 1));
	
	ss.str("");
	ss << "float FinalDepth(float4 value)" << std::endl;
	ss << "{" << std::endl;
	ss << "	return value.z / value.w;" << std::endl;
	ss << "}" << std::endl;
	ss << "void " << DSCommon::MAIN_FP_NAME << "(" << std::endl;
	ss << "	float2 iTexCoord : TEXCOORD0," << std::endl;
	ss << "	float3 iRay : TEXCOORD1," << std::endl;
	ss << "	out float4 oColor : COLOR," << std::endl;
	ss << "	out float oDepth : DEPTH," << std::endl;
	ss << "	uniform sampler cTex0: register(s0)," << std::endl;
	ss << "	uniform sampler cTex1: register(s1)," << std::endl;
	ss << "	uniform float4 cAmbientColor," << std::endl;
	ss << "	uniform float4x4 cProjection," << std::endl;
	ss << "	uniform float cFarClipDistance" << std::endl;
	ss << "	)" << std::endl;

	ss << "{" << std::endl;
	
	// Depth + Normal
	ss << "	half4 a0 = tex2D(cTex0, iTexCoord);" << std::endl;

	// Specular + Diffuse Color
	ss << "	half4 a1 = tex2D(cTex1, iTexCoord);" << std::endl;
	
	// Clip fragment if depth is too close, so the skybox can be rendered on the background
	ss << "	clip(a0.a - 0.0001);" << std::endl;
	
	// Calculate Ambient-color
	ss << "	oColor = float4(cAmbientColor * float4(a1.rgb, 0));" << std::endl;
	
	// Calculate Depth
	ss << "	float3 viewPos = normalize(iRay) * cFarClipDistance * a0.a;" << std::endl;
	ss << "	float4 projPos = mul(cProjection, float4(viewPos, 1));" << std::endl;
	ss << "	oDepth = FinalDepth(projPos);" << std::endl;
	ss << "}" << std::endl;

	program = CreateHighLevelGpuProgram(AMBIENT_LIGHT_FP_NAME, ss, DSCommon::MAIN_FP_NAME, Ogre::GPT_FRAGMENT_PROGRAM);
	params = program->getDefaultParameters();	
	params->setNamedAutoConstant("cAmbientColor", Ogre::GpuProgramParameters::ACT_AMBIENT_LIGHT_COLOUR);
	params->setNamedAutoConstant("cProjection", Ogre::GpuProgramParameters::ACT_PROJECTION_MATRIX);
	params->setNamedAutoConstant("cFarClipDistance", Ogre::GpuProgramParameters::ACT_FAR_CLIP_DISTANCE);
	
	ss.str("");
	ss << "void " << DSCommon::MAIN_VP_NAME << "(" << std::endl;
	ss << "	float4 iPosition : POSITION," << std::endl;
	ss << "	out float4 oPosition : POSITION," << std::endl;
	ss << "	out float4 oTexCoord : TEXCOORD0," << std::endl;
	ss << "	uniform float4x4 cWorldViewProj" << std::endl;
	ss << "	)" << std::endl;

	ss << "{" << std::endl;
	ss << "	float4 projPos = mul(cWorldViewProj, iPosition);" << std::endl;
	ss << "	oTexCoord = projPos;" << std::endl;
	ss << "	oPosition = projPos;" << std::endl;
	ss << "}" << std::endl;

	program = CreateHighLevelGpuProgram(DSCommon::GEOMETRY_LIGHT_VP_NAME, ss, DSCommon::MAIN_VP_NAME, Ogre::GPT_VERTEX_PROGRAM);
	params = program->getDefaultParameters();	
	params->setNamedAutoConstant("cWorldViewProj", Ogre::GpuProgramParameters::ACT_WORLDVIEWPROJ_MATRIX);
}


void CreateMaterials(void)
{
	Ogre::MaterialManager &manager = Ogre::MaterialManager::getSingleton();

	Ogre::Pass *pass = NULL;
	Ogre::TextureUnitState *texture = NULL;

	Ogre::MaterialPtr material = manager.create(SHOW_ALBEDO_MATERIAL_NAME, Engine::RESOURCES_DEFERREDSHADING);
	{
		pass = material->getTechnique(0)->getPass(0);
		{
			pass->setCullingMode(Ogre::CULL_NONE);
			pass->setManualCullingMode(Ogre::MANUAL_CULL_NONE);
			pass->setDepthCheckEnabled(false);
			pass->setFragmentProgram(SHOW_ALBEDO_FP_NAME);

			texture = pass->createTextureUnitState();
			{
				texture->setTextureFiltering(Ogre::TFO_NONE);
			}
		}
	}

	material = manager.create(SHOW_DEPTH_MATERIAL_NAME, Engine::RESOURCES_DEFERREDSHADING);
	{
		pass = material->getTechnique(0)->getPass(0);
		{
			pass->setCullingMode(Ogre::CULL_NONE);
			pass->setManualCullingMode(Ogre::MANUAL_CULL_NONE);
			pass->setDepthCheckEnabled(false);
			pass->setFragmentProgram(SHOW_DEPTH_FP_NAME);

			texture = pass->createTextureUnitState();
			{
				texture->setTextureFiltering(Ogre::TFO_NONE);
			}
		}
	}

	material = manager.create(SHOW_NORMAL_MATERIAL_NAME, Engine::RESOURCES_DEFERREDSHADING);
	{
		pass = material->getTechnique(0)->getPass(0);
		{
			pass->setCullingMode(Ogre::CULL_NONE);
			pass->setManualCullingMode(Ogre::MANUAL_CULL_NONE);
			pass->setDepthCheckEnabled(false);
			pass->setFragmentProgram(SHOW_NORMAL_FP_NAME);

			texture = pass->createTextureUnitState();
			{
				texture->setTextureFiltering(Ogre::TFO_NONE);
			}
		}
	}

	material = manager.create(SHOW_SPECULAR_MATERIAL_NAME, Engine::RESOURCES_DEFERREDSHADING);
	{
		pass = material->getTechnique(0)->getPass(0);
		{
			pass->setCullingMode(Ogre::CULL_NONE);
			pass->setManualCullingMode(Ogre::MANUAL_CULL_NONE);
			pass->setDepthCheckEnabled(false);
			pass->setFragmentProgram(SHOW_SPECULAR_FP_NAME);

			texture = pass->createTextureUnitState();
			{
				texture->setTextureFiltering(Ogre::TFO_NONE);
			}
		}
	}

	material = manager.create(DSCommon::AMBIENT_LIGHT_MATERIAL_NAME, Engine::RESOURCES_DEFERREDSHADING);
	{
		pass = material->getTechnique(0)->getPass(0);
		{
			pass->setLightingEnabled(false);
			pass->setDepthWriteEnabled(true);
			pass->setDepthCheckEnabled(true);
			pass->setVertexProgram(DSCommon::LIGHTING_VP_NAME);
			pass->setFragmentProgram(AMBIENT_LIGHT_FP_NAME);

			texture = pass->createTextureUnitState();
			{
				texture->setContentType(Ogre::TextureUnitState::CONTENT_COMPOSITOR);
				texture->setCompositorReference(DSCommon::GBUFFER_NAME, GBBUFFER_TEXTURE_NAME, 0);
				texture->setTextureFiltering(Ogre::TFO_NONE);
			}

			texture = pass->createTextureUnitState();
			{
				texture->setContentType(Ogre::TextureUnitState::CONTENT_COMPOSITOR);
				texture->setCompositorReference(DSCommon::GBUFFER_NAME, GBBUFFER_TEXTURE_NAME, 1);
				texture->setTextureFiltering(Ogre::TFO_NONE);
			}
		}
	}

	material = manager.create(DSCommon::GEOMETRY_LIGHT_MATERIAL_NAME, Engine::RESOURCES_DEFERREDSHADING);
	{
		pass = material->getTechnique(0)->getPass(0);
		{
			pass->setSceneBlendingOperation(Ogre::SBO_ADD);
			pass->setLightingEnabled(false);
			pass->setDepthWriteEnabled(false);
			pass->setDepthCheckEnabled(true);
			pass->setVertexProgram(DSCommon::LIGHTING_VP_NAME);

			texture = pass->createTextureUnitState();
			{
				texture->setContentType(Ogre::TextureUnitState::CONTENT_COMPOSITOR);
				texture->setCompositorReference(DSCommon::GBUFFER_NAME, GBBUFFER_TEXTURE_NAME, 0);
				texture->setTextureFiltering(Ogre::TFO_NONE);
				//texture->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP);
			}

			texture = pass->createTextureUnitState();
			{
				texture->setContentType(Ogre::TextureUnitState::CONTENT_COMPOSITOR);
				texture->setCompositorReference(DSCommon::GBUFFER_NAME, GBBUFFER_TEXTURE_NAME, 1);
				texture->setTextureFiltering(Ogre::TFO_NONE);
				//texture->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP);
			}
		}
	}

	material = material->clone(DSCommon::QUAD_LIGHT_MATERIAL_NAME);
	{
		pass = material->getTechnique(0)->getPass(0);
		{
			pass->setDepthCheckEnabled(false);
		}
	}
}


void CreateCompositors(void)
{
	Ogre::CompositorManager &manager = Ogre::CompositorManager::getSingleton();
	Ogre::MaterialManager &materialManager = Ogre::MaterialManager::getSingleton();

	Ogre::CompositionTechnique *technique = NULL;
	Ogre::CompositionTechnique::TextureDefinition *texture = NULL;
	Ogre::CompositionTargetPass *targetPass = NULL;
	Ogre::CompositionPass *pass = NULL;

	const char *Temp_Texture_Name = "Temp_RT";

	Ogre::CompositorPtr compositor = manager.create(DSCommon::GBUFFER_NAME, Engine::RESOURCES_DEFERREDSHADING);
	{
		technique = compositor->createTechnique();
		{
			texture = technique->createTextureDefinition(GBBUFFER_TEXTURE_NAME);
			texture->formatList.push_back(Ogre::PF_FLOAT16_RGBA);
			texture->formatList.push_back(Ogre::PF_FLOAT16_RGBA);
			texture->fsaa = false;
			texture->scope = Ogre::CompositionTechnique::TS_CHAIN;

			targetPass = technique->createTargetPass();
			{
				targetPass->setOutputName(GBBUFFER_TEXTURE_NAME);
				targetPass->setInputMode(Ogre::CompositionTargetPass::IM_NONE);
				targetPass->setShadowsEnabled(false);
				targetPass->setMaterialScheme(DSCommon::SCHEME_GBUFFER);

				pass = targetPass->createPass();
				{
					pass->setType(Ogre::CompositionPass::PT_CLEAR);
					//pass->setClearColour(
				}

				pass = targetPass->createPass();
				{
					pass->setType(Ogre::CompositionPass::PT_RENDERSCENE);
					pass->setFirstRenderQueue(DSCommon::FIRST_RENDER_QUEUE);
					pass->setLastRenderQueue(DSCommon::LAST_RENDER_QUEUE);
				}
			}
		}
	}

	compositor = manager.create(DSCommon::SHOW_ALBEDO_NAME, Engine::RESOURCES_DEFERREDSHADING);
	{
		technique = compositor->createTechnique();
		{
			texture = technique->createTextureDefinition(Temp_Texture_Name);
			texture->refCompName = DSCommon::GBUFFER_NAME;
			texture->refTexName = GBBUFFER_TEXTURE_NAME;

			targetPass = technique->getOutputTargetPass();
			{
				targetPass->setInputMode(Ogre::CompositionTargetPass::IM_NONE);

				pass = targetPass->createPass();
				{
					pass->setType(Ogre::CompositionPass::PT_RENDERQUAD);
					pass->setMaterial(materialManager.getByName(SHOW_ALBEDO_MATERIAL_NAME));
					pass->setInput(0, Temp_Texture_Name, 1);
				}
			}
		}
	}

	compositor = manager.create(DSCommon::SHOW_DEPTH_NAME, Engine::RESOURCES_DEFERREDSHADING);
	{
		technique = compositor->createTechnique();
		{
			texture = technique->createTextureDefinition(Temp_Texture_Name);
			texture->refCompName = DSCommon::GBUFFER_NAME;
			texture->refTexName = GBBUFFER_TEXTURE_NAME;

			targetPass = technique->getOutputTargetPass();
			{
				targetPass->setInputMode(Ogre::CompositionTargetPass::IM_NONE);

				pass = targetPass->createPass();
				{
					pass->setType(Ogre::CompositionPass::PT_RENDERQUAD);
					pass->setMaterial(materialManager.getByName(SHOW_DEPTH_MATERIAL_NAME));
					pass->setInput(0, Temp_Texture_Name, 0);
				}
			}
		}
	}

	compositor = manager.create(DSCommon::SHOW_LIT_NAME, Engine::RESOURCES_DEFERREDSHADING);
	{
		technique = compositor->createTechnique();
		{
			texture = technique->createTextureDefinition(Temp_Texture_Name);
			texture->refCompName = DSCommon::GBUFFER_NAME;
			texture->refTexName = GBBUFFER_TEXTURE_NAME;

			targetPass = technique->getOutputTargetPass();
			{
				targetPass->setInputMode(Ogre::CompositionTargetPass::IM_NONE);

				pass = targetPass->createPass();
				{
					pass->setType(Ogre::CompositionPass::PT_CLEAR);
				}
				
				// Render skies and other Pre-GBuffer objects
				pass = targetPass->createPass();
				{
					pass->setType(Ogre::CompositionPass::PT_RENDERSCENE);
					pass->setFirstRenderQueue(Ogre::RENDER_QUEUE_BACKGROUND);
					pass->setLastRenderQueue(DSCommon::FIRST_RENDER_QUEUE - 1);
				}
				
				// Render the lights and their meshes
				pass = targetPass->createPass();
				{
					pass->setType(Ogre::CompositionPass::PT_RENDERCUSTOM);
					pass->setCustomType(DSCommon::DEFERRED_LIGHTING_PASS);
				}

				// Render the objects that skipped rendering into the GBuffer
				pass = targetPass->createPass();
				{
					pass->setType(Ogre::CompositionPass::PT_RENDERSCENE);
					pass->setMaterialScheme(DSCommon::SCHEME_NO_GBUFFER);
					pass->setFirstRenderQueue(DSCommon::FIRST_RENDER_QUEUE);
					pass->setLastRenderQueue(DSCommon::LAST_RENDER_QUEUE);
				}
				
				// Render the Post-GBuffer render queue objects
				pass = targetPass->createPass();
				{
					pass->setType(Ogre::CompositionPass::PT_RENDERSCENE);
					pass->setFirstRenderQueue(DSCommon::LAST_RENDER_QUEUE + 1);
				}
			}
		}
	}

	compositor = manager.create(DSCommon::SHOW_NORMAL_NAME, Engine::RESOURCES_DEFERREDSHADING);
	{
		technique = compositor->createTechnique();
		{
			texture = technique->createTextureDefinition(Temp_Texture_Name);
			texture->refCompName = DSCommon::GBUFFER_NAME;
			texture->refTexName = GBBUFFER_TEXTURE_NAME;

			targetPass = technique->getOutputTargetPass();
			{
				targetPass->setInputMode(Ogre::CompositionTargetPass::IM_NONE);

				pass = targetPass->createPass();
				{
					pass->setType(Ogre::CompositionPass::PT_RENDERQUAD);
					pass->setMaterial(materialManager.getByName(SHOW_NORMAL_MATERIAL_NAME));
					pass->setInput(0, Temp_Texture_Name, 0);
				}
			}
		}
	}

	compositor = manager.create(DSCommon::SHOW_SPECULAR_NAME, Engine::RESOURCES_DEFERREDSHADING);
	{
		technique = compositor->createTechnique();
		{
			texture = technique->createTextureDefinition(Temp_Texture_Name);
			texture->refCompName = DSCommon::GBUFFER_NAME;
			texture->refTexName = GBBUFFER_TEXTURE_NAME;

			targetPass = technique->getOutputTargetPass();
			{
				targetPass->setInputMode(Ogre::CompositionTargetPass::IM_NONE);

				pass = targetPass->createPass();
				{
					pass->setType(Ogre::CompositionPass::PT_RENDERQUAD);
					pass->setMaterial(materialManager.getByName(SHOW_SPECULAR_MATERIAL_NAME));
					pass->setInput(0, Temp_Texture_Name, 1);
				}
			}
		}
	}
}


void DSCommon::CreateResources(void)
{
	Ogre::ResourceGroupManager::getSingleton().createResourceGroup(Engine::RESOURCES_DEFERREDSHADING);

	CreateHighLevelGpuPrograms();

	CreateMaterials();

	CreateCompositors();
}


void DSCommon::UpdateParameters(const Ogre::Camera *Camera)
{
	m_FarCorner = Camera->getViewMatrix(true) * Camera->getWorldSpaceCorners()[4];
}
END_NAMESPACE