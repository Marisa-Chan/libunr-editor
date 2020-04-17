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

#include "EdEditor.h"

#include "EdToolFrame.h"
#include "EdConfigFrame.h"
#include "EdGamePrompt.h"
#include "EdBrowsers.h"

//EdEditorFrame - Central Editor window.
class EdEditorFrame : public wxFrame
{
public:
    EdEditorFrame( const wxString& Title, const wxPoint& Pos, const wxSize& Size );
    ~EdEditorFrame();

    void DoTick( wxIdleEvent& event ) { EdEditor::DoTick( event ); }

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
        ID_BrowserTexture,
        ID_BrowserMesh,
        ID_ViewLog,
        ID_ActiveTools,
        ID_MapEditor,
        ID_MeshEditor,
        ID_Manual,
        ID_AdvancedOptions,
        ID_ObjectPlay,
        ID_ObjectOpen,
        ID_ObjectProperties,
        ID_ObjectExport
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

    wxDECLARE_EVENT_TABLE();
};

class EdAbout : public wxFrame
{
public:
    EdAbout( wxWindow* Parent );

    void OnClose( wxCommandEvent& event );
};

class EditorApp : public wxApp
{
public:
    virtual bool OnInit();
    
};
