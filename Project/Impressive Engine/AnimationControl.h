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

#pragma once

#include "StdAfxwxWidgets.h"
#include "StdAfxOGRE.h"
#include "IPropertiesListener.h"

BEGIN_NAMESPACE
class RenderControl;

class AnimationControl : public wxWindow, public IPropertiesListener
{
public:
	AnimationControl(wxWindow *Parent);
	//~AnimationControl(void);

	void Initialize(void);

	void Update(void);
	
	void SelectMesh(const wxString &FileName);
	//void SelectSkeletonFile(const wxString &FileName);

	Ogre::AnimationState *GetCurrentAnimationState(void);
	Ogre::AnimationStateSet *GetCurrentAnimationStateSet(void);
	Ogre::Animation *GetCurrentAnimation(void);

	void OnPropertyChanged(/*wxPropertyGridManager *Manager, */const wxPropertyGridEvent &event);
	
	wxString GetName(void)
	{
		return "Animation";
	}
	
	wxString GetCaption(void)
	{
		return "Animation Editor";
	}

	static AnimationControl *Create(wxWindow *Parent)
	{
		if (!m_ThisPointer)
			m_ThisPointer = new AnimationControl(Parent);

		return m_ThisPointer;
	}

	static AnimationControl *GetPointer(void)
	{
		return m_ThisPointer;
	}

private:
	void DoUpdateAnimation(void);

private:
	DECLARE_EVENT_TABLE()

	enum
	{
		ID_BUTTON_LIST = 1000,
		ID_BUTTON_PLAY,
		ID_BUTTON_PAUSE,
		ID_BUTTON_PROPERTIES
	};
	
	void OnSize(wxSizeEvent &event);
	void OnAnimationSelected(wxCommandEvent &event);
	void OnFrameSliderChanged(wxScrollEvent &event);
	void OnPlayButton(wxCommandEvent &event);
	void OnPauseButton(wxCommandEvent &event);
	void OnPropertiesButton(wxCommandEvent &event);

private:
	static AnimationControl *m_ThisPointer;

	RenderControl *m_RenderControl;
	wxListBox *m_AnimationsList;
	wxSlider *m_FrameSlider;
	wxBitmapButton *m_PlayButton;
	wxBitmapButton *m_PauseButton;
	wxBitmapButton *m_PropertiesButton;

	//wxString m_LastLoadedSkeleton;
	bool m_IsPlaying;
	float m_LastUpdateTime;
	unsigned short m_FPS;

	Ogre::SceneManager *m_SceneManager;
	Ogre::SceneNode *m_SceneNode;
	Ogre::AnimationState *m_LastAnimation;
};

END_NAMESPACE
#endif
