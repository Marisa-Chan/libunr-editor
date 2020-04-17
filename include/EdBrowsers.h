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
 * EdBrowsers.h - Editor Browser Headers
 * 
 * Headers are a class of EdToolFrame that work with UObjects in some form.
 * Most notably, Rendering Actor Trees, Package contents, inspecting UObjects and their Properties.
 *
 * written by Jesse 'Hyzoran' Kowalik
 *========================================================================
*/
#pragma once

#include <wx/treelist.h>

#include "EdToolFrame.h"



//========================================================================
// EdBrowser - Abstract Browser class.
// Also contains any common utility classes to deal with handling UObjects.
class EdBrowser : public EdToolFrame
{
public:
    EdBrowser( wxString Title, bool bStartDocked = false );
    ~EdBrowser();

    virtual void ObjectUpdate() = 0; //Populate the associated control.
    static ThreadReturnType StaticThreadObjectUpdate( void* Browser ); //Notify Browser that UObject/UClass/UPackage pools have been updated or modified in some way.

    void PackageSelect( UPackage* Package = NULL );
    UPackage* GetSelectedPackage();
    void PackageListUpdate();

    bool bIsBrowser = true;

protected:

    void EVT_New( wxCommandEvent& event );
    void EVT_Open( wxCommandEvent& event );
    void EVT_Import( wxCommandEvent& event );
    void EVT_Export( wxCommandEvent& event );

    wxMenuBar* m_MenuBar;
      wxMenu* m_MenuFile;

    wxBoxSizer* m_VSizer;

    wxPanel* m_HeaderPanel;
      wxBoxSizer* m_HeaderSizer;

      wxComboBox* m_PackageCtrl = NULL;
      wxCheckBox* m_CheckFilterPackage = NULL;
      bool m_bUpdatePackageList = false;

    wxString m_dirPath = EdEditor::gc_SubDir_U;

private:

    wxDECLARE_EVENT_TABLE();
};

//========================================================================
// EdClassBrowser - Renders a class tree, given a root class.
class EdClassBrowser : public EdBrowser
{
public:
    EdClassBrowser( UClass* Root, bool bStartDocked = false );
    ~EdClassBrowser();

    void ObjectUpdate();
private: void recursePopulate( UClass* Parent, wxTreeListItem ParentItem, TArray<UClass*>& ExpandedAry ); //Recursively populate the tree under a given parent class and tree item.
public:
    void EVT_ObjectMenu( wxTreeListEvent& event );

protected:

  wxTreeListCtrl* m_Ctrl;
  UClass* m_Root;

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
    EdObjectBrowser( TArray<UClass*>& Classes, bool bExactClass = false, bool bStartDocked = false, UPackage* Package = NULL );

    void ObjectUpdate();

    void EVT_ObjectMenu( wxTreeListEvent& event );
    void EVT_FilterPackage( wxCommandEvent& event );
    void EVT_FilterPackageCtrl( wxCommandEvent& event );

protected:

  TArray<UClass*> m_Classes;
  bool m_bExactClass;
  wxTreeListCtrl* m_Ctrl;

private:

  wxString getName( TArray<UClass*>& Classes );

  wxDECLARE_EVENT_TABLE();
};

//========================================================================
// EdPackageBrowser - Inspects the contents and properties of Package objects,
// such as Import/Export tables, Name tables, Headers, etc.
class EdPackageBrowser : public EdBrowser
{
public:
    EdPackageBrowser( UPackage* Package, bool bStartDocked = false );
};

//========================================================================
// EdPropertyBrowser - Inspects a given UObject and renders not only its properties,
// but any associated raw data.
class EdPropertyBrowser : public EdBrowser
{
public:
    EdPropertyBrowser( UObject* Obj );
};