 /*===========================================================================*\
|*  libunr - An open source Unreal Engine 1 implementation in library form   *|
|*  Copyright (C) 2018-2019  Adam W.E. Smith                                 *|
|*  libunr-editor - An open source development suite for Unreal Engine 1     *|
|*  games and libunr                                                         *|
|*  Copyright (C) 2018-2019  Adam W.E. Smith                                 *|
|*                                                                           *|
|*  This program is free software: you can redistribute it and/or modify     *|
|*  it under the terms of the GNU Affero General Public License as           *|
|*  published by the Free Software Foundation, either version 3 of the       *|
|*  License, or (at your option) any later version.                          *|
|*                                                                           *|
|*  This program is distributed in the hope that it will be useful,          *|
|*  but WITHOUT ANY WARRANTY; without even the implied warranty of           *|
|*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *|
|*  GNU Affero General Public License for more details.                      *|
|*                                                                           *|
|*  You should have received a copy of the GNU Affero General Public License *|
|*  along with this program.  If not, see <https://www.gnu.org/licenses/>.   *|
\*===========================================================================*/

/*========================================================================
 * libunr-editor.h - Editor executable.
 * 
 * written by Jesse 'Hyzoran' Kowalik
 *========================================================================
*/
#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <wx/statline.h>

#include <libunr/Util/TArray.h>

#include "Components.h"

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
    
    static bool NewTool( const EdToolFrame* Tool );
    static bool KillTool( const EdToolFrame* Tool );
    static void Editor_Log( const wxString& Msg );
    static EdEditorFrame* GetFrame();
    
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
    
    wxDECLARE_EVENT_TABLE();
    
private:
    
    static EdEditorFrame* sm_Frame; //Reference to the one instance of EdEditorFrame.
    static TArray<EdToolFrame*> sm_ToolArray;
    static size_t sm_EmptySlots;
    
    bool m_bAboutUp; //Is there already an about instance?
};

class WXAPP_EdEditor : public wxApp
{
public:
    virtual bool OnInit();
};
