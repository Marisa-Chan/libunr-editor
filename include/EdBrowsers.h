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
#include <wx/listctrl.h>
#include <wx/splitter.h>

#include "EdToolFrame.h"

/*-----------------------------------------------------------------------------
 * EdBrowser
 * Abstract Browser class.
-----------------------------------------------------------------------------*/
class EdBrowser : public wxWindow
{
public:
  EdBrowser( wxWindow* Parent );
  ~EdBrowser();

  virtual void ObjectUpdate( bool m_bUpdatePackageList = true ) = 0; //Update Main browser Ctrl

  enum EBrowserType
  {
    BT_Package,
    BT_Class,
    BT_Sound,
    BT_Music,
    BT_Texture,
    BT_Mesh,
    BT_Level,
    BT_TypeCount // Keep last, so it's always equal to the number of browser types
  };

protected:

  wxBoxSizer* m_VSizer;

  wxPanel* m_HeaderPanel;
  wxBoxSizer* m_HeaderSizer;
    EdEditor::EdUPackageCtrl* m_PackageCtrl;
    wxCheckBox* m_CheckFilterPackage;

  void EventFilterPackage( wxCommandEvent& event );
  void EventFilterPackageCheck( wxCommandEvent& event );

private:
  wxDECLARE_EVENT_TABLE();
};

/*-----------------------------------------------------------------------------
 * EdPackageHeader
-----------------------------------------------------------------------------*/
class EdPackageHeader : public wxWindow
{
public:
  EdPackageHeader( wxWindow* Parent );

  void Update( UPackage* Package = NULL );

protected:
  wxBoxSizer* m_VSizer;
  wxListCtrl* m_PackageInfo;
  wxListCtrl* m_PackageFlags;
};

/*-----------------------------------------------------------------------------
 * EdGenerationsTable
-----------------------------------------------------------------------------*/
class EdGenerationsTable : public wxWindow
{
public:
  EdGenerationsTable( wxWindow* Parent );

  void Update( UPackage* Package = NULL );

protected:
  wxListCtrl* m_GenerationInfo;
};

/*-----------------------------------------------------------------------------
 * EdNameTable
-----------------------------------------------------------------------------*/
class EdNameTable : public wxWindow
{
public:
  EdNameTable( wxWindow* Parent );

  void Update( UPackage* Package = NULL );

protected:
  wxBoxSizer* m_VSizer;
  wxListCtrl* m_NameTable;
};

/*-----------------------------------------------------------------------------
 * EdExportTable
-----------------------------------------------------------------------------*/
class EdExportTable : public wxWindow
{
public:
  EdExportTable( wxWindow* Parent );

  void Update( UPackage* Package = NULL );

protected:
  wxBoxSizer* m_VSizer;
  wxListCtrl* m_ExportTable;
};

/*-----------------------------------------------------------------------------
 * EdExportTree
-----------------------------------------------------------------------------*/
class EdExportTree : public wxWindow
{
public:
  EdExportTree( wxWindow* Parent );

  void Update( UPackage* Package = NULL );

protected:
  void recurseTree( u32 TargetExport, wxTreeListItem TargetNode, TArray<FExport>& Exports, UPackage* Pkg );

  wxBoxSizer* m_VSizer;
  wxTreeListCtrl* m_ExportTree;
};

/*-----------------------------------------------------------------------------
 * EdImportTable
-----------------------------------------------------------------------------*/
class EdImportTable : public wxWindow
{
public:
  EdImportTable( wxWindow* Parent );

  void Update( UPackage* Package = NULL );

protected:
  wxBoxSizer* m_VSizer;
  wxListCtrl* m_ImportTable;
};

/*-----------------------------------------------------------------------------
 * EdImportTree
-----------------------------------------------------------------------------*/
class EdImportTree : public wxWindow
{
public:
  EdImportTree( wxWindow* Parent );

  void Update( UPackage* Package = NULL );

protected:
  void recurseTree( int TargetImport, wxTreeListItem TargetNode, TArray<FImport>& Imports, UPackage* Pkg );

  wxBoxSizer* m_VSizer;
  wxTreeListCtrl* m_ImportTree;
};


/*-----------------------------------------------------------------------------
 * EdPackageBrowser
 * Inspects the contents and properties of Package objects, such as 
 * Import/Export tables, Name tables, Headers, etc.
-----------------------------------------------------------------------------*/
class EdPackageBrowser : public EdBrowser
{
public:
  EdPackageBrowser( wxWindow* Parent, UPackage* Package = NULL, bool bStartDocked = false );

  virtual void ObjectUpdate( bool m_bUpdatePackageList = true );

protected:

  wxSplitterWindow* m_MainView;
    wxNotebook* m_Notebook;
      EdPackageHeader* m_PackageHeader;
      EdGenerationsTable* m_GenTable;
      EdNameTable* m_NameTable;
      EdExportTable* m_ExportTable;
      EdExportTree* m_ExportTree;
      EdImportTable* m_ImportTable;
      EdImportTree* m_ImportTree;
    EdEditor::UObjectPreviewWindow* m_ViewWindow;

private:
  wxDECLARE_EVENT_TABLE();
};

/*-----------------------------------------------------------------------------
 * EdClassBrowser
 * Renders a class tree, given a root class.
-----------------------------------------------------------------------------*/
class EdClassBrowser : public EdBrowser
{
public:
  EdClassBrowser( wxWindow* Parent, UClass* Root, bool bStartDocked = false );
  ~EdClassBrowser();

  virtual void ObjectUpdate( bool bUpdatePackageList = true );

protected:
  wxTreeListCtrl* m_Ctrl;
  UClass* m_Root;

private:
  void recursePopulate( UClass* Parent, wxTreeListItem ParentItem, TArray<UClass*>& ExpandedAry ); //Recursively populate the tree under a given parent class and tree item.
  void EventObjectMenu( wxTreeListEvent& event );

  wxDECLARE_EVENT_TABLE();
};

/*-----------------------------------------------------------------------------
 * EdObjectBrowser 
 * Renders all objects matching given class(es), sorting by group and Package.
 * Contains list and tile view modes, and a preview window.
 * Package: if NULL, browse in all Packages, else, filter by the given package.
-----------------------------------------------------------------------------*/
class EdObjectBrowser : public EdBrowser
{
public:
  EdObjectBrowser( wxWindow* Parent, TArray<UClass*>& Classes, bool bExactClass = false, bool bStartDocked = false, UPackage* Package = NULL );

  virtual void ObjectUpdate( bool bUpdatePackageList = true );

  void EventObjectMenu( wxTreeListEvent& event );

protected:
  TArray<UClass*> m_Classes;
  bool m_bExactClass; // If true, only Objects whose Class==Classes[x] will be considered, otherwise, Class.IsA( Classes[x] ) will be used instead.
  wxTreeListCtrl* m_Ctrl;

private:
  wxDECLARE_EVENT_TABLE();
};

/*-----------------------------------------------------------------------------
 * EdPropertyBrowser 
 * Inspects a given UObject and renders not only its properties,
 * but any associated raw data.
-----------------------------------------------------------------------------*/
class EdPropertyBrowser : public EdBrowser
{
public:
  EdPropertyBrowser( wxWindow* Parent, UObject* Obj );

  virtual void ObjectUpdate( bool bUpdatePackageList = true );

private:
  wxDECLARE_EVENT_TABLE();
};

/*-----------------------------------------------------------------------------
 * EdMasterBrowser 
 * ToolFrame that manages tabs for every browser type
-----------------------------------------------------------------------------*/
class EdMasterBrowser : public EdToolFrame
{
public:
  EdMasterBrowser( EdBrowser::EBrowserType Type, wxWindow* Parent = EdEditor::sm_MainFrame, wxString Title = "Browser", bool bStartDocked = false );

  virtual void ObjectUpdate( bool bUpdatePackageList = true );
  void PageChange();

  void EventNew( wxCommandEvent& event );
  void EventOpen( wxCommandEvent& event );
  void EventImport( wxCommandEvent& event );
  void EventExport( wxCommandEvent& event );

  void EventPageChanged( wxBookCtrlEvent& event );

private:
  wxNotebook* m_Ctrl = NULL;

  wxMenuBar* m_MenuBar;
  wxMenu* m_MenuFile;

  wxBoxSizer* m_VSizer;

  wxPanel* m_HeaderPanel;
  wxBoxSizer* m_HeaderSizer;

  EdBrowser* m_Browsers[7];

  wxString m_dirPath = EdEditor::gc_SubDir_U;

  EdBrowser::EBrowserType BrowserType;

  wxDECLARE_EVENT_TABLE();
};