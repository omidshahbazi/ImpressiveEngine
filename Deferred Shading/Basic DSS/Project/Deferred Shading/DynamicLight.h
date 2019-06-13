#pragma once

#include "LightBase.h"
#include "LightMaterialGenerator.h"

class DynamicLight : public LightBase
{
public:
	enum LightType
	{
		LT_DIRECTIONAL = 0,
		LT_POINT,
		LT_SPOTLIGHT
	};

public:
	DynamicLight(Ogre::SceneManager *SceneManager, DSSMaterialGenerator *DSSMaterialGenerator);
	~DynamicLight(void);

	void SetAttenuation(const float &Range, const float &Constant, const float &Linear, const float &Quadratic);
	void SetSpecularColor(const Ogre::ColourValue &Color);

	void Update(Ogre::Camera *Camera/*, const Ogre::LightList *LightList*/);

	float getSquaredViewDepth(const Ogre::Camera *Camera) const;
	void getWorldTransforms(Ogre::Matrix4 *xform) const;
	const Ogre::MaterialPtr &getMaterial(void) const
	{
		return m_Material;
	}

	const bool GetCastChadows(void) const;

	virtual const LightType &GetLightType(void) const = 0;

	Ogre::Light *GetOgreLight(void) const 
	{
		return m_Light;
	}

private:
	void UpdateMaterial(const Ogre::Camera *Camera);

	void UpdateParameters(const Ogre::Camera *Camera);

	void UpdateLightSettings(void);

	virtual const float GetOuterRadius(void) = 0;

	virtual bool IsCameraInside(const Ogre::Camera *Camera) = 0;

	virtual void RebuildGeometry(const float &Radius) = 0;
	
protected:
	Ogre::Light *m_Light;

	unsigned int m_Settings;
	
	bool m_IgnoreWorld;

private:
	DSSMaterialGenerator *m_DSSMaterialGenerator;

	Ogre::MaterialPtr m_Material;

	Ogre::LightList m_LightAsList;
};

