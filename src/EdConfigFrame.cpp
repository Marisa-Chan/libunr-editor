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
EdConfigFrame::EdConfigFrame( FConfig* Config, wxString Section, bool bDock, EdGamePrompt* ModalWindow )
    :   EdToolFrame( wxString(" Configuration Editor [" + Section + wxString("]"), bDock, ModalWindow ), m_Section( Section ), m_bLockSection( bLockSection )
{
  m_Category = Config->GetCategoryFromName( Section.ToAscii() );

  wxBoxSizer* vsizer = new wxBoxSizer( wxVERTICAL );

    m_Ctrl = new wxTreeListCtrl();
      vsizer.Add( m_Ctrl, 1 );

    wxPanel* panel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxSize( -1, 48 ) );
      vsizer.Add( panel, 0 );

    wxBoxSizer* hsizer = new wxBoxSizer( wxHORIZONTAL );

      wxButton* buttonApply = new wxButton( panel, ID_Apply, "Apply", wxDefaultPosition, wxDefaultSize, wxBU_RIGHT );
        hsizer.Add( buttonConfirm, 0, wxRIGHT | wxALIGN_RIGHT );

    panel->SetSizer(hsizer);

  SetSizer(vsizer);

  Refresh();
}

void EdConfigFrame::Apply()
{
  if ( m_Changed.IsEmpty() ) //buffer empty, skip.
    return;

  TArray<wxTreeListItem> items; //Selected config data to update.
  EdConfigConfirm confirmDialog( m_Changed, m_Ctrl, items );

  confirmDialog.ShowModal( true );

  if( items.IsEmpty() ) //We didn't select any or cancelled, do nothing.
    return;

  for ( size_t i = 0; i < items.Size(); i++ )
  {
    ConfigDataItem* itemData = ConfigDataItem*( m_Ctrl->GetItemData( items[i] ) );

    itemData->WriteConfig();
  }

  m_Changed.Clear();
  Refresh();
}

void EdConfigFrame::Refresh()
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

    wxTreeListItem itemConfig = m_Ctrl->AppendItem( rootItem, currentConfig->GetName(), NO_IMAGE, NO_IMAGE, new ConfigDataItem( currentConfig ) );

    //Populate Categories
    for( size_t i2 = 0; i2 < currentConfig->GetCategories()->Size(), i2++ )
    {
      FConfigCategory* currentCategory = currentConfig->(*GetCategories())[i2] );

      wxTreeListItem itemCategory = m_Ctrl->AppendItem( itemConfig, currentCategory->Name, NO_IMAGE, NO_IMAGE, new ConfigDataItem( currentConfig, currentCategory ) );

      //Populate Entries
      for (size_t i3 = 0; i3 < currentCategory->Entries->Size(), i3++ )
      {
        FConfigEntry* currentEntry = currentCategory->(*Entries)[i3];

        RecurseEntry( itemCategory, currentEntry, ConfigDataItem( currentConfig, currentCategory, currentEntry ) );
      }
    }
  }
}

void EdConfigFrame::RecurseEntry( wxTreeListItem ParentItem, FConfigEntry* Entry, ConfigDataItem BaseData )
{
  wxTreeListItem newItem;

  if( Entry->StructVars.IsEmpty() )
  {
    for( size_t i = 0; i < Entry->Values.Size(); i++ )
    {
      newItem = m_Ctrl->AppendItem( ParentItem, Entry->Name + wxString("[") << i + wxString("]"), NO_IMAGE, NO_IMAGE, ConfigDataItem( BaseData.m_Config, BaseData.m_Category, BaseData.m_Entry, i, BaseData.m_Parent ) );
    }
  }
  else
  {
    for( size_t i = 0; i < Entry->StructVars.Size(); i++ )
    {
      newItem = m_Ctrl->AppendItem( ParentItem, Entry->Name + wxString("[") << i + wxString("]"), NO_IMAGE, NO_IMAGE, ConfigDataItem( BaseData.m_Config, BaseData.m_Category, BaseData.m_Entry, i, BaseData.m_Parent ) );

      RecurseEntry( newItem, Entry->StructVars[i], ConfigDataItem( BaseData.m_Config, BaseData.m_Category, Entry->StructVars[i], -1, Entry ) );
    }
  }
}

void EdConfigFrame::EVT_Apply( wxCommandEvent& event )
{
  Apply();
  event.skip();
}

void EdConfigFrame::EVT_Refresh( wxCommandEvent& event )
{
  Refresh();
  event.skip();
}

void EdConfigFrame::EVT_ObjectMenu( wxCommandEvent& event )
{
  event.skip();
}

void EdConfigFrame::EVT_ObjectPlay( wxCommandEvent& event )
{
  event.skip();
}

void EdConfigFrame::EVT_ObjectOpen( wxCommandEvent& event )
{
  event.skip();
}

void EdConfigFrame::OnExit( wxCommandEvent& event )
{

  Apply();
}

//========================================================================
// ConfigDataItem

EdConfigFrame::ConfigDataItem::ConfigDataItem( FConfig* Config, FConfigCategory* Category, FConfigEntry* Entry, int Index, ConfigDataItem* Parent) : m_Config( Config ), m_Category( Category ), m_Entry( Entry ), m_Index( Index ), m_Parent( Parent )
{}

EdConfigFrame::ConfigDataItem::ConfigDataItem( wxString ConfigStr, wxString CategoryStr, wxString EntryStr, ConfigDataItem* Parent ) : m_ConfigStr( ConfigStr ), m_CategoryStr( CategoryStr ), m_EntryStr( EntryStr ), m_Index( Index ), m_Parent( Parent ), m_bFake( true )
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
    : wxDialog( Parent, wxID_ANY, wxString( "Commit Config Changes"), wxDefaultPosition, wxDefaultSize, wxCLOSE_BOX | wxSTAY_ON_TOP | wxRESIZE_BORDER ), m_OutSelected( OutSelected ), m_Items( Changed )
{
  wxArrayString StrAry;

  for( size_t i = 0; i < Changed.Size(); i++ )
  {
    ConfigDataItem* currentData = ConfigDataItem*( m_Ctrl->GetItemData( Changed[i] ) );

    StrAry.Add( currentData->GetDataName() );
  }

  wxBoxSizer* vsizer = new wxBoxSizer( wxVERTICAL );
    m_Ctrl = new wxCheckListBox( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, Changed.Size(), StrAry, wxLB_EXTENDED | wxLB_HSCROLL );
      vsizer.Add( m_Ctrl, 1 );

    wxPanel* panel = new wxPanel( this );
      vsizer.Add( panel, 1 );

      wxBoxSizer* hsizer = new wxBoxSizer( wxHORIZONTAL );

      wxButton* buttonConfirm = new wxButton( panel, ID_Confirm, "Confirm", wxDefaultPosition, wxDefaultSize, wxBU_LEFT );
        hsizer.Add( buttonConfirm, 0, wxLEFT | wxALIGN_LEFT );
      wxButton* buttonCancel = new wxButton( panel, wxID_Cancel, "Cancel", wxDefaultPosition, wxDefaultSize, wxBU_LEFT );
        hsizer.Add( buttonCancel, 0, wxLEFT | wxALIGN_LEFT );

     panel->SetSizer( hszier );
  
  SetSizer( vsizer );
}

EdConfigFrame::EdConfigConfirm::EVT_Confirm( wxCommandEvent& event )
{
  for( size_t i = 0; i < m_Ctrl->GetCount(); i++ )
  {
    if( IsChecked( i ) )
    {
      m_OutSelected->PushBack( m_Items[i] );
    }
  }

  EndModal();
  Close( true );
}