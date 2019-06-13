#include "ManualMeshGenerator.h"


void ManualMeshGenerator::CreateQuad(Ogre::VertexData *&VertexData)
{
	assert(VertexData);

	VertexData->vertexCount = 4;
	VertexData->vertexStart = 0;

	Ogre::VertexDeclaration* vertexDecl = VertexData->vertexDeclaration;
	Ogre::VertexBufferBinding* bind = VertexData->vertexBufferBinding;

	vertexDecl->addElement(0, 0, Ogre::VET_FLOAT3, Ogre::VES_POSITION);

	Ogre::HardwareVertexBufferSharedPtr vbuf = 
		Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
		vertexDecl->getVertexSize(0),
		VertexData->vertexCount,
		Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);

	// Bind buffer
	bind->setBinding(0, vbuf);
	// Upload data
	float data[]={
		-1,1,-1,  // corner 1
		-1,-1,-1, // corner 2
		1,1,-1,   // corner 3
		1,-1,-1}; // corner 4
		vbuf->writeData(0, sizeof(data), data, true);
}


void ManualMeshGenerator::CreateSphere(Ogre::VertexData*& vertexData, Ogre::IndexData*& indexData, float radius, int nRings, int nSegments, bool bNormals, bool bTexCoords)
{
	assert(vertexData && indexData);

	// define the vertex format
	Ogre::VertexDeclaration* vertexDecl = vertexData->vertexDeclaration;
	size_t currOffset = 0;
	// positions
	vertexDecl->addElement(0, currOffset, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
	currOffset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);

	if (bNormals)
	{
		// normals
		vertexDecl->addElement(0, currOffset, Ogre::VET_FLOAT3, Ogre::VES_NORMAL);
		currOffset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);

	}
	// two dimensional texture coordinates
	if (bTexCoords)
	{
		vertexDecl->addElement(0, currOffset, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES, 0);
	}

	// allocate the vertex buffer
	vertexData->vertexCount = (nRings + 1) * (nSegments+1);
	Ogre::HardwareVertexBufferSharedPtr vBuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(vertexDecl->getVertexSize(0), vertexData->vertexCount, Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY, false);
	Ogre::VertexBufferBinding* binding = vertexData->vertexBufferBinding;
	binding->setBinding(0, vBuf);
	float* pVertex = static_cast<float*>(vBuf->lock(Ogre::HardwareBuffer::HBL_DISCARD));

	// allocate index buffer
	indexData->indexCount = 6 * nRings * (nSegments + 1);
	indexData->indexBuffer = Ogre::HardwareBufferManager::getSingleton().createIndexBuffer(Ogre::HardwareIndexBuffer::IT_16BIT, indexData->indexCount, Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY, false);
	Ogre::HardwareIndexBufferSharedPtr iBuf = indexData->indexBuffer;
	unsigned short* pIndices = static_cast<unsigned short*>(iBuf->lock(Ogre::HardwareBuffer::HBL_DISCARD));

	float fDeltaRingAngle = (Ogre::Math::PI / nRings);
	float fDeltaSegAngle = (2 * Ogre::Math::PI / nSegments);
	unsigned short wVerticeIndex = 0 ;

	// Generate the group of rings for the sphere
	for( int ring = 0; ring <= nRings; ring++ ) {
		float r0 = radius * sinf (ring * fDeltaRingAngle);
		float y0 = radius * cosf (ring * fDeltaRingAngle);

		// Generate the group of segments for the current ring
		for(int seg = 0; seg <= nSegments; seg++) {
			float x0 = r0 * sinf(seg * fDeltaSegAngle);
			float z0 = r0 * cosf(seg * fDeltaSegAngle);

			// Add one vertex to the strip which makes up the sphere
			*pVertex++ = x0;
			*pVertex++ = y0;
			*pVertex++ = z0;

			if (bNormals)
			{
				Ogre::Vector3 vNormal = Ogre::Vector3(x0, y0, z0).normalisedCopy();
				*pVertex++ = vNormal.x;
				*pVertex++ = vNormal.y;
				*pVertex++ = vNormal.z;
			}
			if (bTexCoords)
			{
				*pVertex++ = (float) seg / (float) nSegments;
				*pVertex++ = (float) ring / (float) nRings;			
			}

			if (ring != nRings) 
			{
				// each vertex (except the last) has six indices pointing to it
				*pIndices++ = wVerticeIndex + nSegments + 1;
				*pIndices++ = wVerticeIndex;               
				*pIndices++ = wVerticeIndex + nSegments;
				*pIndices++ = wVerticeIndex + nSegments + 1;
				*pIndices++ = wVerticeIndex + 1;
				*pIndices++ = wVerticeIndex;
				wVerticeIndex ++;
			}
		}; // end for seg
	} // end for ring

	// Unlock
	vBuf->unlock();
	iBuf->unlock();
}


void ManualMeshGenerator::CreateCone(Ogre::VertexData*& vertexData, Ogre::IndexData*& indexData, float radius , float height, int nVerticesInBase)
{
	assert(vertexData && indexData);

	// define the vertex format
	Ogre::VertexDeclaration* vertexDecl = vertexData->vertexDeclaration;
	// positions
	vertexDecl->addElement(0, 0, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
	
	// allocate the vertex buffer
	vertexData->vertexCount = nVerticesInBase + 1;
	Ogre::HardwareVertexBufferSharedPtr vBuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(vertexDecl->getVertexSize(0), vertexData->vertexCount, Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY, false);
	Ogre::VertexBufferBinding* binding = vertexData->vertexBufferBinding;
	binding->setBinding(0, vBuf);
	float* pVertex = static_cast<float*>(vBuf->lock(Ogre::HardwareBuffer::HBL_DISCARD));

	// allocate index buffer - cone and base
	indexData->indexCount = (3 * nVerticesInBase) + (3 * (nVerticesInBase - 2));
	indexData->indexBuffer = Ogre::HardwareBufferManager::getSingleton().createIndexBuffer(Ogre::HardwareIndexBuffer::IT_16BIT, indexData->indexCount, Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY, false);
	Ogre::HardwareIndexBufferSharedPtr iBuf = indexData->indexBuffer;
	unsigned short* pIndices = static_cast<unsigned short*>(iBuf->lock(Ogre::HardwareBuffer::HBL_DISCARD));

	//Positions : cone head and base
	for (int i=0; i<3; i++)
		*pVertex++ = 0.0f;

	//Base :
	float fDeltaBaseAngle = (2 * Ogre::Math::PI) / nVerticesInBase;
	for (int i=0; i<nVerticesInBase; i++)
	{
		float angle = i * fDeltaBaseAngle;
		*pVertex++ = radius * cosf(angle);
		*pVertex++ = height;
		*pVertex++ = radius * sinf(angle);
	}

	//Indices :
	//Cone head to vertices
	for (int i=0; i<nVerticesInBase; i++)
	{
		*pIndices++ = 0;
		*pIndices++ = (i%nVerticesInBase) + 1;
		*pIndices++ = ((i+1)%nVerticesInBase) + 1;
	}
	//Cone base
	for (int i=0; i<nVerticesInBase-2; i++)
	{
		*pIndices++ = 1;
		*pIndices++ = i + 3;
		*pIndices++ = i + 2;
	}

	// Unlock
	vBuf->unlock();
	iBuf->unlock();
}


void ManualMeshGenerator::CreateSquarePyramid(Ogre::VertexData*& vertexData, Ogre::IndexData*& indexData, float BaseEdgeSize, float Height)
{
	//http://en.wikipedia.org/wiki/Pyramid_%28geometry%29

	assert(vertexData && indexData);

	float halfSize = BaseEdgeSize / 2.f;

	// define the vertex format
	Ogre::VertexDeclaration* vertexDecl = vertexData->vertexDeclaration;

	// positions
	vertexDecl->addElement(0, 0, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
	vertexDecl->addElement(1, 0, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES);
	
	// allocate the vertex buffer
	vertexData->vertexCount = 5;
	Ogre::HardwareVertexBufferSharedPtr vBuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(vertexDecl->getVertexSize(0), vertexData->vertexCount, Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY, false);
	Ogre::HardwareVertexBufferSharedPtr vBufTexCoord = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(vertexDecl->getVertexSize(0), vertexData->vertexCount, Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY, false);
	Ogre::VertexBufferBinding* binding = vertexData->vertexBufferBinding;
	binding->setBinding(0, vBuf);
	binding->setBinding(1, vBufTexCoord);
	float* pVertex = static_cast<float*>(vBuf->lock(Ogre::HardwareBuffer::HBL_DISCARD));
	float* pTexCoord = static_cast<float*>(vBufTexCoord->lock(Ogre::HardwareBuffer::HBL_DISCARD));

	// allocate index buffer - pyramid and base
	indexData->indexCount = 6;
	indexData->indexBuffer = Ogre::HardwareBufferManager::getSingleton().createIndexBuffer(Ogre::HardwareIndexBuffer::IT_16BIT, indexData->indexCount, Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY, false);
	Ogre::HardwareIndexBufferSharedPtr iBuf = indexData->indexBuffer;
	unsigned short* pIndices = static_cast<unsigned short*>(iBuf->lock(Ogre::HardwareBuffer::HBL_DISCARD));

	//Positions : head and base
	for (int i=0; i<3; i++)
		*pVertex++ = 0.f;
	*pTexCoord++ = 0;
	*pTexCoord++ = 0;

	//Base :
	*pVertex++ = -halfSize;
	*pVertex++ = Height;
	*pVertex++ = -halfSize;
	*pTexCoord++ = 0;
	*pTexCoord++ = 1;
	
	*pVertex++ = -halfSize;
	*pVertex++ = Height;
	*pVertex++ = halfSize;
	*pTexCoord++ = 0;
	*pTexCoord++ = 0;
	
	*pVertex++ = halfSize;
	*pVertex++ = Height;
	*pVertex++ = -halfSize;
	*pTexCoord++ = 1;
	*pTexCoord++ = 0;
	
	*pVertex++ = halfSize;
	*pVertex++ = Height;
	*pVertex++ = halfSize;
	*pTexCoord++ = 1;
	*pTexCoord++ = 1;


	//Indices :
	//head to vertices
	for (int i=0; i<4; i++)
	{
		*pIndices++ = 0;
		*pIndices++ = (i%4) + 1;
		*pIndices++ = ((i+1)%4) + 1;
	}
	//base
	for (int i=0; i<4-2; i++)
	{
		*pIndices++ = 1;
		*pIndices++ = i + 3;
		*pIndices++ = i + 2;
	}

	// Unlock
	vBuf->unlock();
	vBufTexCoord->unlock();
	iBuf->unlock();
}