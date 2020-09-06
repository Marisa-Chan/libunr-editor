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
  m_PackageInfo->AppendColumn( "Value", wxLIST_FORMAT_LEFT, 1024 );

  m_VSizer->Add( text, 0 );
  m_VSizer->Add( m_PackageInfo, 1, wxEXPAND );

  text = new wxStaticText( this, wxID_ANY, "Pkg Flags" );
  m_VSizer->Add( text, 0 );

  m_PackageFlags = new wxListCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxLC_HRULES | wxLC_VRULES | wxLC_NO_HEADER , wxDefaultValidator, wxString( "Pkg Flags" ) );
  m_PackageFlags->AppendColumn( "Flag", wxLIST_FORMAT_LEFT, 128 );
  m_PackageFlags->AppendColumn( "Value", wxLIST_FORMAT_LEFT, 1024 );

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
  m_PackageFlags->InsertItem( 0, "Flags (Hex)" );
  m_PackageFlags->InsertItem( 1, "Flags");

  u32 flags = Pkg->GetHeader()->PackageFlags;

  m_PackageFlags->SetItem( 0, 1, wxString::Format( "0x%08x", flags ) );
  m_PackageFlags->SetItem( 1, 1, UObject::PackageFlagsString( flags ).Data() );
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
  m_VSizer = new wxBoxSizer( wxVERTICAL );

  m_NameTable = new wxListCtrl( this, wxID_ANY, wxDefaultPosition, wxSize( -1, -1), wxLC_REPORT | wxLC_HRULES | wxLC_VRULES, wxDefaultValidator, wxString( "Pkg Info" ) );
  m_NameTable->AppendColumn( "Num.", wxLIST_FORMAT_LEFT, 96 );
  m_NameTable->AppendColumn( "Name", wxLIST_FORMAT_LEFT, 256 );
  m_NameTable->AppendColumn( "Flags", wxLIST_FORMAT_LEFT, 1024 );

  m_VSizer->Add( m_NameTable, 1, wxEXPAND );

  SetSizer( m_VSizer );

  Show();
}

void EdNameTable::Update( UPackage* Pkg )
{
  m_NameTable->DeleteAllItems();

  if( Pkg == NULL )
    return;

  if( Pkg->GetStream() == NULL )
  {
    m_NameTable->DeleteAllColumns();
    m_NameTable->AppendColumn( " ", wxLIST_FORMAT_LEFT, 256 );
    m_NameTable->InsertItem( 0, "NATIVE PACKAGE" );

    return;
  }

  TArray<FNameEntry>& names = Pkg->GetNameTable();

  for( int i = 0; i<names.Size(); i++ )
  {
    //Num
    wxString str = wxString::Format( wxT("%i"), i ) + wxString( " ( " ) + wxString::Format( "0x%x", i ) + wxString( " )" );
    m_NameTable->InsertItem( i, str );

    //Name
    str = names[i].Data;
    m_NameTable->SetItem( i, 1, str );

    //Flags
    str = UObject::ObjectFlagsString( names[i].Flags ).Data();
    m_NameTable->SetItem( i, 2, str );
  }
}

EdExportTable::EdExportTable( wxWindow* Parent )
  : wxListCtrl( Parent, wxID_ANY )
{
  m_VSizer = new wxBoxSizer( wxVERTICAL );

  m_ExportTable = new wxListCtrl( this, wxID_ANY, wxDefaultPosition, wxSize( -1, -1), wxLC_REPORT | wxLC_HRULES | wxLC_VRULES, wxDefaultValidator, wxString( "Pkg Info" ) );

  m_ExportTable->AppendColumn( "Num.", wxLIST_FORMAT_LEFT, 96 );
  m_ExportTable->AppendColumn( "Group", wxLIST_FORMAT_LEFT, 256 );
  m_ExportTable->AppendColumn( "Name", wxLIST_FORMAT_LEFT, 256 );
  m_ExportTable->AppendColumn( "Class", wxLIST_FORMAT_LEFT, 128 );
  m_ExportTable->AppendColumn( "Super", wxLIST_FORMAT_LEFT, 128 );
  m_ExportTable->AppendColumn( "Size", wxLIST_FORMAT_LEFT, 128 );
  m_ExportTable->AppendColumn( "Offset", wxLIST_FORMAT_LEFT, 128 );
  m_ExportTable->AppendColumn( "Flags", wxLIST_FORMAT_LEFT, 1024 );

  m_VSizer->Add( m_ExportTable, 1, wxEXPAND );

  SetSizer( m_VSizer );

  Show();
}

void EdExportTable::Update( UPackage* Pkg )
{
  m_ExportTable->DeleteAllItems();

  if( Pkg == NULL )
    return;

  if( Pkg->GetStream() == NULL )
  {
    m_ExportTable->DeleteAllColumns();
    m_ExportTable->AppendColumn( " ", wxLIST_FORMAT_LEFT );
    m_ExportTable->InsertItem( 0, "NATIVE PACKAGE" );

    return;
  }

  TArray<FExport>& exports = Pkg->GetExportTable();

  for( int i = 0; i<exports.Size(); i++ )
  {
    //Num
    wxString str = wxString::Format( "%i", exports[i].Index ) + wxString( " ( " ) + wxString::Format( "0x%x", i ) + wxString( " )" );
    m_ExportTable->InsertItem( i, str );

    //Group
    u32 grp = exports[i].Group;

    str = "";

    while( grp != 0 )
    {
      str += Pkg->ResolveNameFromObjRef( grp );
      str += ".";
      grp = exports[ CalcObjRefValue(grp) ].Group;
    }
    str += Pkg->ResolveNameFromObjRef( exports[i].Group );

    m_ExportTable->SetItem( i, 1, str );

    //Name
    str = Pkg->ResolveNameFromIdx( exports[i].ObjectName );
    m_ExportTable->SetItem( i, 2, str );

    //Class
    str = Pkg->ResolveNameFromObjRef( exports[i].Class );
    m_ExportTable->SetItem( i, 3, str );

    //Super
    str = Pkg->ResolveNameFromObjRef( exports[i].Super );
    m_ExportTable->SetItem( i, 4, str );

    //Size
    str = wxString::Format( "%i", exports[i].SerialSize );
    m_ExportTable->SetItem( i, 5, str );

    //Offset
    str = wxString::Format( "0x%08x", exports[i].SerialOffset );
    m_ExportTable->SetItem( i, 6, str );

    //Flags
    str = UObject::PackageFlagsString( exports[i].ObjectFlags ).Data();
    m_ExportTable->SetItem( i, 7, str );

  }
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
  m_VSizer = new wxBoxSizer( wxVERTICAL );

  m_ImportTable = new wxListCtrl( this, wxID_ANY, wxDefaultPosition, wxSize( -1, -1), wxLC_REPORT | wxLC_HRULES | wxLC_VRULES, wxDefaultValidator, wxString( "Pkg Info" ) );

  m_ImportTable->AppendColumn( "Num.", wxLIST_FORMAT_LEFT, 96 );
  m_ImportTable->AppendColumn( "Package/Group", wxLIST_FORMAT_LEFT, 256 );
  m_ImportTable->AppendColumn( "Name", wxLIST_FORMAT_LEFT, 256 );
  m_ImportTable->AppendColumn( "Class", wxLIST_FORMAT_LEFT, 128 );
  m_ImportTable->AppendColumn( "Class Package", wxLIST_FORMAT_LEFT, 128);

  m_VSizer->Add( m_ImportTable, 1, wxEXPAND );

  SetSizer( m_VSizer );

  Show();
}

void EdImportTable::Update( UPackage* Pkg )
{
  m_ImportTable->DeleteAllItems();

  if( Pkg == NULL )
    return;

  if( Pkg->GetStream() == NULL )
  {
    m_ImportTable->DeleteAllColumns();
    m_ImportTable->AppendColumn( " ", wxLIST_FORMAT_LEFT );
    m_ImportTable->InsertItem( 0, "NATIVE PACKAGE" );

    return;
  }

  TArray<FImport>& imports = Pkg->GetImportTable();

  for( int i = 0; i<imports.Size(); i++ )
  {
    //Num
    wxString str = wxString::Format( "%i", imports[i].Index ) + wxString( " ( " ) + wxString::Format( "0x%x", i ) + wxString( " )" );
    m_ImportTable->InsertItem( i, str );

    FImport ass1 = imports[i];
    int ass = imports[i].Package;

    //Package/Group
    str = Pkg->ResolveNameFromIdx( imports[ CalcObjRefValue(imports[i].Package) ].ObjectName );
    m_ImportTable->SetItem( i, 1, str );

    //Name
    str = Pkg->ResolveNameFromIdx( imports[i].ObjectName );
    m_ImportTable->SetItem( i, 2, str );

    //Class
    str = Pkg->ResolveNameFromIdx( imports[i].ClassName );
    m_ImportTable->SetItem( i, 3, str );

    //Class Package
    str = Pkg->ResolveNameFromIdx( imports[i].ClassPackage );
    m_ImportTable->SetItem( i, 4, str );

  }
}

EdImportTree::EdImportTree( wxWindow* Parent )
  : wxTreeListCtrl( Parent, wxID_ANY )
{
}

void EdImportTree::Update( UPackage* Pkg )
{
}
