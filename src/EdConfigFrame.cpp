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
 * EdConfigFrame.cpp
 *
 * written by Jesse 'Hyzoran' Kowalik
 *========================================================================
*/

#include "EdConfigFrame.h"

//========================================================================
// EdConfigFrame
EdConfigFrame::EdConfigFrame( wxWindow* Parent, FConfig* Config, wxString Section, bool bDock, EdGamePrompt* ModalPrompt )
    :   EdToolFrame( Parent, wxString( wxString("Configuration Editor [") + Section + wxString("]") ), bDock ),
    m_ModalPrompt( ModalPrompt )
{
  if( Section.IsEmpty() || Section == wxString( "" ) )
  {
    m_Category = NULL;
  }
  else
   m_Category = Config->GetCategoryFromName( Section.ToAscii() );

  m_Config = Config;

  wxBoxSizer* hsizer = new wxBoxSizer( wxHORIZONTAL );

    m_Ctrl = new wxTreeListCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN );
      hsizer->Add( m_Ctrl, 1, wxEXPAND );

    wxPanel* panel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxSize( 48, -1 ) );
      hsizer->Add( panel, 0, wxEXPAND );

    wxBoxSizer* vsizer = new wxBoxSizer( wxVERTICAL );

      wxButton* buttonApply = new wxButton( panel, ID_Apply, "Apply", wxDefaultPosition, wxDefaultSize );
        vsizer->Add( buttonApply, 0 );

    panel->SetSizer(vsizer);

  SetSizer(hsizer);

  //Contruct control columns
  m_Ctrl->AppendColumn( wxString("Config") );
  m_Ctrl->AppendColumn( wxString("Entry"), 128 );
  m_Ctrl->AppendColumn( wxString("Value"), 128 );
  ConfigRefresh();
  Show(true);
}

EdConfigFrame::~EdConfigFrame()
{
    OnExit();
}

void EdConfigFrame::Apply()
{
  if ( m_Changed.IsEmpty() ) //buffer empty, skip.
    return;

  TArray<wxTreeListItem> items; //Selected config data to update.
  EdConfigConfirm confirmDialog( this, m_Changed, m_Ctrl, items );

  confirmDialog.ShowModal();

  if( items.IsEmpty() ) //We didn't select any or cancelled, do nothing.
    return;

  for ( size_t i = 0; i < items.Size(); i++ )
  {
    ConfigDataItem* itemData = (ConfigDataItem*)m_Ctrl->GetItemData( items[i] );

    itemData->WriteConfig();
  }

  m_Changed.Clear();
  ConfigRefresh();
}

void EdConfigFrame::ConfigRefresh()
{
  m_Ctrl->DeleteAllItems(); //Clear original state.

  //Get the list of Configs to parse.
  TArray<FConfig*> Configs;

  if( m_Config != NULL )
    Configs.PushBack( m_Config );
  else
  {
    Configs.PushBack( GLibunrConfig );
    Configs.PushBack( GGameConfig );
    Configs.PushBack( GUserConfig );
  }

  wxTreeListItem rootItem = m_Ctrl->GetRootItem();

  for( size_t i = 0; i < Configs.Size(); i++ )
  {
    //Populate Configs
    FConfig* currentConfig = Configs[i];

    wxTreeListItem itemConfig = m_Ctrl->AppendItem( rootItem, wxString( currentConfig->GetName() ) + wxString(".ini"), -1, -1, new ConfigDataItem( currentConfig ) );
    //Populate Categories

    if( m_Category != NULL )
    {
      wxTreeListItem itemCategory = m_Ctrl->AppendItem( itemConfig, m_Category->Name, -1, -1, new ConfigDataItem( currentConfig, m_Category ) );

      //Populate Entries
      for ( size_t i3 = 0; i3 < m_Category->Entries->Size(); i3++ )
      {
        FConfig::FConfigEntry* currentEntry = m_Category->Entries->operator[](i3);

        RecurseEntry( itemCategory, currentEntry, ConfigDataItem( currentConfig, m_Category, currentEntry ) );
      }

      m_Ctrl->Expand( itemCategory );
    }
    else
    {
      for( size_t i2 = 0; i2 < currentConfig->GetCategories()->Size(); i2++ )
      {
        FConfig::FConfigCategory* currentCategory = currentConfig->GetCategories()->operator[](i2);

        wxTreeListItem itemCategory = m_Ctrl->AppendItem( itemConfig, currentCategory->Name, -1, -1, new ConfigDataItem( currentConfig, currentCategory ) );

        //Populate Entries
        for ( size_t i3 = 0; i3 < currentCategory->Entries->Size(); i3++ )
        {
          FConfig::FConfigEntry* currentEntry = currentCategory->Entries->operator[](i3);

          RecurseEntry( itemCategory, currentEntry, ConfigDataItem( currentConfig, currentCategory, currentEntry ) );
        }
      }
    }
  }
}

void EdConfigFrame::RecurseEntry( wxTreeListItem ParentItem, FConfig::FConfigEntry* Entry, ConfigDataItem BaseData )
{
  wxTreeListItem newItem;

  if( Entry->StructVars == NULL || Entry->StructVars->IsEmpty() )
  {
    for( size_t i = 0; i < Entry->Values->Size(); i++ )
    {
      newItem = m_Ctrl->AppendItem( ParentItem, wxString(""), -1, -1, new ConfigDataItem( BaseData.m_Config, BaseData.m_Category, BaseData.m_Entry, i, BaseData.m_Parent ) );

      //Generate Entry name
      m_Ctrl->SetItemText( newItem, 1, wxString( Entry->Name + ( wxString("[") << i  ) + wxString("]") ) );

      //Generate Entry Value
      m_Ctrl->SetItemText( newItem, 2, Entry->Values->operator[](i) );
    }
  }
  else
  {
    for( size_t i = 0; i < Entry->StructVars->Size(); i++ )
    {
      newItem = m_Ctrl->AppendItem( ParentItem, wxString(""), -1, -1, new ConfigDataItem( BaseData.m_Config, BaseData.m_Category, BaseData.m_Entry, i, BaseData.m_Parent ) );

      //Generate Entry name
      m_Ctrl->SetItemText( newItem, 1, wxString( Entry->Name + ( wxString("[") << i  ) + wxString("]") ) );

      //Generate Entry Values

      RecurseEntry( newItem, Entry->StructVars->operator[](i), ConfigDataItem( BaseData.m_Config, BaseData.m_Category, Entry->StructVars->operator[](i), -1, &BaseData ) );
    }
  }
}

void EdConfigFrame::EventApply( wxCommandEvent& event )
{
  Apply();
  event.Skip();
}

void EdConfigFrame::EventRefresh( wxCommandEvent& event )
{
  ConfigRefresh();
  event.Skip();
}

void EdConfigFrame::EventObjectMenu( wxCommandEvent& event )
{
  event.Skip();
}

void EdConfigFrame::EventObjectPlay( wxCommandEvent& event )
{
  event.Skip();
}

void EdConfigFrame::EventObjectOpen( wxCommandEvent& event )
{
  event.Skip();
}

void EdConfigFrame::OnExit()
{
  Apply();
  if( m_ModalPrompt != NULL )
  {
      m_ModalPrompt->OnEnable();
  }
}

//========================================================================
// ConfigDataItem

EdConfigFrame::ConfigDataItem::ConfigDataItem( FConfig* Config, FConfig::FConfigCategory* Category, FConfig::FConfigEntry* Entry, int Index, ConfigDataItem* Parent) : m_Config( Config ), m_Category( Category ), m_Entry( Entry ), m_Index( Index ), m_Parent( Parent )
{}

EdConfigFrame::ConfigDataItem::ConfigDataItem( wxString ConfigStr, wxString CategoryStr, wxString EntryStr, ConfigDataItem* Parent ) : m_ConfigStr( ConfigStr ), m_CategoryStr( CategoryStr ), m_EntryStr( EntryStr ), m_Parent( Parent ), m_bFake( true )
{}

wxString EdConfigFrame::ConfigDataItem::GetDataName()
{
  wxString out = wxString("");

  if( m_bFake )
  {
    if( m_Parent == NULL )
    {
      out += m_ConfigStr;
      out += wxString(".");
      out += m_CategoryStr;
    }
    else
    {
      out += m_Parent->GetDataName();
    }
    out += wxString(".");
    out += m_EntryStr;
    out += wxString("[");
    out << m_Index;
    out += wxString("]");
  }
  else
  {
    if( m_Parent == NULL )
    {
      out += m_Config->GetName();
      out += wxString(".");
      out += wxString( m_Category->Name );
    }
    else
    {
      out += m_Parent->GetDataName();
    }
    out += wxString(".");
    out += m_Entry->Name;
    out += wxString("[");
    out << m_Index;
    out += wxString("]");
  }

  return out;
}

bool EdConfigFrame::ConfigDataItem::WriteConfig()
{
  //TODO:
  return false;
}

//========================================================================
// EdConfigConfirm

EdConfigFrame::EdConfigConfirm::EdConfigConfirm( EdConfigFrame* Parent,  TArray<wxTreeListItem>& Changed, wxTreeListCtrl* Ctrl, TArray<wxTreeListItem>& OutSelected ) 
    : wxDialog( Parent, wxID_ANY, wxString( "Commit Config Changes"), wxDefaultPosition, wxDefaultSize, wxCLOSE_BOX | wxSTAY_ON_TOP | wxRESIZE_BORDER ), m_OutSelected( &OutSelected ), m_Items( Changed )
{
  wxArrayString StrAry;

  for( size_t i = 0; i < Changed.Size(); i++ )
  {
    ConfigDataItem* currentData = (ConfigDataItem*)Ctrl->GetItemData( Changed[i] );

    StrAry.Add( currentData->GetDataName() );
  }

  wxBoxSizer* vsizer = new wxBoxSizer( wxVERTICAL );
    m_Ctrl = new wxCheckListBox( (wxWindow*)this, wxID_ANY, wxDefaultPosition, wxDefaultSize, StrAry, wxLB_EXTENDED | wxLB_HSCROLL );
      vsizer->Add( m_Ctrl, 1 );

    wxPanel* panel = new wxPanel( this );
      vsizer->Add( panel, 1 );

      wxBoxSizer* hsizer = new wxBoxSizer( wxHORIZONTAL );

      wxButton* buttonConfirm = new wxButton( panel, ID_Confirm, "Confirm", wxDefaultPosition, wxDefaultSize, wxBU_LEFT );
        hsizer->Add( buttonConfirm, 0, wxLEFT | wxALIGN_LEFT );
      wxButton* buttonCancel = new wxButton( panel, wxID_CANCEL, "Cancel", wxDefaultPosition, wxDefaultSize, wxBU_LEFT );
        hsizer->Add( buttonCancel, 0, wxLEFT | wxALIGN_LEFT );

     panel->SetSizer( hsizer );
  
  SetSizer( vsizer );
}

void EdConfigFrame::EdConfigConfirm::EventConfirm( wxCommandEvent& event )
{
  for( size_t i = 0; i < m_Ctrl->GetCount(); i++ )
  {
    if( m_Ctrl->IsChecked( i ) )
    {
      m_OutSelected->PushBack( m_Items[i] );
    }
  }

  EndModal( 0 );
  Close( true );
}

wxBEGIN_EVENT_TABLE(EdConfigFrame, wxFrame)
    EVT_BUTTON(ID_Apply, EdConfigFrame::EventApply)
    EVT_BUTTON(ID_Refresh, EdConfigFrame::EventRefresh)
wxEND_EVENT_TABLE()