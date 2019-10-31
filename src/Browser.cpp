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
 * Browsers.cpp - View browser implementations
 *b
 * written by Jesse 'Hyzoran' Kowalik
*========================================================================
*/

#include "EditorMain.h"
#include "Browser.h"

EdBrowser::EdBrowser( UClass* InType, bool bDock, wxSize Size, bool bShowViewButtons )
  : EdToolFrame( bDock, Size ), m_Type( InType )
{
  m_ToolType = TOOL_Browser;
  wxMenu* menuFile = new wxMenu();
  m_MenuFile = new wxMenu();

  m_MenuFile->Append( ID_BrowserNew, "&New...", "Create a new Package" );
  m_MenuFile->Append( ID_BrowserOpen, "&Open...", "Load a new Package" );
  m_MenuFile->Append( ID_BrowserSave, "&Save... ", "Save modified Packages" );

  m_MenuFile->AppendSeparator();

  m_MenuFile->Append( ID_BrowserImport, "&Import... ", "Import Assets into Package" );
  m_MenuFile->Append( ID_BrowserExport, "&Export... ", "Export Objects from package" );

  m_MenuFile->AppendSeparator();

  m_MenuFile->Append( wxID_EXIT, "&Exit...", "" );

  m_MenuBar = new wxMenuBar();
  m_MenuBar->Append( m_MenuFile, "&File" );

  SetMenuBar( m_MenuBar );
  SetMinSize( wxSize( 540, 384 ) );
  m_WindowAreaSizer = new wxBoxSizer( wxVERTICAL );
  m_OptionsBar = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxSize( -1, C_TOOLBUTTONSIZE ), wxBORDER_SIMPLE );
  m_OptionsSizer = new wxBoxSizer( wxHORIZONTAL );

  if ( bShowViewButtons )
  {
    m_ShowListView = new wxButton( m_OptionsBar, ID_ShowListView, "List", wxDefaultPosition, wxSize( C_TOOLBUTTONSIZE, C_TOOLBUTTONSIZE ) );
    m_ShowTileView = new wxButton( m_OptionsBar, ID_ShowTileView, "Tile", wxDefaultPosition, wxSize( C_TOOLBUTTONSIZE, C_TOOLBUTTONSIZE ) );

    m_OptionsSizer->Add( m_ShowListView, 0, wxALIGN_LEFT );
    m_OptionsSizer->Add( m_ShowTileView, 0, wxALIGN_LEFT );
  }

  m_OptionsBar->SetSizer( m_OptionsSizer );
  m_WindowAreaSizer->Add( m_OptionsBar, 0, wxALIGN_TOP | wxEXPAND );
}

EdBrowser::~EdBrowser()
{
}

void EdBrowser::Update()
{
}

void EdBrowser::ListUpdate()
{
  TArray<wxTreeItemId> packages;
  TArray<u32> groups;
  TArray<wxTreeItemId> groupIDs;

  //Clear previous state.
  m_ListView->DeleteAllItems();

  wxTreeItemId root = m_ListView->AddRoot( "" ); //Dummy root

  //Populate Packages
  for ( size_t i = 0; i < (*UPackage::GetLoadedPackages()).Size(); i++ )
  {
    groups.Clear(); //Reset groups for next package.
    groupIDs.Clear(); //Reset groups for next package.

    UPackage* currentPackage = (*UPackage::GetLoadedPackages())[i];
    packages.PushBack( m_ListView->AppendItem( root, currentPackage->Name.Data(), -1, -1,
      new wxObjectItemData( currentPackage ) ) );

    //Populate Objects of Package
    for ( size_t j = 0; j < currentPackage->GetExportTable().Size(); j++ )
    {
      FExport* currentExport = &currentPackage->GetExportTable()[j];

      if ( currentExport->Obj == NULL ) //Invalid Object, skip.
        continue;

      //Object Type match, check against existing groups for this package and populate.
      if ( currentExport->Obj->IsA( m_Type ) )
      {
        bool bFoundGroup = false;

        //Check against so-far populated groups for this package and see if it matches.
        for ( size_t k = 0; k < groups.Size(); k++ )
        {
          if ( currentExport->Group == groups[k] ) //Found group, add under group.
          {
            m_ListView->AppendItem( groupIDs[k], currentExport->Obj->Name.Data(), -1, -1,
              new wxObjectItemData( currentExport->Obj ) );
            bFoundGroup = true;
          }
        }

        //We didnt find a group for this object, make a new one.
        if ( !bFoundGroup )
        {
          groupIDs.PushBack( m_ListView->AppendItem( packages[i],
            currentPackage->GetNameEntryByObjRef( currentExport->Group )->Data ) );

          groups.PushBack( currentExport->Group );

          m_ListView->AppendItem( groupIDs.Back(), currentExport->Obj->Name.Data(), -1, -1,
            new wxObjectItemData( currentExport->Obj ) );
        }
      }
    }
  }

  //Clear out empty packages.
  for ( size_t i = 0; i < packages.Size(); i++ )
  {
    if ( !(m_ListView->ItemHasChildren( packages[i] )) )
    {
      m_ListView->Delete( packages[i] );
    }
  }

  EdBrowser::Update();
}

void EdBrowser::TileUpdate()
{
  EdBrowser::Update();
}

void EdBrowser::ConstructPackageButtons()
{
  m_OptionsSizer->AddSpacer( C_TOOLBUTTONSIZE );

  m_OpenPackage = new wxBitmapButton( m_OptionsBar, ID_BrowserOpen, EdEditorFrame::sm_icoDir, wxDefaultPosition, wxSize( C_TOOLBUTTONSIZE, C_TOOLBUTTONSIZE ) );
  m_SavePackage = new wxBitmapButton( m_OptionsBar, ID_BrowserSave, EdEditorFrame::sm_icoSave, wxDefaultPosition, wxSize( C_TOOLBUTTONSIZE, C_TOOLBUTTONSIZE ) );
  m_LoadFullPackage = new wxButton( m_OptionsBar, ID_FullPackageLoad, "", wxDefaultPosition, wxSize( C_TOOLBUTTONSIZE, C_TOOLBUTTONSIZE ) );

  m_OptionsSizer->Add( m_OpenPackage, 0, 0 );
  m_OptionsSizer->Add( m_SavePackage, 0, 0 );
  m_OptionsSizer->Add( m_LoadFullPackage, 0, 0 );

  m_OptionsSizer->AddSpacer( C_TOOLBUTTONSIZE );
}

void EdBrowser::ListConstruct()
{
  m_ListView = new wxTreeCtrl( this, ID_ListView, wxDefaultPosition, wxDefaultSize, wxTR_MULTIPLE | wxTR_HIDE_ROOT | wxTR_HAS_BUTTONS );
  m_ListView->SetMinSize( wxSize( 300, 256 ) );

  m_WindowAreaSizer->Add( m_ListView, 1, wxALIGN_TOP | wxEXPAND );

  FinishConstruct();
}

void EdBrowser::TileConstruct()
{
  FinishConstruct();
}

void EdBrowser::FinishConstruct()
{
  m_Dock = new wxButton( m_OptionsBar, ID_BrowserDock, "Dock", wxDefaultPosition, wxSize( 32, 24 ) );
  m_OptionsSizer->AddStretchSpacer();
  m_OptionsSizer->Add( m_Dock, 0, 0 );
  SetSizer( m_WindowAreaSizer );

  //Initialize Packagelist with all loaded Packages.
  PackagesAdded( 0 );
  ObjectsAdded();

  Show( true );

  Refresh();
  Update();
}

void EdBrowser::PackagesAdded( size_t PackageStartIndex )
{
}

void EdBrowser::PackagesRemoved()
{
}

void EdBrowser::ObjectsAdded()
{
  Update();
}

void EdBrowser::ObjectsRemoved()
{
  Update();
}

void EdBrowser::OnExit( wxCommandEvent& event )
{
  EdToolFrame::OnExit( event );
}

void EdBrowser::OnBrowserNew( wxCommandEvent& event )
{
  event.Skip();
}

void EdBrowser::OnBrowserOpen( wxCommandEvent& event )
{
  wxFileDialog openFileDialog( this, "Open Unreal Package", "", "", "UE Package Files (*.u, *.utx, *.uax\
    , *.umx, *.usm, *.unr)|*.u;*.u;*.utx;*.uax;*.umx;*.usm;*.unr", wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_MULTIPLE );

  if ( openFileDialog.ShowModal() == wxID_CANCEL )
    return;

  wxArrayString filePaths;
  openFileDialog.GetPaths( filePaths );
  EdEditorFrame::LoadPackages( filePaths );

  event.Skip();
}

void EdBrowser::OnBrowserSave( wxCommandEvent& event )
{
  event.Skip();
}

void EdBrowser::OnBrowserImport( wxCommandEvent& event )
{
  event.Skip();
}

void EdBrowser::OnBrowserExport( wxCommandEvent& event )
{
  event.Skip();
}

void EdBrowser::OnBrowserDock( wxCommandEvent& event )
{
  m_bDocked = !m_bDocked;
  Update();
  event.Skip();
}

wxBEGIN_EVENT_TABLE( EdBrowser, wxFrame )
  EVT_MENU( wxID_EXIT, EdBrowser::OnExit )
  EVT_MENU( ID_BrowserNew, EdBrowser::OnBrowserNew )
  EVT_MENU( ID_BrowserOpen, EdBrowser::OnBrowserOpen )
  EVT_MENU( ID_BrowserSave, EdBrowser::OnBrowserSave )
  EVT_MENU( ID_BrowserImport, EdBrowser::OnBrowserImport )
  EVT_MENU( ID_BrowserExport, EdBrowser::OnBrowserExport )
  EVT_CHECKBOX( ID_BrowserDock, EdBrowser::OnBrowserDock )
wxEND_EVENT_TABLE()
