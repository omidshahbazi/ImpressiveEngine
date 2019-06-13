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
#include "ScriptTypesDefinition.h"
#include "ScriptRegister.h"
#include "Application.h"
#include "CoreFunctionality.h"
#include "AtmosphereManager.h"
#include "ViewPortsManager.h"
#include "MeshGameObject.h"
#include "LightGameObject.h"
#include "TerrainGameObject.h"
#include "ParticleGameObject.h"
#include "TrackAnimator.h"
#include "Ray.h"
#include "HID.h"
#include "IO.h"
#include "RegistryKey.h"
//#include "TreeParser.h"
//#include "TreeWriter.h"
#include "Time.h"

#ifdef EDIT_MODE
#include "DebugDrawer.h"
#endif

#include "StdAfxOGRE.h"
#include <stdio.h>

#include "AIEngine.h"
#include "GameMap.h"
#include "BaseObject.h"
#include "MovingObject.h"
#include "AIObject.h"
#include "AIEnemyObject.h"
#include "AINPCObject.h"
#include "AISellerObject.h"
#include "SteeringBehaviors.h"
#include "CustomerStand.h"
#include "AISoldierObject.h"
#include "AICommanderObject.h"
#include "IPhysicsEngine.h"


BEGIN_NAMESPACE
ScriptTypesDefinition *Singleton<ScriptTypesDefinition>::m_Instance = NULL;


//ScriptTypesDefinition::ScriptTypesDefinition(void)
//{
//}


//ScriptTypesDefinition::~ScriptTypesDefinition(void)
//{
//}


void ScriptTypesDefinition::Initialize(void)
{	
	ADD_KEYWORD("break do end else elseif function if local nil not and or repeat return then until for while true false ");
	ADD_KEYWORD("this");
	
#ifdef DEBUG
	ADD_KEYWORD("DEBUG_FILE_NAME DEBUG_FUNCTION")
#endif

	CALL_SCRIPT_REGISTRATION(CoreFunctionality)
	
	CALL_SCRIPT_REGISTRATION(Ray)

	CALL_SCRIPT_REGISTRATION(Scriptable)

	CALL_SCRIPT_REGISTRATION(GameObject)

	CALL_SCRIPT_REGISTRATION(MeshGameObject)

	CALL_SCRIPT_REGISTRATION(CameraGameObject)
	
	CALL_SCRIPT_REGISTRATION(LightGameObject)
	
	CALL_SCRIPT_REGISTRATION(TerrainGameObject)
	
	CALL_SCRIPT_REGISTRATION(ParticleGameObject)
	
	CALL_SCRIPT_REGISTRATION(Animator)
	
	CALL_SCRIPT_REGISTRATION(TrackAnimator)

	CALL_SCRIPT_REGISTRATION(Scene)

	CALL_SCRIPT_REGISTRATION(AtmosphereManager)

	CALL_SCRIPT_REGISTRATION(HID)
	
	CALL_SCRIPT_REGISTRATION(RegistryKey)

	CALL_SCRIPT_REGISTRATION(File)

	CALL_SCRIPT_REGISTRATION(Log)

	CALL_SCRIPT_REGISTRATION(IO)

	//CALL_SCRIPT_REGISTRATION(TreeReader)

	CALL_SCRIPT_REGISTRATION(Time)

#ifdef EDIT_MODE
	CALL_SCRIPT_REGISTRATION(DebugDrawer)
#endif

	CALL_SCRIPT_REGISTRATION(AIEngine)

	CALL_SCRIPT_REGISTRATION(GameMap)

	CALL_SCRIPT_REGISTRATION(BaseObject)

	CALL_SCRIPT_REGISTRATION(MovingObject)

	CALL_SCRIPT_REGISTRATION(AIObject)

	CALL_SCRIPT_REGISTRATION(AIEnemyObject)
	
	CALL_SCRIPT_REGISTRATION(AINPCObject)

	CALL_SCRIPT_REGISTRATION(AISellerObject)

	CALL_SCRIPT_REGISTRATION(SteeringBehaviors)

	CALL_SCRIPT_REGISTRATION(CustomerStand)

	CALL_SCRIPT_REGISTRATION(AISoldierObject)

	CALL_SCRIPT_REGISTRATION(AICommanderObject)

	CALL_SCRIPT_REGISTRATION(IPhysicsEngine)

	VirtualMachine &vm = VirtualMachine::GetInstance();
	vm.GetGlobalTable()["Core"] = CoreFunctionality::GetInstancePointer();
	vm.GetGlobalTable()["HID"] = HID::GetInstancePointer();
	vm.GetGlobalTable()["IO"] = IO::GetInstancePointer();
	vm.GetGlobalTable()["Log"] = Log::GetInstancePointer();
	vm.GetGlobalTable()["Time"] = Time::GetInstancePointer();
#ifdef EDIT_MODE
	vm.GetGlobalTable()["DebugDrawer"] = DebugDrawer::GetInstancePointer();
#endif
	vm.GetGlobalTable()["AIEngine"] = AIEngine::GetInstancePointer();
	vm.GetGlobalTable()["GameMap"] = GameMap::GetInstancePointer();
}
END_NAMESPACE