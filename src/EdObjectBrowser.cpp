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

EdObjectBrowser::EdObjectBrowser( TArray<UClass*>& Classes, bool bExactClass, bool bStartDocked, UPackage* Package )
  : EdBrowser( getName( Classes ), bStartDocked ), m_Classes( Classes ), m_bExactClass( bExactClass )
{

  //Set properties relating to browser types.
  if( Classes.Size() == 1 )
  {
    if( Classes[0]->IsA( USound::StaticClass() ) || Classes[0] == USound::StaticClass() )
    {
      m_dirPath = EdEditor::gc_SubDir_UAX;
      SetIcon( EdEditor::g_icoSound );
    }
    else if( Classes[0]->IsA( UMusic::StaticClass() ) || Classes[0] == UMusic::StaticClass() )
    {
      m_dirPath = EdEditor::gc_SubDir_UMX;
      SetIcon( EdEditor::g_icoMusic );
    }
    else if( Classes[0]->IsA( UTexture::StaticClass() ) || Classes[0] == UTexture::StaticClass() )
    {
      m_dirPath = EdEditor::gc_SubDir_UTX;
      SetIcon( EdEditor::g_icoTexture );
    }
    else if( Classes[0]->IsA( ULevel::StaticClass() ) || Classes[0] == ULevel::StaticClass() )
    {
      m_dirPath = EdEditor::gc_SubDir_UNR;
      SetIcon( EdEditor::g_icoMisc );
    }
    else if( Classes[0]->IsA( UMesh::StaticClass() ) || Classes[0] == UModel::StaticClass() )
    {
      m_dirPath = EdEditor::gc_SubDir_USM;
      SetIcon( EdEditor::g_icoMesh );
    }
    else
    {
      m_dirPath = wxString(" ");
      SetIcon( EdEditor::g_icoMisc );
    }
  }
  else
  {
    m_dirPath = wxString(" ");
    SetIcon( EdEditor::g_icoMisc );
  }

  m_PackageCtrl = new wxComboBox( m_HeaderPanel, ID_FilterPackageCtrl, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_SORT );
    PackageListUpdate();
    PackageSelect( Package );
    m_HeaderSizer->Add( m_PackageCtrl, 0, wxALIGN_CENTRE_VERTICAL);

  m_CheckFilterPackage = new wxCheckBox( m_HeaderPanel, ID_FilterPackage, wxString( "Filter by Package" ) );
    m_CheckFilterPackage->SetValue( false );
    m_HeaderSizer->Add( m_CheckFilterPackage, 0, wxALIGN_CENTRE_VERTICAL);

  m_Ctrl = new wxTreeListCtrl( this, ID_Ctrl, wxDefaultPosition, wxDefaultSize, wxTL_MULTIPLE | wxTL_NO_HEADER );

    m_VSizer->Add( m_Ctrl, 1, wxEXPAND );

  m_Ctrl->AppendColumn( wxString("Object"), wxCOL_WIDTH_AUTOSIZE, wxALIGN_LEFT, wxCOL_RESIZABLE | wxCOL_SORTABLE  );
  m_Ctrl->AppendColumn( wxString("Class"), wxCOL_WIDTH_AUTOSIZE, wxALIGN_LEFT, wxCOL_RESIZABLE | wxCOL_SORTABLE  );

  Show();

  ObjectUpdate();
}

void EdObjectBrowser::ObjectUpdate()
{
  //Clear tree for update
  //TODO: Optimize by tracking what objects have been removed or addded instead of regenerating entire tree.
  m_Ctrl->DeleteAllItems();

  if( m_bUpdatePackageList )
    PackageListUpdate();

  m_bUpdatePackageList = true;

  //Populate Packages
  for( size_t i = 0; i<UPackage::GetLoadedPackages()->Size(); i++ )
  {
    UPackage* currentPackage;

    if( m_CheckFilterPackage->GetValue() )
    {
      currentPackage = GetSelectedPackage();

      if( currentPackage == NULL )
          return;
    }
    else
      currentPackage = (*(UPackage::GetLoadedPackages()))[i];

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

    if( m_CheckFilterPackage->GetValue() ) //If we are only doing one package, break.
      break;
  } //End Package

  for ( wxTreeListItem item = m_Ctrl->GetFirstItem(); item.IsOk(); item = m_Ctrl->GetNextItem( item ) )
  {
    m_Ctrl->Expand( item );
  }
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
    else if( Classes[0]->IsA( UMesh::StaticClass() ) || Classes[0] == UMesh::StaticClass() )
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

void EdObjectBrowser::EVT_FilterPackage( wxCommandEvent& event )
{
  m_bUpdatePackageList = false;
  ObjectUpdate();
}

void EdObjectBrowser::EVT_FilterPackageCtrl( wxCommandEvent& event )
{
  if( !m_CheckFilterPackage->GetValue() )
    return;

  m_bUpdatePackageList = false;
  ObjectUpdate();
}

wxBEGIN_EVENT_TABLE( EdObjectBrowser, EdBrowser )
  EVT_TREELIST_ITEM_CONTEXT_MENU( ID_Ctrl, EdObjectBrowser::EVT_ObjectMenu )
  EVT_CHECKBOX( ID_FilterPackage, EdObjectBrowser::EVT_FilterPackage )
  EVT_COMBOBOX( ID_FilterPackageCtrl, EdObjectBrowser::EVT_FilterPackageCtrl )
wxEND_EVENT_TABLE()