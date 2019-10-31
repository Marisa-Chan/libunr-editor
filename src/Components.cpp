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
 * Components.cpp - Various internal components of the editor.
 *
 * written by Jesse 'Hyzoran' Kowalik
 *========================================================================
*/

#include "EditorMain.h"
#include "Components.h"

bool EdAbout::sm_IsOpen = false;

EdToolFrame::EdToolFrame( bool bStartDocked, wxSize Size )
  : wxFrame( EdEditorFrame::GetMainFrame(), -1, "Editor Tool", wxDefaultPosition, Size )
{
  m_MyID = EdEditorFrame::RegisterTool( this );
}

EdToolFrame::~EdToolFrame()
{
  EdEditorFrame::UnregisterTool( m_MyID );
}

void EdToolFrame::OnExit( wxCommandEvent& event )
{
  Close( true );
}

wxSize EdToolFrame::GetFrameSize()
{
  wxDisplay display( wxDisplay::GetFromWindow( EdEditorFrame::GetMainFrame() ) );
  wxRect screen = display.GetClientArea();

  return wxSize( screen.width / 2, screen.height / 2 );
}

EdAbout::EdAbout( wxWindow* parent )
  : wxFrame( parent, -1, "About", wxDefaultPosition, wxSize( 420, 400 ) )
{
  wxBoxSizer* ContentSizer = new wxBoxSizer( wxVERTICAL );

  wxStaticText* aboutlibunr = new wxStaticText( this, -1, "LIBUNR\nCopyright (C) 2018-2019 Adam Smith", wxDefaultPosition, wxDefaultSize );
  ContentSizer->Add( aboutlibunr, 0, wxALL | wxALIGN_CENTRE_HORIZONTAL, 2 );

  wxStaticText* aboutlibunr2 = new wxStaticText( this, -1, "Open Source Re-Implementation of Unreal Engine 1", wxDefaultPosition, wxDefaultSize );
  ContentSizer->Add( aboutlibunr2, 0, wxALL | wxALIGN_CENTRE_HORIZONTAL, 2 );
  aboutlibunr2->Wrap( 400 );

  wxTextCtrl* libunrlink = new wxTextCtrl ( this, -1, "https://bitbucket.org/Xaleros/libunr/src/master/", wxDefaultPosition, wxSize( 400, 20 ), wxTE_READONLY );
  ContentSizer->Add( libunrlink, 0, wxALL | wxALIGN_CENTRE_HORIZONTAL, 2 );

  wxStaticText* aboutlibunreditor = new wxStaticText( this, -1, "LIBUNR-EDITOR\nCopyright (C) 2018-2019 Adam Smith\nWritten by Jesse \"Hyzoran\" Kowalik", wxDefaultPosition, wxDefaultSize );
  ContentSizer->Add( aboutlibunreditor, 0, wxALL | wxALIGN_CENTRE_HORIZONTAL, 2 );

  wxStaticText* aboutlibunreditor2 = new wxStaticText( this, -1, "Graphical Frontend and Development suite for libunr", wxDefaultPosition, wxDefaultSize );
  ContentSizer->Add( aboutlibunreditor2, 0, wxALL | wxALIGN_CENTRE_HORIZONTAL, 2 );
  aboutlibunreditor2->Wrap( 400 );

  wxTextCtrl* openuelink = new wxTextCtrl( this, -1, "https://bitbucket.org/Xaleros/OpenUE/", wxDefaultPosition, wxSize( 400, 20 ), wxTE_READONLY );
  ContentSizer->Add( openuelink, 0, wxALL | wxALIGN_CENTRE_HORIZONTAL, 2 );

  wxPanel* padding = new wxPanel( this );
  ContentSizer->Add( padding, 1, wxALL, 0 );

  wxButton* closeb = new wxButton( this, wxID_OK, "Ok" );
  ContentSizer->Add( closeb, 0, wxALIGN_BOTTOM | wxALIGN_CENTRE_HORIZONTAL );

  Connect( wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( EdAbout::OnClose ) );

  SetSizer( ContentSizer );

  closeb->SetFocus();
  Centre();

  sm_IsOpen = true;

  SetMinSize( wxSize( 420, 400 ) );
  Show( true );
}

void EdAbout::OnClose( wxCommandEvent& event )
{
  sm_IsOpen = false;
  Close( true );
}

bool EdAbout::IsOpened()
{
  return sm_IsOpen;
}

