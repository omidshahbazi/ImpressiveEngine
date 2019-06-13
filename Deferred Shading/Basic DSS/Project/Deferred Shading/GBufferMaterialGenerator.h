#pragma once

#include "DSSMaterialGenerator.h"


class GBufferMaterialGenerator : public DSSMaterialGenerator
{
public:
	enum GBufferSettings
	{
		//(Regular) Textures
		GBS_NO_TEXTURES = 0x00000000,
		GBS_ONE_TEXTURE = 0x00000001,
		GBS_TWO_TEXTURES = 0x00000002,
		GBS_THREE_TEXTURES = 0x00000003,
		GBS_TEXTURE_MASK = 0x0000000F,
		
		//Material properties
		GBS_HAS_DIFFUSE_COLOUR = 0x00000010,

		//The number of texture coordinate sets
		GBS_NO_TEXCOORDS = 0x00000000,
		GBS_ONE_TEXCOORD = 0x00000100,
		GBS_TWO_TEXCOORDS = 0x00000200,
		GBS_TEXCOORD_MASK = 0x00000700,

		//Do we have a normal map
		GBS_NORMAL_MAP = 0x00000800,

		//Are we skinned?
		GBS_SKINNED = 0x00010000
	};

public:
	GBufferMaterialGenerator(void);

private:
	const Ogre::MaterialPtr GenerateTemplateMaterial(unsigned int Settings);
	const Ogre::GpuProgramPtr GenerateVertexProgram(unsigned int Settings);
	const Ogre::GpuProgramPtr GenerateFragmentProgram(unsigned int Settings);
};