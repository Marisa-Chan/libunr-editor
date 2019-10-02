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
 * Browsers.h - Browser class, for parsing Packages and their contents.
 * 
 * written by Jesse 'Hyzoran' Kowalik, designed berated by Adam 'Xaleros' Smith
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

#include "Components.h"

enum EBrowserType
{
    BRWFLG_Class = 0,
    BRWFLG_Audio = 1,
    BRWFLG_Music = 2,
    BRWFLG_Texture = 3,
    BRWFLG_Mesh = 4,
    BRWFLG_Level = 5,
    BRWFLG_Package = 6
};

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
    //Browser mode flags.
    
	EdBrowser( EBrowserType BrowserFlags, bool bDock = false );
	
	virtual void SYS_NewPackages( size_t PackageStartIndex );
    virtual void SYS_NewObjects( size_t ObjectStartIndex );
    virtual void SYS_PackagesRemoved();
    virtual void SYS_ObjectsRemoved();
    
    //View Preferences
    EBrowserType m_Mode;
    bool m_bShowPackage = true; //Show package after object name
    
    static wxIcon m_icoPackage;
    static wxIcon m_icoClass;
    static wxIcon m_icoAudio;
    static wxIcon m_icoMusic;
    static wxIcon m_icoGraphics;
    static wxIcon m_icoMesh;
    
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
    
    void update(); //Update interface (does not do rendering, call respective update below for that)
    
    void packageUpdate(); //Render view in Package mode.
    void classUpdate(); //Render view in Class Tree mode.
        void recurseExpandPopulate( TArray<UClass*>& ExpandedAry, wxTreeItemId Parent, 
            wxTreeItemIdValue Cookie = 0 );
        wxTreeItemId addTreeItem( wxTreeItemId Parent, UClass* Obj );
        
    void listUpdate(); //Render view in List mode (Audio, Music )
    void tileUpdate(); //Render view in Tile mode (Textures, Meshes, Levels)
    
    //Package mode
    UPackage* m_SelectedPackage = NULL;
    wxWindow* m_PackageHeader = NULL;
        wxListCtrl* m_PackageInfo = NULL;
        wxListCtrl* m_PackageFlags = NULL;
    wxListCtrl* m_NameTable = NULL;
    wxWindow* m_ExportTable = NULL;
    wxWindow* m_ImportTable = NULL;
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
    
    wxDECLARE_EVENT_TABLE();
};
