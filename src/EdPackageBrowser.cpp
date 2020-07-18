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
* EdPackageBrowser.cpp
*
* written by Jesse 'Hyzoran' Kowalik
*========================================================================
*/

#include "EdEditor.h"
#include "EdBrowsers.h"

//------------------------------------------------------------------------
// PackageBrowser
//------------------------------------------------------------------------
EdPackageBrowser::EdPackageBrowser( wxWindow* Parent, UPackage* Package, bool bStartDocked )
  : EdBrowser( Parent )
{
  m_HeaderSizer->Detach( m_CheckFilterPackage );
  m_CheckFilterPackage->Destroy();
  m_CheckFilterPackage = NULL;

  m_MainView = new wxSplitterWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize );
    m_VSizer->Add( m_MainView, 1, wxEXPAND );
    m_Notebook = new wxNotebook( m_MainView, EdToolFrame::ID_Ctrl, wxDefaultPosition, wxDefaultSize, wxNB_TOP );
    m_ViewWindow = new EdEditor::UObjectPreviewWindow( m_MainView );

  //m_MainView->SplitVertically( m_Notebook, m_ViewWindow );
  m_MainView->Initialize( m_Notebook );

  //Construct Package tabs
  m_PackageHeader = new EdPackageHeader( m_Notebook );

  m_Notebook->InsertPage( 0, m_PackageHeader, wxString( "Header" ), true );

  m_NameTable = new EdNameTable( m_Notebook );
  m_Notebook->InsertPage( 1, m_NameTable, wxString( "Name Table" ), false );

  m_ExportTable = new EdExportTable( m_Notebook );
  m_Notebook->InsertPage( 2, m_ExportTable, wxString( "Export Table" ), false );

  m_ExportTree = new EdExportTree( m_Notebook );
  m_Notebook->InsertPage( 3, m_ExportTree, wxString( "Export Tree" ), false );

  m_ImportTable = new EdImportTable( m_Notebook );
  m_Notebook->InsertPage( 4, m_ImportTable, wxString( "Import Table" ), false );

  m_ImportTree = new EdImportTree( m_Notebook );
  m_Notebook->InsertPage( 5, m_ImportTree, wxString( "Import Tree" ), false );

  ObjectUpdate();

  m_MainView->Show();
  m_Notebook->Show();
  Show();

  SetLabel( wxString( "Pkg Browser" ) );
}
//TODO: Multithreaded implementation
void EdPackageBrowser::ObjectUpdate( bool bUpdatePackageList )
{
  if( bUpdatePackageList )
    m_PackageCtrl->PackageListUpdate();

  UPackage* currentPackage = m_PackageCtrl->GetSelectedPackage();

  m_PackageHeader->Update( currentPackage );
  m_NameTable->Update( currentPackage );
  m_ExportTable->Update( currentPackage );
  m_ExportTree->Update( currentPackage );
  m_ImportTable->Update( currentPackage );
  m_ImportTree->Update( currentPackage );
}

wxBEGIN_EVENT_TABLE( EdPackageBrowser, EdBrowser )
  EVT_COMBOBOX( EdToolFrame::ID_PackageCtrl, EdBrowser::EventFilterPackage )
wxEND_EVENT_TABLE()

//------------------------------------------------------------------------
// EdPackageHeader
//------------------------------------------------------------------------
EdPackageHeader::EdPackageHeader( wxWindow* Parent )
  : wxWindow( Parent, wxID_ANY )
{
  m_VSizer = new wxBoxSizer( wxVERTICAL );

  wxStaticText* text = new wxStaticText( this, wxID_ANY, "Pkg Flags" );
  m_PackageInfo = new wxListCtrl( this, wxID_ANY, wxDefaultPosition, wxSize( -1, 232), wxLC_REPORT | wxLC_HRULES | wxLC_VRULES | wxLC_NO_HEADER , wxDefaultValidator, wxString( "Pkg Info" ) );
  m_PackageInfo->AppendColumn( "Flag", wxLIST_FORMAT_LEFT, 256 );
  m_PackageInfo->AppendColumn( "Value", wxLIST_FORMAT_LEFT, 256 );

  m_VSizer->Add( text, 0 );
  m_VSizer->Add( m_PackageInfo, 1, wxEXPAND );

  text = new wxStaticText( this, wxID_ANY, "Pkg Flags" );
  m_VSizer->Add( text, 0 );

  m_PackageFlags = new wxListCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxLC_HRULES | wxLC_VRULES | wxLC_NO_HEADER , wxDefaultValidator, wxString( "Pkg Flags" ) );
  m_PackageFlags->AppendColumn( "Flag", wxLIST_FORMAT_LEFT, 256 );
  m_PackageFlags->AppendColumn( "Value", wxLIST_FORMAT_LEFT, 256 );

  m_VSizer->Add( m_PackageFlags, 1, wxEXPAND );

  SetSizer( m_VSizer );

  Show();
}

void EdPackageHeader::Update( UPackage* Pkg )
{
  m_PackageInfo->DeleteAllItems();
  m_PackageFlags->DeleteAllItems();

  //Info

  if( Pkg == NULL )
    return;

  if( Pkg->GetStream() == NULL )
  {
    m_PackageInfo->InsertItem( 0, "Pkg Version" );
    m_PackageInfo->SetItem( 0, 1, wxString("NATIVE PACKAGE") );

    return;
  }

  m_PackageInfo->InsertItem( 0, "Pkg Version" );
  m_PackageInfo->InsertItem( 1, "File Size (bytes)" );
  m_PackageInfo->InsertItem( 2, "License Mode" );
  m_PackageInfo->InsertItem( 3, "Name Count" );
  m_PackageInfo->InsertItem( 4, "Name Offset" );
  m_PackageInfo->InsertItem( 5, "Export Count" );
  m_PackageInfo->InsertItem( 6, "Export Offset" );
  m_PackageInfo->InsertItem( 7, "Import Count" );
  m_PackageInfo->InsertItem( 8, "Import Offset" );
  m_PackageInfo->InsertItem( 9, "Heritage Count" );
  m_PackageInfo->InsertItem( 10, "Heritage Offset" );
  m_PackageInfo->InsertItem( 11, "GUID" );

  m_PackageInfo->SetItem( 0, 1, wxString("") << Pkg->GetHeader()->PackageVersion );

  //Get Package filesize, preserving original seek value just in case it was being used by something.

  size_t where = Pkg->GetStream()->Tell();
  Pkg->GetStream()->Seek( 0, End );

  m_PackageInfo->SetItem( 1, 1, wxString( "" ) << Pkg->GetStream()->Tell() );

  Pkg->GetStream()->Seek( where, Begin );

  m_PackageInfo->SetItem( 2, 1, wxString( "" ) << Pkg->GetHeader()->LicenseMode );
  m_PackageInfo->SetItem( 3, 1, wxString( "" ) << Pkg->GetHeader()->NameCount );
  m_PackageInfo->SetItem( 4, 1, wxString( "" ) << Pkg->GetHeader()->NameOffset );
  m_PackageInfo->SetItem( 5, 1, wxString( "" ) << Pkg->GetHeader()->ExportCount );
  m_PackageInfo->SetItem( 6, 1, wxString( "" ) << Pkg->GetHeader()->ExportOffset );
  m_PackageInfo->SetItem( 7, 1, wxString( "" ) << Pkg->GetHeader()->ImportCount );
  m_PackageInfo->SetItem( 8, 1, wxString( "" ) << Pkg->GetHeader()->ImportOffset );
  m_PackageInfo->SetItem( 9, 1, wxString( "" ) << Pkg->GetHeader()->HeritageCount );
  m_PackageInfo->SetItem( 10, 1, wxString( "" ) << Pkg->GetHeader()->HeritageOffset );

  //Convert GUID byte array to hex characters, with hyphens, like utpt does.
  wxString GUID;
  char const hex_chars[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

  GUID += "{ ";

  for( size_t i = 0; i < 16; i++ )
  {
    GUID += hex_chars[ ( Pkg->GetHeader()->GUID[i] & 0xF0 ) >> 4];
    GUID += hex_chars[ ( Pkg->GetHeader()->GUID[i] & 0x0F ) >> 0];

    if ( i == 4 )
      GUID += '-';
    if ( i == 6 )
      GUID += '-';
    if ( i == 8 )
      GUID += '-';
    if ( i == 10 )
      GUID += '-';
  }

  GUID += " }";

  m_PackageInfo->SetItem( 11, 1, GUID );

  //Flags
  m_PackageFlags->InsertItem( 0, "Flags" );
  m_PackageFlags->InsertItem( 1, "PKG_AllowDownload");
  m_PackageFlags->InsertItem( 2, "PKG_ClientOptional");
  m_PackageFlags->InsertItem( 3, "PKG_ServerSideOnly");
  m_PackageFlags->InsertItem( 4, "PKG_BrokenLinks");
  m_PackageFlags->InsertItem( 5, "PKG_Unsecure");
  m_PackageFlags->InsertItem( 6, "PKG_Need");

  u32 flags = Pkg->GetHeader()->PackageFlags;

  wxString flagHex;

  flagHex += hex_chars[ ( flags & 0xF0000000 ) >> 28 ];
  flagHex += hex_chars[ ( flags & 0x0F000000 ) >> 24 ];
  flagHex += hex_chars[ ( flags & 0x00F00000 ) >> 20 ];
  flagHex += hex_chars[ ( flags & 0x000F0000 ) >> 16 ];
  flagHex += hex_chars[ ( flags & 0x0000F000 ) >> 12 ];
  flagHex += hex_chars[ ( flags & 0x00000F00 ) >> 8 ];
  flagHex += hex_chars[ ( flags & 0x000000F0 ) >> 4 ];
  flagHex += hex_chars[ ( flags & 0x0000000F ) ];

  m_PackageFlags->SetItem( 0, 1, wxString( "0x" ) + flagHex );
 
  wxString flagValues[2];
  flagValues[0] = "false";
  flagValues[1] = "true";

  m_PackageFlags->SetItem( 1, 1, flagValues[ (bool)( flags & 0x00000001 ) ] );
  m_PackageFlags->SetItem( 2, 1, flagValues[ (bool)( flags & 0x00000002 ) ] );
  m_PackageFlags->SetItem( 3, 1, flagValues[ (bool)( flags & 0x00000004 ) ] );
  m_PackageFlags->SetItem( 4, 1, flagValues[ (bool)( flags & 0x00000008 ) ] );
  m_PackageFlags->SetItem( 5, 1, flagValues[ (bool)( flags & 0x00000010 ) ] );
  m_PackageFlags->SetItem( 6, 1, flagValues[ (bool)( flags & 0x00008000 ) ] );
}

EdGenerationsTable::EdGenerationsTable( wxWindow* Parent )
  : wxListCtrl( Parent, wxID_ANY )
{
}

void EdGenerationsTable::Update( UPackage* Pkg )
{
}

EdNameTable::EdNameTable( wxWindow* Parent )
  : wxListCtrl( Parent, wxID_ANY )
{
}

void EdNameTable::Update( UPackage* Pkg )
{
}

EdExportTable::EdExportTable( wxWindow* Parent )
  : wxListCtrl( Parent, wxID_ANY )
{
}

void EdExportTable::Update( UPackage* Pkg )
{
}

EdExportTree::EdExportTree( wxWindow* Parent )
  : wxTreeListCtrl( Parent, wxID_ANY )
{
}

void EdExportTree::Update( UPackage* Pkg )
{
}

EdImportTable::EdImportTable( wxWindow* Parent )
  : wxListCtrl( Parent, wxID_ANY )
{
}

void EdImportTable::Update( UPackage* Pkg )
{
}

EdImportTree::EdImportTree( wxWindow* Parent )
  : wxTreeListCtrl( Parent, wxID_ANY )
{
}

void EdImportTree::Update( UPackage* Pkg )
{
}
