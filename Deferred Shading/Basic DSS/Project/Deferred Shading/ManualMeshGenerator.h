#pragma once

#include <Ogre.h>

class ManualMeshGenerator
{
public:
	static void CreateQuad(Ogre::VertexData *&VertexData);
	static void CreateSphere(Ogre::VertexData*& vertexData, Ogre::IndexData*& indexData, float radius, int nRings, int nSegments, bool bNormals, bool bTexCoords);
	static void CreateCone(Ogre::VertexData*& vertexData, Ogre::IndexData*& indexData, float radius , float height, int nVerticesInBase);
};

