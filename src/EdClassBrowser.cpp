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
* EdClassBrowser.cpp
*
* written by Jesse 'Hyzoran' Kowalik
*========================================================================
*/

#include "EdEditor.h"
#include "EdBrowsers.h"

#define CTRLFONT() SetFont( wxFont(8, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false ) )

EdClassBrowser::EdClassBrowser( wxWindow* Parent, UClass* Root, bool bStartDocked ) 
  : EdBrowser( Parent ), m_Root( Root )
{
  //We do not use the package ctrl.
  m_HeaderSizer->Detach( m_PackageCtrl );
  m_HeaderSizer->Detach( m_CheckFilterPackage );
  m_PackageCtrl->Destroy();
  m_CheckFilterPackage->Destroy();
  m_PackageCtrl = NULL;
  m_CheckFilterPackage = NULL;

  m_Ctrl = new wxTreeListCtrl( this, EdToolFrame::ID_Ctrl, wxDefaultPosition, wxDefaultSize, wxTL_MULTIPLE );

    m_VSizer->Add( m_Ctrl, 1, wxEXPAND );

  //Columns: |Class|Package|Dirty|

  m_Ctrl->AppendColumn( wxString("Class"), 256, wxALIGN_LEFT, wxCOL_RESIZABLE | wxCOL_SORTABLE );
  m_Ctrl->AppendColumn( wxString("Package"), wxCOL_WIDTH_AUTOSIZE, wxALIGN_LEFT, wxCOL_RESIZABLE );

  m_Ctrl->CTRLFONT();

  SetLabel( wxString( "Class Browser [" ) + wxString( Root->Name.Data() ) + wxString( "]" ) );

  Show();

  ObjectUpdate();
}

EdClassBrowser::~EdClassBrowser()
{
  delete m_Ctrl;
}

void EdClassBrowser::ObjectUpdate( bool bUpdatePackageList )
{
  //First, preserve any expanded members, so that they will be expanded when the control is re-populated.
  TArray<UClass*> expanded;

  for ( wxTreeListItem item = m_Ctrl->GetFirstItem(); item.IsOk(); item = m_Ctrl->GetNextItem( item ) )
  {
    UClass* currentClass = (UClass*)( ((EdEditor::UObjectClientData*)( m_Ctrl->GetItemData( item ) ))->GetObject() );

    if( ( currentClass != NULL ) && ( m_Ctrl->IsExpanded( item ) ) )
    {
      expanded.PushBack( currentClass );
    }
  }

  //Clear tree for update
  //TODO: Optimize by tracking what objects have been removed or addded instead of regenerating entire tree.
  m_Ctrl->DeleteAllItems();

  //Add the root Class

  wxTreeListItem rootItem = m_Ctrl->AppendItem( m_Ctrl->GetRootItem(), m_Root->Name.Data(), NULL, NULL, new EdEditor::UObjectClientData( m_Root ) );

  m_Ctrl->SetItemText( rootItem, 1, m_Root->Pkg->Name.Data() );

  //Populate entire class tree beneath this root class
  recursePopulate( m_Root, rootItem, expanded );

  //Sort
  m_Ctrl->SetSortColumn( 0, true );

  //Expand Root Item
  m_Ctrl->Expand( rootItem );
}

//TODO: This code will get exponentially slower the bigger the class pool is, optimize in the future if possible.
void EdClassBrowser::recursePopulate( UClass* Parent, wxTreeListItem ParentItem, TArray<UClass*>& ExpandedAry )
{
  TArray<UClass*>* Classes = UObject::GetGlobalClassPool();

  //Iterate through class pool
  for( size_t i = 0; i<Classes->Size(); i++ )
  {
    UClass* currentClass = (*Classes)[i];

    if( currentClass == NULL ) //Invalid class
      continue;

    //If child of parent class
    if( currentClass->SuperClass == Parent )
    {
      //Add this class to tree
      wxTreeListItem item = m_Ctrl->AppendItem( ParentItem, currentClass->Name.Data(), NULL, NULL, new EdEditor::UObjectClientData( currentClass ) );

      //Package column
      if( currentClass->Pkg != NULL )
        m_Ctrl->SetItemText( item, 1, currentClass->Pkg->Name.Data() );

      //Continue to children
      recursePopulate( currentClass, item, ExpandedAry );

      //Check if this item was expanded before and preserve its state.
      for( size_t i2 = 0; i2<ExpandedAry.Size(); i2++ )
      {
        if( ExpandedAry[i2] == currentClass )
        {
          m_Ctrl->Expand( item );
          break;
        }
      }
    }
  }
}

void EdClassBrowser::EVT_ObjectMenu( wxTreeListEvent& event )
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

wxBEGIN_EVENT_TABLE( EdClassBrowser, EdBrowser )
  EVT_TREELIST_ITEM_CONTEXT_MENU( EdToolFrame::ID_Ctrl, EdClassBrowser::EVT_ObjectMenu)
wxEND_EVENT_TABLE()