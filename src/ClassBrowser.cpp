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
 * ClassBrowser.cpp - Class browser implementation
 *
 * written by Jesse 'Hyzoran' Kowalik & Adam 'Xaleros' Smith
 *========================================================================
*/

#include "EditorMain.h"
#include "ClassBrowser.h"

EdClassBrowser::EdClassBrowser( bool bDock, wxSize Size )
  : EdBrowser( UClass::StaticClass(), bDock, Size, false )
{
  SetLabel( wxString( "Class Browser" ) );
  SetIcon( EdEditorFrame::sm_icoClass );

  m_subDirType = (wxString*)&EdEditorFrame::csm_SubDir_U;

  m_CheckShowPackage = new wxCheckBox( m_OptionsBar, ID_BrowserShowPackage, "Show Package" );
  m_OptionsSizer->Add( m_CheckShowPackage, 0, wxALIGN_LEFT );

  ListConstruct();
}

EdClassBrowser::~EdClassBrowser()
{
}

wxTreeItemId EdClassBrowser::AddTreeItem( wxTreeItemId Parent, UClass* Obj )
{
  if ( m_CheckShowPackage->GetValue() )
  {
    return m_ListView->AppendItem( Parent, wxString( Obj->Name.Data() )
      + wxString( " (" ) + wxString( Obj->Pkg->Name.Data() ) + wxString( ") " ),
      -1, -1, new wxObjectItemData( Obj ) );
  }
  else
  {
    return m_ListView->AppendItem( Parent, Obj->Name.Data(), -1, -1,
      new wxObjectItemData( Obj ) );
  }
}

void EdClassBrowser::RecurseExpandPopulate( TArray<UClass*>& ExpandedAry, wxTreeItemId Parent, wxTreeItemIdValue Cookie )
{
  wxTreeItemId id;

  if ( !Cookie )
    id = m_ListView->GetFirstChild( Parent, Cookie );
  else
    id = m_ListView->GetNextChild( Parent, Cookie );

  if ( !id.IsOk() )
    return;

  if ( m_ListView->IsExpanded( id ) )
    ExpandedAry.PushBack( (UClass*)((wxObjectItemData*)m_ListView->GetItemData( id ))->m_Obj );

  if ( m_ListView->ItemHasChildren( id ) )
    RecurseExpandPopulate( ExpandedAry, id );

  RecurseExpandPopulate( ExpandedAry, Parent, Cookie );
}

void EdClassBrowser::Update()
{
  TArray<UClass*> expandedObjects; //Previously Expanded UObjects.
  TArray<wxTreeItemId> toExpand;

  if ( m_ListView->GetCount() > 0 )
    RecurseExpandPopulate( expandedObjects, m_ListView->GetRootItem() );

  //Clear previous state.
  m_ListView->DeleteAllItems();

  wxTreeItemId root = m_ListView->AddRoot( "" ); //Dummy root

  //We track previous populated parent classes so that new classes be subbed below them.
  TArray<wxTreeItemId> parents;
  TArray<wxTreeItemId> newParents;

  //Add Object class.
  parents.PushBack( AddTreeItem( root, UObject::StaticClass() ) );

  wxTreeItemId object = parents[0];

  bool bBuildTree = true;

  while ( bBuildTree )
  {
    bBuildTree = false;

    //For each Object in the classpool, we compare it against each parent, if true, then 
    //nest it beneath the parent and add it to the list of new parents.
    //If we ever find a class to add, we tell the loop to proceed again.
    TArray<UClass*>* GClassPool = UObject::GetGlobalClassPool();
    for ( size_t i = 0; i < GClassPool->Size(); i++ )
    {
      UClass* currentClass = (*GClassPool)[i];
      currentClass = currentClass;

      for ( size_t j = 0; j < parents.Size(); j++ )
      {
        if ( currentClass->SuperClass == ((wxObjectItemData*)m_ListView->GetItemData( parents[j] ))->m_Obj )
        {
          bBuildTree = true;
          newParents.PushBack( AddTreeItem( parents[j], currentClass ) );

          //Check if added Object was expanded before
          for ( size_t k = 0; k < expandedObjects.Size(); k++ )
          {
            if ( expandedObjects[k] == currentClass )
              toExpand.PushBack( newParents.Back() );
          }

          break;
        }
      }
    }

    //Sort Alphanumeric
    for ( size_t j = 0; j < parents.Size(); j++ )
      m_ListView->SortChildren( parents[j] );

    parents.Clear();
    parents = newParents;
    newParents.Clear();
  }

  m_ListView->Expand( object );

  //Remember Expanded
  for ( size_t i = 0; i < toExpand.Size(); i++ )
    m_ListView->Expand( toExpand[i] );
}

void EdClassBrowser::OnClassTree( wxCommandEvent& event )
{
  event.Skip();
}

void EdClassBrowser::OnBrowserShowPackage( wxCommandEvent& event )
{
  Update();
  event.Skip();
}

wxBEGIN_EVENT_TABLE( EdClassBrowser, EdBrowser )
  EVT_CHECKBOX( ID_BrowserShowPackage, EdClassBrowser::OnBrowserShowPackage )
wxEND_EVENT_TABLE()