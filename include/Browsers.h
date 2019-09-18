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

enum
{
    BRWFLG_Class = 1<<0,
    BRWFLG_Audio = 1<<1,
    BRWFLG_Music = 1<<2,
    BRWFLG_Graphics = 1<<3,
    BRWFLG_Mesh = 1<<4,
    BRWFLG_Package = ( BRWFLG_Class | BRWFLG_Audio | BRWFLG_Music | BRWFLG_Graphics | BRWFLG_Mesh )
};

enum EBrowserViewMode
{
    VIEW_Raw = 0, //UTPT-Style mode, analyze all objects in package.
    VIEW_Thumbnail = 1, //UE4-Content browser-style mode, preview assets in tiles.
    VIEW_List = 2 //Legacy UE2-style list mode. Still uses tree for Classes.
};


enum
{
    ID_BrowserNew,
    ID_BrowserOpen,
    ID_BrowserSave,
    ID_BrowserImport,
    ID_BrowserExport,
    ID_BrowserViewMode_Choice,
    ID_BrowserViewMode_Raw,
    ID_BrowserViewMode_Thumbnail,
    ID_BrowserViewMode_List,
    ID_BrowserViewMode_Preview,
    ID_BrowserViewMode_Class,
    ID_BrowserViewMode_Audio,
    ID_BrowserViewMode_Music,
    ID_BrowserViewMode_Graphics,
    ID_BrowserViewMode_Mesh,
    ID_BrowserViewMode_Package,
    ID_BrowserDock,
    ID_BrowserObjectMode,
    ID_BrowserListMode,
    ID_BrowserTileMode
};


class EdBrowser : public EdToolFrame
{
public:
    //Browser mode flags.
    
	EdBrowser( int BrowserFlags, bool bDock = false );
	
	void UpdatePackageList( const wxArrayString& NewList );
	
	TArray<UPackage*> GetEnabledPackages();
    
    int m_BrowserFlags;
    EBrowserViewMode m_ViewMode;
    bool m_bPreview = false; //Preview the contents of a file in List and Raw mode.
    bool m_bTreeView = true; //If true, package filtering is disabled btw.
    
    static wxIcon m_icoPackage;
    static wxIcon m_icoClass;
    static wxIcon m_icoAudio;
    static wxIcon m_icoMusic;
    static wxIcon m_icoGraphics;
    static wxIcon m_icoMesh;
    
private:

    void OnExit( wxCommandEvent& event );
    void EVT_BrowserNew( wxCommandEvent& event ){};
    void EVT_BrowserOpen( wxCommandEvent& event );
    void EVT_BrowserSave( wxCommandEvent& event ){};
    void EVT_BrowserImport( wxCommandEvent& event ){};
    void EVT_BrowserExport( wxCommandEvent& event ){};
    void EVT_ViewChoice( wxCommandEvent& event );
    void EVT_BrowserViewMode_Raw( wxCommandEvent& event );
    void EVT_BrowserViewMode_Thumbnail( wxCommandEvent& event );
    void EVT_BrowserViewMode_List( wxCommandEvent& event );
    void EVT_BrowserViewMode_Preview( wxCommandEvent& event );
    void EVT_BrowserViewMode_Class( wxCommandEvent& event );
    void EVT_BrowserViewMode_Audio( wxCommandEvent& event );
    void EVT_BrowserViewMode_Music( wxCommandEvent& event );
    void EVT_BrowserViewMode_Graphics( wxCommandEvent& event );
    void EVT_BrowserViewMode_Mesh( wxCommandEvent& event );
    void EVT_BrowserViewMode_Package( wxCommandEvent& event );
    void EVT_BrowserDock( wxCommandEvent& event );
    
    void update(); //Update layout.
    
    void objectUpdate();
    void listUpdate();
    wxTreeItemId addTreeItem( UClass* Class, wxTreeItemId* Parent );
    void tileUpdate();
   
    //Menu elements
    wxMenu* m_MenuFile = NULL;
    wxMenu* m_MenuViewMode = NULL;
    wxMenu* m_MenuView = NULL;
    wxMenuBar* m_MenuBar = NULL;
    
    wxBoxSizer* m_WindowArea = NULL;
        wxPanel* m_OptionsBar = NULL;
            wxChoice* m_ViewModeChoice = NULL;
            wxCheckBox* m_ViewCheck_Class;
            wxCheckBox* m_ViewCheck_Audio;
            wxCheckBox* m_ViewCheck_Music;
            wxCheckBox* m_ViewCheck_Graphics;
            wxCheckBox* m_ViewCheck_Mesh;
            wxCheckBox* m_ViewCheck_Package;
             
        wxSplitterWindow* m_MainSplitter = NULL;
            wxSplitterWindow* m_ViewSplitter = NULL;
                    wxWindow* OldWindow = NULL;
                    wxTreeCtrl* m_View_Object = NULL; //For Object mode.
                    wxTreeCtrl* m_View_List = NULL; //For List mode.
                    wxListCtrl* m_View_Tile = NULL; //For Tile Mode.
                    
                    wxPanel* m_ObjectBar = NULL;
                    wxPanel* m_ListBar = NULL;
                    
                wxPanel* m_PreviewWindow = NULL;
                
            wxCheckListBox* m_PackagesList = NULL;
    
    wxDECLARE_EVENT_TABLE();
};
