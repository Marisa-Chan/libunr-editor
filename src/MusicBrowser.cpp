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
 * MusicBrowser.h - Music browser definition
 *
 * written by Jesse 'Hyzoran' Kowalik & Adam 'Xaleros' Smith
 *========================================================================
*/

#include "EditorMain.h"
#include "MusicBrowser.h"
#include <Engine/UMusic.h>

EdMusicBrowser::EdMusicBrowser( bool bDock, wxSize Size )
  : EdBrowser( UMusic::StaticClass(), bDock, Size )
{
  SetLabel( wxString( "Music Browser" ) );
  SetIcon( EdEditorFrame::sm_icoSound );

  m_subDirType = (wxString*)&EdEditorFrame::csm_SubDir_UMX;

  ConstructPackageButtons();

  // Add play and stop buttons
  m_PlayMusicButton = new wxButton( m_OptionsBar, ID_PlayMusic, "Play", wxDefaultPosition, wxSize( C_TOOLBUTTONSIZE, C_TOOLBUTTONSIZE ) );
  m_StopMusicButton = new wxButton( m_OptionsBar, ID_StopMusic, "Stop", wxDefaultPosition, wxSize( C_TOOLBUTTONSIZE, C_TOOLBUTTONSIZE ) );

  m_OptionsSizer->Add( m_PlayMusicButton, 0, wxALIGN_LEFT );
  m_OptionsSizer->Add( m_StopMusicButton, 0, wxALIGN_LEFT );

  // Add section box
  m_SongSectionBox = new wxTextCtrl( m_OptionsBar, ID_SongSection, "", wxDefaultPosition, wxSize( C_TOOLBUTTONSIZE, C_TOOLBUTTONSIZE ) );
  m_OptionsSizer->Add( m_SongSectionBox, 0, wxALIGN_LEFT );

  ListConstruct();
}

EdMusicBrowser::~EdMusicBrowser()
{
}

void EdMusicBrowser::Update()
{
  ListUpdate();
}

void EdMusicBrowser::OnPlayMusic( wxCommandEvent& event )
{
  unsigned long SongSection;
  m_SongSectionBox->GetValue().ToULong( &SongSection );
  GEngine->Audio->PlayMusic( (UMusic*)m_SelectedObject, SongSection, MTRAN_Instant );
}

void EdMusicBrowser::OnStopMusic( wxCommandEvent& event )
{
  // TODO: Maybe make this configurable as to how quickly the user wants the music to stop
  GEngine->Audio->StopMusic( MTRAN_SlowFade );
}

wxBEGIN_EVENT_TABLE( EdMusicBrowser, EdBrowser )
  EVT_BUTTON( ID_PlayMusic, EdMusicBrowser::OnPlayMusic )
  EVT_BUTTON( ID_StopMusic, EdMusicBrowser::OnStopMusic )
wxEND_EVENT_TABLE()
