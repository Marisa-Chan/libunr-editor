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
 * EdBrowsers.h - Editor Browsers
 *
 * written by Jesse 'Hyzoran' Kowalik
 *========================================================================
*/
#pragma once

#include <wx/treelist.h>

#include "EdToolFrame.h"

//========================================================================
// EdBrowser - Abstract Browser class.
class EdBrowser : public wxWindow
{
public:
  //========================================================================
  // Standard Browser types
  #define BROWSERTYPE_COUNT 7

  enum E_BrowserType
  {
    E_BrowserType_Package = 0,
    E_BrowserType_Class = 1,
    E_BrowserType_Sound = 2,
    E_BrowserType_Music = 3,
    E_BrowserType_Texture = 4,
    E_BrowserType_Mesh = 5,
    E_BrowserType_Level = 6
  };

public:
  EdBrowser( wxWindow* Parent );
  ~EdBrowser();

  virtual void ObjectUpdate( bool m_bUpdatePackageList = true ) = 0; //Update Main browser Ctrl

  void EVT_FilterPackage( wxCommandEvent& event );
  void EVT_FilterPackageCtrl( wxCommandEvent& event );

protected:

  wxBoxSizer* m_VSizer;

  wxPanel* m_HeaderPanel;
    wxBoxSizer* m_HeaderSizer;

    EdEditor::EdUPackageCtrl* m_PackageCtrl = NULL;
    wxCheckBox* m_CheckFilterPackage = NULL;

private:
    wxDECLARE_EVENT_TABLE();
};

//========================================================================
// EdPackageBrowser - Inspects the contents and properties of Package objects,
// such as Import/Export tables, Name tables, Headers, etc.
class EdPackageBrowser : public EdBrowser
{
public:
  EdPackageBrowser( wxWindow* Parent, UPackage* Package = NULL, bool bStartDocked = false );

  void ObjectUpdate( bool m_bUpdatePackageList = true );

private:
};

//========================================================================
// EdClassBrowser - Renders a class tree, given a root class.
class EdClassBrowser : public EdBrowser
{
public:
  EdClassBrowser( wxWindow* Parent, UClass* Root, bool bStartDocked = false );
  ~EdClassBrowser();

  void ObjectUpdate( bool bUpdatePackageList = true );
private: 
  void recursePopulate( UClass* Parent, wxTreeListItem ParentItem, TArray<UClass*>& ExpandedAry ); //Recursively populate the tree under a given parent class and tree item.
public:
  void EVT_ObjectMenu( wxTreeListEvent& event );

protected:
  wxTreeListCtrl* m_Ctrl;
  UClass* m_Root;

public:
  wxDECLARE_EVENT_TABLE();
};

//========================================================================
// EdObjectBrowser - Renders all objects matching given class(es), sorting by group and Package.
// Contains list and tile view modes, and a preview window.
// bExactClass: If true, only Objects whose Class==Classes[x] will be considered, otherwise, Class.IsA( Classes[x] ) will be used instead.
// Package: if NULL, browse in all Packages, else, filter by the given package.
class EdObjectBrowser : public EdBrowser
{
public:
  EdObjectBrowser( wxWindow* Parent, TArray<UClass*>& Classes, bool bExactClass = false, bool bStartDocked = false, UPackage* Package = NULL );

  void ObjectUpdate( bool bUpdatePackageList = true );

  void EVT_ObjectMenu( wxTreeListEvent& event );

protected:
  TArray<UClass*> m_Classes;
  bool m_bExactClass;
  wxTreeListCtrl* m_Ctrl;

private:
  wxString getName( TArray<UClass*>& Classes );

public:
  wxDECLARE_EVENT_TABLE();
};

//========================================================================
// EdPropertyBrowser - Inspects a given UObject and renders not only its properties,
// but any associated raw data.
class EdPropertyBrowser : public EdBrowser
{
public:
  EdPropertyBrowser( wxWindow* Parent, UObject* Obj );

  void ObjectUpdate( bool bUpdatePackageList = true );

public:
  wxDECLARE_EVENT_TABLE();
};

//========================================================================
// EdMasterBrowser - ToolFrame that manages tabs for every browser type
class EdMasterBrowser : public EdToolFrame
{

public:
  EdMasterBrowser( EdBrowser::E_BrowserType Type, wxWindow* Parent = EdEditor::sm_MainFrame, wxString Title = "Browser", bool bStartDocked = false );

  void ObjectUpdate( bool bUpdatePackageList = true );
  void PageChange();

  void EVT_New( wxCommandEvent& event );
  void EVT_Open( wxCommandEvent& event );
  void EVT_Import( wxCommandEvent& event );
  void EVT_Export( wxCommandEvent& event );

  void EVT_PageChanged( wxBookCtrlEvent& event );

private:
  wxNotebook* m_Ctrl = NULL;

  wxMenuBar* m_MenuBar;
  wxMenu* m_MenuFile;

  wxBoxSizer* m_VSizer;

  wxPanel* m_HeaderPanel;
  wxBoxSizer* m_HeaderSizer;

  EdBrowser* m_Browsers[7];

  wxString m_dirPath = EdEditor::gc_SubDir_U;

public:
  wxDECLARE_EVENT_TABLE();
};