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
 * Browsers.h - Browser class definitions
 * 
 * written by Jesse 'Hyzoran' Kowalik & Adam 'Xaleros' Smith
 *========================================================================
*/
#pragma once

#include <wx/wxprec.h>
#include <wx/wx.h>

#include <Core/UPackage.h>
#include <wx/notebook.h>
#include <wx/splitter.h>
#include <wx/treectrl.h>
#include <wx/listctrl.h>
#include <wx/statline.h>
#include <wx/dataview.h>

#include "Components.h"

enum
{
  ID_BrowserNew,
  ID_BrowserOpen,
  ID_BrowserSave,
  ID_BrowserImport,
  ID_BrowserExport,
  ID_PackageList,
  ID_Browser_Dock,
  ID_Browser_ShowPackage,
  ID_ClassTree,
  ID_PackageTab
};


class EdBrowser : public EdToolFrame
{
public: 
	EdBrowser( UClass* InType, bool bDock = false, wxSize Size = EdToolFrame::GetFrameSize() );
	~EdBrowser();
	
	virtual void NewPackages( size_t PackageStartIndex );
  virtual void ObjectsAdded();
  virtual void PackagesRemoved();
  virtual void ObjectsRemoved();
    
  // View Preferences
  bool m_bShowPackage = true; //Show package after object name

  // Runtime Variables
  UClass* Type; // The object type this browser displays
    
  static wxIcon m_icoPackage;
  static wxIcon m_icoClass;
  static wxIcon m_icoSound;
  static wxIcon m_icoMusic;
  static wxIcon m_icoGraphics;
  static wxIcon m_icoMesh;
    
protected:
  virtual void ListConstruct();
  virtual void TileConstruct();
  virtual void FinishConstruct();
  void update(); //Update interface (does not do rendering, call respective update below for that)

  void listUpdate(); //Render view in List mode (Audio, Music )
  void tileUpdate(); //Render view in Tile mode (Textures, Meshes, Levels)
  void packageUpdate(); //Render view in Package mode.
  void classUpdate(); //Render view in Class Tree mode.
  void recurseExpandPopulate( TArray<UClass*>& ExpandedAry, wxTreeItemId Parent, wxTreeItemIdValue Cookie = 0 );
  wxTreeItemId addTreeItem( wxTreeItemId Parent, UClass* Obj );

  wxBoxSizer* optionsSizer;

  //Package mode
  UPackage* m_SelectedPackage = NULL;
  wxWindow* m_PackageHeader = NULL;
  wxListCtrl* m_PackageInfo = NULL;
  wxListCtrl* m_PackageFlags = NULL;
  wxListCtrl* m_NameTable = NULL;
  wxListCtrl* m_ExportTable = NULL;
  wxDataViewCtrl* m_ExportTree = NULL;
  wxListCtrl* m_ImportTable = NULL;
  wxDataViewCtrl* m_ImportTree = NULL;
  wxWindow* m_ViewPane = NULL; //Object preview tab
  wxSplitterWindow* m_MainSplitter = NULL;
  wxComboBox* m_PackagesList = NULL;
  wxNotebook* m_TabWindow = NULL;

  //Class/List Mode
  wxTreeCtrl* m_ListView = NULL;
  wxCheckBox* m_Check_ShowPackage = NULL;

  wxMenu* m_MenuFile = NULL;
  wxMenuBar* m_MenuBar = NULL;
  wxBoxSizer* m_WindowAreaSizer = NULL;
  wxPanel* m_OptionsBar = NULL;
  wxCheckBox* m_Check_Dock = NULL;

private:
  void OnExit( wxCommandEvent& event );
  void EVT_BrowserNew( wxCommandEvent& event );
  void EVT_BrowserOpen( wxCommandEvent& event );
  void EVT_BrowserSave( wxCommandEvent& event );
  void EVT_BrowserImport( wxCommandEvent& event );
  void EVT_BrowserExport( wxCommandEvent& event );
  void EVT_PackageList( wxCommandEvent& event );
  void EVT_Browser_Dock( wxCommandEvent& event );
  void EVT_Browser_ShowPackage( wxCommandEvent& event );
  void EVT_ClassTree( wxCommandEvent& event );
    
  wxDECLARE_EVENT_TABLE();
};

class EdClassBrowser : public EdBrowser
{
public:
  EdClassBrowser( bool bDock = false, wxSize Size = EdToolFrame::GetFrameSize() );
  ~EdClassBrowser();

  virtual void ObjectsAdded();
  virtual void ObjectsRemoved();
};

class EdTextureBrowser : public EdBrowser
{
public:
  EdTextureBrowser( bool bDock = false, wxSize Size = EdToolFrame::GetFrameSize() );
  ~EdTextureBrowser();

  virtual void ObjectsAdded();
  virtual void ObjectsRemoved();
};

class EdMeshBrowser : public EdBrowser
{
public:
  EdMeshBrowser( bool bDock = false, wxSize Size = EdToolFrame::GetFrameSize() );
  ~EdMeshBrowser();

  virtual void ObjectsAdded();
  virtual void ObjectsRemoved();
};

class EdLevelBrowser : public EdBrowser
{
public:
  EdLevelBrowser( bool bDock = false, wxSize Size = EdToolFrame::GetFrameSize() );
  ~EdLevelBrowser();

  virtual void ObjectsAdded();
  virtual void ObjectsRemoved();
};

class EdSoundBrowser : public EdBrowser
{
public:
  EdSoundBrowser( bool bDock = false, wxSize Size = EdToolFrame::GetFrameSize() );
  ~EdSoundBrowser();

  virtual void ObjectsAdded();
  virtual void ObjectsRemoved();
};

class EdMusicBrowser : public EdBrowser
{
public:
  EdMusicBrowser( bool bDock = false, wxSize Size = EdToolFrame::GetFrameSize() );
  ~EdMusicBrowser();

  virtual void ObjectsAdded();
  virtual void ObjectsRemoved();
};

class EdPackageBrowser : public EdBrowser
{
public:
  EdPackageBrowser( bool bDock = false, wxSize Size = EdToolFrame::GetFrameSize() );
  ~EdPackageBrowser();

  virtual void ObjectsAdded();
  virtual void ObjectsRemoved();
};
