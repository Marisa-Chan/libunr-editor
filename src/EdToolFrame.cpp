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

EdToolFrame::EdToolFrame( wxWindow* Parent, wxString Title, bool bStartDocked)
    : wxFrame( Parent, wxID_ANY, Title, wxDefaultPosition, DefaultFrameSize( Parent )),
    m_Parent( Parent ),
    m_bDocked( bStartDocked )
{
    if( Parent == EdEditor::sm_MainFrame && EdEditor::sm_MainFrame != NULL )
    {
      size_t m_Id = EdToolFrame::StaticAddTool( this );
    }
}

EdToolFrame::~EdToolFrame()
{
    if( m_Parent == EdEditor::sm_MainFrame && EdEditor::sm_MainFrame != NULL )
    {
        EdToolFrame::StaticRemoveTool( m_Id );
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

void EdToolFrame::EVT_Exit( wxCommandEvent& event )
{
  OnExit( event );
}

TArray<EdToolFrame*>* EdToolFrame::GetTools()
{
    return &sm_Tools;
}

TArray<EdToolFrame*> EdToolFrame::sm_Tools;

size_t EdToolFrame::StaticAddTool( EdToolFrame* Tool )
{
  sm_Tools.PushBack( Tool );
  return sm_Tools.Size()-1;
}

bool EdToolFrame::StaticRemoveTool( size_t Id )
{
  sm_Tools.Erase( Id );
  return true;
}

wxBEGIN_EVENT_TABLE(EdToolFrame, wxFrame)
  EVT_BUTTON(ID_Exit, EdToolFrame::EVT_Exit)
  EVT_MENU(ID_Exit, EdToolFrame::EVT_Exit)
wxEND_EVENT_TABLE()