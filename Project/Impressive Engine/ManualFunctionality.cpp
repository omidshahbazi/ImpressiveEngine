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

#include "ManualFunctionality.h"
#include "ScenesHolder.h"
#include "Engine.h"
#include "Mathematics.h"


#ifdef EDIT_MODE
#include "MaterialEditorControl.h"
#endif


BEGIN_NAMESPACE
const char *ManualFunctionality::MANUAL_MESH_PLANE = "PlaneManulaMesh";
//const char *ManualFunctionality::MANUAL_MESH_BOX = "BoxManulaMesh";
//const char *ManualFunctionality::MANUAL_MESH_SPHERE = "SphereManulaMesh";

//ManualFunctionality *Singleton<ManualFunctionality>::m_Instance = NULL;


//ManualFunctionality::ManualFunctionality(void)
//{
//}
//
//
//ManualFunctionality::~ManualFunctionality(void)
//{
//}


void ManualFunctionality::InitializeManualMeshes(void)
{
	Ogre::MeshManager &mm = Ogre::MeshManager::getSingleton();

	Ogre::Plane plane;
	plane.normal = Ogre::Vector3::UNIT_Y;
	plane.d = 0;
	mm.createPlane(MANUAL_MESH_PLANE, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane, 1000, 1000, 50, 50, true, 1, 1, 1, Ogre::Vector3::UNIT_Z);
}


Ogre::ManualObject *ManualFunctionality::CreateManualObject(Ogre::SceneNode *Parent, const Ogre::String &Name, Ogre::RenderQueueGroupID RenderQueue)
{
	Ogre::ManualObject *obj = NULL;

	if (ScenesHolder::GetInstance().GetCurrentScene()->GetSceneManager()->hasManualObject(Name))
	{
		obj = ScenesHolder::GetInstance().GetCurrentScene()->GetSceneManager()->getManualObject(Name);
	}
	else
	{
		obj = ScenesHolder::GetInstance().GetCurrentScene()->GetSceneManager()->createManualObject(Name);
		obj->setRenderQueueGroup(RenderQueue);
		obj->setDynamic(true);

		Parent->attachObject(obj);
	}

	obj->setCastShadows(false);

	return obj;
}


void ManualFunctionality::CreateSphere(const Ogre::String &Name, const float Radius, const int Rings, const int Segments, bool Normals, bool TexCoords)
{
	Ogre::MeshPtr pSphere = Ogre::MeshManager::getSingleton().createManual(Name, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	Ogre::SubMesh *pSphereVertex = pSphere->createSubMesh();
	pSphere->sharedVertexData = new Ogre::VertexData();

	CreateSphere(pSphere->sharedVertexData, pSphereVertex->indexData, Radius, Rings, Segments, Normals  /*need normals*/, TexCoords /*need texture co-ordinates*/);

	// Generate face list
	pSphereVertex->useSharedVertices = true;

	// the original code was missing this line:
	pSphere->_setBounds(Ogre::AxisAlignedBox(Ogre::Vector3(-Radius, -Radius, -Radius), Ogre::Vector3(Radius, Radius, Radius)), false);
	pSphere->_setBoundingSphereRadius(Radius);
	// this line makes clear the mesh is loaded (avoids memory leaks)
	pSphere->load();
}


void ManualFunctionality::CreateSphere(Ogre::VertexData *&VertexData, Ogre::IndexData *&IndexData, float Radius, int Rings, int Segments, bool Normals, bool TexCoords)
{
	//assert(vertexData && indexData);

	// define the vertex format
	Ogre::VertexDeclaration* vertexDecl = VertexData->vertexDeclaration;
	size_t currOffset = 0;
	// positions
	vertexDecl->addElement(0, currOffset, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
	currOffset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);

	if (Normals)
	{
		// normals
		vertexDecl->addElement(0, currOffset, Ogre::VET_FLOAT3, Ogre::VES_NORMAL);
		currOffset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);

	}
	// two dimensional texture coordinates
	if (TexCoords)
	{
		vertexDecl->addElement(0, currOffset, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES, 0);
		currOffset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT2);
	}

	// allocate the vertex buffer
	VertexData->vertexCount = (Rings + 1) * (Segments+1);
	Ogre::HardwareVertexBufferSharedPtr vBuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(vertexDecl->getVertexSize(0), VertexData->vertexCount, Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY, false);
	Ogre::VertexBufferBinding* binding = VertexData->vertexBufferBinding;
	binding->setBinding(0, vBuf);
	float* pVertex = static_cast<float*>(vBuf->lock(Ogre::HardwareBuffer::HBL_DISCARD));

	// allocate index buffer
	IndexData->indexCount = 6 * Rings * (Segments + 1);
	IndexData->indexBuffer = Ogre::HardwareBufferManager::getSingleton().createIndexBuffer(Ogre::HardwareIndexBuffer::IT_16BIT, IndexData->indexCount, Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY, false);
	Ogre::HardwareIndexBufferSharedPtr iBuf = IndexData->indexBuffer;
	unsigned short* pIndices = static_cast<unsigned short*>(iBuf->lock(Ogre::HardwareBuffer::HBL_DISCARD));

	float fDeltaRingAngle = (Ogre::Math::PI / Rings);
	float fDeltaSegAngle = (2 * Ogre::Math::PI / Segments);
	unsigned short wVerticeIndex = 0 ;

	// Generate the group of rings for the sphere
	for( int ring = 0; ring <= Rings; ring++ ) {
		float r0 = Radius * sinf (ring * fDeltaRingAngle);
		float y0 = Radius * cosf (ring * fDeltaRingAngle);

		// Generate the group of segments for the current ring
		for(int seg = 0; seg <= Segments; seg++) {
			float x0 = r0 * sinf(seg * fDeltaSegAngle);
			float z0 = r0 * cosf(seg * fDeltaSegAngle);

			// Add one vertex to the strip which makes up the sphere
			*pVertex++ = x0;
			*pVertex++ = y0;
			*pVertex++ = z0;

			if (Normals)
			{
				Ogre::Vector3 vNormal = Ogre::Vector3(x0, y0, z0).normalisedCopy();
				*pVertex++ = vNormal.x;
				*pVertex++ = vNormal.y;
				*pVertex++ = vNormal.z;
			}
			if (TexCoords)
			{
				*pVertex++ = (float)seg / (float)Segments;
				*pVertex++ = (float)ring / (float)Rings;			
			}

			if (ring != Rings) 
			{
				// each vertex (except the last) has six indices pointing to it
				*pIndices++ = wVerticeIndex + Segments + 1;
				*pIndices++ = wVerticeIndex;               
				*pIndices++ = wVerticeIndex + Segments;
				*pIndices++ = wVerticeIndex + Segments + 1;
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


void ManualFunctionality::CreateCone(Ogre::VertexData*& VertexData, Ogre::IndexData*& IndexData, float Radius , float Height, int VerticesInBase)
{
	assert(VertexData && IndexData);

	// define the vertex format
	Ogre::VertexDeclaration* vertexDecl = VertexData->vertexDeclaration;
	// positions
	vertexDecl->addElement(0, 0, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
	
	// allocate the vertex buffer
	VertexData->vertexCount = VerticesInBase + 1;
	Ogre::HardwareVertexBufferSharedPtr vBuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(vertexDecl->getVertexSize(0), VertexData->vertexCount, Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY, false);
	Ogre::VertexBufferBinding* binding = VertexData->vertexBufferBinding;
	binding->setBinding(0, vBuf);
	float* pVertex = static_cast<float*>(vBuf->lock(Ogre::HardwareBuffer::HBL_DISCARD));

	// allocate index buffer - cone and base
	IndexData->indexCount = (3 * VerticesInBase) + (3 * (VerticesInBase - 2));
	IndexData->indexBuffer = Ogre::HardwareBufferManager::getSingleton().createIndexBuffer(Ogre::HardwareIndexBuffer::IT_16BIT, IndexData->indexCount, Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY, false);
	Ogre::HardwareIndexBufferSharedPtr iBuf = IndexData->indexBuffer;
	unsigned short* pIndices = static_cast<unsigned short*>(iBuf->lock(Ogre::HardwareBuffer::HBL_DISCARD));

	//Positions : cone head and base
	for (int i=0; i<3; i++)
		*pVertex++ = 0.0f;

	//Base :
	float fDeltaBaseAngle = (2 * Ogre::Math::PI) / VerticesInBase;
	for (int i=0; i<VerticesInBase; i++)
	{
		float angle = i * fDeltaBaseAngle;
		*pVertex++ = Radius * cosf(angle);
		*pVertex++ = Height;
		*pVertex++ = Radius * sinf(angle);
	}

	//Indices :
	//Cone head to vertices
	for (int i=0; i<VerticesInBase; i++)
	{
		*pIndices++ = 0;
		*pIndices++ = (i%VerticesInBase) + 1;
		*pIndices++ = ((i+1)%VerticesInBase) + 1;
	}
	//Cone base
	for (int i=0; i<VerticesInBase-2; i++)
	{
		*pIndices++ = 1;
		*pIndices++ = i + 3;
		*pIndices++ = i + 2;
	}

	// Unlock
	vBuf->unlock();
	iBuf->unlock();
}


void ManualFunctionality::CreateQuad(Ogre::VertexData *&VertexData)
{
	//assert(vertexData);

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


void ManualFunctionality::AddTriangleList(Ogre::ManualObject *Object, const Ogre::String &Material, std::vector<Ogre::Vector3> VectorList, const Ogre::ColourValue &Color)
{
	Object->begin(Material, Ogre::RenderOperation::OT_TRIANGLE_LIST);

	for (unsigned int i = 0; i < VectorList.size(); i++)
	{
		Object->position(VectorList[i]);
		Object->colour(Color);
	}

	Object->end();
}


void ManualFunctionality::AddTriangleStrip(Ogre::ManualObject *Object, const Ogre::String &Material, std::vector<Ogre::Vector3> VectorList, const Ogre::ColourValue &Color)
{
	Object->begin(Material, Ogre::RenderOperation::OT_TRIANGLE_STRIP);

	for (unsigned int i = 0; i < VectorList.size(); i++)
	{
		Object->position(VectorList[i]);
		Object->colour(Color);
	}

	Object->end();
}


void ManualFunctionality::AddTriangleFan(Ogre::ManualObject *Object, const Ogre::String &Material, std::vector<Ogre::Vector3> VectorList, const Ogre::ColourValue &Color)
{
	Object->begin(Material, Ogre::RenderOperation::OT_TRIANGLE_FAN);

	for (unsigned int i = 0; i < VectorList.size(); i++)
	{
		Object->position(VectorList[i]);
		Object->colour(Color);
	}

	Object->end();
}


void ManualFunctionality::AddBox(Ogre::ManualObject *Object, float Width, float Height, float Depth, const Ogre::Vector3 &Offset, const Ogre::Vector3 &Look, const Ogre::String &Material, const Ogre::ColourValue &Color)
{
	Ogre::Quaternion rotation = Mathematics::GetRotationFromHeading(Look);

	std::vector<Ogre::Vector3> sides;
	std::vector<Ogre::Vector3> top;
	std::vector<Ogre::Vector3> bottom;

	const float x1 = -Width / 2;
	const float x2 = Width / 2;
	const float y1 = -Height / 2;
	const float y2 = Height / 2;
	const float z1 = -Depth / 2;
	const float z2 = Depth / 2;

	sides.push_back(Offset + Ogre::Vector3(x1, y1, z1));
	sides.push_back(Offset + Ogre::Vector3(x1, y2, z1));
	sides.push_back(Offset + Ogre::Vector3(x2, y1, z1));
	sides.push_back(Offset + Ogre::Vector3(x2, y2, z1));
	sides.push_back(Offset + Ogre::Vector3(x2, y1, z2));
	sides.push_back(Offset + Ogre::Vector3(x2, y2, z2));
	sides.push_back(Offset + Ogre::Vector3(x1, y1, z2));
	sides.push_back(Offset + Ogre::Vector3(x1, y2, z2));
	sides.push_back(Offset + Ogre::Vector3(x1, y1, z1));
	sides.push_back(Offset + Ogre::Vector3(x1, y2, z1));
	top.push_back(Offset + Ogre::Vector3(x1, y1, z2));
	top.push_back(Offset + Ogre::Vector3(x1, y1, z1));
	top.push_back(Offset + Ogre::Vector3(x2, y1, z2));
	top.push_back(Offset + Ogre::Vector3(x2, y1, z1));
	bottom.push_back(Offset + Ogre::Vector3(x1, y2, z1));
	bottom.push_back(Offset + Ogre::Vector3(x1, y2, z2));
	bottom.push_back(Offset + Ogre::Vector3(x2, y2, z1));
	bottom.push_back(Offset + Ogre::Vector3(x2, y2, z2));

	AddTriangleStrip(Object, Material, top, Color);
	AddTriangleStrip(Object, Material, sides, Color);
	AddTriangleStrip(Object, Material, bottom, Color);
}


void ManualFunctionality::AddLineBox(Ogre::ManualObject *Object, float Length, float Radius, unsigned int Segments, float HeadSize, const Ogre::Vector3 &Offset, const Ogre::Vector3 &Look, const Ogre::String &Material, const Ogre::ColourValue &Color)
{
	Ogre::Quaternion rotation = Mathematics::GetRotationFromHeading(Look);

	std::vector<Ogre::Vector3> cylinder;
	std::vector<Ogre::Vector3> cylinderCap;

	const float z1 = 0;
	const float z2 = Length - HeadSize;
	const float z3 = Length;
	const float angleStep = Ogre::Math::TWO_PI / Segments;

	cylinder.push_back(rotation * Ogre::Vector3(Radius, 0, z1));
	cylinder.push_back(rotation * Ogre::Vector3(Radius, 0, z2));
	cylinderCap.push_back(rotation * Ogre::Vector3(0, 0, z1));
	cylinderCap.push_back(rotation * Ogre::Vector3(Radius, 0, z1));

	float x, y;

	for (unsigned int i = 0; i <= Segments; ++i)
	{
		x = Ogre::Math::Cos(i * angleStep);
		y = Ogre::Math::Sin(i * angleStep);

		cylinder.push_back(rotation * Ogre::Vector3(Radius * x, Radius * -y, z1));
		cylinder.push_back(rotation * Ogre::Vector3(Radius * x, Radius * -y, z2));
		cylinderCap.push_back(rotation * Ogre::Vector3(Radius * x, Radius * -y, z1));
	}

	AddTriangleStrip(Object, Material, cylinder, Color);
	AddTriangleFan(Object, Material, cylinderCap, Color);

	AddBox(Object, HeadSize, HeadSize, HeadSize, Offset + (Ogre::Vector3(z2, z2, z2) * Look), Look, Material, Color);
}


void ManualFunctionality::AddArrow(Ogre::ManualObject *Object, float Length, float Radius, float LengthHead, float RadiusHead, unsigned int Segments, const Ogre::Vector3 &Look, const Ogre::String &Material, const Ogre::ColourValue &Color)
{
	Ogre::Quaternion rotation = Mathematics::GetRotationFromHeading(Look);

	std::vector<Ogre::Vector3> cone;
	std::vector<Ogre::Vector3> coneCap;
	std::vector<Ogre::Vector3> cylinder;
	std::vector<Ogre::Vector3> cylinderCap;

	const float z1 = 0;
	const float z2 = Length - LengthHead;
	const float z3 = Length;
	const float angleStep = Ogre::Math::TWO_PI / Segments;

	cone.push_back(rotation * Ogre::Vector3(0, 0, z3));
	cone.push_back(rotation * Ogre::Vector3(RadiusHead, 0, z2));
	coneCap.push_back(rotation * Ogre::Vector3(0, 0, z2));
	coneCap.push_back(rotation * Ogre::Vector3(RadiusHead, 0, z2));
	cylinder.push_back(rotation * Ogre::Vector3(Radius, 0, z1));
	cylinder.push_back(rotation * Ogre::Vector3(Radius, 0, z2));
	cylinderCap.push_back(rotation * Ogre::Vector3(0, 0, z1));
	cylinderCap.push_back(rotation * Ogre::Vector3(Radius, 0, z1));

	float x, y;

	for (unsigned int i = 0; i <= Segments; ++i)
	{
		x = Ogre::Math::Cos(i * angleStep);
		y = Ogre::Math::Sin(i * angleStep);

		cone.push_back(rotation * Ogre::Vector3(RadiusHead * x, RadiusHead * y, z2));
		coneCap.push_back(rotation * Ogre::Vector3(RadiusHead * x, RadiusHead * -y, z2));
		cylinder.push_back(rotation * Ogre::Vector3(Radius * x, Radius * -y, z1));
		cylinder.push_back(rotation * Ogre::Vector3(Radius * x, Radius * -y, z2));
		cylinderCap.push_back(rotation * Ogre::Vector3(Radius * x, Radius * -y, z1));
	}

	AddTriangleFan(Object, Material, cone, Color);
	AddTriangleFan(Object, Material, coneCap, Color);
	AddTriangleStrip(Object, Material, cylinder, Color);
	AddTriangleFan(Object, Material, cylinderCap, Color);

	//Object->setBoundingBox(Ogre::AxisAlignedBox(
}


void ManualFunctionality::AddCircle(Ogre::ManualObject *Object, float Radius, float Width, unsigned int Segments, const Ogre::Vector3 &Offset, const Ogre::Vector3 &Look, const Ogre::String &Material, const Ogre::ColourValue &Color)
{
	Ogre::Quaternion rotation = Mathematics::GetRotationFromHeading(Look);

	std::vector<Ogre::Vector3> outer;
	std::vector<Ogre::Vector3> side1;
	std::vector<Ogre::Vector3> side2;
	std::vector<Ogre::Vector3> inner;

	const float r1 = Radius - Width;
	const float r2 = Radius;
	const float z1 = Width / 2;
	const float z2 = -Width / 2;
	const float angleStep = Ogre::Math::TWO_PI / Segments;

	outer.push_back(rotation * Ogre::Vector3(r2, 0, z1));
	outer.push_back(rotation * Ogre::Vector3(r2, 0, z2));
	side1.push_back(rotation * Ogre::Vector3(r1, 0, z1));
	side1.push_back(rotation * Ogre::Vector3(r2, 0, z1));
	side2.push_back(rotation * Ogre::Vector3(r2, 0, z2));
	side2.push_back(rotation * Ogre::Vector3(r1, 0, z2));
	inner.push_back(rotation * Ogre::Vector3(r1, 0, z2));
	inner.push_back(rotation * Ogre::Vector3(r1, 0, z1));

	for (unsigned int i = 0; i <= Segments; i++)
	{
		float x = Ogre::Math::Cos(i * angleStep);
		float y = Ogre::Math::Sin(i * angleStep);

		outer.push_back(rotation * Ogre::Vector3(x * r2, y * r2, z1));
		outer.push_back(rotation * Ogre::Vector3(x * r2, y * r2, z2));
		side1.push_back(rotation * Ogre::Vector3(x * r1, y * r1, z1));
		side1.push_back(rotation * Ogre::Vector3(x * r2, y * r2, z1));
		side2.push_back(rotation * Ogre::Vector3(x * r2, y * r2, z2));
		side2.push_back(rotation * Ogre::Vector3(x * r1, y * r1, z2));
		inner.push_back(rotation * Ogre::Vector3(x * r1, y * r1, z2));
		inner.push_back(rotation * Ogre::Vector3(x * r1, y * r1, z1));
	}

	AddTriangleStrip(Object, Material, outer, Color);
	AddTriangleStrip(Object, Material, side1, Color);
	AddTriangleStrip(Object, Material, side2, Color);
	AddTriangleStrip(Object, Material, inner, Color);
}


void ManualFunctionality::AddSphere(Ogre::ManualObject *Object, float Radius, unsigned int Rings, unsigned int Segments, const Ogre::String &Material, const Ogre::ColourValue &Color)
{
	float fDeltaRingAngle = Ogre::Math::PI / Rings;
	float fDeltaSegAngle = 2 * Ogre::Math::PI / Segments;
	unsigned short wVerticeIndex = 0 ;
 
	Object->begin(Material, Ogre::RenderOperation::OT_TRIANGLE_LIST);

	// Generate the group of rings for the sphere
	for(unsigned int ring = 0; ring <= Rings; ring++) 
	{
		float r0 = Radius * sinf (ring * fDeltaRingAngle);
		float y0 = Radius * cosf (ring * fDeltaRingAngle);
 
		// Generate the group of segments for the current ring
		for(unsigned int seg = 0; seg <= Segments; seg++) 
		{
			float x0 = r0 * sinf(seg * fDeltaSegAngle);
			float z0 = r0 * cosf(seg * fDeltaSegAngle);
 
			// Add one vertex to the strip which makes up the sphere
			Object->position(x0, y0, z0);
			Object->normal(Ogre::Vector3(x0, y0, z0).normalisedCopy());
			Object->textureCoord((float) seg / (float) Segments, (float) ring / (float) Rings);
			Object->colour(Color);
 
			if (ring != Rings) 
			{
				// each vertex (except the last) has six indicies pointing to it
				Object->index(wVerticeIndex + Segments + 1);
				Object->index(wVerticeIndex);               
				Object->index(wVerticeIndex + Segments);
				Object->index(wVerticeIndex + Segments + 1);
				Object->index(wVerticeIndex + 1);
				Object->index(wVerticeIndex);

				wVerticeIndex ++;
			}
		}
	}

	Object->end();

	Ogre::MeshPtr mesh = Object->convertToMesh(Object->getName());
	mesh->_setBounds(Ogre::AxisAlignedBox(Ogre::Vector3(-Radius, -Radius, -Radius), Ogre::Vector3(Radius, Radius, Radius)), false);
 
	mesh->_setBoundingSphereRadius(Radius);
	unsigned short src, dest;
	if (!mesh->suggestTangentVectorBuildParams(Ogre::VES_TANGENT, src, dest))
		mesh->buildTangentVectors(Ogre::VES_TANGENT, src, dest);
}


void ManualFunctionality::UnloadResource(Ogre::ResourceManager *ResourceManager, const Ogre::String &Name)
{
	Ogre::ResourcePtr rPtr = ResourceManager->getByName(Name);
    if (rPtr.isNull())
    {
        Ogre::StringUtil::StrStreamType msg;
        msg << "Resource no longer exists: " << Name;
        Ogre::LogManager::getSingleton().logMessage(msg.str());
        return;
    }
 
    rPtr->unload();
    if (rPtr->isLoaded())
    {
        Ogre::StringUtil::StrStreamType msg;
        msg << "Resource was not unloaded: " << Name;
        Ogre::LogManager::getSingleton().logMessage(msg.str());
    }
    else
    {
        Ogre::StringUtil::StrStreamType msg;
        msg << "Resource was unloaded: " << Name;
        Ogre::LogManager::getSingleton().logMessage(msg.str());
    }
 
    ResourceManager->remove(Name);
    rPtr = ResourceManager->getByName(Name);
    if (rPtr.isNull())
    {
        Ogre::StringUtil::StrStreamType msg;
        msg << "Resource was removed: " << Name;
        Ogre::LogManager::getSingleton().logMessage(msg.str());
    }
    else
    {
        Ogre::StringUtil::StrStreamType msg;
        msg << "Resource was not removed: " << Name;
        Ogre::LogManager::getSingleton().logMessage(msg.str());
    }
 
    if (ResourceManager->resourceExists(Name))
    {
        Ogre::StringUtil::StrStreamType msg;
        msg << "Resource still exists: " << Name;
        Ogre::LogManager::getSingleton().logMessage(msg.str());
    }
    else
    {
        Ogre::StringUtil::StrStreamType msg;
        msg << "Resource no longer exists: " << Name;
        Ogre::LogManager::getSingleton().logMessage(msg.str());
    }
}


void ManualFunctionality::LoadResource(Ogre::ResourceManager *ResourceManager, const Ogre::String &Name, const Ogre::String &GroupName)
{
    Ogre::ResourcePtr rPtr = ResourceManager->load(Name, GroupName);
    if (rPtr.isNull())
    {
        Ogre::StringUtil::StrStreamType msg;
        msg << "Resource no longer exists: " << Name;
        Ogre::LogManager::getSingleton().logMessage(msg.str());
        return;
    }
 
    rPtr->reload();
    if (rPtr->isLoaded())
    {
        Ogre::StringUtil::StrStreamType msg;
        msg << "Resource was reloaded: " << Name;
        Ogre::LogManager::getSingleton().logMessage(msg.str());
    }
    else
    {
        Ogre::StringUtil::StrStreamType msg;
        msg << "Resource was not reloaded: " << Name;
        Ogre::LogManager::getSingleton().logMessage(msg.str());
    }
 
    if (ResourceManager->resourceExists(Name))
    {
        Ogre::StringUtil::StrStreamType msg;
        msg << "Resource still exists: " << Name;
        Ogre::LogManager::getSingleton().logMessage(msg.str());
    }
    else
    {
        Ogre::StringUtil::StrStreamType msg;
        msg << "Resource no longer exists: " << Name;
        Ogre::LogManager::getSingleton().logMessage(msg.str());
    }
}


void ManualFunctionality::UnloadMaterial(const Ogre::String &FileName)
{
    if (FileName.empty())
    {
        Ogre::LogManager::getSingleton().logMessage("FileName is empty.");
        return;
    }
 
    Ogre::DataStreamPtr stream = Ogre::ResourceGroupManager::getSingleton().openResource(FileName);
    if(!stream.isNull())
    {
        try
        {
            while(!stream->eof())
            {
                std::string line = stream->getLine();
                Ogre::StringUtil::trim(line);
 
                ///
                /// UNLOAD MATERIALS
                ///
                if (Ogre::StringUtil::startsWith(line, "material"))
                {
                    Ogre::vector<Ogre::String>::type vec = Ogre::StringUtil::split(line," \t:");
                    bool skipFirst = true;
                    for (Ogre::vector<Ogre::String>::type::iterator it = vec.begin(); it < vec.end(); ++it)
                    {
                        if (skipFirst)
                        {
                            skipFirst = false;
                            continue;
                        }
                        std::string match = (*it);
                        Ogre::StringUtil::trim(match);
                        if (!match.empty())
                        {
                            UnloadResource(Ogre::MaterialManager::getSingletonPtr(), match);
                            break;
                        }
                    }
                }
            }
        }
        catch (Ogre::Exception &e)
        {
            Ogre::StringUtil::StrStreamType msg;
            msg << "Exception: FILE: " << __FILE__ << " LINE: " << __LINE__ << " DESC: " << e.getFullDescription() << std::endl;
            Ogre::LogManager::getSingleton().logMessage(msg.str());
        }
    }
    stream->close();
}


void ManualFunctionality::UnloadVertexProgram(const Ogre::String &FileName)
{
    if (FileName.empty())
    {
        Ogre::LogManager::getSingleton().logMessage("Material filename is empty.");
        return;
    }
 
    Ogre::DataStreamPtr stream = Ogre::ResourceGroupManager::getSingleton().openResource(FileName);
    if(!stream.isNull())
    {
        try
        {
            while(!stream->eof())
            {
                std::string line = stream->getLine();
                Ogre::StringUtil::trim(line);
 
                ///
                /// UNLOAD VERTEX PROGRAMS
                ///
                if (Ogre::StringUtil::startsWith(line, "vertex_program") && !Ogre::StringUtil::startsWith(line, "vertex_program_ref"))
                {
                    Ogre::vector<Ogre::String>::type vec = Ogre::StringUtil::split(line," \t");
                    bool skipFirst = true;
                    for (Ogre::vector<Ogre::String>::type::iterator it = vec.begin(); it < vec.end(); ++it)
                    {
                        if (skipFirst)
                        {
                            skipFirst = false;
                            continue;
                        }
                        std::string match = (*it);
                        Ogre::StringUtil::trim(match);
                        if (!match.empty())
                        {
                            UnloadResource(Ogre::HighLevelGpuProgramManager::getSingletonPtr(), match);
                            break;
                        }
                    }
                }
            }
        }
        catch (Ogre::Exception &e)
        {
            Ogre::StringUtil::StrStreamType msg;
            msg << "Exception: FILE: " << __FILE__ << " LINE: " << __LINE__ << " DESC: " << e.getFullDescription() << std::endl;
            Ogre::LogManager::getSingleton().logMessage(msg.str());
        }
    }
    stream->close();
}


void ManualFunctionality::UnloadFragmentProgram(const Ogre::String &FileName)
{
    if (FileName.empty())
    {
        Ogre::LogManager::getSingleton().logMessage("Material filename is empty.");
        return;
    }
 
    Ogre::DataStreamPtr stream = Ogre::ResourceGroupManager::getSingleton().openResource(FileName);
    if(!stream.isNull())
    {
        try
        {
            while(!stream->eof())
            {
                std::string line = stream->getLine();
                Ogre::StringUtil::trim(line);
 
                ///
                /// UNLOAD FRAGMENT PROGRAMS
                ///
                if (Ogre::StringUtil::startsWith(line, "fragment_program") && !Ogre::StringUtil::startsWith(line, "fragment_program_ref"))
                {
                    Ogre::vector<Ogre::String>::type vec = Ogre::StringUtil::split(line," \t");
                    bool skipFirst = true;
                    for (Ogre::vector<Ogre::String>::type::iterator it = vec.begin(); it < vec.end(); ++it)
                    {
                        if (skipFirst)
                        {
                            skipFirst = false;
                            continue;
                        }
                        std::string match = (*it);
                        Ogre::StringUtil::trim(match);
                        if (!match.empty())
                        {
                            UnloadResource(Ogre::HighLevelGpuProgramManager::getSingletonPtr(), match);
                            break;
                        }
                    }
                }
            }
        }
        catch (Ogre::Exception &e)
        {
            Ogre::StringUtil::StrStreamType msg;
            msg << "Exception: FILE: " << __FILE__ << " LINE: " << __LINE__ << " DESC: " << e.getFullDescription() << std::endl;
            Ogre::LogManager::getSingleton().logMessage(msg.str());
        }
    }
    stream->close();
}


void ManualFunctionality::ReloadMaterial(const Ogre::String &Name, const Ogre::String &GroupName, const Ogre::String &FileName, bool ParseMaterialScript)
{
    if (Name.empty())
    {
        Ogre::LogManager::getSingleton().logMessage("Material name is empty.");
        return;
    }
 
    if (GroupName.empty())
    {
        Ogre::LogManager::getSingleton().logMessage("Group name is empty.");
        return;
    }
 
    if (FileName.empty())
    {
        Ogre::LogManager::getSingleton().logMessage("Filename is empty.");
        return;
    }
 
    UnloadMaterial(FileName);
    UnloadVertexProgram(FileName);
    UnloadFragmentProgram(FileName);
 
    if (ParseMaterialScript)
    {
        Ogre::DataStreamPtr stream = Ogre::ResourceGroupManager::getSingleton().openResource(FileName);
        if(!stream.isNull())
        {
            try
            {
                Ogre::MaterialManager::getSingleton().parseScript(stream, GroupName);
                Ogre::MaterialPtr materialPtr = Ogre::MaterialManager::getSingleton().getByName(Name);
                if (!materialPtr.isNull())
                {
                    materialPtr->compile();
                    materialPtr->load();
                }
            }
            catch (Ogre::Exception &e)
            {
                Ogre::StringUtil::StrStreamType msg;
                msg << "Exception: FILE: " << __FILE__ << " LINE: " << __LINE__ << " DESC: " << e.getFullDescription() << std::endl;
                Ogre::LogManager::getSingleton().logMessage(msg.str());
            }
        }
        stream->close();
 
        ///
        /// RELOAD MATERIAL SCRIPT CONTENTS
        ///
        stream = Ogre::ResourceGroupManager::getSingleton().openResource(FileName);
        if(!stream.isNull())
        {
            try
            {
                ///
                /// RELOAD ALL MATERIAL CONTENTS IN FILE
                ///
                while(!stream->eof())
                {
                    std::string line = stream->getLine();
                    Ogre::StringUtil::trim(line);
                    ///
                    /// RELOAD MATERIALS
                    ///
                    if (Ogre::StringUtil::startsWith(line, "material"))
                    {
                        Ogre::vector<Ogre::String>::type vec = Ogre::StringUtil::split(line," \t:");
                        bool skipFirst = true;
                        for (Ogre::vector<Ogre::String>::type::iterator it = vec.begin(); it < vec.end(); ++it)
                        {
                            if (skipFirst)
                            {
                                skipFirst = false;
                                continue;
                            }
                            std::string match = (*it);
                            Ogre::StringUtil::trim(match);
                            if (!match.empty())
                            {
								if (match == "anisotropic2")
                                LoadResource(Ogre::MaterialManager::getSingletonPtr(), match, GroupName);
                                break;
                            }
                        }
                    }
                    ///
                    /// RELOAD VERTEX PROGRAMS
                    ///
                    if (Ogre::StringUtil::startsWith(line, "vertex_program") && !Ogre::StringUtil::startsWith(line, "vertex_program_ref"))
                    {
                        Ogre::vector<Ogre::String>::type vec = Ogre::StringUtil::split(line," \t");
                        bool skipFirst = true;
                        for (Ogre::vector<Ogre::String>::type::iterator it = vec.begin(); it < vec.end(); ++it)
                        {
                            if (skipFirst)
                            {
                                skipFirst = false;
                                continue;
                            }
                            std::string match = (*it);
                            Ogre::StringUtil::trim(match);
                            if (!match.empty())
                            {
                                LoadResource(Ogre::HighLevelGpuProgramManager::getSingletonPtr(), match, GroupName);
                                break;
                            }
                        }
                    }
                    ///
                    /// RELOAD FRAGMENT PROGRAMS
                    ///
                    if (Ogre::StringUtil::startsWith(line, "fragment_program") && !Ogre::StringUtil::startsWith(line, "fragment_program_ref"))
                    {
                        Ogre::vector<Ogre::String>::type vec = Ogre::StringUtil::split(line," \t");
                        bool skipFirst = true;
                        for (Ogre::vector<Ogre::String>::type::iterator it = vec.begin(); it < vec.end(); ++it)
                        {
                            if (skipFirst)
                            {
                                skipFirst = false;
                                continue;
                            }
                            std::string match = (*it);
                            Ogre::StringUtil::trim(match);
                            if (!match.empty())
                            {
                                LoadResource(Ogre::HighLevelGpuProgramManager::getSingletonPtr(), match, GroupName);
                                break;
                            }
                        }
                    }
                }
            }
            catch (Ogre::Exception &e)
            {
                Ogre::StringUtil::StrStreamType msg;
                msg << "Exception: FILE: " << __FILE__ << " LINE: " << __LINE__ << " DESC: " << e.getFullDescription() << std::endl;
                Ogre::LogManager::getSingleton().logMessage(msg.str());
            }
        }
        stream->close();
 
        try
        {
            // Do a render test if it fails, leave materials unloaded
            Ogre::Root::getSingleton().renderOneFrame();
            return;
        }
        catch (Ogre::Exception &e)
        {
            UnloadVertexProgram(FileName);
 
            Ogre::StringUtil::StrStreamType msg;
            msg << "Render test failed. Unloading vertex programs." << std::endl;
            msg << "Exception: FILE: " << __FILE__ << " LINE: " << __LINE__ << " DESC: " << e.getFullDescription() << std::endl;
            Ogre::LogManager::getSingleton().logMessage(msg.str());
        }
 
        try
        {
            // Do a render test if it fails, leave materials unloaded
            Ogre::Root::getSingleton().renderOneFrame();
        }
        catch (Ogre::Exception &e)
        {
            // Don't load the script this time
            ReloadMaterial(Name, GroupName, FileName, false);
 
            Ogre::StringUtil::StrStreamType msg;
            msg << "Render test failed. Unloading materials." << std::endl;
			msg << "Exception: FILE: " << __FILE__ << " LINE: " << __LINE__ << " DESC: " << e.getFullDescription() << std::endl;
            Ogre::LogManager::getSingleton().logMessage(msg.str());
        }
    }
}

	
Ogre::MaterialPtr ManualFunctionality::CreateColorMaterial(float R, float G, float B, const Ogre::String &Name, bool ForceDepth)
{
	return CreateColorMaterial(Ogre::ColourValue(R, G, B), Name, ForceDepth);
}


Ogre::MaterialPtr ManualFunctionality::CreateColorMaterial(float R, float G, float B, float A, const Ogre::String &Name, bool ForceDepth)
{
	return CreateColorMaterial(Ogre::ColourValue(R, G, B, A), Name, ForceDepth);
}


Ogre::MaterialPtr ManualFunctionality::CreateColorMaterial(const Ogre::ColourValue &Color, const Ogre::String &Name, bool ForceDepth)
{
	// Works fine in PhysicsFramework
	Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().create(Name, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	
	material->setAmbient(Color);
	material->setDiffuse(Color);
	material->setLightingEnabled(false);
	material->setColourWriteEnabled(true);

	//Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().create(Name, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

	////material->getTechnique(0)->getPass(0)->createTextureUnitState("White.jpg");

	//material->getTechnique(0)->getPass(0)->setAmbient(Color);
	//material->getTechnique(0)->getPass(0)->setDiffuse(Color);
	//material->setReceiveShadows(false);
	//material->setLightingEnabled(false);

	if (ForceDepth)
		material->getTechnique(0)->getPass(0)->setDepthFunction(Ogre::CMPF_ALWAYS_PASS);

	return material;
}

#ifdef EDIT_MODE
void ManualFunctionality::MakePassAsDiffuse(Ogre::Pass *Pass)
{
	Pass->setName("");

	Pass->setVertexProgram("");
	Pass->setFragmentProgram("");

	unsigned int count = Pass->getNumTextureUnitStates();
	for (unsigned int i = 1; i < count; i++)
		Pass->removeTextureUnitState(1);
}


void ManualFunctionality::MakePassAsBumpDiffuse(Ogre::Pass *Pass)
{
	Pass->setName(MaterialEditorControl::PASS_BUMP_NAME);

	Pass->setVertexProgram(MaterialEditorControl::PROGRAM_BUMPVP_NAME);
	Pass->setFragmentProgram(MaterialEditorControl::PROGRAM_BUMPFP_NAME);

	Ogre::GpuProgramParametersSharedPtr &param = Pass->getVertexProgramParameters();
	param->setNamedAutoConstant("worldViewProjMatrix", Ogre::GpuProgramParameters::ACT_WORLDVIEWPROJ_MATRIX);

	param = Pass->getFragmentProgramParameters();
	param->setNamedConstant("uvNormalScale", Ogre::Vector2(1, 1).ptr(), 2, 1);

	param->setNamedConstant("ambient", Ogre::ColourValue(0.5f, 0.5f, 0.5f, 1));
	param->setNamedConstant("diffuseColor", Ogre::ColourValue(0.75f, 0.8f, 1, 1));

	param->setNamedConstant("reflectivity", 0.f);
	
	param->setNamedConstant("specularPower0", 5.f);
	param->setNamedConstant("specularPower1", 5.f);
	
	param->setNamedConstant("specularMultiplier0", 0.75f);
	param->setNamedConstant("specularMultiplier1", 0.75f);
	
	param->setNamedConstant("useSpecular", 0.f);

	if (Pass->getNumTextureUnitStates() < 1)
		Pass->createTextureUnitState();
	Pass->getTextureUnitState(0)->setTextureNameAlias(MaterialEditorControl::TEXALIAS_DIFFUSE_NAME);
	
	if (Pass->getNumTextureUnitStates() < 2)
		Pass->createTextureUnitState();
	Pass->getTextureUnitState(1)->setTextureNameAlias(MaterialEditorControl::TEXALIAS_NORMAL_NAME);
}


void ManualFunctionality::MakePassAsBumpSpecularDiffuse(Ogre::Pass *Pass)
{
	MakePassAsBumpDiffuse(Pass);

	Ogre::GpuProgramParametersSharedPtr &param = Pass->getFragmentProgramParameters();
	
	param->setNamedConstant("useSpecular", 1.f);
	
	if (Pass->getNumTextureUnitStates() < 3)
		Pass->createTextureUnitState()->setTextureNameAlias(MaterialEditorControl::TEXALIAS_SPECULAR_NAME);
}
#endif

void ManualFunctionality::ClearPassMaps(Ogre::SceneManager *SceneManager)
{
	if (SceneManager)
	{
		SceneManager->getRenderQueue()->clear(true);
	}
	else
	{
		Ogre::SceneManagerEnumerator::SceneManagerIterator it = Engine::GetInstance().GetGraphic()->getSceneManagerIterator();

		while (it.hasMoreElements())
		{
			it.getNext()->getRenderQueue()->clear(true);
		}
	}

	Ogre::Pass::processPendingPassUpdates();
}


void ManualFunctionality::UpdateForMaterialChanges(Ogre::SceneManager *SceneManager)
{
	//if (Ogre::Pass::getDirtyHashList().size() != 0 || Ogre::Pass::getPassGraveyard().size() != 0)
	//{
	//	Ogre::SceneManagerEnumerator::SceneManagerIterator it = Engine::GetInstance().GetGraphic()->getSceneManagerIterator();

	//	while (it.hasMoreElements())
	//	{
	//		Ogre::RenderQueue::QueueGroupIterator groupIt = it.getNext()->getRenderQueue()->_getQueueGroupIterator();

	//		while(groupIt.hasMoreElements())
	//		{
	//			groupIt.getNext()->clear(false);
	//		}
	//	}

	//	Ogre::Pass::processPendingPassUpdates();
	//}

	if (SceneManager)
	{
		UpdateSceneNodes(SceneManager->getRootSceneNode());
	}
	else
	{
		Ogre::SceneManagerEnumerator::SceneManagerIterator it = Engine::GetInstance().GetGraphic()->getSceneManagerIterator();

		while (it.hasMoreElements())
		{
			UpdateSceneNodes(it.getNext()->getRootSceneNode());
		}
	}
}


void ManualFunctionality::UpdateSceneNodes(Ogre::SceneNode *SceneNode)
{
	Ogre::Entity *entity = NULL;
	Ogre::SceneNode::ObjectIterator objIt = SceneNode->getAttachedObjectIterator();
	while (objIt.hasMoreElements())
	{
		entity = dynamic_cast<Ogre::Entity*>(objIt.getNext());

		if (entity)
			for (unsigned int i = 0; i < entity->getNumSubEntities(); i++)
			{
				entity->getSubEntity(i)->setMaterial((Ogre::MaterialPtr)Ogre::MaterialManager::getSingleton().getByName(entity->getSubEntity(i)->getMaterialName()));
			}
	}
	entity = NULL;

	Ogre::Node::ChildNodeIterator nodeIt = SceneNode->getChildIterator();
	while (nodeIt.hasMoreElements())
	{
		UpdateSceneNodes((Ogre::SceneNode*)nodeIt.getNext());
	}
}
END_NAMESPACE

//#endif