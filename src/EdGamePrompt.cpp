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
* EdGamePrompt.cpp
*
* written by Jesse 'Hyzoran' Kowalik
*========================================================================
*/

#include "EdGamePrompt.h"
#include "EdConfigFrame.h"

EdGamePrompt::EdGamePrompt( TArray<char*>* Names, int& OutGameIndex )
  : wxDialog( NULL, wxID_ANY, wxString("Select Game Configuration"), wxDefaultPosition, wxDefaultSize, wxRESIZE_BORDER | wxSYSTEM_MENU | wxCLOSE_BOX | wxCAPTION | wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxBORDER_THEME ), m_OutGameIndex( &OutGameIndex )
{
  wxArrayString StrAry;

  for( size_t i = 0; i< Names->Size(); i++ )
  {
    StrAry.Add( wxString( (*Names)[i] ) );
  }
  hsizer = new wxBoxSizer( wxHORIZONTAL );
    
    m_Ctrl = new wxListBox( this, ID_ListBox, wxDefaultPosition, wxSize( -1, -1 ), StrAry, wxLB_SINGLE | wxBORDER_SUNKEN );
      hsizer->Add( m_Ctrl, 1, wxLEFT | wxEXPAND );

    wxPanel* panel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxSize( 48, -1 ) );
      hsizer->Add( panel, 0, wxRIGHT | wxEXPAND );

      wxBoxSizer* vsizer = new wxBoxSizer( wxVERTICAL );

      wxButton* buttonSelect = new wxButton( panel, ID_Select, "Select", wxDefaultPosition, wxDefaultSize, wxBU_TOP );
        vsizer->Add( buttonSelect, 0, wxTOP | wxALIGN_TOP );
      wxButton* buttonEdit = new wxButton( panel, ID_Edit, "Edit", wxDefaultPosition, wxDefaultSize, wxBU_TOP );
        vsizer->Add( buttonEdit, 0, wxTOP | wxALIGN_TOP );
      wxButton* buttonCancel = new wxButton( panel, wxID_CANCEL, "Cancel", wxDefaultPosition, wxDefaultSize, wxBU_TOP );
        vsizer->Add( buttonCancel, 0, wxTOP | wxALIGN_TOP );

    panel->SetSizer( vsizer );

  SetSizer( hsizer );

  Show(true);
}

void EdGamePrompt::PromptRefresh()
{
  FConfig::FConfigCategory* category = GLibunrConfig->GetCategoryFromName("Game");
  FConfig::FConfigEntry* entry = category->GetEntryFromName("Name");
  wxArrayString strAry;

  for( size_t i = 0; i < entry->Values->Size(); i++ )
  {
    strAry.Add( (*entry->Values)[i] );
  }

  wxListBox* newbox = new wxListBox( this, ID_ListBox, wxDefaultPosition, wxDefaultSize, strAry, wxLB_SINGLE | wxBORDER_SUNKEN );

  hsizer->Replace( m_Ctrl, newbox );
  delete m_Ctrl;
  m_Ctrl = newbox;
  hsizer->Layout();
}

void EdGamePrompt::Select( int Choice )
{
  *m_OutGameIndex = Choice;
  EndModal( 0 );
  Close( true );
}

void EdGamePrompt::OnEnable()
{
  Enable(true);
  PromptRefresh();
}

void EdGamePrompt::EVT_Select( wxCommandEvent& event )
{
  Select( m_Ctrl->GetSelection() );
  m_EditWindow = NULL;
  event.Skip();
}

void EdGamePrompt::EVT_Edit( wxCommandEvent& event )
{
  m_EditWindow = new EdConfigFrame( this, GLibunrConfig, "Game", false, this );
  Disable();
  event.Skip();
}

void EdGamePrompt::EVT_ListBox( wxCommandEvent& event )
{
  Select( event.GetSelection() );
  event.Skip();
}

void EdGamePrompt::OnCancel( wxCommandEvent& event )
{
  *m_OutGameIndex = -1;
}

wxBEGIN_EVENT_TABLE(EdGamePrompt, wxDialog)
    EVT_BUTTON(ID_Select, EdGamePrompt::EVT_Select)
    EVT_BUTTON(ID_Edit, EdGamePrompt::EVT_Edit)
    EVT_BUTTON(ID_ListBox, EdGamePrompt::EVT_ListBox)
    EVT_LISTBOX_DCLICK(ID_Select, EdGamePrompt::EVT_Select)
wxEND_EVENT_TABLE()