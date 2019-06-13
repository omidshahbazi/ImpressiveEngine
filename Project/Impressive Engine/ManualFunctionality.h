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
//#ifdef EDIT_MODE

#pragma once

//#include "Singleton.h"
#include "StdAfxOGRE.h"
#include "Common.h"

BEGIN_NAMESPACE
class ManualFunctionality/* : public Singleton<ManualFunctionality>*/
{
public:
	//ManualFunctionality(void);
	//~ManualFunctionality(void);

	static void InitializeManualMeshes(void);

	static Ogre::ManualObject *CreateManualObject(Ogre::SceneNode *Parent, const Ogre::String &Name, Ogre::RenderQueueGroupID RenderQueue = Ogre::RENDER_QUEUE_1);

	static void CreateSphere(const Ogre::String &Name, const float Radius, const int Rings, const int Segments, bool Normals, bool TexCoords);

	// Fill up a fresh copy of VertexData and IndexData with a sphere's coords given the number of rings and the number of segments
	static void CreateSphere(Ogre::VertexData *&VertexData, Ogre::IndexData *&IndexData, float Radius, int Rings, int Segments, bool Normals, bool TexCoords);

	static void CreateCone(Ogre::VertexData*& VertexData, Ogre::IndexData*& IndexData, float Radius , float Height, int VerticesInBase);

	// Fill up a fresh copy of VertexData with a normalized quad
	static void CreateQuad(Ogre::VertexData *&VertexData);

	static void AddTriangleList(Ogre::ManualObject *Object, const Ogre::String &Material, std::vector<Ogre::Vector3> VectorList, const Ogre::ColourValue &Color = Ogre::ColourValue::White);
	static void AddTriangleStrip(Ogre::ManualObject *Object, const Ogre::String &Material, std::vector<Ogre::Vector3> VectorList, const Ogre::ColourValue &Color = Ogre::ColourValue::White);
	static void AddTriangleFan(Ogre::ManualObject *Object, const Ogre::String &Material, std::vector<Ogre::Vector3> VectorList, const Ogre::ColourValue &Color = Ogre::ColourValue::White);

	static void AddBox(Ogre::ManualObject *Object, float Width, float Height, float Depth, const Ogre::Vector3 &Offset, const Ogre::Vector3 &Look, const Ogre::String &Material, const Ogre::ColourValue &Color = Ogre::ColourValue::White);
	static void AddLineBox(Ogre::ManualObject *Object, float Length, float Radius, unsigned int Segments, float HeadSize, const Ogre::Vector3 &Offset, const Ogre::Vector3 &Look, const Ogre::String &Material, const Ogre::ColourValue &Color = Ogre::ColourValue::White);
	static void AddArrow(Ogre::ManualObject *Object, float Length, float Radius, float LengthHead, float RadiusHead, unsigned int Segments, const Ogre::Vector3 &Look, const Ogre::String &Material, const Ogre::ColourValue &Color = Ogre::ColourValue::White);
	static void AddCircle(Ogre::ManualObject *Object, float Radius, float Width, unsigned int Segments, const Ogre::Vector3 &Offset, const Ogre::Vector3 &Look, const Ogre::String &Material, const Ogre::ColourValue &Color = Ogre::ColourValue::White);
	static void AddSphere(Ogre::ManualObject *Object, float Radius, unsigned int Rings, unsigned int Segments, const Ogre::String &Material, const Ogre::ColourValue &Color = Ogre::ColourValue::White);

	static void UnloadResource(Ogre::ResourceManager *ResourceManager, const Ogre::String &Name);
	static void LoadResource(Ogre::ResourceManager *ResourceManager, const Ogre::String &Name, const Ogre::String &GroupName);

	static void UnloadMaterial(const Ogre::String &FileName);
	static void UnloadVertexProgram(const Ogre::String &FileName);
	static void UnloadFragmentProgram(const Ogre::String &FileName);
	static void ReloadMaterial(const Ogre::String &Name, const Ogre::String &GroupName, const Ogre::String &Filename, bool ParseMaterialScript);

	static Ogre::MaterialPtr CreateColorMaterial(float R, float G, float B, const Ogre::String &Name, bool ForceDepth);
	static Ogre::MaterialPtr CreateColorMaterial(float R, float G, float B, float A, const Ogre::String &Name, bool ForceDepth);
	static Ogre::MaterialPtr CreateColorMaterial(const Ogre::ColourValue &Color, const Ogre::String &Name, bool ForceDepth);
	
#ifdef EDIT_MODE
	static void MakePassAsDiffuse(Ogre::Pass *Pass);
	static void MakePassAsBumpDiffuse(Ogre::Pass *Pass);
	static void MakePassAsBumpSpecularDiffuse(Ogre::Pass *Pass);
#endif
	
	static void ClearPassMaps(Ogre::SceneManager *SceneManager = 0);
	static void UpdateForMaterialChanges(Ogre::SceneManager *SceneManager = 0);
	
private:
	static void UpdateSceneNodes(Ogre::SceneNode *SceneNode);

public:
	static const char *MANUAL_MESH_PLANE;
	//static const char *MANUAL_MESH_BOX;
	//static const char *MANUAL_MESH_SPHERE;
};
END_NAMESPACE

//#endif