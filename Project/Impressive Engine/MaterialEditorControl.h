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
class MaterialPageControl;


class TechniquePropertyListener : public IPropertiesListener
{
public:
	TechniquePropertyListener(Ogre::Technique *Selected);

private:
	void OnPropertyChanged(/*wxPropertyGridManager *Manager, */const wxPropertyGridEvent &event);

private:
	Ogre::Technique *m_Selected;
};


class PassPropertyListener : public IPropertiesListener
{
public:
	PassPropertyListener(Ogre::Pass *Selected);

private:
	void OnPropertyChanged(/*wxPropertyGridManager *Manager, */const wxPropertyGridEvent &event);

private:
	Ogre::Pass *m_Selected;
};


class MaterialEditorControl : public wxWindow, public Ogre::MaterialSerializer::Listener
{
public:
	MaterialEditorControl(wxWindow *Parent);
	//~MaterialEditorControl(void);

	void Initialize(void);

	void Update(void);

	void SelectMaterial(const wxString &MaterialName);
	void Refresh(void);
	void Save(void);
	void SaveCore(void);

	void RefreshMaterialProperties(void);

	static MaterialEditorControl *Create(wxWindow *Parent)
	{
		if (!m_ThisPointer)
			m_ThisPointer = new MaterialEditorControl(Parent);

		return m_ThisPointer;
	}

	static MaterialEditorControl *GetPointer(void)
	{
		return m_ThisPointer;
	}

	wxString GetName(void)
	{
		return "MaterialEditor";
	}
	
	wxString GetCaption(void)
	{
		return "Material Editor";
	}

	void RefreshScript(void);

	RenderControl *_GetRenderControl(void)
	{
		return m_RenderControl;
	}
	
	enum MaterialType
	{
		MT_Diffuse = 0,
		MT_SpecularDiffuse,
		MT_BumpDiffuse,
		MT_BumpSpecularDiffuse
	};

private:
	void SaveScript(void);
	Ogre::MaterialPtr GetCurrentMaterial(void);
	MaterialType GetTypeOfMaterial(const Ogre::MaterialPtr &Material);

private:

	DECLARE_EVENT_TABLE()

	enum
	{
		IDB_SAVE = 1000,
		IDB_APPLY,
		IDB_OBJECT_ITERATION
	};
	
	void OnSize(wxSizeEvent &event);
	void OnMaterialSelected(wxCommandEvent &event);
	//void OnTechniqueSelected(wxCommandEvent &event);
	void OnPassSelected(wxCommandEvent &event);
	void OnSave(wxCommandEvent &event);
	void OnApply(wxCommandEvent &event);
	void OnObjectIteration(wxCommandEvent &event);

	void materialEventRaised(Ogre::MaterialSerializer *Serializer, Ogre::MaterialSerializer::SerializeEvent Event, bool &Skip, const Ogre::Material *Material);
	void techniqueEventRaised(Ogre::MaterialSerializer *Serializer, Ogre::MaterialSerializer::SerializeEvent Event, bool &Skip, const Ogre::Technique *Technique);
	void passEventRaised(Ogre::MaterialSerializer *Serializer, Ogre::MaterialSerializer::SerializeEvent Event, bool &Skip, const Ogre::Pass *Pass);
	void gpuProgramRefEventRaised(Ogre::MaterialSerializer *Serializer, Ogre::MaterialSerializer::SerializeEvent Event, bool &Skip, const Ogre::String &Attribute, const Ogre::GpuProgramPtr &Program, const Ogre::GpuProgramParametersSharedPtr &Params, Ogre::GpuProgramParameters *DefaultParams);
	void textureUnitStateEventRaised(Ogre::MaterialSerializer *Serializer, Ogre::MaterialSerializer::SerializeEvent Event, bool &Skip, const Ogre::TextureUnitState *TextureUnit);
	
public:
	static const char *PASS_BUMP_NAME;

	static const char *PROGRAM_BUMPVP_NAME;
	static const char *PROGRAM_BUMPFP_NAME;

	static const char *TEXALIAS_DIFFUSE_NAME;
	static const char *TEXALIAS_NORMAL_NAME;
	static const char *TEXALIAS_SPECULAR_NAME;
	static const char *TEXALIAS_REFLECTION_NAME;

private:
	static MaterialEditorControl *m_ThisPointer;

	wxToolBar *m_ToolBar;
	wxChoice *m_MaterialsList;
	//wxChoice *m_TechniquesList;
	wxChoice *m_PassesList;

	Ogre::SceneManager *m_SceneManager;
	Ogre::SceneNode *m_SceneNodeCube;
	Ogre::SceneNode *m_SceneNodeSphere;
	Ogre::SceneNode *m_SceneNodeCurrent;
	TechniquePropertyListener *m_TechniquePropertyListener;
	PassPropertyListener *m_PassPropertyListener;
	
	wxNotebook *m_Notebook;
	RenderControl *m_RenderControl;
	MaterialPageControl *m_ScriptPage;

	Ogre::String m_ImportText, m_InheritanceText;
	bool m_IsThisBumpPass;
};
END_NAMESPACE

#endif