#include "Cloth.h"
#include "MeshInfo.h"
#include "PseudoGameObject.h"
#include "IeConvert.h"

MeshInfo *Cloth::GetMeshInfo(PseudoGameObject* gameObject, const bool ProcessIndices)
{
	MovableObject* object = gameObject->OriginNode->getAttachedObject(0);
	Entity* entity = static_cast<Entity*>(object);
	if(!entity) return 0;

	MeshParams Params;
	Params.Scale = gameObject->GetScale();

	return new MeshInfo(entity->getMesh(), Params, ProcessIndices);
}



Cloth::Cloth(const String& name,
			 PxPhysics *Physics,
			 PxCooking *Cooking,
			 PseudoGameObject* GameObject,
			 PxVec3 &Gravity) : ManualObject(name),
								m_Physics(Physics),
								m_Cooking(Cooking),
								m_GameObject(GameObject),
								m_ClothRabric(NULL),
								m_Cloth(NULL)
{

	MeshInfo *meshInfo = GetMeshInfo(GameObject, true);
	m_ClothRabric = CreateClothFabric(meshInfo, Gravity);

	PxTransform tr(IeConvert::ToPxTransform(GameObject->GetTransform()));

	m_Cloth = m_Physics->createCloth(tr, *m_ClothRabric, ParticlePositions(), *CollisionData(), PxClothFlag::eSWEPT_CONTACT);

	if(m_Cloth)
	{ 
		PxClothPhaseSolverConfig config;  
		config.solverType= PxClothPhaseSolverConfig::eFAST;
		config.stiffness = 0.8f;
		config.stretchStiffness = 0.5f;
		config.stretchLimit = 0.6f;

		m_Cloth->setPhaseSolverConfig(PxClothFabricPhaseType::eBENDING,  config) ; 
		m_Cloth->setPhaseSolverConfig(PxClothFabricPhaseType::eSTRETCHING, config) ; 
		m_Cloth->setPhaseSolverConfig(PxClothFabricPhaseType::eSHEARING, config) ; 
		m_Cloth->setPhaseSolverConfig(PxClothFabricPhaseType::eSTRETCHING_HORIZONTAL, config) ;
  
		m_Cloth->setDampingCoefficient(0.125f);    
	}

	GameObject->OriginNode->detachAllObjects();
	GameObject->OriginNode->attachObject(this);
}


Cloth::~Cloth(void)
{
	m_Cooking->release();
}


PxClothFabric *Cloth::CreateClothFabric(MeshInfo* meshInfo, PxVec3 &Gravity)
{

	m_nbVectices = meshInfo->Vertices.size();
	m_VertexBuffer = meshInfo->GetVerticesVectorPtr();

	clothDesc.points.count = m_nbVectices;
	clothDesc.points.stride = sizeof(PxVec3);
	clothDesc.points.data = m_VertexBuffer;

	m_nbIndices = meshInfo->Indices.size();
	m_IndexBuffer = meshInfo->GetIndicesUIntPtr();

	clothDesc.triangles.count = m_nbIndices / 3;
	clothDesc.triangles.stride = 3 * sizeof(unsigned int);
	clothDesc.triangles.data = m_IndexBuffer;

	m_nbNormals = m_nbVectices;
	m_NormalBuffer = meshInfo->GetNormalsVectorPtr();

	clothDesc.edgeFlags = 0;

	PxDefaultMemoryOutputStream buf;
	if (!m_Cooking->cookClothFabric(clothDesc, Gravity, buf))
	{
		return NULL;
	}

	PxDefaultMemoryInputData input(buf.getData(), buf.getSize());

	return m_Physics->createClothFabric(input);
}


PxClothCollisionSphere spheres[2] =
{
		{PxVec3(-1.0f, 0.0f, 0.0f), 0.5f},
		{PxVec3( 1.0f, 0.0f, 0.0f), 0.25f}
};

PxU32 capsulePairs[] = { 0, 1 };

	PxClothCollisionData *Cloth::CollisionData(void)
{
	PxClothCollisionData *cd = new PxClothCollisionData();

	//m_Collider[0].pos = PxVec3(-1.0f, 0.0f, 0.0f);
	//m_Collider[0].radius = 0.5f;

	//m_Collider[1].pos = PxVec3( 1.0f, 0.0f, 0.0f);
	//m_Collider[1].radius = 0.25f;



	cd->spheres = spheres;
	cd->numSpheres = 2;
	cd->pairIndexBuffer = capsulePairs;
	cd->numPairs = 1;

	//cd->setToDefault();

	//cd->numSpheres = 1;
	//cd->pairIndexBuffer = 0;

	//m_BoxCollider.pos = PxVec3(0.0f,10,0.0f);
	//m_BoxCollider.radius = 1;
	//cd->spheres = &m_BoxCollider;


return cd;
}


PxClothParticle *Cloth::ParticlePositions(void)
{
	PxClothParticle *points = new PxClothParticle[m_nbVectices];
	for (int i = 0; i < m_nbVectices; i++)
	{
		points[i].pos = m_VertexBuffer[i];
		if (i == 0)
		{
			points[i].invWeight = 0;
		}
		else
		{
			points[i].invWeight = 1.0f;
		}
	}

	return points;
}


IeTransform Cloth::GetGlobalPose(void)
{
	return IeConvert::ToIeTransform(m_Cloth->getGlobalPose());
}


PxCloth *Cloth::GetClothActor(void)
{
	return m_Cloth;
}


void Cloth::Update(void)
{
	//m_Cloth->setCollisionSpheres(m_Collider);

	PxClothReadData *pData = m_Cloth->lockClothReadData();
	PxClothParticle *pParticles = const_cast<PxClothParticle*>(pData->particles);

	for (int i = 0; i < m_nbVectices; i++)
	{
		m_VertexBuffer[i] = pParticles[i].pos;
		if (m_VertexBuffer[i].y < 0)
		{
			m_VertexBuffer[i].y = 0;
			pParticles[i].pos.y = 0;
		}
	}

	pData->unlock();

	for(unsigned int i = 0; i < m_nbIndices / 3; i+=3)
	{
		PxVec3 p1 = m_VertexBuffer[m_IndexBuffer[i]];
		PxVec3 p2 = m_VertexBuffer[m_IndexBuffer[i+1]];
		PxVec3 p3 = m_VertexBuffer[m_IndexBuffer[i+2]];
		PxVec3 n  = (p2-p1).cross(p3-p1);

		m_NormalBuffer[m_IndexBuffer[i]] += (n / 3.0f); 
		m_NormalBuffer[m_IndexBuffer[i+1]]  += n/3.0f ; 
		m_NormalBuffer[m_IndexBuffer[i+2]]  += n/3.0f ;    
	}

	for(unsigned int i = 0; i < m_nbNormals; i++)
	{ 
		PxVec3& n  = m_NormalBuffer[i];
		n = n.getNormalized();
	}
}


void Cloth::Render(void)
{
	clear();
	begin("Clothing", RenderOperation::OT_TRIANGLE_LIST);
	for (int i = 0; i < m_nbVectices; i++)
	{
		position(m_VertexBuffer[i].x, m_VertexBuffer[i].y, m_VertexBuffer[i].z);
	}

	for (int i = 0; i < m_nbIndices / 3; i +3)
	{
		normal(m_NormalBuffer[i].x, m_NormalBuffer[i].y, m_NormalBuffer[i].z);
		triangle(m_IndexBuffer[i * 3], m_IndexBuffer[i * 3 + 1], m_IndexBuffer[i * 3 + 2]);
	}
	end();
}