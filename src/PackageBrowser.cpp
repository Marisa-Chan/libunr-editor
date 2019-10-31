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
 * PackageBrowser.cpp - Package Browser code
 *
 * written by Jesse 'Hyzoran' Kowalik & Adam 'Xaleros' Smith
 *========================================================================
*/

#include "EditorMain.h"
#include "PackageBrowser.h"

EdPackageBrowser::EdPackageBrowser( bool bDock, wxSize Size )
  : EdBrowser( UPackage::StaticClass(), bDock, Size )
{
  SetLabel( wxString( "Package Browser" ) );
  SetIcon( EdEditorFrame::sm_icoPackage );

  m_PackagesList = new wxComboBox( m_OptionsBar, ID_PackageList, "", wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_SORT );
  m_OptionsSizer->Add( m_PackagesList, 0, wxALIGN_LEFT );

  m_MainSplitter = new wxSplitterWindow( this, wxID_ANY );
  m_WindowAreaSizer->Add( m_MainSplitter, 1, wxEXPAND );

  m_TabWindow = new wxNotebook( m_MainSplitter, ID_PackageTab, wxDefaultPosition, wxDefaultSize, wxNB_TOP | wxNB_FIXEDWIDTH );

  m_PackageHeader = new wxWindow( m_TabWindow, wxID_ANY );
  wxBoxSizer* headerSizer = new wxBoxSizer( wxVERTICAL );

  m_PackageInfo = m_PackageFlags = new wxListCtrl( m_PackageHeader, wxID_ANY, wxDefaultPosition, wxSize( -1, 256 ), wxLC_REPORT );
  m_PackageInfo->AppendColumn( "Item", wxLIST_FORMAT_LEFT, 200 );
  m_PackageInfo->AppendColumn( "Value", wxLIST_FORMAT_LEFT, 450 );
  headerSizer->Add( m_PackageInfo, 0, wxEXPAND | wxALIGN_LEFT );

  wxStaticLine* Line = new wxStaticLine( m_PackageHeader, wxID_ANY, wxDefaultPosition, wxSize( -1, 16 ) );

  headerSizer->Add( Line, 0, wxEXPAND | wxALIGN_LEFT );

  m_PackageFlags = new wxListCtrl( m_PackageHeader, wxID_ANY, wxDefaultPosition, wxSize( -1, 164 ), wxLC_REPORT );
  m_PackageFlags->AppendColumn( "Flag", wxLIST_FORMAT_LEFT, 200 );
  m_PackageFlags->AppendColumn( "Value", wxLIST_FORMAT_LEFT, 450 );
  headerSizer->Add( m_PackageFlags, 0, wxEXPAND | wxALIGN_LEFT );

  m_PackageHeader->SetSizer( headerSizer );

  m_NameTable = new wxListCtrl( m_TabWindow, wxID_ANY, wxDefaultPosition, wxSize( -1, -1 ), wxLC_REPORT );

  m_NameTable->AppendColumn( "#", wxLIST_FORMAT_LEFT, 100 );
  m_NameTable->AppendColumn( "Name", wxLIST_FORMAT_LEFT, 200 );
  m_NameTable->AppendColumn( "Flags", wxLIST_FORMAT_LEFT, 450 );

  m_ExportTable = new wxListCtrl( m_TabWindow, wxID_ANY, wxDefaultPosition, wxSize( -1, -1 ), wxLC_REPORT );

  m_ExportTable->AppendColumn( "#", wxLIST_FORMAT_LEFT, 100 );
  m_ExportTable->AppendColumn( "Group", wxLIST_FORMAT_LEFT, 200 );
  m_ExportTable->AppendColumn( "Name", wxLIST_FORMAT_LEFT, 200 );
  m_ExportTable->AppendColumn( "Class", wxLIST_FORMAT_LEFT, 150 );
  m_ExportTable->AppendColumn( "Super", wxLIST_FORMAT_LEFT, 150 );
  m_ExportTable->AppendColumn( "Size", wxLIST_FORMAT_LEFT, 50 );
  m_ExportTable->AppendColumn( "Offset", wxLIST_FORMAT_LEFT, 50 );
  m_ExportTable->AppendColumn( "Flags", wxLIST_FORMAT_LEFT, 450 );

  m_ExportTree = new wxDataViewCtrl( m_TabWindow, wxID_ANY, wxDefaultPosition,
    wxSize( -1, -1 ), wxDV_MULTIPLE | wxDV_ROW_LINES | wxDV_VERT_RULES | wxDV_HORIZ_RULES );

  m_ImportTable = new wxListCtrl( m_TabWindow, wxID_ANY,
    wxDefaultPosition, wxSize( -1, -1 ), wxLC_REPORT );

  m_ImportTable->AppendColumn( "#", wxLIST_FORMAT_LEFT, 100 );
  m_ImportTable->AppendColumn( "Name", wxLIST_FORMAT_LEFT, 200 );
  m_ImportTable->AppendColumn( "Class", wxLIST_FORMAT_LEFT, 300 );

  m_ImportTree = new wxDataViewCtrl( m_TabWindow, wxID_ANY, wxDefaultPosition,
    wxSize( -1, -1 ), wxDV_MULTIPLE | wxDV_ROW_LINES | wxDV_VERT_RULES | wxDV_HORIZ_RULES );

  m_TabWindow->AddPage( m_PackageHeader, "Package Header", true );
  m_TabWindow->AddPage( m_NameTable, "Name Table", false );
  m_TabWindow->AddPage( m_ExportTable, "Export Table", false );
  m_TabWindow->AddPage( m_ExportTree, "Export Tree", false );
  m_TabWindow->AddPage( m_ImportTable, "Import Table", false );
  m_TabWindow->AddPage( m_ImportTree, "Import Tree", false );

  m_ViewPane = new wxWindow( m_MainSplitter, wxID_ANY, wxDefaultPosition, wxDefaultSize,
    wxBORDER_RAISED );

  m_MainSplitter->SetSashGravity( 0.75 );
  m_MainSplitter->SplitVertically( m_TabWindow, m_ViewPane );

  FinishConstruct();
}

EdPackageBrowser::~EdPackageBrowser()
{
}

void EdPackageBrowser::PackagesAdded( size_t PackageStartIndex )
{
  if ( m_PackagesList != NULL )
  {
    for ( size_t i = PackageStartIndex; i < (*UPackage::GetLoadedPackages()).Size(); i++ )
    {
      m_PackagesList->Append( (*UPackage::GetLoadedPackages())[i]->Name.Data(),
        (*UPackage::GetLoadedPackages())[i] );
    }
  }
}

void EdPackageBrowser::PackagesRemoved()
{
  if ( m_PackagesList != NULL )
  {
    m_PackagesList->Clear();

    for ( size_t i = 0; i < (*UPackage::GetLoadedPackages()).Size(); i++ )
    {
      m_PackagesList->Append( (*UPackage::GetLoadedPackages())[i]->Name.Data(),
        (*UPackage::GetLoadedPackages())[i] );
    }
  }

  Update();
}

void EdPackageBrowser::ObjectsAdded()
{
  Update();
}

void EdPackageBrowser::ObjectsRemoved()
{
  Update();
}

void EdPackageBrowser::Update()
{
  //Package Header
  if ( m_SelectedPackage != NULL )
  {
    if ( m_SelectedPackage->GetStream() == NULL )
    {
      m_PackageInfo->DeleteAllItems();
      m_PackageFlags->DeleteAllItems();
      m_PackageInfo->InsertItem( 0, "This package is a native package only" );
      return;
    }

    m_PackageInfo->DeleteAllItems();

    m_PackageInfo->InsertItem( 0, "Package Version" );
    m_PackageInfo->InsertItem( 1, "File Size" );
    m_PackageInfo->InsertItem( 2, "Licensee Mode" );
    m_PackageInfo->InsertItem( 3, "Name Count" );
    m_PackageInfo->InsertItem( 4, "Name Offset" );
    m_PackageInfo->InsertItem( 5, "Export Count" );
    m_PackageInfo->InsertItem( 6, "Export Offset" );
    m_PackageInfo->InsertItem( 7, "Import Count" );
    m_PackageInfo->InsertItem( 8, "Import Offset" );
    m_PackageInfo->InsertItem( 9, "Heritage Count" );
    m_PackageInfo->InsertItem( 10, "Heritage Offset" );
    m_PackageInfo->InsertItem( 11, "GUID" );

    m_PackageInfo->SetItem( 0, 1,
      std::to_string( m_SelectedPackage->GetHeader()->PackageVersion ) );

    //Get Package filesize, preserving original seek value just in case it was being used by something.
    size_t where = m_SelectedPackage->GetStream()->Tell();
    m_SelectedPackage->GetStream()->Seek( 0, End );
    m_PackageInfo->SetItem( 1, 1,
      std::to_string( m_SelectedPackage->GetStream()->Tell() ) );
    m_SelectedPackage->GetStream()->Seek( where, Begin );

    m_PackageInfo->SetItem( 2, 1,
      std::to_string( m_SelectedPackage->GetHeader()->LicenseMode ) );
    m_PackageInfo->SetItem( 3, 1,
      std::to_string( m_SelectedPackage->GetHeader()->NameCount ) );
    m_PackageInfo->SetItem( 4, 1,
      std::to_string( m_SelectedPackage->GetHeader()->NameOffset ) );
    m_PackageInfo->SetItem( 5, 1,
      std::to_string( m_SelectedPackage->GetHeader()->ExportCount ) );
    m_PackageInfo->SetItem( 6, 1,
      std::to_string( m_SelectedPackage->GetHeader()->ExportOffset ) );
    m_PackageInfo->SetItem( 7, 1,
      std::to_string( m_SelectedPackage->GetHeader()->ImportCount ) );
    m_PackageInfo->SetItem( 8, 1,
      std::to_string( m_SelectedPackage->GetHeader()->ImportOffset ) );
    m_PackageInfo->SetItem( 9, 1,
      std::to_string( m_SelectedPackage->GetHeader()->HeritageCount ) );
    m_PackageInfo->SetItem( 10, 1,
      std::to_string( m_SelectedPackage->GetHeader()->HeritageOffset ) );

    wxString GUID;
    char const hex_chars[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

    GUID += '{';
    for ( size_t i = 0; i < 16; i++ )
    {
      GUID += hex_chars[(m_SelectedPackage->GetHeader()->GUID[i] & 0xF0) >> 4];
      GUID += hex_chars[(m_SelectedPackage->GetHeader()->GUID[i] & 0x0F) >> 0];

      if ( i == 4 )
        GUID += '-';
      if ( i == 6 )
        GUID += '-';
      if ( i == 8 )
        GUID += '-';
      if ( i == 10 )
        GUID += '-';
    }
    GUID += '}';

    m_PackageInfo->SetItem( 11, 1, GUID );

    m_PackageFlags->DeleteAllItems();

    m_PackageFlags->InsertItem( 0, "PKG_AllowDownload" );
    m_PackageFlags->InsertItem( 1, "PKG_ClientOptional" );
    m_PackageFlags->InsertItem( 2, "PKG_ServerSideOnly" );
    m_PackageFlags->InsertItem( 3, "PKG_BrokenLinks" );
    m_PackageFlags->InsertItem( 4, "PKG_Unsecure" );
    m_PackageFlags->InsertItem( 5, "PKG_Need" );

    if ( m_SelectedPackage->GetHeader()->PackageFlags & 0x0001 )
      m_PackageFlags->SetItem( 0, 1, "true" );
    else
      m_PackageFlags->SetItem( 0, 1, "false" );
    if ( m_SelectedPackage->GetHeader()->PackageFlags & 0x0002 )
      m_PackageFlags->SetItem( 1, 1, "true" );
    else
      m_PackageFlags->SetItem( 1, 1, "false" );
    if ( m_SelectedPackage->GetHeader()->PackageFlags & 0x0004 )
      m_PackageFlags->SetItem( 2, 1, "true" );
    else
      m_PackageFlags->SetItem( 2, 1, "false" );
    if ( m_SelectedPackage->GetHeader()->PackageFlags & 0x0008 )
      m_PackageFlags->SetItem( 3, 1, "true" );
    else
      m_PackageFlags->SetItem( 3, 1, "false" );
    if ( m_SelectedPackage->GetHeader()->PackageFlags & 0x0010 )
      m_PackageFlags->SetItem( 4, 1, "true" );
    else
      m_PackageFlags->SetItem( 4, 1, "false" );
    if ( m_SelectedPackage->GetHeader()->PackageFlags & 0x8000 )
      m_PackageFlags->SetItem( 5, 1, "true" );
    else
      m_PackageFlags->SetItem( 5, 1, "false" );

    //Name Table
    m_NameTable->DeleteAllItems();
    TArray<FNameEntry>& nameTable = m_SelectedPackage->GetNameTable();

    for ( size_t i = 0; i < nameTable.Size(); i++ )
    {
      wxString num = std::to_string( i );
      size_t byteSize;
      if ( i > 0xFFFFFF )
        byteSize = 4;
      else if ( i > 0xFFFF )
        byteSize = 3;
      else if ( i > 0xFF )
        byteSize = 2;
      else
        byteSize = 1;
      num += wxString( " (0x" );

      for ( size_t j = 1; j <= byteSize; j++ )
      {
        num += wxString( hex_chars[((i >> (8 * (byteSize - j))) & 0xF0) >> 4] );
        num += wxString( hex_chars[(i >> (8 * (byteSize - j))) & 0x0F] );
      }
      num += wxString( ")" );

      m_NameTable->InsertItem( i, num );
      m_NameTable->SetItem( i, 1, nameTable[i].Data );

      wxString flags;
      flags.Empty();

      if ( nameTable[i].Flags & 0x00000001 )
        flags += "RF_Transactional,";
      if ( nameTable[i].Flags & 0x00000002 )
        flags += "RF_Unreachable,";
      if ( nameTable[i].Flags & 0x00000004 )
        flags += "RF_Public,";
      if ( nameTable[i].Flags & 0x00000008 )
        flags += "RF_TagImp,";
      if ( nameTable[i].Flags & 0x00000010 )
        flags += "RF_TagExp,";
      if ( nameTable[i].Flags & 0x00000020 )
        flags += "RF_SourceModified,";
      if ( nameTable[i].Flags & 0x00000040 )
        flags += "RF_TagGarbage,";
      if ( nameTable[i].Flags & 0x00000200 )
        flags += "RF_NeedLoad,";
      if ( nameTable[i].Flags & 0x00000400 )
        flags += "RF_HighlightedName,";
      if ( nameTable[i].Flags & 0x00000800 )
        flags += "RF_InSingularFunc,";
      if ( nameTable[i].Flags & 0x00001000 )
        flags += "RF_Suppress,";
      if ( nameTable[i].Flags & 0x00002000 )
        flags += "RF_InEndState,";
      if ( nameTable[i].Flags & 0x00004000 )
        flags += "RF_Transient,";
      if ( nameTable[i].Flags & 0x00008000 )
        flags += "RF_PreLoading,";
      if ( nameTable[i].Flags & 0x00010000 )
        flags += "RF_LoadForClient,";
      if ( nameTable[i].Flags & 0x00020000 )
        flags += "RF_LoadForServer,";
      if ( nameTable[i].Flags & 0x00040000 )
        flags += "RF_LoadForEdit,";
      if ( nameTable[i].Flags & 0x00080000 )
        flags += "RF_Standalone,";
      if ( nameTable[i].Flags & 0x00100000 )
        flags += "RF_NotForClient,";
      if ( nameTable[i].Flags & 0x00200000 )
        flags += "RF_NotForServer,";
      if ( nameTable[i].Flags & 0x00400000 )
        flags += "RF_NotForEdit,";
      if ( nameTable[i].Flags & 0x00800000 )
        flags += "RF_Destroyed,";
      if ( nameTable[i].Flags & 0x01000000 )
        flags += "RF_NeedPostLoad,";
      if ( nameTable[i].Flags & 0x02000000 )
        flags += "RF_HasStack,";
      if ( nameTable[i].Flags & 0x04000000 )
        flags += "RF_Native,";
      if ( nameTable[i].Flags & 0x08000000 )
        flags += "RF_Marked,";
      if ( nameTable[i].Flags & 0x10000000 )
        flags += "RF_ErrorShutdown,";
      if ( nameTable[i].Flags & 0x20000000 )
        flags += "RF_DebugPostLoad,";
      if ( nameTable[i].Flags & 0x40000000 )
        flags += "RF_DebugSerialize,";
      if ( nameTable[i].Flags & 0x80000000 )
        flags += "RF_DebugDestroy,";

      flags.RemoveLast(); //Remove last comma.
      m_NameTable->SetItem( i, 2, flags );
    }

    //Export Table/Tree
    m_ExportTable->DeleteAllItems();
    TArray<FExport>& exportTable = m_SelectedPackage->GetExportTable();

    for ( size_t i = 0; i < exportTable.Size(); i++ )
    {
      size_t index = exportTable[i].Index;

      wxString num = std::to_string( index );

      size_t byteSize;
      if ( index > 0xFFFFFF )
        byteSize = 4;
      else if ( index > 0xFFFF )
        byteSize = 3;
      else if ( index > 0xFF )
        byteSize = 2;
      else
        byteSize = 1;

      num += wxString( " (0x" );

      for ( size_t j = 1; j <= byteSize; j++ )
      {
        num += wxString( hex_chars[((index >> (8 * (byteSize - j))) & 0xF0) >> 4] );
        num += wxString( hex_chars[(index >> (8 * (byteSize - j))) & 0x0F] );
      }
      num += wxString( ")" );

      //Table
      m_ExportTable->InsertItem( i, num );
      m_ExportTable->SetItem( i, 1,
        m_SelectedPackage->GetNameEntryByObjRef( exportTable[i].Group )->Data );
      m_ExportTable->SetItem( i, 2,
        m_SelectedPackage->GetNameEntry( exportTable[i].ObjectName )->Data );
      m_ExportTable->SetItem( i, 3,
        m_SelectedPackage->GetNameEntryByObjRef( exportTable[i].Class )->Data );
      m_ExportTable->SetItem( i, 4,
        m_SelectedPackage->GetNameEntryByObjRef( exportTable[i].Super )->Data );
      m_ExportTable->SetItem( i, 5, std::to_string( CINDEX( exportTable[i].SerialSize ).Value ) );
      m_ExportTable->SetItem( i, 6, std::to_string( CINDEX( exportTable[i].SerialOffset ).Value ) );

      wxString flags;
      flags.Empty();

      if ( exportTable[i].ObjectFlags & 0x00000001 )
        flags += "RF_Transactional,";
      if ( exportTable[i].ObjectFlags & 0x00000002 )
        flags += "RF_Unreachable,";
      if ( exportTable[i].ObjectFlags & 0x00000004 )
        flags += "RF_Public,";
      if ( exportTable[i].ObjectFlags & 0x00000008 )
        flags += "RF_TagImp,";
      if ( exportTable[i].ObjectFlags & 0x00000010 )
        flags += "RF_TagExp,";
      if ( exportTable[i].ObjectFlags & 0x00000020 )
        flags += "RF_SourceModified,";
      if ( exportTable[i].ObjectFlags & 0x00000040 )
        flags += "RF_TagGarbage,";
      if ( exportTable[i].ObjectFlags & 0x00000200 )
        flags += "RF_NeedLoad,";
      if ( exportTable[i].ObjectFlags & 0x00000400 )
        flags += "RF_HighlightedName,";
      if ( exportTable[i].ObjectFlags & 0x00000800 )
        flags += "RF_InSingularFunc,";
      if ( exportTable[i].ObjectFlags & 0x00001000 )
        flags += "RF_Suppress,";
      if ( exportTable[i].ObjectFlags & 0x00002000 )
        flags += "RF_InEndState,";
      if ( exportTable[i].ObjectFlags & 0x00004000 )
        flags += "RF_Transient,";
      if ( exportTable[i].ObjectFlags & 0x00008000 )
        flags += "RF_PreLoading,";
      if ( exportTable[i].ObjectFlags & 0x00010000 )
        flags += "RF_LoadForClient,";
      if ( exportTable[i].ObjectFlags & 0x00020000 )
        flags += "RF_LoadForServer,";
      if ( exportTable[i].ObjectFlags & 0x00040000 )
        flags += "RF_LoadForEdit,";
      if ( exportTable[i].ObjectFlags & 0x00080000 )
        flags += "RF_Standalone,";
      if ( exportTable[i].ObjectFlags & 0x00100000 )
        flags += "RF_NotForClient,";
      if ( exportTable[i].ObjectFlags & 0x00200000 )
        flags += "RF_NotForServer,";
      if ( exportTable[i].ObjectFlags & 0x00400000 )
        flags += "RF_NotForEdit,";
      if ( exportTable[i].ObjectFlags & 0x00800000 )
        flags += "RF_Destroyed,";
      if ( exportTable[i].ObjectFlags & 0x01000000 )
        flags += "RF_NeedPostLoad,";
      if ( exportTable[i].ObjectFlags & 0x02000000 )
        flags += "RF_HasStack,";
      if ( exportTable[i].ObjectFlags & 0x04000000 )
        flags += "RF_Native,";
      if ( exportTable[i].ObjectFlags & 0x08000000 )
        flags += "RF_Marked,";
      if ( exportTable[i].ObjectFlags & 0x10000000 )
        flags += "RF_ErrorShutdown,";
      if ( exportTable[i].ObjectFlags & 0x20000000 )
        flags += "RF_DebugPostLoad,";
      if ( exportTable[i].ObjectFlags & 0x40000000 )
        flags += "RF_DebugSerialize,";
      if ( exportTable[i].ObjectFlags & 0x80000000 )
        flags += "RF_DebugDestroy,";

      flags.RemoveLast(); //Remove last comma.
      m_ExportTable->SetItem( i, 7, flags );

      //Tree
    }

    //Import Table
    m_ImportTable->DeleteAllItems();
    TArray<FImport>& importTable = m_SelectedPackage->GetImportTable();

    for ( size_t i = 0; i < importTable.Size(); i++ )
    {
      wxString num = std::to_string( i );

      size_t byteSize;
      if ( i > 0xFFFFFF )
        byteSize = 4;
      else if ( i > 0xFFFF )
        byteSize = 3;
      else if ( i > 0xFF )
        byteSize = 2;
      else
        byteSize = 1;

      num += wxString( " (0x" );

      for ( size_t j = 1; j <= byteSize; j++ )
      {
        num += wxString( hex_chars[((i >> (8 * (byteSize - j))) & 0xF0) >> 4] );
        num += wxString( hex_chars[(i >> (8 * (byteSize - j))) & 0x0F] );
      }
      num += wxString( ")" );

      m_ImportTable->InsertItem( i, num );
      m_ImportTable->SetItem( i, 1,
        m_SelectedPackage->ResolveNameFromIdx( importTable[i].ObjectName ) );
      m_ImportTable->SetItem( i, 2,
        wxString( m_SelectedPackage->ResolveNameFromIdx( importTable[i].ClassPackage ) ) +
        wxString( "." ) +
        wxString( m_SelectedPackage->ResolveNameFromIdx( importTable[i].ClassName ) ) );
    }
  }
  
  // Is this actually needed?
  // EdBrowser::Update();
}

void EdPackageBrowser::OnPackageList( wxCommandEvent& event )
{
  //TODO: Detect if package is unloaded somehow, otherwise this will surely cause a segfault.
  m_SelectedPackage = (UPackage*)event.GetClientData();
  Update();
  event.Skip();
}

wxBEGIN_EVENT_TABLE( EdPackageBrowser, EdBrowser )
  EVT_COMBOBOX( ID_PackageList, EdPackageBrowser::OnPackageList )
wxEND_EVENT_TABLE();