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

#define CTRLFONT() SetFont( wxFont(8, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_MEDIUM, false ) )

EdObjectBrowser::EdObjectBrowser( wxWindow* Parent, TArray<UClass*>& Classes, bool bExactClass, bool bStartDocked, UPackage* Package )
  : EdBrowser( Parent ), m_Classes( Classes ), m_bExactClass( bExactClass )
{
  m_Ctrl = new wxTreeListCtrl( this, EdToolFrame::ID_Ctrl, wxDefaultPosition, wxDefaultSize, wxTL_MULTIPLE );

    m_VSizer->Add( m_Ctrl, 1, wxEXPAND );

  m_Ctrl->AppendColumn( wxString("Object"), wxCOL_WIDTH_AUTOSIZE, wxALIGN_LEFT, wxCOL_RESIZABLE | wxCOL_SORTABLE  );
  m_Ctrl->AppendColumn( wxString("Class"), wxCOL_WIDTH_AUTOSIZE, wxALIGN_LEFT, wxCOL_RESIZABLE | wxCOL_SORTABLE  );

  m_Ctrl->CTRLFONT();

  Show();

  ObjectUpdate();

  wxString labelName;

  if( m_Classes.Size() == 1 )
    labelName = wxString( m_Classes[0]->Name.Data() );
  else
    labelName = wxString( "Object" );

  SetLabel( labelName + wxString( " Browser" ) );
}

void EdObjectBrowser::ObjectUpdate( bool bUpdatePackageList )
{
  //Clear tree for update
  //TODO: Optimize by tracking what objects have been removed or addded instead of regenerating entire tree.
  m_Ctrl->DeleteAllItems();

  if( bUpdatePackageList )
    m_PackageCtrl->PackageListUpdate();

  //Populate Packages
  for( size_t i = 0; i<UPackage::GetLoadedPackages()->Size(); i++ )
  {
    UPackage* currentPackage;

    if( m_CheckFilterPackage->GetValue() )
    {
      currentPackage = m_PackageCtrl->GetSelectedPackage();

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
    {
      if( currentPackageItem.IsOk() ) //Should return false if package has no qualifying items.
        m_Ctrl->Expand( currentPackageItem );
      break;
    }
  } //End Package

}

void EdObjectBrowser::EventObjectMenu( wxTreeListEvent& event )
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

  EdEditor::UObjectContextMenu* objectmenu = new EdEditor::UObjectContextMenu( this, objects );
}

wxBEGIN_EVENT_TABLE( EdObjectBrowser, EdBrowser )
  EVT_TREELIST_ITEM_CONTEXT_MENU( EdToolFrame::ID_Ctrl, EdObjectBrowser::EventObjectMenu )
wxEND_EVENT_TABLE()