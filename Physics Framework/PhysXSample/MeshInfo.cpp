#include "MeshInfo.h"


MeshInfo::MeshInfo(const MeshPtr Mesh, MeshParams &Params, const bool ProcessIndices) : m_Mesh(Mesh),
																						m_Params(Params),
																						m_ProcessIndices(ProcessIndices)
{
	unsigned int numVertices = 0;
	unsigned int numIndices = 0;

	ProcessVertexData(numVertices, numIndices);

	Vertices.resize(numVertices);
	Normals.resize(numVertices);
	Indices.resize(numIndices);
	Materials.resize(numIndices / 3);

	ProcessPositionElement(numIndices);

	if (m_ProcessIndices)
	{
		MergeVertices(0.1f);
	}

	RemoveDuplicatedVertices();
}


MeshInfo::~MeshInfo(void)
{
}


void MeshInfo::ProcessVertexData(unsigned int &numVertices, unsigned int &numIndices)
{
	bool added_shared = false;

	for (int i = 0; i < m_Mesh->getNumSubMeshes(); i++)
	{
		SubMesh* submesh = m_Mesh->getSubMesh(i);

		if (submesh->useSharedVertices)
		{
			if (!added_shared)
			{
				numVertices += m_Mesh->sharedVertexData->vertexCount;
				added_shared = true;
			}
		}
		else if (submesh->vertexData)
		{
			numVertices += submesh->vertexData->vertexCount;
		}

		if (m_Params.AddBackfaces)
		{
			numIndices += submesh->indexData->indexCount * 2;
		}
		else
		{
			numIndices += submesh->indexData->indexCount;
		}
	}
}


void MeshInfo::ProcessPositionElement(const unsigned int &sharedIndeces)
{
	unsigned int addedVertices = 0;

	VertexData *SharedVertexDate = m_Mesh->sharedVertexData;

	if (SharedVertexDate)
	{
		const VertexElement* posElem = SharedVertexDate->vertexDeclaration->findElementBySemantic(VES_POSITION);
		HardwareVertexBufferSharedPtr buffer = SharedVertexDate->vertexBufferBinding->getBuffer(posElem->getSource());
		unsigned char* pVertices = static_cast<unsigned char*>(buffer->lock(HardwareBuffer::HBL_READ_ONLY));

		float* pFloat;

		for (unsigned int i = 0; i < SharedVertexDate->vertexCount; i++)
		{
			posElem->baseVertexPointerToElement(pVertices, &pFloat);

			Vector3 vec = Vector3(0, 0, 0);
			vec.x = (*pFloat++) * m_Params.Scale.x;
			vec.y = (*pFloat++) * m_Params.Scale.y;
			vec.z = (*pFloat++) * m_Params.Scale.z;
			Vertices[i] = vec;

			pVertices += buffer->getVertexSize();
		}

		buffer->unlock();

		//const VertexElement* normElem = SharedVertexDate->vertexDeclaration->findElementBySemantic(VES_NORMAL);
		//HardwareVertexBufferSharedPtr normBuffer = SharedVertexDate->vertexBufferBinding->getBuffer(normElem->getSource());
		//unsigned char* pNormals = static_cast<unsigned char*>(buffer->lock(HardwareBuffer::HBL_NORMAL));

		//float* pNormFloat;

		//for (unsigned int i = 0; i < SharedVertexDate->vertexCount; i++)
		//{
		//	normElem->baseVertexPointerToElement(pVertices, &pNormFloat);

		//	Vector3 vec = Vector3(0, 0, 0);
		//	vec.x = (*pFloat++);
		//	vec.y = (*pFloat++);
		//	vec.z = (*pFloat++);
		//	Normals[i] = vec;

		//	pNormals += buffer->getVertexSize();
		//}

		//normBuffer->unlock();

		addedVertices += SharedVertexDate->vertexCount;
	}

	for (unsigned int i = 0; i < m_Mesh->getNumSubMeshes(); i++)
	{
		SubMesh* submesh = m_Mesh->getSubMesh(i);

		VertexData *VertexData = submesh->vertexData;

		if (VertexData)
		{
			const VertexElement* posElem = VertexData->vertexDeclaration->findElementBySemantic(VES_POSITION);

			HardwareVertexBufferSharedPtr buffer = VertexData->vertexBufferBinding->getBuffer(posElem->getSource());

			unsigned char* pVertices = static_cast<unsigned char*>(buffer->lock(HardwareBuffer::HBL_READ_ONLY));

			float* pFloat;

			for (unsigned int x = addedVertices; x < addedVertices + VertexData->vertexCount; x++)
			{
				posElem->baseVertexPointerToElement(pVertices, &pFloat);

				Vector3 vec = Vector3(0, 0, 0);
				vec.x = (*pFloat++) * m_Params.Scale.x;
				vec.y = (*pFloat++) * m_Params.Scale.y;
				vec.z = (*pFloat++) * m_Params.Scale.z;
				Vertices[x] = vec;

				pVertices += buffer->getVertexSize();
			}

			buffer->unlock();

			//const VertexElement* normElem = VertexData->vertexDeclaration->findElementBySemantic(VES_NORMAL);
			//HardwareVertexBufferSharedPtr normBuffer = VertexData->vertexBufferBinding->getBuffer(normElem->getSource());
			//unsigned char* pNormals = static_cast<unsigned char*>(buffer->lock(HardwareBuffer::HBL_NORMAL));

			//float* pNormFloat;

			////for (unsigned int i = 0; i < VertexData->vertexCount; i++)
			////{
			////	normElem->baseVertexPointerToElement(pVertices, &pNormFloat);

			////	Vector3 vec = Vector3(0, 0, 0);
			////	vec.x = (*pFloat++);
			////	vec.y = (*pFloat++);
			////	vec.z = (*pFloat++);
			////	Normals[i] = vec;

			////	pNormals += buffer->getVertexSize();
			////}

			//normBuffer->unlock();

			addedVertices += submesh->vertexData->vertexCount;
		}

		if (m_ProcessIndices)
		{
			ProcessIndexData(submesh, sharedIndeces);
		}
	}
}


void MeshInfo::ProcessIndexData(SubMesh* subMesh, const unsigned int &sharedIndeces)
{
	unsigned int addedIndeces = 0;
	unsigned int addedMaterialIndices = 0;

	IndexData *IndexData = subMesh->indexData;

	if (IndexData)
	{
		HardwareIndexBufferSharedPtr buffer = IndexData->indexBuffer;

		unsigned int *pIndices;

		if (IndexData->indexBuffer->getType() == HardwareIndexBuffer::IT_32BIT)
		{
			pIndices = static_cast<unsigned int*>(buffer->lock(HardwareBuffer::HBL_READ_ONLY));
		}
		else
		{
			unsigned short* pShortIndices = static_cast<unsigned short*>(buffer->lock(HardwareBuffer::HBL_READ_ONLY));

			pIndices = new unsigned int[IndexData->indexCount];

			for (unsigned int i = 0; i < IndexData->indexCount; i++)
			{
				pIndices[i] = static_cast<unsigned int>(pShortIndices[i]);
			}
		}

		unsigned int bufferIndex = 0;
		unsigned indexOffset = 0;

		if (m_Params.AddBackfaces)
		{
			unsigned int numTris = IndexData->indexCount / 3;
			unsigned int x = addedIndeces;

			for (unsigned int i = 0; i < numTris; i++)
			{
				if (subMesh->useSharedVertices)
				{
					if (pIndices[bufferIndex] > x)
					{
						Indices[x] = pIndices[bufferIndex] + indexOffset;
					}
					else
					{
						Indices[x] = pIndices[bufferIndex];
					}
					bufferIndex++;
					if (pIndices[bufferIndex] > x)
					{
						Indices[x + 1] = pIndices[bufferIndex] + indexOffset;
					}
					else
					{
						Indices[x + 1] = pIndices[bufferIndex];
					}
					bufferIndex++;
					if (pIndices[bufferIndex] > x)
					{
						Indices[x + 2] = pIndices[bufferIndex] + indexOffset;
					}
					else
					{
						Indices[x + 2] = pIndices[bufferIndex];
					}
					bufferIndex++;
				}
				else
				{
					Indices[x] = pIndices[bufferIndex] + indexOffset;
					bufferIndex++;

					Indices[x + 1] = pIndices[bufferIndex] + indexOffset;
					bufferIndex++;

					Indices[x + 2] = pIndices[bufferIndex] + indexOffset;
					bufferIndex++;
				}
				
				Indices[x + 3] = Indices[x + 2];
				Indices[x + 4] = Indices[x + 1];
				Indices[x + 5] = Indices[x];

				x += 6;
			}

			addedIndeces += IndexData->indexCount * 2;
		}
		else
		{
			for (unsigned int i = addedIndeces; i < addedIndeces + IndexData->indexCount; i++)
			{
				if (subMesh->useSharedVertices)
				{
					if (pIndices[bufferIndex] > indexOffset)
					{
						Indices[i] = pIndices[bufferIndex] + indexOffset;
					}
					else
					{
						Indices[i] = pIndices[bufferIndex];
					}
				}
				else
				{
					Indices[i] = pIndices[bufferIndex] + indexOffset;
				}

				bufferIndex++;
			}

			addedIndeces += IndexData->indexCount;

			if (IndexData->indexBuffer->getType() != HardwareIndexBuffer::IT_32BIT)
			{
				delete[] pIndices;
			}

			buffer->unlock();

			unsigned int numTris = IndexData->indexCount / 3;

			if (m_Params.AddBackfaces)
			{
				numTris *= 2;
			}

			for (unsigned int i = addedMaterialIndices; i < addedMaterialIndices + numTris; i++)
			{
				Materials[i] = subMesh->getMaterialName();
			}

			addedMaterialIndices += numTris;
		}

		if(subMesh->vertexData)
		{
			indexOffset += subMesh->vertexData->vertexCount;
		}
	}
}



unsigned int MeshInfo::ExtractOctree(OctreeNode* pNode, unsigned int iVertexCount, int* aiIndexTable, Vector3* aNewVertices)
{
	for (std::list<int>::const_iterator it = pNode->liIndices.begin(); it != pNode->liIndices.end(); it++)
	{
		aiIndexTable[*it] = iVertexCount;
		aNewVertices[iVertexCount++] = pNode->vPos;
		for (int iSubNode = 0; iSubNode < 8; iSubNode++)
		{
			if (pNode->aSubNodes[iSubNode])
			{
				iVertexCount = ExtractOctree(pNode->aSubNodes[iSubNode], iVertexCount, aiIndexTable, aNewVertices);
				delete pNode->aSubNodes[iSubNode];
				pNode->aSubNodes[iSubNode] = nullptr;
			}
		}
	}

	return iVertexCount;
}


#define IS_IN_BOX(v1,v2,d) ((v1.x<=v2.x+d) && (v1.x>=v2.x-d) && (v1.y<=v2.y+d) && (v1.y>=v2.y-d) && (v1.z<=v2.z+d) && (v1.z>=v2.z-d))

#define EIGHTH_SPACE_INDEX(v1,v2) (((v1.x>v2.x)?4:0)+((v1.y>v2.y)?2:0)+((v1.z>v2.z)?1:0))

void MeshInfo::MergeVertices(float fMergeDist)
{
	OctreeNode root;
	root.vPos = Vertices[0];
	unsigned int iVertex = 0;
	unsigned int numAdded = 0;

	for (; iVertex < Vertices.size(); iVertex++)
	{
		OctreeNode* pCurrentNode = &root;
		
		while (true)
		{
			if (IS_IN_BOX(Vertices[iVertex], pCurrentNode->vPos, fMergeDist))
			{
				pCurrentNode->liIndices.push_back(iVertex);
				break;
			}
			else
			{
				int iSubNode = EIGHTH_SPACE_INDEX(pCurrentNode->vPos, Vertices[iVertex]);

				if (pCurrentNode->aSubNodes[iSubNode])
				{
					pCurrentNode = pCurrentNode->aSubNodes[iSubNode];
				}
				else
				{
					pCurrentNode->aSubNodes[iSubNode] = new OctreeNode();
					pCurrentNode = pCurrentNode->aSubNodes[iSubNode];
					pCurrentNode->vPos = Vertices[iVertex];
				}
			}
		}
	}

	int* aiIndexTable = new int[Vertices.size()];
	Vector3* aNewVertices = new Vector3[Vertices.size()];

	unsigned int numNewVertices = ExtractOctree(&root, 0, aiIndexTable, aNewVertices);
	for (unsigned int index = 0; index < Indices.size(); index++)
	{
		Indices[index] = aiIndexTable[Indices[index]];
	}

	Vertices.reserve(numNewVertices);
	for (iVertex = 0; iVertex < numNewVertices; iVertex++)
	{
		Vertices[iVertex] = aNewVertices[iVertex];
	}

	delete[] aiIndexTable;
	delete[] aNewVertices;

	std::vector<STri> vTris;
	vTris.resize(Indices.size() / 3);
	unsigned int numTrisCoppied = 0;
	unsigned int iTris = 0;

	for (; iTris < Indices.size() / 3; iTris++)
	{
		int index1 = Indices[iTris * 3 + 0];
		int index2 = Indices[iTris * 3 + 1];
		int index3 = Indices[iTris * 3 + 2];

		if (index1 == index2 || index1 == index3 || index2 == index3)
		{
			continue;
		}

		vTris[numTrisCoppied++] = STri(index1, index2, index3, Materials[iTris]);;
	}

	vTris.resize(numTrisCoppied);
	std::sort(vTris.begin(), vTris.end());

	numTrisCoppied = 0;
	STri lastTri;

	for (iTris = 0; iTris < vTris.size(); iTris++)
	{
		if (lastTri != vTris[iTris])
		{
			Indices[numTrisCoppied * 3 + 0] = vTris[iTris].i1;
			Indices[numTrisCoppied * 3 + 1] = vTris[iTris].i2;
			Indices[numTrisCoppied * 3 + 2] = vTris[iTris].i3;
			lastTri = vTris[iTris];
			numTrisCoppied++;
		}
	}

	Materials.resize(numTrisCoppied);
	Indices.resize(numTrisCoppied);
}


void MeshInfo::RemoveDuplicatedVertices()
{
	std::vector<Vector3> newVertices;

	for (unsigned int i = 0; i < Vertices.size(); i++)
	{
		std::vector<Vector3>::const_iterator it;
		it = std::find(newVertices.begin(), newVertices.end(), Vertices[i]);
		if(it == newVertices.end())
		{
			newVertices.push_back(Vertices[i]);
		}
	}

	Vertices.clear();
	Vertices = newVertices;
}


PxVec3* MeshInfo::GetVerticesVectorPtr(void)
{
	PxVec3* pVertices = new PxVec3[Vertices.size()];

	for (unsigned int i = 0; i < Vertices.size(); i++)
	{
		pVertices[i] = PxVec3(Vertices[i].x, Vertices[i].y,Vertices[i].z);
	}

	return pVertices;
}


PxVec3* MeshInfo::GetNormalsVectorPtr(void)
{
	PxVec3* pNormals = new PxVec3[Normals.size()];

	for (unsigned int i = 0; i < Normals.size(); i++)
	{
		pNormals[i] = PxVec3(Normals[i].x, Normals[i].y,Normals[i].z);
	}

	return pNormals;
}


float* MeshInfo::GetVerticesFloatPtr(void)
{
	float *fVertices = new float[Vertices.size() * 3];

	for (unsigned int i = 0; i < Vertices.size(); i++)
	{
		fVertices[i * 3 + 0] = Vertices[i].x;
		fVertices[i * 3 + 1] = Vertices[i].y;
		fVertices[i * 3 + 2] = Vertices[i].z;
	}

	return fVertices;
}


unsigned int* MeshInfo::GetIndicesUIntPtr(void)
{
	unsigned int *uiIndices = new unsigned int[Indices.size()];

	for (unsigned int i = 0; i < Indices.size(); i++)
	{
		uiIndices[i] = Indices[i];
	}

	return uiIndices;
}

