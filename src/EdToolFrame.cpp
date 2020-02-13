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
 * EdMain.cpp
 *
 * written by Jesse 'Hyzoran' Kowalik
 *========================================================================
*/

#include "EdToolFrame.h"
#include "EdGamePrompt.h"

TArray<EdToolFrame*> EdToolFrame::sm_Tools;
size_t EdToolFrame::sm_EmptySlots;

EdToolFrame::EdToolFrame( wxWindow* Parent, wxString Title, bool bStartDocked)
    : wxFrame( Parent, wxID_ANY, Title, wxDefaultPosition, DefaultFrameSize( Parent )),
    m_Parent( Parent ),
    m_bDocked( bStartDocked )
{
    if( Parent == EdEditor::sm_MainFrame && EdEditor::sm_MainFrame != NULL )
    {
        m_MyID = EdToolFrame::RegisterTool( this );
    }
}

EdToolFrame::~EdToolFrame()
{
    if( m_Parent == EdEditor::sm_MainFrame && EdEditor::sm_MainFrame != NULL )
    {
        EdToolFrame::UnregisterTool( m_MyID );
    }
}

wxSize EdToolFrame::DefaultFrameSize( wxWindow* Parent )
{
    wxDisplay* display;

    if( Parent != NULL )
        display = new wxDisplay( wxDisplay::GetFromWindow( Parent ) );
    else
        display = new wxDisplay( static_cast<unsigned int>(0) );

    wxRect screen = display->GetClientArea();
    int x, y;

    x = screen.width / 2;
    y = screen.height / 2;

    if ( x < C_MINTOOLSIZE_X )
        x = C_MINTOOLSIZE_X;
    if ( y < C_MINTOOLSIZE_Y )
        y = C_MINTOOLSIZE_Y;

    delete display;

    return wxSize( x, y );
}

void EdToolFrame::OnExit( wxCommandEvent& event )
{
    Close( true );
}

TArray<EdToolFrame*>* EdToolFrame::GetTools()
{
    return &sm_Tools;
}

size_t EdToolFrame::RegisterTool( EdToolFrame* Tool )
{
    if ( sm_EmptySlots > 0 ) //A slot was freed earlier, find and use that slot.
    {
        for (size_t i = 0; i < sm_Tools.Size(); i++)
        {
            if ( sm_Tools[i] == NULL)
            {
                sm_Tools[i] = Tool;
                sm_EmptySlots--; //We used a slot.
                return i;
            }
        }
        //None found? Something went wrong.
        GLogf(LOG_WARN, "EdToolFrame::RegTool() : sm_EmptySlots > 0, but none found in sm_ToolArray!");
        GLogf(LOG_WARN, "Possible memory corruption... Pushing new tool to end of array...");
    }
    sm_Tools.PushBack((EdToolFrame*)Tool);
    return sm_Tools.Size() - 1;
}

bool EdToolFrame::UnregisterTool( size_t id )
{
    if ( sm_Tools[id] != NULL )
    {
        sm_Tools[id] = NULL;
        sm_EmptySlots++;
        return true;
    }
    GLogf(LOG_WARN, "EdEditor::UnregisterTool() : Unregistered tool that does not exist in sm_ToolArray!");
    return false;
}

wxBEGIN_EVENT_TABLE(EdToolFrame, wxFrame)
wxEND_EVENT_TABLE()