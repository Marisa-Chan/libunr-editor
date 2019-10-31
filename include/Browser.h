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
 * Browsers.h - Browser class definitions
 * 
 * written by Jesse 'Hyzoran' Kowalik & Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once

#include <wx/wxprec.h>
#include <wx/wx.h>
#include <wx/notebook.h>
#include <wx/splitter.h>
#include <wx/treectrl.h>
#include <wx/listctrl.h>
#include <wx/statline.h>
#include <wx/dataview.h>
#include "Components.h"
#include <Core/UPackage.h>

enum
{
  ID_BrowserNew,
  ID_BrowserOpen,
  ID_BrowserSave,
  ID_BrowserImport,
  ID_BrowserExport,
  ID_PackageList,
  ID_BrowserDock,
  ID_BrowserShowPackage,
  ID_ListView,
  ID_PackageTab,
  ID_ShowListView,
  ID_ShowTileView,
  ID_FullPackageLoad
};

struct wxObjectItemData : public wxTreeItemData
{
  wxObjectItemData( UObject* Obj ) : m_Obj( Obj ) {}
  UObject* m_Obj = NULL;
};

class EdBrowser : public EdToolFrame
{
public: 
  EdBrowser( UClass* InType, bool bDock = false, wxSize Size = EdToolFrame::GetFrameSize(), bool bShowViewButtons = true );
	~EdBrowser();
	
  virtual void PackagesAdded( size_t PackageStartIndex );
  virtual void PackagesRemoved();
  virtual void ObjectsAdded();
  virtual void ObjectsRemoved();

  // Runtime Variables
  UClass* m_Type; // The object type this browser displays
    
protected:
  virtual void Update(); // Updates the view of this browser
  virtual void ListUpdate();
  virtual void TileUpdate();

  virtual void ConstructPackageButtons();
  virtual void ListConstruct(); 
  virtual void TileConstruct();
  virtual void FinishConstruct(); // Generic constructor finalizer for all browser types

  wxBoxSizer* m_OptionsSizer;
  wxTreeCtrl* m_ListView;

  wxMenu* m_MenuFile = NULL;
  wxMenuBar* m_MenuBar = NULL;
  wxBoxSizer* m_WindowAreaSizer = NULL;
  wxPanel* m_OptionsBar = NULL;
  wxButton* m_ShowListView = NULL;
  wxButton* m_ShowTileView = NULL;
  wxButton* m_Dock = NULL;

  // Optional buttons that child classes may opt to use
  wxButton* m_OpenPackage;
  wxButton* m_SavePackage;
  wxButton* m_LoadFullPackage;

private:
  void OnExit( wxCommandEvent& event );
  void OnBrowserNew( wxCommandEvent& event );
  void OnBrowserOpen( wxCommandEvent& event );
  void OnBrowserSave( wxCommandEvent& event );
  void OnBrowserImport( wxCommandEvent& event );
  void OnBrowserExport( wxCommandEvent& event );
  void OnBrowserDock( wxCommandEvent& event );
    
  wxDECLARE_EVENT_TABLE();
};
