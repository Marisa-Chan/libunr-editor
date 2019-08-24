 /*===========================================================================*\
|*  libunr - An open source Unreal Engine 1 implementation in library form   *|
|*  Copyright (C) 2018-2019  Adam W.E. Smith                                 *|
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
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "Components.h"

enum EBrowserFlags
{
    BRWFLG_Package = 0x000000FF, //Everything, render all objects.
    BRWFLG_Class = 0x00000002,
    BRWFLG_Audio = 0x00000004,
    BRWFLG_Music = 0x00000008,
    BRWFLG_Graphics = 0x00000010,
    BRWFLG_Mesh = 0x00000020
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
    ID_BrowserViewMode_Raw,
    ID_BrowserViewMode_Thumbnail,
    ID_BrowserViewMode_List,
    ID_BrowserViewMode_Preview,
    ID_BrowserViewMode_Class,
    ID_BrowserViewMode_Audio,
    ID_BrowserViewMode_Music,
    ID_BrowserViewMode_Graphics,
    ID_BrowserViewMode_Mesh,
    ID_BrowserDock
};

class EdBrowser : public EdToolFrame
{
public:
    //Browser mode flags.
    
	EdBrowser( EBrowserFlags BrowserFlags, const wxString& Title, const wxPoint& Pos, const 
wxSize& Size );
    
    EBrowserFlags m_BrowserFlags;
    EBrowserViewMode m_ViewMode;
    bool m_bPreview = true; //Preview the contents of a file in List and Raw mode.
    
private:
    void renderView(); //Update browser pane
    void renderMenu();
    
    void EVT_BrowserNew( wxCommandEvent& event ){};
    void EVT_BrowserOpen( wxCommandEvent& event ){};
    void EVT_BrowserSave( wxCommandEvent& event ){};
    void EVT_BrowserImport( wxCommandEvent& event ){};
    void EVT_BrowserExport( wxCommandEvent& event ){};
    void EVT_BrowserViewMode_Raw( wxCommandEvent& event ){};
    void EVT_BrowserViewMode_Thumbnail( wxCommandEvent& event ){};
    void EVT_BrowserViewMode_List( wxCommandEvent& event ){};
    void EVT_BrowserViewMode_Preview( wxCommandEvent& event ){};
    void EVT_BrowserViewMode_Class( wxCommandEvent& event ){};
    void EVT_BrowserViewMode_Audio( wxCommandEvent& event ){};
    void EVT_BrowserViewMode_Music( wxCommandEvent& event ){};
    void EVT_BrowserViewMode_Graphics( wxCommandEvent& event ){};
    void EVT_BrowserViewMode_Mesh( wxCommandEvent& event ){};
    void EVT_BrowserDock( wxCommandEvent& event ){};
   
    wxMenu* m_MenuFile = NULL;
    wxMenu* m_MenuViewMode = NULL;
    wxMenu* m_MenuView = NULL;
    wxMenuBar* m_MenuBar = NULL;
    
    wxDECLARE_EVENT_TABLE();
};
