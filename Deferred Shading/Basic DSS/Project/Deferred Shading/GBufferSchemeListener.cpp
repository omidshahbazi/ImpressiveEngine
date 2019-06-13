#include "GBufferSchemeListener.h"
#include "DeferredShadingSystem.h"
#include "Definition.h"


const char *PATTERN_NORMAL_MAP = "normal";


struct PassProperties
{
	PassProperties(void) :
		IsDeferred(true),
		IsSkinned(false),
		HasDiffuseColor(false),
		NormalMap(NULL)
	{
	}

	bool IsDeferred;
	bool IsSkinned;
	bool HasDiffuseColor;

	std::vector<Ogre::TextureUnitState*> RegularTextures;
	Ogre::TextureUnitState *NormalMap;
};


bool CheckNormalMap(Ogre::TextureUnitState *Texture, PassProperties &Properties)
{
	bool isNormal = false;

	Ogre::String text = Texture->getTextureNameAlias();
	Ogre::StringUtil::toLowerCase(text);
	if (text.find(PATTERN_NORMAL_MAP) != Ogre::String::npos)
	{
		isNormal = true;
	}
	else
	{
		text = Texture->getTextureName();
		Ogre::StringUtil::toLowerCase(text);
		if (text.find(PATTERN_NORMAL_MAP) != Ogre::String::npos)
		{
			isNormal = true;
		}
	}

	if (isNormal)
	{
		if (Properties.NormalMap)
		{
			OGRE_EXCEPT(Ogre::Exception::ERR_DUPLICATE_ITEM, "Multiple normal map patterns matches", "GBufferSchemeListener::CheckNormalMap");
		}

		Properties.NormalMap = Texture;
	}

	return isNormal;
}


PassProperties InspectPass(Ogre::Pass *Pass, unsigned short LODIndex, const Ogre::Renderable *Renderable)
{
	PassProperties prop;

	prop.IsSkinned = (Pass->hasVertexProgram() ? Pass->getVertexProgram()->isSkeletalAnimationIncluded() : false);

	Ogre::TextureUnitState *tex = NULL;
	for (unsigned short i = 0; i < Pass->getNumTextureUnitStates(); i++)
	{
		tex = Pass->getTextureUnitState(i);

		if (!CheckNormalMap(tex, prop))
			prop.RegularTextures.push_back(tex);
		
		if (tex->getEffects().size())
			prop.IsDeferred = false;
	}

	prop.HasDiffuseColor = !(Pass->getDiffuse() == Ogre::ColourValue::White);
	
	//Check transparency
	//TODO : Better ways to do this
	if (Pass->getDestBlendFactor() != Ogre::SBF_ZERO)
		prop.IsDeferred = false;

	return prop;
}


unsigned int GetSettings(const PassProperties &Properties)
{
	unsigned int settings = 0;

	switch (Properties.RegularTextures.size())
	{
	case 0:
		{
			ENABLE_BIT(settings, GBufferMaterialGenerator::GBS_NO_TEXTURES);

			if (Properties.NormalMap)
				ENABLE_BIT(settings, GBufferMaterialGenerator::GBS_ONE_TEXCOORD);
			else
				ENABLE_BIT(settings, GBufferMaterialGenerator::GBS_NO_TEXCOORDS);
		} break;
		
	case 1:
		{
			ENABLE_BIT(settings, GBufferMaterialGenerator::GBS_ONE_TEXTURE);
			ENABLE_BIT(settings, GBufferMaterialGenerator::GBS_ONE_TEXCOORD);
		} break;
		
	case 2:
		{
			ENABLE_BIT(settings, GBufferMaterialGenerator::GBS_TWO_TEXTURES);

			//TODO : When do we use two texcoords?
			ENABLE_BIT(settings, GBufferMaterialGenerator::GBS_ONE_TEXCOORD);
		} break;
		
	case 3:
		{
			ENABLE_BIT(settings, GBufferMaterialGenerator::GBS_THREE_TEXTURES);
			ENABLE_BIT(settings, GBufferMaterialGenerator::GBS_ONE_TEXCOORD);
		} break;

	default:
		{
			OGRE_EXCEPT(Ogre::Exception::ERR_NOT_IMPLEMENTED, "Can not generate G-Buffer materials for '>3 Regular-Texture' objects", "GBufferSchemeListener::GetSettings");
		} break;
	}

	if (Properties.NormalMap)
		ENABLE_BIT(settings, GBufferMaterialGenerator::GBS_NORMAL_MAP);

	if (Properties.IsSkinned)
		ENABLE_BIT(settings, GBufferMaterialGenerator::GBS_SKINNED);

	if (Properties.HasDiffuseColor)
		ENABLE_BIT(settings, GBufferMaterialGenerator::GBS_HAS_DIFFUSE_COLOUR);

	return settings;
}


void FillPass(Ogre::Pass *Pass, Ogre::Pass *OriginalPass, const PassProperties &Properties)
{
	//Reference the correct textures. Normal map first!
	unsigned int texIndex = 0;

	if (Properties.NormalMap)
	{
		*Pass->getTextureUnitState(texIndex) = *Properties.NormalMap;
		texIndex++;
	}

	for (unsigned short i = 0; i < Properties.RegularTextures.size(); i++)
	{
		*Pass->getTextureUnitState(texIndex) = *Properties.RegularTextures[i];
		texIndex++;
	}

	Pass->setAmbient(OriginalPass->getAmbient());
	Pass->setDiffuse(OriginalPass->getDiffuse());
	Pass->setSpecular(OriginalPass->getSpecular());
	Pass->setShininess(OriginalPass->getShininess());
	Pass->setCullingMode(OriginalPass->getCullingMode());
	Pass->setLightingEnabled(false);
}


Ogre::Technique *GBufferSchemeListener::handleSchemeNotFound(unsigned short schemeIndex, const Ogre::String &schemeName, Ogre::Material *originalMaterial, unsigned short lodIndex, const Ogre::Renderable *rend)
{
	Ogre::MaterialManager &manager = Ogre::MaterialManager::getSingleton();

	Ogre::String activeScheme = manager.getActiveScheme();
	manager.setActiveScheme(manager.DEFAULT_SCHEME_NAME);

	Ogre::Technique *originalTech = originalMaterial->getBestTechnique(lodIndex, rend);
	manager.setActiveScheme(activeScheme);

	Ogre::Technique *GBufferTech = originalMaterial->createTechnique();
	GBufferTech->removeAllPasses();
	GBufferTech->setSchemeName(DeferredShadingSystem::SCHEME_GBUFFER);

	Ogre::Technique *noGBufferTech = originalMaterial->createTechnique();
	noGBufferTech->removeAllPasses();
	noGBufferTech->setSchemeName(DeferredShadingSystem::SCHEME_NO_GBUFFER);

	Ogre::Pass *originalPass = NULL;
	Ogre::Pass *bufferPass = NULL;
	for (unsigned short i = 0; i < originalTech->getNumPasses(); i++)
	{
		originalPass = originalTech->getPass(i);

		PassProperties prop = InspectPass(originalPass, lodIndex, rend);

		if (prop.IsDeferred)
		{
			bufferPass = GBufferTech->createPass();

			const Ogre::MaterialPtr &material = m_MaterialGenerator.GetMaterial(GetSettings(prop));
			material->load();
			
			//We assume that the GBuffer technique contains only one pass. But its true.
			*bufferPass = *(material->getTechnique(0)->getPass(0));
			FillPass(bufferPass, originalPass, prop);
		}
		else
		{
			//Just copy the technique so it gets rendered regularly
			bufferPass = noGBufferTech->createPass();
			*bufferPass = *originalPass;
		}
	}

	return GBufferTech;
}