/*===========================================================================*\
|*  libunr-editor - An open source development suite for Unreal Engine 1     *|
|*  Copyright (C) 2018-2019  Adam W.E. Smith                                 *|
|*                                                                           *|
|*  This program is free software: you can redistribute it and/or modify     *|
|*  it under the terms of the GNU General Public License as published by     *|
|*  the Free Software Foundation, either version 3 of the License, or        *|
|*  (at your option) any later version.                                      *|
|*                                                                           *|
|*  This program is distributed in the hope that it will be useful,          *|
|*  but WITHOUT ANY WARRANTY; without even the implied warranty of           *|
|*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *|
|*  GNU General Public License for more details.                             *|
|*                                                                           *|
|*  You should have received a copy of the GNU General Public License        *|
|*  along with this program. If not, see <https://www.gnu.org/licenses/>.    *|
\*===========================================================================*/

/*========================================================================
 * EdMain.h - Main Editor interface
 *
 * written by Jesse 'Hyzoran' Kowalik
 *========================================================================
*/
#pragma once

#include <wx/wxprec.h>
#include <wx/wx.h>
#include <wx/aui/aui.h>
#include <wx/statline.h>
#include <wx/display.h>

#include <libunr.h>

#include "EdConfigFrame.h"
#include "EdGamePrompt.h"
#include "EdToolFrame.h"
#include "EdBrowsers.h"

#define C_BUTTONSIZE 34
#define C_BUTTONCOLOUR wxColour( 130, 130, 130 )
#define C_TOOLBUTTONSIZE 28

//EdEditorFrame - Forms the core of the Editor, and manages all Frontend-Level systems.
//Most notably, tracks and manages all EdToolFrame instances.
class EdEditorFrame : public wxFrame
{
public:
    EdEditorFrame(const wxString& Title, const wxPoint& Pos, const wxSize& Size);
    ~EdEditorFrame();

//EdToolFrame management.
    size_t RegisterTool(EdToolFrame* Tool);
    bool UnregisterTool(size_t id);

private:
    static TArray<EdToolFrame*> m_ToolArray;
    static size_t m_EmptySlots;

public:
//Libunr Interaction
    void LoadPackages(const wxArrayString& Paths);

//Object Interaction
    void ObjectMenu(UObject* Obj);
    void ObjectPlay(UObject* Obj);
    void ObjectProperties(UObject* Obj);

    wxString GetGameDir();

//Events
    enum
    {
        ID_New,
        ID_Open,
        ID_Save,
        ID_Import,
        ID_Export,
        ID_Preferences,
        ID_BrowserPackage,
        ID_BrowserClass,
        ID_BrowserSound,
        ID_BrowserMusic,
        ID_BrowserGraphics,
        ID_BrowserMesh,
        ID_ViewLog,
        ID_ActiveTools,
        ID_MapEditor,
        ID_MeshEditor,
        ID_Manual,
        ID_AdvancedOptions
    };

//Event Handlers
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void EVT_New(wxCommandEvent& event); //Prompt to create new map/package/object.
    void EVT_Open(wxCommandEvent& event); //Prompt to open existing package.
    void EVT_Save(wxCommandEvent& event); //Prompt to save modified packages.
    void EVT_Import(wxCommandEvent& event); //Prompt to import Assets to a package.
    void EVT_Export(wxCommandEvent& event); //Prompt to export an object from a package.
    void EVT_Preferences(wxCommandEvent& event);
    void EVT_BrowserPackage(wxCommandEvent& event);
    void EVT_BrowserClass(wxCommandEvent& event);
    void EVT_BrowserSound(wxCommandEvent& event);
    void EVT_BrowserMusic(wxCommandEvent& event);
    void EVT_BrowserTexture(wxCommandEvent& event);
    void EVT_BrowserMesh(wxCommandEvent& event);
    void EVT_BrowserGroup(wxCommandEvent& event);
    void EVT_ViewLog(wxCommandEvent& event);
    void EVT_ActiveTools(wxCommandEvent& event); //List active EdToolFrame instances.
    void EVT_MapEditor(wxCommandEvent& event);
    void EVT_MeshEditor(wxCommandEvent& event);
    void EVT_Manual(wxCommandEvent& event);
    void EVT_AdvancedOptions( wxCommandEvent& event );

//Static Resource references
    static wxBitmap sm_bmpNew;
    static wxBitmap sm_bmpDir;
    static wxBitmap sm_bmpSave;
    static wxBitmap sm_bmpPackage;
    static wxBitmap sm_bmpClass;
    static wxBitmap sm_bmpSound;
    static wxBitmap sm_bmpMusic;
    static wxBitmap sm_bmpGraphics;
    static wxBitmap sm_bmpMesh;
    static wxBitmap sm_bmpGroup;

    static wxIcon sm_icoNew;
    static wxIcon sm_icoDir;
    static wxIcon sm_icoSave;
    static wxIcon sm_icoPackage;
    static wxIcon sm_icoClass;
    static wxIcon sm_icoSound;
    static wxIcon sm_icoMusic;
    static wxIcon sm_icoGraphics;
    static wxIcon sm_icoMesh;
    static wxIcon sm_icoGroup;

//subdir suffix for different package types.
    static wxString csm_SubDir_U;
    static wxString csm_SubDir_UAX;
    static wxString csm_SubDir_UMX;
    static wxString csm_SubDir_UTX;
    static wxString csm_SubDir_UNR;
    static wxString csm_SubDir_USM;
    static wxString csm_SubDir_USA;

    wxDECLARE_EVENT_TABLE();
};

extern EdEditorFrame* g_MainFrame;

class EdObjectPopup : public wxMenu
{
public:
    EdObjectPopup(UObject* Obj);
};

class EdAbout : public wxFrame
{
public:
    EdAbout();
};

class EditorApp : public wxApp
{
public:
    virtual bool OnInit();
    void DoTick(wxIdleEvent& event);

    static int GamePromptHandler( TArray<char*>* Names );
    int sm_SelectedGame;

private:
    double CurrentTime;
    double LastTime;
};
