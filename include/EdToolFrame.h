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
 * EdToolFrame.h - Editor Tools.
 *
 * written by Jesse 'Hyzoran' Kowalik
 *========================================================================
*/
#pragma once

#include "EdEditor.h"

#define C_MINTOOLSIZE_X 512
#define C_MINTOOLSIZE_Y 384

//========================================================================
// EdToolFrame

class EdToolFrame : public wxFrame
{
public:
    EdToolFrame( wxWindow* Parent = EdEditor::sm_MainFrame, wxString Title = "EdToolFrame", bool bStartDocked = false );
    ~EdToolFrame();

    static wxSize DefaultFrameSize( wxWindow* Parent );

//Event Handlers
    virtual void OnExit( wxCommandEvent& event );

//Static Interface

    static TArray<EdToolFrame*>* GetTools();

    wxDECLARE_EVENT_TABLE();

protected:

    wxWindow* m_Parent;
    
//UObject Interface

    void ObjectMenu( UObject* Obj );
    void ObjectPlay( UObject* Obj );
    void ObjectExport( UObject* Obj );
    void ObjectExport( TArray<UObject*>& Objs );
    void ObjectProperties( UObject* Obj );

    bool m_bDocked;
    bool m_bDockable = true; //This Tool class is dockable, e.g., shows up as a tool window and is registered in sm_Tools.
    int m_MyID;

//Statics - Tool Management

    static size_t RegisterTool( EdToolFrame* Tool );
    static bool UnregisterTool( size_t id );
        static TArray<EdToolFrame*> sm_Tools;
        static size_t sm_EmptySlots;
};