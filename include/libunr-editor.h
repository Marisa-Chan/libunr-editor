/*===========================================================================*\
|*  libunr-editor - An open source development suite for Unreal Engine 1     *|
|*  games and libunr                                                         *|
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
 * libunr-editor.h - Editor executable.
 * 
 * written by Jesse 'Hyzoran' Kowalik
 *========================================================================
*/
#pragma once

#include <wx/wxprec.h>
#include <wx/wx.h>

#include <wx/aui/aui.h>

#include <wx/statline.h>

#include <libunr.h>

#include "Components.h"
#include "Browsers.h"

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
	ID_BrowserAudio,
    ID_BrowserMusic,
	ID_BrowserGraphics,
	ID_BrowserMesh,
	ID_ViewLog,
	ID_ActiveTools,
	ID_MapEditor,
	ID_MeshEditor,
	ID_Manual
};

//There should only ever be one EdEditor Frame
class EdEditorFrame : public wxFrame
{
public:
    EdEditorFrame( const wxString& Title, const wxPoint& Pos, const wxSize& Size );
    ~EdEditorFrame();
    
    static bool NewTool( const EdToolFrame* Tool );
    static bool KillTool( const EdToolFrame* Tool );
    static void Editor_Log( const wxString& Msg );
    static EdEditorFrame* GetFrame();
    static TArray<UPackage*>* GetPackages();
    static void LoadPackages( const wxArrayString& Paths );
    
    //Events
    void OnExit( wxCommandEvent& event );
    void OnAbout( wxCommandEvent& event );
    void EVT_New( wxCommandEvent& event );
    void EVT_Open( wxCommandEvent& event );
    void EVT_Save( wxCommandEvent& event );
    void EVT_Import( wxCommandEvent& event );
    void EVT_Export( wxCommandEvent& event );
    void EVT_Preferences( wxCommandEvent& event );
    void EVT_BrowserPackage( wxCommandEvent& event );
    void EVT_BrowserClass( wxCommandEvent& event );
    void EVT_BrowserAudio( wxCommandEvent& event );
    void EVT_BrowserMusic( wxCommandEvent& event );
    void EVT_BrowserGraphics( wxCommandEvent& event );
    void EVT_BrowserMesh( wxCommandEvent& event );
    void EVT_ViewLog( wxCommandEvent& event );
    void EVT_ActiveTools( wxCommandEvent& event );
    void EVT_MapEditor( wxCommandEvent& event );
    void EVT_MeshEditor( wxCommandEvent& event );
    void EVT_Manual( wxCommandEvent& event );
    
    static wxIcon m_File;
    static wxIcon m_Dir;
    static wxIcon m_Save;
    
    wxDECLARE_EVENT_TABLE();
    
private:
    
    static EdEditorFrame* sm_Frame; //Reference to the one instance of EdEditorFrame.
    static TArray<EdToolFrame*> sm_ToolArray;
    static size_t sm_EmptySlots;
    static TArray<UPackage*> sm_Packages;
    
    bool m_bAboutUp; //Is there already an about instance?
    
};

enum
{
	ID_GameChoice,
	ID_NewGameChoice,
	ID_DirDialog,
	ID_Confirm
};

class EdGamePrompt : public wxDialog
{
public:
    EdGamePrompt( TArray<char*>* Names );
    
    int GetSelected();
    
    wxDECLARE_EVENT_TABLE();
    
private:
    wxListBox* m_ChoiceBox;
    
private:
    void EVT_ChoiceSelect( wxCommandEvent& event );
    void EVT_NewGame( wxCommandEvent& event );
    
    int m_GameSelect = -1;
    TArray<char*>* m_Names;
    size_t m_GameSize;
};

class EdNewGameDialog : public wxDialog
{
public:
    EdNewGameDialog( size_t NewIndex, int* ChoiceOut );
    
    wxDECLARE_EVENT_TABLE();
    
private:
    wxTextCtrl* m_NameField;
    wxTextCtrl* m_ExeField;
    wxTextCtrl* m_PathField;
    
    int m_GameSize;
    
private:
    void EVT_Confirm( wxCommandEvent& event );
    void EVT_DirDialog( wxCommandEvent& event );
};

class WXAPP_EdEditor : public wxApp
{
public:
    virtual bool OnInit();
    
    static int GamePromptHandler( TArray<char*>* Names );
};
