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

EdToolFrame::EdToolFrame( wxString& Title, bool bStartDocked, EdGamePrompt* ModalWindow )
    : wxFrame( g_MainFrame, wxID_ANY, Title, wxDefaultPosition, DefaultFrameSize(), wxFRAME_FLOAT_ON_PARENT | wxFRAME_TOOL_WINDOW )
{
    m_ModalWindow = ModalWindow;

    if( m_ModalWindow == NULL )
    {
      m_MyID = g_MainFrame->RegisterTool( this );
    }
    Show(true);
}

EdToolFrame::~EdToolFrame()
{
  if( m_ModalWindow == NULL )
  {
    g_MainFrame->UnregisterTool( m_MyID );
  }
}

wxSize EdToolFrame::DefaultFrameSize()
{
    wxDisplay display (wxDisplay::GetFromWindow( EdEditorFrame::GetMainFrame() ) );
    wxRect screen = display.GetClientArea();
    int x, y;

    x = screen.Width / 2;
    y = screen.Height / 2;

    if ( x < C_MINTOOLSIZE_X )
        x = C_MINTOOLSIZE_X;
    if ( y < C_MINTOOLSIZE_Y )
        y = C_MINTOOLSIZE_Y;

    return returnSize;
}

void EdToolFrame::OnExit( wxCommandEvent& event )
{
    if( m_ModalWindow != NULL )
    {
      m_ModalWindow->Enable();
      m_ModalWindow->OnEnable();
    }

    Close(True);
}