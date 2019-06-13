#pragma once

#include <vector>
#include <list>

#include "Ogre.h"
#include "PxPhysicsAPI.h"

using namespace Ogre;
using namespace physx;

class MeshParams
{
public:
	Vector3 Scale;
	bool AddBackfaces;

	MeshParams(void) : Scale(1, 1, 1), AddBackfaces(false) {};
	~MeshParams(void) {};
};

class MeshInfo
{
private:
	struct STri
	{
		STri(){i1=-1;i2=-1;i3=-1;}
		STri(int iIndex1, int iIndex2, int iIndex3, Ogre::String material, bool bSort=true)
		{
			if(!bSort)
			{
				i1=iIndex1;
				i2=iIndex2;
				i3=iIndex3;
				return;
			}

			//rotate indices
			if(iIndex2<iIndex1)
			{
				if(iIndex3<iIndex2)
				{//index 3 is the smallest
					i1=iIndex3;
					i2=iIndex1;
					i3=iIndex2;
				}
				else
				{
					i1=iIndex2;
					i2=iIndex3;
					i3=iIndex1;
				}
			}
			else
			{
				i1=iIndex1;
				i2=iIndex2;
				i3=iIndex3;
			}
			mat = material;
		}

		bool operator !=(STri& op){if(op.i1!=i1 || op.i2!=i2 || op.i3!=i3) return true; return false;}
		bool operator <(STri& op)
		{
			if(op.i1 != i1)
			{
				return i1 < op.i1;
			}
			if(op.i2 != i2)
			{
				return i2 < op.i2;
			}

			return i3 < op.i3;
		}

		int i1, i2, i3;

		Ogre::String mat;
	};

	struct OctreeNode
	{
		OctreeNode(){vPos.x=0.0f;vPos.y=0.0f;vPos.z=0.0f;
		aSubNodes[0]=0;aSubNodes[1]=0;aSubNodes[2]=0;aSubNodes[3]=0;aSubNodes[4]=0;aSubNodes[5]=0;aSubNodes[6]=0;aSubNodes[7]=0;}
		Vector3 vPos;
		OctreeNode* aSubNodes[8];
		std::list<int> liIndices;
	};

	MeshPtr m_Mesh;
	MeshParams m_Params;
	bool m_ProcessIndices;

public:
	std::vector<Vector3> Vertices;
	std::vector<Vector3> Normals;
	std::vector<int> Indices;
	std::vector<String> Materials;

public:
	MeshInfo(MeshPtr mesh, MeshParams &Params, const bool ProcessIndices = false);
	~MeshInfo(void);

	void ProcessVertexData(unsigned int &numVertices, unsigned int &numIndices);

	void ProcessPositionElement(const unsigned int &sharedIndeces);

	void ProcessIndexData(SubMesh* subMesh, const unsigned int &sharedIndeces);

	void MergeVertices(float fMergeDist = 1e-3f);

	unsigned int ExtractOctree(OctreeNode* pNode, unsigned int iVertexCount, int* aiIndexTable, Vector3* aNewVertices);

	void RemoveDuplicatedVertices(void);

	PxVec3* GetVerticesVectorPtr(void);

	PxVec3* GetNormalsVectorPtr(void);

	float* GetVerticesFloatPtr(void);

	unsigned int* GetIndicesUIntPtr(void);
};

