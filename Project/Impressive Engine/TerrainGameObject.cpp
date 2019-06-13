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
#include "TerrainGameObject.h"
#include "GameMap.h"
#include "NavigationGraph.h"
#include "Scene.h"
#include "Time.h"
#include "Log.h"
#include "ScenesHolder.h"
#include "ScriptRegister.h"

#include "SparseGraph.h"

#ifdef EDIT_MODE
#include "EditorScene.h"
#endif


BEGIN_NAMESPACE
TerrainGameObject::TerrainGameObject(const Ogre::String &Name, Scene *Holder, GameObject *Parent) :
	GameObject(Name, Holder, Parent),
	m_TerrainGroup(NULL),
	m_Terrain(NULL),
	m_NavMeshNotLoaded(false)
{
	m_TerrainGroup = new Ogre::TerrainGroup(m_Holder->GetSceneManager(), Ogre::Terrain::ALIGN_X_Z, 513, 1000);
}


TerrainGameObject::~TerrainGameObject(void)
{
	m_TerrainGroup->removeAllTerrains();

	delete m_TerrainGroup;
}


void TerrainGameObject::Start(void)
{
	GameObject::Start();
}


bool TerrainGameObject::Update(void)
{
	return GameObject::Update();
}

#ifdef EDIT_MODE
bool TerrainGameObject::Serialize(Attributes *Attributes)
{
	GameObject::Serialize(Attributes);
	
	Ogre::Terrain::ImportData &data = m_TerrainGroup->getDefaultImportSettings();

	Attributes->AddString("HeightMap", m_HeightMapName);
	Attributes->AddFloat("Height", data.inputScale);
	Attributes->AddInteger("Size", data.terrainSize);
	Attributes->AddFloat("WorldSize", data.worldSize);

	Ogre::String index = "";
	for (unsigned int i = 0; i < data.layerList.size(); i++)
	{
		index = Ogre::StringConverter::toString(i);
		
		Attributes->AddString("TextureName" + index, data.layerList[i].textureNames[0].c_str());
		Attributes->AddFloat("TextureSize" + index, data.layerList[i].worldSize);
	}

	Attributes->AddString("NavMeshFileName", m_NavMeshFileName);

	return true;
}
#endif

bool TerrainGameObject::Deserialize(Attributes *Attributes)
{
	//m_PositionBeforeInvisibility = Attributes->GetVector3("Position");

	GameObject::Deserialize(Attributes);

	Ogre::Terrain::LayerInstanceList layers;

	int i = 0;
	Ogre::String index = "";
	Ogre::String value = "";
	while (true)
	{
		index = Ogre::StringConverter::toString(i);
		value = Attributes->GetString(("TextureName" + index).c_str());

		if (value.size() == 0)
			break;

		Ogre::Terrain::LayerInstance ins;
		ins.textureNames.push_back(value);
		ins.worldSize = Attributes->GetFloat(("TextureSize" + index).c_str());
		layers.push_back(ins);

		i++;
	}

	Prepare(Attributes->GetString("HeightMap"),
		Attributes->GetFloat("Height"),
		Attributes->GetInteger("Size"),
		Attributes->GetFloat("WorldSize"), 
		&layers);

	m_NavMeshFileName = Attributes->GetString("NavMeshFileName");

	m_NavMeshNotLoaded = (m_NavMeshFileName.size() > 0 ? true : false);

	GameMap::GetInstance().SetTerrain(this);

	return true;
}


bool TerrainGameObject::Prepare(const Ogre::String &HeightMapName, float TerrainHeight, unsigned int TerrainSize, float TerrainWorldSize, Ogre::Terrain::LayerInstanceList *LayerList)
{
	m_TerrainGroup->removeTerrain(0, 0);

	unsigned int startTime = Time::GetInstance().GetRealTime();
	
	m_HeightMapName = HeightMapName;

	m_TerrainGroup->SetTerrainSize(TerrainSize);
	m_TerrainGroup->SetTerrainWorldSize(TerrainWorldSize);

	Ogre::Terrain::ImportData &data = m_TerrainGroup->getDefaultImportSettings();
	data.terrainSize = TerrainSize;
	data.worldSize = TerrainWorldSize;
	data.inputScale = TerrainHeight;
	data.minBatchSize = 17;
	data.maxBatchSize = 65;
	data.layerList = *LayerList;

	data.inputImage = new Ogre::Image;
#ifdef EDIT_MODE
	try
	{
#endif
		data.inputImage->load(m_HeightMapName, Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME);
#ifdef EDIT_MODE
	}
	catch (Ogre::Exception&)
	{
		Log::GetInstance().AddError("Height map with name [" + m_HeightMapName + "] doesn't exists in resources");
		
		SetVisible(false);
		ShowBoundingBox(false);
		SetAsRemoved(true);
		m_Parent->RemoveChild(this);

		((EditorScene*)m_Holder)->RefreshSceneControlContent();

		return false;
	}
#endif

	m_TerrainGroup->defineTerrain(0, 0, &data);
	m_TerrainGroup->loadAllTerrains();
	m_Terrain = m_TerrainGroup->getTerrain(0, 0);

	m_WorldAABB = m_Terrain->getWorldAABB();
	
	Log::GetInstance().AddInfo("Terrain [" + m_Name + "] was generated in " + Ogre::StringConverter::toString(Time::GetInstance().GetRealTime() - startTime) + "ms");

	return true;
}


//void TerrainGameObject::GenerateNodes(void)
//{
//	unsigned int startTime = Time::GetInstance().GetRealTime();
//
//	XMLWriter *xml = new XMLWriter("C:/1.xml");
//
//	xml->WriteHeader();
//	xml->WriteElement("NAVMESH", false, "Terrain", m_Name.c_str());
//	xml->WriteLineBreak();
//
//	Attributes *att = NULL;
//
//	for (unsigned int x = 0; x < m_Terrain->getSize(); x++)
//		for (unsigned int y = 0; y < m_Terrain->getSize(); y++)
//		{
//			xml->WriteElement("POINT", false, "X", Ogre::StringConverter::toString(x).c_str(), "Y", Ogre::StringConverter::toString(y).c_str());
//			xml->WriteLineBreak();
//
//			att = new Attributes;
//			att->AddVector3("Point", m_Terrain->convertPosition(Ogre::Terrain::POINT_SPACE, Ogre::Vector3(x, y, m_Terrain->getHeightAtPoint(x, y)), Ogre::Terrain::WORLD_SPACE));
//			att->Write(xml);
//			delete att;
//
//			xml->WriteLineBreak();
//			
//			xml->WriteClosingTag("POINT");
//			xml->WriteLineBreak();
//
//			//Ogre::Vector3 vec = m_Terrain->convertPosition(Ogre::Terrain::POINT_SPACE, Ogre::Vector3(x, y, m_Terrain->getHeightAtPoint(x, y)), Ogre::Terrain::WORLD_SPACE);
//			//vec += Ogre::Vector3(2, 0, 3);
//			//Ogre::Vector3 xy = m_Terrain->convertPosition(Ogre::Terrain::WORLD_SPACE, vec, Ogre::Terrain::POINT_SPACE);
//			//xy = m_Terrain->convertPosition(Ogre::Terrain::WORLD_SPACE, vec, Ogre::Terrain::POINT_SPACE);
//		}
//
//	xml->WriteClosingTag("NAVMESH");
//
//	delete xml;
//
//	Log::GetInstance().AddInfo("Navigation Mesh for terrain [" + m_Name + "] was generated in " + Ogre::StringConverter::toString(Time::GetInstance().GetRealTime() - startTime) + "ms");
//
//
//	
//
//	//std::list<Ogre::Vector3> points;
//
//	//float *hData = m_Terrain->getHeightData();
//	//unsigned int count = TerrainSize * TerrainSize;
//	//for (unsigned int i = 0; i < count; i++)
//	//	points.push_back(Ogre::Vector3(0, *(hData + i), 0));
//}


//void TerrainGameObject::SetVisible(bool Value)
//{
//	GameObject::SetVisible(Value);
//
//	if (Value)
//		SetPosition(m_PositionBeforeInvisibility);
//	else
//	{
//		m_PositionBeforeInvisibility = GetPosition();
//		SetPosition(0, -100000, 0);
//	}
//}


BEGIN_DEFINE_SCRIPT_REGISTRATION(TerrainGameObject)
	BEGIN_DEFINE_TYPE_BASE(GameObject, TerrainGameObject, "TerrainGameObject")

	END_DEFINE_TYPE()
END_DEFINE_SCRIPT_REGISTRATION()

END_NAMESPACE