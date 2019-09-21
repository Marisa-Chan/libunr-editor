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

struct ObjectItemPair
{
    ObjectItemPair( UObject* inObj, wxTreeItemId inItem )
    {
        Obj = inObj;
        Item = inItem;
    }
        
    UObject* Obj;
    wxTreeItemId Item;
};

enum
{
    BRWFLG_Class = 1<<0,
    BRWFLG_Audio = 1<<1,
    BRWFLG_Music = 1<<2,
    BRWFLG_Graphics = 1<<3,
    BRWFLG_Mesh = 1<<4,
    BRWFLG_Level = 1<<5,
    BRWFLG_Package = ( BRWFLG_Class | BRWFLG_Audio | BRWFLG_Music | BRWFLG_Graphics | BRWFLG_Mesh | BRWFLG_Level )
};

enum EBrowserViewMode
{
    VIEW_Raw = 0, //UTPT-Style mode, analyze all objects in package.
    VIEW_Thumbnail = 1, //UE4-Content browser-style mode, preview assets in tiles.
    VIEW_List = 2 //Legacy UE2-style list mode. Still uses tree for Classes.
};

enum ESortMode //How to sort objects, no effect for classes in tree mode.
{
    SORT_Alpha = 0,
    SORT_AlphaPackage = 1,
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
    ID_BrowserTileMode,
    ID_BrowserShowPackage,
    ID_BrowserSortAscending,
    ID_BrowserSortMode_Alpha,
    ID_BrowserSortMode_Package,
    ID_BrowserViewMode_Level,
    ID_PackageList
};


class EdBrowser : public EdToolFrame
{
public:
    //Browser mode flags.
    
	EdBrowser( int BrowserFlags, bool bDock = false );
	
	void UpdatePackageList( const wxArrayString& NewList );
	
	TArray<UPackage*> GetEnabledPackages();
    
    //View Preferences
    int m_BrowserFlags;
    EBrowserViewMode m_ViewMode;
    bool m_bPreview = false; //Preview the contents of a file in List and Raw mode.
    bool m_bShowPackage = true; //Show package after object name
    ESortMode m_SortMode = SORT_AlphaPackage;
    bool m_bSortAscending = true; //If true, sorts A-Z, 0-9 instead of Z-A, 9-0.
    bool m_bClassTree = true; //Use class tree in List mode.
    
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
    void EVT_BrowserViewMode_Level( wxCommandEvent& event );
    void EVT_BrowserViewMode_Package( wxCommandEvent& event );
    void EVT_BrowserDock( wxCommandEvent& event );
    void EVT_BrowserShowPackage( wxCommandEvent& event );
    void EVT_BrowserSortAscending( wxCommandEvent& event );
    void EVT_BrowserSortMode_Alpha( wxCommandEvent& event );
    void EVT_BrowserSortMode_Package( wxCommandEvent& event );
    void EVT_PackagesList( wxCommandEvent& event );
    
    void update(); //Update layout.
    
    void objectUpdate();
    void listUpdate();
    wxTreeItemId addTreeItem( UObject* Class, wxTreeItemId Parent, 
        TArray<UObject*>& EAry, TArray<wxTreeItemId>& EId );
        
    void addObjectItem( UObject* Obj, wxTreeItemId Parent );
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
            wxCheckBox* m_ViewCheck_Level;
            wxCheckBox* m_ViewCheck_Package;
             
        wxSplitterWindow* m_MainSplitter = NULL;
            wxSplitterWindow* m_ViewSplitter = NULL;
                    wxWindow* OldWindow = NULL;
                    wxTreeCtrl* m_View_Object = NULL; //For Object mode.
                    
                    wxTreeCtrl* m_View_List = NULL; //For List mode.
                    
                        TArray<ObjectItemPair> m_ListObjects; //Store references to UObjects so each 
                        //wxTreeCtrlID can reference what it represents.
                        TArray<wxTreeItemId> m_MiscExpands; //For remember category expands
                        //0 - Audio
                        //1 - Music
                        //2 - Textures
                        //3 - Mesh
                        //4 - Level
                    
                    wxListCtrl* m_View_Tile = NULL; //For Tile Mode.
                    
                    wxPanel* m_ObjectBar = NULL;
                    wxPanel* m_ListBar = NULL;
                    
                wxPanel* m_PreviewWindow = NULL;
                
            wxCheckListBox* m_PackagesList = NULL;
    
    wxDECLARE_EVENT_TABLE();
};
