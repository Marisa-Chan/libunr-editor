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
* EdObjectBrowser.cpp
*
* written by Jesse 'Hyzoran' Kowalik
*========================================================================
*/

#include "EdEditor.h"
#include "EdBrowsers.h"

EdObjectBrowser::EdObjectBrowser( TArray<UClass*>& Classes, bool bExactClass, bool bStartDocked )
  : EdBrowser( getName( Classes ), bStartDocked ), m_Classes( Classes ), m_bExactClass( bExactClass )
{

  //Set default open directory based on class type
  if( Classes.Size() == 1 )
  {
    if( Classes[0]->IsA( USound::StaticClass() ) || Classes[0] == USound::StaticClass() )
    {
      m_dirPath = EdEditor::gc_SubDir_UAX;
    }
    else if( Classes[0]->IsA( UMusic::StaticClass() ) || Classes[0] == UMusic::StaticClass() )
    {
      m_dirPath = EdEditor::gc_SubDir_UMX;
    }
    else if( Classes[0]->IsA( UTexture::StaticClass() ) || Classes[0] == UTexture::StaticClass() )
    {
      m_dirPath = EdEditor::gc_SubDir_UTX;
    }
    else if( Classes[0]->IsA( ULevel::StaticClass() ) || Classes[0] == ULevel::StaticClass() )
    {
      m_dirPath = EdEditor::gc_SubDir_UNR;
    }
    else if( Classes[0]->IsA( UModel::StaticClass() ) || Classes[0] == UModel::StaticClass() )
    {
      m_dirPath = EdEditor::gc_SubDir_USM;
    }
    else
    {
      m_dirPath = wxString(" ");
    }
  }
  else
  {
    m_dirPath = wxString(" ");
  }

  m_Ctrl = new wxTreeListCtrl( this, ID_Ctrl, wxDefaultPosition, wxDefaultSize, wxTL_MULTIPLE | wxTL_NO_HEADER );

    m_VSizer->Add( m_Ctrl, 1, wxEXPAND );

  m_Ctrl->AppendColumn( wxString("Object"), wxCOL_WIDTH_AUTOSIZE, wxALIGN_LEFT, wxCOL_RESIZABLE | wxCOL_SORTABLE  );
  m_Ctrl->AppendColumn( wxString("Class"), wxCOL_WIDTH_AUTOSIZE, wxALIGN_LEFT, wxCOL_RESIZABLE | wxCOL_SORTABLE  );

  Show();

  ObjectUpdate();
}

void EdObjectBrowser::ObjectUpdate()
{
  //First, preserve any expanded members, so that they will be expanded when the control is re-populated.
  TArray<UObject*> expanded;
  TArray<u32> expandedGroups; //Used instead for group items ( Object == NULL and Group != 0 )

  wxTreeListItem item = m_Ctrl->GetFirstItem();

  for ( wxTreeListItem item = m_Ctrl->GetFirstItem(); item.IsOk(); item = m_Ctrl->GetNextItem( item ) )
  {
    UObject* currentObject = ((EdEditor::UObjectClientData*)( m_Ctrl->GetItemData( item ) ))->GetObject();
    u32 currentGroup = ((EdEditor::UObjectClientData*)( m_Ctrl->GetItemData( item ) ))->GetGroup();

    if( ( m_Ctrl->IsExpanded( item ) ) )
    {
      if( currentObject != NULL )
      {
        expanded.PushBack( currentObject );
      }
      else if( currentGroup != 0 )
      {
        expandedGroups.PushBack( currentGroup );
      }
    }
  }

  //Clear tree for update
  //TODO: Optimize by tracking what objects have been removed or addded instead of regenerating entire tree.
  m_Ctrl->DeleteAllItems();

  //Populate Packages
  for( size_t i = 0; i<UPackage::GetLoadedPackages()->Size(); i++ )
  {
    UPackage* currentPackage = (*(UPackage::GetLoadedPackages()))[i];

    if( currentPackage == NULL ) //Invalid Package
      continue;

    wxTreeListItem currentPackageItem;
    TArray<wxTreeListItem> groupItems;

    //Populate Objects, each time an object is found compare populate its group and populate object under that group.
    for( size_t i2 = 0; i2<currentPackage->GetExportTable().Size(); i2++ )
    {
      bool bClassMatch = false;
      wxTreeListItem groupItem;
      FExport* currentExport = &(currentPackage->GetExportTable()[i2]);

      if( currentExport->Obj == NULL )//Invalid Object
          continue;

      //Check object meets class criteria, if so, add it.
      for( size_t i3 = 0; i3<m_Classes.Size(); i3++ )
      {
        if( m_bExactClass )
        {
          bClassMatch = currentExport->Obj->Class == m_Classes[i3];
        }
        else
        {
          bClassMatch = currentExport->Obj->IsA( m_Classes[i3] );
        }
        if( bClassMatch )
          break;
      }

      //Match, add this object.
      if( bClassMatch )
      {
        //Add Package if not added before.
        if( !currentPackageItem.IsOk() )
        {
          currentPackageItem = m_Ctrl->AppendItem( m_Ctrl->GetRootItem(), currentPackage->Name.Data(), NULL, NULL, new EdEditor::UObjectClientData( currentPackage ) );
        }

        //Search for group to add.
        for( size_t i3 = 0; i3<groupItems.Size(); i3++ )
        {
          if( ((EdEditor::UObjectClientData*)(m_Ctrl->GetItemData( groupItems[i3] )))->GetGroup() == currentExport->Group )
          {
            groupItem = groupItems[i3];
          }
        }

        //No group found, make it.
        if( !groupItem.IsOk() )
        {
          groupItem = m_Ctrl->AppendItem( currentPackageItem, currentPackage->GetNameEntryByObjRef( currentExport->Group )->Data, NULL, NULL, new EdEditor::UObjectClientData( NULL, currentExport->Group ) );
          groupItems.PushBack( groupItem );
        }

        //Finally, add the object.
        wxTreeListItem item = m_Ctrl->AppendItem( groupItem, currentExport->Obj->Name.Data(), NULL, NULL, new EdEditor::UObjectClientData( currentExport->Obj ) );
        m_Ctrl->SetItemText( item, 1, currentExport->Obj->Class->Name.Data() );
      }

    } //End Object

    //Check if Package was expanded before.
    for( size_t i2 = 0; i2<expanded.Size(); i2++ )
    {
      if( currentPackage == expanded[i2] )
      {
        m_Ctrl->Expand( currentPackageItem );
        break;
      }
    }

    //Check expanded groups.
    for( size_t i2 = 0; i2<groupItems.Size(); i2++ )
    {
      u32 currentGroup = ((EdEditor::UObjectClientData*)(m_Ctrl->GetItemData( groupItems[i2] )))->GetGroup();

      for( size_t i3 = 0; i3<expandedGroups.Size(); i3++ )
      {
        if( currentGroup == expandedGroups[i3] )
        {
          m_Ctrl->Expand( groupItems[i2] );
          break;
        }
      }
    }
  } //End Package
}

wxString EdObjectBrowser::getName( TArray<UClass*>& Classes )
{
  wxString str;

  if( Classes.Size() == 1 )
  {
    if( Classes[0]->IsA( USound::StaticClass() ) || Classes[0] == USound::StaticClass() )
    {
      str = wxString( "Sound" );
    }
    else if( Classes[0]->IsA( UMusic::StaticClass() ) || Classes[0] == UMusic::StaticClass() )
    {
      str = wxString( "Music" );
    }
    else if( Classes[0]->IsA( UTexture::StaticClass() ) || Classes[0] == UTexture::StaticClass() )
    {
      str = wxString( "Texture" );
    }
    else if( Classes[0]->IsA( ULevel::StaticClass() ) || Classes[0] == ULevel::StaticClass() )
    {
      str = wxString( "Level" );
    }
    else if( Classes[0]->IsA( UModel::StaticClass() ) || Classes[0] == UModel::StaticClass() )
    {
      str = wxString( "Mesh" );
    }
    else
    {
      str = wxString( "Object" );
      m_dirPath = wxString(" ");
    }
  }
  else
  {
    str = wxString("");
    m_dirPath = wxString(" ");
  }

  return str + wxString( " Browser");
}

void EdObjectBrowser::EVT_ObjectMenu( wxTreeListEvent& event )
{
  wxTreeListItems items;

  if( m_Ctrl->GetSelections( items ) < 1 )
  {
    event.Skip();
    return;
  }

  TArray<UObject*> objects;

  for( size_t i = 0; i<items.size(); i++ )
  {
    UObject* currentObject = ( (EdEditor::UObjectClientData*)( m_Ctrl->GetItemData( items[i] ) ) )->GetObject();

    if( currentObject != NULL )
      objects.PushBack( currentObject );
  }

  new EdEditor::UObjectContextMenu( this, objects );
}

wxBEGIN_EVENT_TABLE( EdObjectBrowser, EdBrowser )
  EVT_TREELIST_ITEM_CONTEXT_MENU( ID_Ctrl, EdObjectBrowser::EVT_ObjectMenu )
wxEND_EVENT_TABLE()