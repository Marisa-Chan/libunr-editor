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

/*-----------------------------------------------------------------------------
 * EdEditorFrame
 * Central Editor window
-----------------------------------------------------------------------------*/
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
  void EventNew(wxCommandEvent& event); //Prompt to create new map/package/object.
  void EventOpen(wxCommandEvent& event); //Prompt to open existing package.
  void EventSave(wxCommandEvent& event); //Prompt to save modified packages.
  void EventImport(wxCommandEvent& event); //Prompt to import Assets to a package.
  void EventExport(wxCommandEvent& event); //Prompt to export an object from a package.
  void EventPreferences(wxCommandEvent& event);
  void EventBrowserPackage(wxCommandEvent& event);
  void EventBrowserClass(wxCommandEvent& event);
  void EventBrowserSound(wxCommandEvent& event);
  void EventBrowserMusic(wxCommandEvent& event);
  void EventBrowserTexture(wxCommandEvent& event);
  void EventBrowserMesh(wxCommandEvent& event);
  void EventBrowserGroup(wxCommandEvent& event);
  void EventViewLog(wxCommandEvent& event);
  void EventActiveTools(wxCommandEvent& event); //List active EdToolFrame instances.
  void EventMapEditor(wxCommandEvent& event);
  void EventMeshEditor(wxCommandEvent& event);
  void EventManual(wxCommandEvent& event);
  void EventAdvancedOptions( wxCommandEvent& event );

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
