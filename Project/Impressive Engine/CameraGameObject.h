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
#pragma once


#include "GameObject.h"
#include "ViewPortsManager.h"
#include "RenderControlBase.h"
#include "Ray.h"


BEGIN_NAMESPACE
class CameraGameObject : public GameObject
{
public:
	CameraGameObject(const Ogre::String &Name, Scene *Holder, GameObject *Parent = NULL);
	~CameraGameObject(void);
	
#ifdef EDIT_MODE
	bool Update(void);

	bool Serialize(Attributes *Attributes);
#endif
	bool Deserialize(Attributes *Attributes);
	
	void SetInitializeState(void);
	void ResetToInitializeState(void);
	
#ifdef EDIT_MODE
	void BeginChangeProperties(void);
	void EndChangeProperties(void);
#endif

	void SetIsMain(bool Value)
	{
		m_IsMain = Value;
	}

	bool GetIsMain(void) const
	{
		return m_IsMain;
	}

	void SetScale(const Ogre::Vector3 &Scale)
	{
		// Nothing
		//m_SceneNode->setScale(Ogre::Vector3::UNIT_SCALE);
	}
	
	void SetScale(Ogre::Real X, Ogre::Real Y, Ogre::Real Z)
	{
		// Nothing
		//m_SceneNode->setScale(Ogre::Vector3::UNIT_SCALE);
	}

	void Pitch(Ogre::Real Value)
	{
		m_Camera->pitch(Ogre::Degree(Value));
	}

	void Yaw(Ogre::Real Value)
	{
		m_Camera->yaw(Ogre::Degree(Value));
	}

	void Roll(Ogre::Real Value)
	{
		m_Camera->roll(Ogre::Degree(Value));
	}

	const Ogre::Vector3 GetRotation(void)
	{
		return Ogre::Vector3(GetOrientation().getPitch().valueDegrees(), GetOrientation().getYaw().valueDegrees(), GetOrientation().getRoll().valueDegrees());
	}
	
	void SetRotation(Ogre::Real X, Ogre::Real Y, Ogre::Real Z)
	{	
		SetOrientation(Ogre::Quaternion());

		m_Camera->pitch(Ogre::Degree(X));
		m_Camera->yaw(Ogre::Degree(Y));
		m_Camera->roll(Ogre::Degree(Z));
	}
	
	void SetRotation(const Ogre::Vector3 &Rotation)
	{
		SetRotation(Rotation.x, Rotation.y, Rotation.z);
	}

	const Ogre::Quaternion &GetOrientation(void)
	{
		return m_Camera->getOrientation();
	}
	
	void SetOrientation(const Ogre::Quaternion &Quaternion)
	{
		m_Camera->setOrientation(Quaternion);
	}

	void SetOrientation(Ogre::Real W, Ogre::Real X, Ogre::Real Y, Ogre::Real Z)
	{
		m_Camera->setOrientation(Ogre::Quaternion(W, X, Y, Z));
	}

	const Ogre::AxisAlignedBox &GetWorldAABB(void) const
	{
#ifdef EDIT_MODE
		return m_BoxEntity->getWorldBoundingBox(true);
#else
		return Ogre::AxisAlignedBox::BOX_NULL;
#endif
	}

	GameObjectType GetType(void) const
	{
		return GOT_CAMERA;
	}

	void SetPolygonMode(Ogre::PolygonMode Mode)
	{
		m_Camera->setPolygonMode(Mode);
	}

	Ogre::PolygonMode GetPolygonMode(void) const
	{
		return m_Camera->getPolygonMode();
	}

	void Move(const Ogre::Vector3 &Vector)
	{
		m_Camera->move(Vector);
	}

	void MoveRelative(const Ogre::Vector3 &Vector)
	{
		m_Camera->moveRelative(Vector);
	}

	void SetDirection(Ogre::Real X, Ogre::Real Y, Ogre::Real Z)
	{
		m_Camera->setDirection(X, Y, Z);
	}

	void SetDirection(const Ogre::Vector3 &Vector)
	{
		m_Camera->setDirection(Vector);
	}

	Ogre::Vector3 GetDirection(void) const
	{
		return m_Camera->getDirection();
	}

	Ogre::Vector3 GetUp(void) const
	{
		return m_Camera->getUp();
	}

	Ogre::Vector3 GetRight(void) const
	{
		return m_Camera->getRight();
	}

	void LookAt(const Ogre::Vector3 &TargetPoint)
	{
		m_Camera->lookAt(TargetPoint);
	}

	void LookAt(Ogre::Real X, Ogre::Real Y, Ogre::Real Z)
	{
		m_Camera->lookAt(X, Y, Z);
	}

	void SetLODBias(Ogre::Real Factor)
	{
		if (Factor <= 0)
			Factor = 0.1f;

		m_Camera->setLodBias(Factor);
	}

	Ogre::Real GetLODBias(void) const
	{
		return m_Camera->getLodBias();
	}

	/*Ogre::Ray*/Ray *GetCameraToViewportRay(Ogre::Real X, Ogre::Real Y) const
	{
		return new Ray(m_Camera->getCameraToViewportRay(X, Y));
	}

	void SetFOVY(float FOV)
	{
		if (FOV <= 0)
			FOV = 0.1f;
		
		if (FOV >= 180)
			FOV = 179.9f;

		m_Camera->setFOVy(Ogre::Radian(Ogre::Degree(FOV)));
	}

	float GetFOVY(void) const
	{
		return m_Camera->getFOVy().valueDegrees();
	}

	bool SetFarClipDistance(Ogre::Real FarPlane);

	Ogre::Real GetFarClipDistance(void) const
	{
		return m_Camera->getFarClipDistance();
	}

	void SetNearClipDistance(Ogre::Real NearPlane)
	{
		m_Camera->setNearClipDistance(NearPlane);
	}

	Ogre::Real GetNearClipDistance(void) const
	{
		return m_Camera->getNearClipDistance();
	}

	void SetAutoAspectRatio(bool Value)
	{
		m_Camera->setAutoAspectRatio(Value);
	}

	bool GetAutoAspectRatio(void) const
	{
		return m_Camera->getAutoAspectRatio();
	}

	void SetAspectRatio(Ogre::Real Value)
	{
		m_Camera->setAspectRatio(Value);
	}

	Ogre::Real GetAspectRatio(void) const
	{
		return m_Camera->getAspectRatio();
	}

	void SetAutoTracking(GameObject *Target)
	{
		SetAutoTracking(Target, Ogre::Vector3::ZERO);
	}

	void SetAutoTracking(GameObject *Target, const Ogre::Vector3 &Offset)
	{
		if (Target)
			m_Camera->setAutoTracking(true, Target->_GetSceneNode(), Offset);
		else
			m_Camera->setAutoTracking(false);
	}

	void SetProjectionType(Ogre::ProjectionType Type)
	{
		m_Camera->setProjectionType(Type);
	}

	Ogre::ProjectionType GetProjectionType(void) const
	{
		return m_Camera->getProjectionType();
	}

	void SetOrthoWindow(Ogre::Real Width, Ogre::Real Height)
	{
		m_Camera->setOrthoWindow(Width, Height);
	}

	Ogre::Vector2 GetOrthoWindow() const
	{
		return Ogre::Vector2(m_Camera->getOrthoWindowWidth(), m_Camera->getOrthoWindowHeight());
	}

	const Ogre::Matrix3 GetWorldMatrix()
	{
		Ogre::Matrix3 matrix;
		GetOrientation().ToRotationMatrix(matrix);
		return matrix;
	}

	Ogre::Camera *_GetCamera(void)
	{
		return m_Camera;
	}

	DECLARE_SCRIPT_REGISTATION()

protected:
	Ogre::Camera *m_Camera;

#ifdef EDIT_MODE
	Ogre::Entity *m_BoxEntity;
	Ogre::SceneNode *m_Box;
	bool m_SetThisOnViewPort;
#endif
	bool m_IsMain;

private:
	bool m_Init_AutoAspectRatio;
	Ogre::PolygonMode m_Init_PolygonMode;
	Ogre::Vector3 m_Init_Position;
	Ogre::Quaternion m_Init_Orientation;
	float m_Init_LODBias;
	float m_Init_FOVY;
	float m_Init_FarClipDistance;
	float m_Init_NearClipDistance;
	float m_Init_AspectRatio;
	Ogre::ProjectionType m_Init_ProjectionType;
	Ogre::Vector2 m_Init_OrthoWindow;
};
END_NAMESPACE