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
 * Components.h - Various internal components of the editor.
 * 
 * written by Jesse 'Hyzoran' Kowalik
 *========================================================================
*/
#pragma once

#include <wx/wxprec.h>
#include <wx/wx.h>

#include <libunr.h>

#include <wx/wrapsizer.h>

enum
{
    TOOL_Generic = 0,
    TOOL_Browser = 1
};

class EdToolFrame : public wxFrame
{
public:
    EdToolFrame( bool bStartDocked = false, wxSize Size = GetFrameSize() );

    ~EdToolFrame();

    bool m_bDocked = false;
    int m_ToolType = TOOL_Generic;
    size_t m_MyID;
    
    virtual void OnExit( wxCommandEvent& event );

	static wxSize GetFrameSize(); //Return the default framesize for toolframe.
    
    //Sysytem notifications, called by EdEditorFrame for all tools when nessecary.
    //Get a list of new packages loaded.
    virtual void SYS_NewPackages( size_t PackageStartIndex ) = 0;
    virtual void SYS_NewObjects( size_t ObjectStartIndex ) = 0;
    virtual void SYS_PackagesRemoved() = 0; //A Package was removed, please re-process all packages
    virtual void SYS_ObjectsRemoved() = 0; //Objects were removed, please re-process all objects.
};

class EdAbout : public wxFrame
{
//NOTE: If changing any things around here, probably will have to account for new sizing.
public:
	EdAbout( wxWindow* parent, bool* Switch );
	
	void CloseAbout( wxCommandEvent& event );
    
private:
    bool* m_Switch;
};

namespace EdEditor //Editor interface
{
    void* GetFrame();
    size_t NewTool( EdToolFrame* Tool );
    bool KillTool( size_t id );
    void Editor_Log( const wxString& Msg );
    TArray<UPackage*>* GetPackages();
    void LoadPackages( const wxArrayString& Paths );
    
    bool IsPackageModified( size_t I ); //Has the package of this index been modified in memory?
};
