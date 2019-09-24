/*===========================================================================*\
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
 * Browsers.h - Browser class, for parsing Packages and their contents.
 * 
 * written by Jesse 'Hyzoran' Kowalik, designed berated by Adam 'Xaleros' Smith
 *========================================================================
*/
#pragma once

#include <wx/wxprec.h>
#include <wx/wx.h>

#include <Core/UPackage.h>
#include <wx/splitter.h>
#include <wx/treectrl.h>
#include <wx/listctrl.h>

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
    ID_ClassTree
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
    
    void update(); //Update interface.
    
    void objectUpdate( size_t ObjectStartIndex );
    void classUpdate( size_t ObjectStartIndex );
        void recurseExpandPopulate( TArray<UClass*>& ExpandedAry, wxTreeItemId Parent, 
            wxTreeItemIdValue Cookie = 0 );
        wxTreeItemId addTreeItem( wxTreeItemId Parent, UClass* Obj );
        
    void listUpdate( size_t ObjectStartIndex );
    void tileUpdate( size_t ObjectStartIndex );
    
    wxTreeCtrl* m_ListView;
    
    wxMenu* m_MenuFile = NULL;
    wxMenuBar* m_MenuBar = NULL;
    wxBoxSizer* m_WindowAreaSizer = NULL;
    wxPanel* m_OptionsBar = NULL;
    wxCheckBox* m_Check_Dock = NULL;
    wxCheckBox* m_Check_ShowPackage = NULL;
    wxSplitterWindow* m_MainSplitter = NULL;
    wxCheckListBox* m_PackagesList = NULL;
    
    wxDECLARE_EVENT_TABLE();
};
