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
#ifdef EDIT_MODE

#include "AnimationControl.h"
#include "AnimationControl.h"
#include "Engine.h"
#include "Time.h"
#include "Properties.h"
#include "RenderControl.h"
#include "EditorCamera.h"
#include "IO.h"
#include "RegistryKey.h"
#include "MainWindow.h"
#include "Utility.h"


BEGIN_NAMESPACE
BEGIN_EVENT_TABLE(AnimationControl, wxWindow)
	EVT_SIZE(AnimationControl::OnSize)
	EVT_BUTTON(ID_BUTTON_PLAY, AnimationControl::OnPlayButton)
	EVT_BUTTON(ID_BUTTON_PAUSE, AnimationControl::OnPauseButton)
	EVT_BUTTON(ID_BUTTON_PROPERTIES, AnimationControl::OnPropertiesButton)
END_EVENT_TABLE()


AnimationControl *AnimationControl::m_ThisPointer = 0;


AnimationControl::AnimationControl(wxWindow *Parent) :
	wxWindow(Parent, -1),
	m_IsPlaying(false),
	m_LastUpdateTime(0),
	m_FPS(30),
	m_RenderControl(NULL),
	m_SceneManager(NULL),
	m_SceneNode(NULL),
	m_LastAnimation(NULL),
	m_PlayButton(NULL)
{
}


//AnimationControl::~AnimationControl(void)
//{
//}


void AnimationControl::Initialize(void)
{
	m_AnimationsList = new wxListBox(this, ID_BUTTON_LIST, wxDefaultPosition, wxSize(100, -1));
	m_AnimationsList->Connect(wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler(AnimationControl::OnAnimationSelected), NULL, this);
	
	m_FrameSlider = new wxSlider(this, wxID_ANY, 0, 0, 120);
	m_FrameSlider->Connect(wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler(AnimationControl::OnFrameSliderChanged), NULL, this);
	m_FrameSlider->Connect(wxEVT_SCROLL_CHANGED, wxScrollEventHandler(AnimationControl::OnFrameSliderChanged), NULL, this);

	m_PlayButton = new wxBitmapButton(this, ID_BUTTON_PLAY, wxBitmap(Engine::PATH_EDITOR_IMAGE + "Play.png", wxBITMAP_TYPE_ANY));
	m_PauseButton = new wxBitmapButton(this, ID_BUTTON_PAUSE, wxBitmap(Engine::PATH_EDITOR_IMAGE + "Pause.png", wxBITMAP_TYPE_ANY));
	m_PropertiesButton = new wxBitmapButton(this, ID_BUTTON_PROPERTIES, wxBitmap(Engine::PATH_EDITOR_IMAGE + "Properties.png", wxBITMAP_TYPE_ANY));

	///Add these tools to a tool bar with 32*32 of size

	Layout();

	m_SceneManager = Engine::GetInstance().GetGraphic()->createSceneManager(Ogre::ST_GENERIC, "AnimationEditorSceneManager");

	m_RenderControl = new RenderControl(this, "AnimationEditorViewPort", false, new EditorCamera(EditorCamera::ECT_FLY, m_SceneManager->createCamera("AnimationEditorCamera")));

	OnSize(wxSizeEvent());
}


void AnimationControl::Update(void)
{
	if (m_RenderControl)
		m_RenderControl->_Update();

	if (!m_SceneNode || !GetCurrentAnimationState())
		return;

	if (m_IsPlaying)
	{
		const float FRAME_TIME = 1000.f / m_FPS;

		if (m_LastUpdateTime + FRAME_TIME <= Time::GetInstance().GetRealTime())
		{
			m_LastUpdateTime = Time::GetInstance().GetRealTime();

			m_FrameSlider->SetValue(m_FrameSlider->GetValue() + 1);

			if (m_FrameSlider->GetValue() == m_FrameSlider->GetMax())
			{
				if (GetCurrentAnimationState()->getLoop())
				{
					m_FrameSlider->SetValue(m_FrameSlider->GetMin());
				}
				else
					m_IsPlaying = false;
			}

			DoUpdateAnimation();
		}
	}
}


void AnimationControl::SelectMesh(const wxString &FileName)
{
	//Ogre::Animation::setDefaultInterpolationMode(Ogre::Animation::IM_LINEAR);
	//Ogre::Animation::setDefaultRotationInterpolationMode(Ogre:: Animation::RIM_LINEAR);
	 
	if (m_SceneNode)
		m_SceneManager->destroySceneNode(m_SceneNode);

	Ogre::Entity *ent = m_SceneManager->createEntity(STRING_WX_TO_OGRE(FileName));
	m_SceneNode = m_SceneManager->getRootSceneNode()->createChildSceneNode("Temp");
	m_SceneNode->attachObject(ent);

	Ogre::MovableObject *object = m_SceneNode->getAttachedObject(0);
	m_RenderControl->GetCamera()->SetPosition(object->getBoundingBox().getAllCorners()[4] + Ogre::Vector3(100, 100, 100));
	m_RenderControl->GetCamera()->LookAt(object->getBoundingBox().getCenter());
	object = NULL;

	m_AnimationsList->Clear();

	if (ent->getAllAnimationStates())
	{
		Ogre::AnimationStateIterator it = ent->getAllAnimationStates()->getAnimationStateIterator();

		while (it.hasMoreElements())
		{
			m_AnimationsList->Insert(it.getNext()->getAnimationName(), 0);
		}

		if (m_AnimationsList->GetCount())
		{
			m_AnimationsList->Select(0);
			OnAnimationSelected(wxCommandEvent());
		}

		//int maxFramesCount = 0;
		//for (unsigned short i = 0; i < GetCurrentAnimation()->getNumNodeTracks(); i++)
		//	if (maxFramesCount < GetCurrentAnimation()->getNodeTrack(i)->getNumKeyFrames())
		//		maxFramesCount = GetCurrentAnimation()->getNodeTrack(i)->getNumKeyFrames();

		//m_FrameSlider->SetMax(maxFramesCount);
	}
	
	((MainWindow*)GetParent())->ShowWindow(AnimationControl::GetPointer()->GetName());
}


//void AnimationControl::SelectSkeletonFile(const wxString &FileName)
//{
//	if (m_LastLoadedSkeleton.size() > 0)
//		Ogre::SkeletonManager::getSingleton().unload(m_LastLoadedSkeleton.c_str());
//
//	m_AnimationsList->Clear();
//
//	m_LastLoadedSkeleton = FileName;
//
//	if (m_LastLoadedSkeleton.size() < 10)
//		return;
//
//	Ogre::SkeletonPtr skeleton = Ogre::SkeletonManager::getSingleton().load(m_LastLoadedSkeleton.c_str(), Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
//
//	for (unsigned int i = 0; i < skeleton->getNumAnimations(); i++)
//		m_AnimationsList->Insert(skeleton->getAnimation(i)->getName(), 0);
//
//	Ogre::SkeletonManager &ins = Ogre::SkeletonManager::getSingleton();
//	Ogre::ResourceManager::ResourceMapIterator it = ins.getResourceIterator();
//
//	while (it.hasMoreElements())
//	{
//		Ogre::String str = it.getNext()->getName();
//		
//		str = it.getNext()->getName();
//	}
//}


Ogre::AnimationState *AnimationControl::GetCurrentAnimationState(void)
{
	if (m_AnimationsList->GetSelection() >= 0)
	{
		return GetCurrentAnimationStateSet()->getAnimationState(STRING_WX_TO_OGRE(m_AnimationsList->GetString(m_AnimationsList->GetSelection())));
	}

	return NULL;
}


Ogre::AnimationStateSet *AnimationControl::GetCurrentAnimationStateSet(void)
{
	if (m_AnimationsList->GetSelection() >= 0)
	{
		return ((Ogre::Entity*)m_SceneNode->getAttachedObject(0))->getAllAnimationStates();
	}

	return NULL;
}


Ogre::Animation *AnimationControl::GetCurrentAnimation(void)
{
	if (m_AnimationsList->GetSelection() >= 0)
	{
		return ((Ogre::Entity*)m_SceneNode->getAttachedObject(0))->getSkeleton()->getAnimation(STRING_WX_TO_OGRE(m_AnimationsList->GetString(m_AnimationsList->GetSelection())));
	}

	return NULL;
}


void AnimationControl::OnPropertyChanged(/*wxPropertyGridManager *Manager, */const wxPropertyGridEvent &event)
{
	const wxString &name = event.GetPropertyName();

	if (name == "Loop")
	{
		GetCurrentAnimationState()->setLoop(event.GetPropertyValue().GetBool());
	}
	else if (name == "FPS")
	{
		m_FPS = event.GetProperty()->GetValue().GetInteger();
	}
	else if (name == "Start Frame")
	{
		m_FrameSlider->SetMin(event.GetProperty()->GetValue().GetInteger());

		OnPropertiesButton(wxCommandEvent());
	}
	else if (name == "End Frame")
	{
		m_FrameSlider->SetMax(event.GetProperty()->GetValue().GetInteger());

		OnPropertiesButton(wxCommandEvent());
	}
	else if (name == "Current Frame")
	{
		m_FrameSlider->SetValue(event.GetProperty()->GetValue().GetInteger());
	}

	DoUpdateAnimation();
}


void AnimationControl::DoUpdateAnimation(void)
{
	if (m_LastAnimation)
	{
		const float TIME = 1.f / m_FPS;
		m_LastAnimation->setTimePosition(m_FrameSlider->GetValue() * TIME); 
	}
}
	

void AnimationControl::OnSize(wxSizeEvent &event)
{
	if (!m_PlayButton)
		return;

	int y = GetSize().y - m_PlayButton->GetSize().y - 5;

	m_PlayButton->SetPosition(wxPoint(5, y));
	m_PauseButton->SetPosition(wxPoint(m_PlayButton->GetRect().GetRight() + 5, y));
	m_PropertiesButton->SetPosition(wxPoint(GetSize().x - m_PropertiesButton->GetSize().x - 5, y));
	
	y -= m_FrameSlider->GetSize().y + 5;
	m_FrameSlider->SetSize(5, y, GetSize().x - 10, m_FrameSlider->GetSize().y);
	
	y -= 5;

	m_AnimationsList->SetSize(GetSize().x - m_AnimationsList->GetSize().x - 5, 5, m_AnimationsList->GetSize().x, y);

	m_RenderControl->SetSize(5, 5, GetSize().x - m_AnimationsList->GetSize().x - 15, y);
}


void AnimationControl::OnAnimationSelected(wxCommandEvent &event)
{
	if (m_LastAnimation)
	{
		m_LastAnimation->setEnabled(false);
		m_LastAnimation = NULL;
	}

	m_LastAnimation = GetCurrentAnimationState();

	if (m_LastAnimation)
	{
		m_LastAnimation->setEnabled(true);

		m_FrameSlider->SetMax((int)Ogre::Math::Ceil(GetCurrentAnimation()->getLength() * m_FPS));
	}

	OnPropertiesButton(wxCommandEvent());

	m_FrameSlider->SetFocus();
}


void AnimationControl::OnFrameSliderChanged(wxScrollEvent &event)
{
	if (m_LastAnimation)
	{
		DoUpdateAnimation();

		OnPropertiesButton(wxCommandEvent());
	}
}


void AnimationControl::OnPlayButton(wxCommandEvent &event)
{
	m_IsPlaying = true;
	m_LastUpdateTime = 0;
}


void AnimationControl::OnPauseButton(wxCommandEvent &event)
{
	m_IsPlaying = false;
}


void AnimationControl::OnPropertiesButton(wxCommandEvent &event)
{
	if (!GetCurrentAnimationState())
		return;

	Properties &prop = Properties::GetInstance();

	prop.BeginNewProperties(AnimationControl::GetPointer()->GetCaption(), this);

	prop.AddCategory("Animation");
	
	prop.AddPropertyOfFloat("Length", GetCurrentAnimationState()->getLength(), "Length of selected animation in seconds");
	prop.SetReadOnly("Length", true);

	prop.AddPropertyOfBool("Loop", GetCurrentAnimationState()->getLoop());
	prop.AddPropertyOfInt("FPS", m_FPS);
	prop.AddPropertyOfInt("Start Frame", m_FrameSlider->GetMin());
	prop.AddPropertyOfInt("End Frame", m_FrameSlider->GetMax());
	prop.AddPropertyOfInt("Current Frame", m_FrameSlider->GetValue());

	prop.EndNewProperties();
}
END_NAMESPACE

#endif