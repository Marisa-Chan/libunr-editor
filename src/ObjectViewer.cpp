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
 * ObjectViewer.cpp - Object Properties Tool, view and edit Objects.
 *
 * written by Jesse 'Hyzoran' Kowalik
 *========================================================================
*/

#include "ObjectViewer.h"

#define FIELDSIZE 32 //Height of property fields

wxEdPropertyWindow::wxEdPropertyWindow( UObject* Property, wxEdPropertyWindow* parent )
  : wxControl( parent, wxID_ANY, wxDefaultPosition, wxSize( -1, FIELDSIZE ) ), m_Property( Property )
{
  parent->AddChild( this );
}

wxEdPropertyWindow::wxEdPropertyWindow( UObject* Property, wxEdPropertyViewCtrl* parent )
  : wxControl( parent, wxID_ANY, wxDefaultPosition, wxSize( -1, FIELDSIZE ) ), m_Property( Property )
{

}

void wxEdPropertyWindow::PropertyUpdate( bool bReadOnly )
{

}

bool wxEdPropertyWindow::AddChild( wxEdPropertyWindow* Child )
{
  return true;
}

wxEdPropertyViewCtrl::wxEdPropertyViewCtrl( UObject* Obj, wxWindow* parent, bool bReadOnly, wxWindowID id, const wxPoint& pos, const wxSize& size, long style )
  : wxDataViewCtrl( parent, id, pos, size, style ), m_bReadOnly( bReadOnly ), m_Obj( Obj )
{
  TreeUpdate();
}

void wxEdPropertyViewCtrl::TreeUpdate( wxEdPropertyWindow* ToUpdate )
{

}

void wxEdPropertyViewCtrl::SetReadOnly( bool Value )
{
  m_bReadOnly = Value;
}

EdObjectViewer::EdObjectViewer( UObject* Obj, bool bStartDocked, wxSize Size )
  : EdToolFrame( bStartDocked, Size ), m_Obj( Obj )
{
  m_MainFrame = new wxSplitterWindow( this );
  m_PropertyCtrl = new wxEdPropertyViewCtrl( Obj, m_MainFrame );
  m_MainFrame->Initialize( m_PropertyCtrl );

  wxString pkg = "";
  if ( Obj->Pkg != NULL )
    pkg = wxString( Obj->Pkg->Name.Data() ) + wxString( "." );

  wxString cls = "";
  if ( Obj->Class != NULL )
    cls = wxString( " <" ) + wxString( Obj->Class->Name.Data() ) + wxString( ">" );
  else //if( Obj->StaticClass() == UPackage::StaticClass() )
    cls = wxString( " <Package>" );

  SetLabel( wxString( "Properties: " ) + pkg + wxString( Obj->Name.Data() ) + cls );

  Show( true );
}

void EdObjectViewer::PackagesAdded( size_t PackageStartIndex )
{
}

void EdObjectViewer::PackagesRemoved()
{
}

void EdObjectViewer::ObjectsAdded()
{
}

void EdObjectViewer::ObjectsRemoved()
{
}