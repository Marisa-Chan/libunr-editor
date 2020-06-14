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
* EdMasterBrowser.cpp
*
* written by Jesse 'Hyzoran' Kowalik
*========================================================================
*/

#include "EdBrowsers.h"

#define C_BUTTONSIZE 30
#define C_BUTTONCOLOUR wxColour( 130, 130, 130 )

EdMasterBrowser::EdMasterBrowser( EdBrowser::BrowserType Type, wxWindow* Parent, wxString Title, bool bStartDocked )
  : EdToolFrame( Parent, Title, bStartDocked )
{
  m_VSizer = new wxBoxSizer( wxVERTICAL );
  SetSizer( m_VSizer );

  m_MenuFile = new wxMenu();
  m_MenuFile->Append( ID_New, "New...", "Make new Package" );
  m_MenuFile->Append( ID_Open, "Open...", "Load existing package" );
  m_MenuFile->AppendSeparator();
  m_MenuFile->Append( ID_Import, "Import...", "Import an asset" );
  m_MenuFile->Append( ID_Export, "Export...", "Export an object" );
  m_MenuFile->AppendSeparator();
  m_MenuFile->Append( ID_Exit , "Exit...", "" );

  m_MenuBar = new wxMenuBar();
  m_MenuBar->Append( m_MenuFile, "File" );

  SetMenuBar( m_MenuBar );

  m_HeaderPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxSize( -1, 32 ), wxBORDER_SIMPLE );
  m_HeaderSizer = new wxBoxSizer( wxHORIZONTAL );
  m_HeaderPanel->SetSizer( m_HeaderSizer );
  m_VSizer->Add( m_HeaderPanel, 0, wxEXPAND );

  wxBitmapButton* button_New = new wxBitmapButton( m_HeaderPanel, ID_New, EdEditor::g_bmpNew, wxDefaultPosition, wxSize( C_BUTTONSIZE, C_BUTTONSIZE ) );
  m_HeaderSizer->Add(button_New, 0, wxALIGN_CENTRE_VERTICAL);

  wxBitmapButton* button_Open = new wxBitmapButton( m_HeaderPanel, ID_Open, EdEditor::g_bmpDir, wxDefaultPosition, wxSize( C_BUTTONSIZE, C_BUTTONSIZE ) );
  m_HeaderSizer->Add(button_Open, 0, wxALIGN_CENTRE_VERTICAL);

  m_Ctrl = new wxNotebook( this, ID_Ctrl, wxDefaultPosition, wxSize( -1, -1 ), wxNB_TOP | wxNB_FIXEDWIDTH );

  //Setup Browsers
  TArray<UClass*> classes;

  m_Browsers[EdBrowser::BrowserType::BT_Package] = new EdPackageBrowser( m_Ctrl, NULL, false );
  m_Browsers[EdBrowser::BrowserType::BT_Class] = new EdClassBrowser( m_Ctrl, UObject::StaticClass() );
  
  classes.PushBack( USound::StaticClass() );
  m_Browsers[EdBrowser::BrowserType::BT_Sound] = new EdObjectBrowser( m_Ctrl, classes, false );
  classes.Clear();

  classes.PushBack( UMusic::StaticClass() );
  m_Browsers[EdBrowser::BrowserType::BT_Music] = new EdObjectBrowser( m_Ctrl, classes, false );
  classes.Clear();

  classes.PushBack( UTexture::StaticClass() );
  m_Browsers[EdBrowser::BrowserType::BT_Texture] = new EdObjectBrowser( m_Ctrl, classes, false );
  classes.Clear();

  classes.PushBack( UMesh::StaticClass() );
  m_Browsers[EdBrowser::BrowserType::BT_Mesh] = new EdObjectBrowser( m_Ctrl, classes, false );
  classes.Clear();

  classes.PushBack( ULevel::StaticClass() );
  m_Browsers[EdBrowser::BrowserType::BT_Level] = new EdObjectBrowser( m_Ctrl, classes, false );
  classes.Clear();

  m_Ctrl->InsertPage( 0, m_Browsers[EdBrowser::BrowserType::BT_Package], wxString( "Packages" ), false);
  m_Ctrl->InsertPage( 1, m_Browsers[EdBrowser::BrowserType::BT_Class], wxString( "Classes" ), false );
  m_Ctrl->InsertPage( 2, m_Browsers[EdBrowser::BrowserType::BT_Sound], wxString( "Sounds" ), false );
  m_Ctrl->InsertPage( 3, m_Browsers[EdBrowser::BrowserType::BT_Music], wxString( "Music" ), false );
  m_Ctrl->InsertPage( 4, m_Browsers[EdBrowser::BrowserType::BT_Texture], wxString( "Textures" ), false );
  m_Ctrl->InsertPage( 5, m_Browsers[EdBrowser::BrowserType::BT_Mesh], wxString( "Meshes" ), false );
  m_Ctrl->InsertPage( 6, m_Browsers[EdBrowser::BrowserType::BT_Level], wxString( "Level" ), false );

  m_Ctrl->SetSelection( Type );

  m_VSizer->Add( m_Ctrl, 1, wxEXPAND );

  PageChange();

  Show();
  SetLabel( m_Browsers[ Type ]->GetLabel() );
}

void EdMasterBrowser::ObjectUpdate( bool bUpdatePackageList )
{
  
  for( size_t i = 0; i<7; i++ )
  {
    m_Browsers[i]->ObjectUpdate( bUpdatePackageList );
  }
}

void EdMasterBrowser::PageChange()
{
  //Set properties relating to browser types.
  int selection = m_Ctrl->GetSelection();

  switch (selection)
  {
    case EdBrowser::BrowserType::BT_Package:
    {
      m_dirPath = EdEditor::gc_SubDir_U;
      SetIcon( EdEditor::g_icoPackage );
      SetLabel( wxString("Package Browser") );
      break;
    }
    case EdBrowser::BrowserType::BT_Class:
    {
      m_dirPath = EdEditor::gc_SubDir_U;
      SetIcon( EdEditor::g_icoClass );
      SetLabel( wxString("Class Browser") );
      break;
    }
    case EdBrowser::BrowserType::BT_Sound:
    {
      m_dirPath = EdEditor::gc_SubDir_UAX;
      SetIcon( EdEditor::g_icoSound );
      SetLabel( wxString("Sound Browser") );
      break;
    }
    case EdBrowser::BrowserType::BT_Music:
    {
      m_dirPath = EdEditor::gc_SubDir_UMX;
      SetIcon( EdEditor::g_icoMusic );
      SetLabel( wxString("Music Browser") );
      break;
    }
    case EdBrowser::BrowserType::BT_Texture:
    {
      m_dirPath = EdEditor::gc_SubDir_UTX;
      SetIcon( EdEditor::g_icoTexture );
      SetLabel( wxString("Texture Browser") );
      break;
    }
    case EdBrowser::BrowserType::BT_Mesh:
    {
      m_dirPath = EdEditor::gc_SubDir_USM;
      SetIcon( EdEditor::g_icoMesh );
      SetLabel( wxString("Mesh Browser") );
      break;
    }
    case EdBrowser::BrowserType::BT_Level:
    {
      m_dirPath = EdEditor::gc_SubDir_UNR;
      SetIcon( EdEditor::g_icoMisc );
      SetLabel( wxString("Level Browser") );
      break;
    }
    default:
    {
      m_dirPath = wxString(" ");
      SetLabel( wxString("Browser") );
      SetIcon( EdEditor::g_icoMisc );
      break;
    }
  }

  if( selection != -1 )
    SetLabel( m_Browsers[ selection ]->GetLabel() );
}

void EdMasterBrowser::EVT_New( wxCommandEvent& event )
{

}

void EdMasterBrowser::EVT_Open( wxCommandEvent& event )
{
  wxString subdir = EdEditor::GetGameDir() + wxString( m_dirPath );

  wxFileDialog openFileDialog( this, "Open Unreal Package", subdir, "", "UE Package Files (*.u, *.utx, *.uax\
    , *.umx, *.usm, *.unr)|*.u;*.u;*.utx;*.uax;*.umx;*.usm;*.unr", wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_MULTIPLE );

  if ( openFileDialog.ShowModal() == wxID_CANCEL )
    return;

  wxArrayString filePaths;
  openFileDialog.GetPaths( filePaths );
  EdEditor::LoadPackages( filePaths );
}

void EdMasterBrowser::EVT_Import( wxCommandEvent& event )
{
}

void EdMasterBrowser::EVT_Export( wxCommandEvent& event )
{
  new EdEditor::UObjectExportDialog( ((EdEditor::UObjectClientData*)( event.GetClientObject() ))->GetObject() );
}

void EdMasterBrowser::EVT_PageChanged( wxBookCtrlEvent& event )
{
  PageChange();
}

wxBEGIN_EVENT_TABLE( EdMasterBrowser, EdToolFrame )
  EVT_BUTTON(ID_New, EdMasterBrowser::EVT_New)
  EVT_MENU(ID_New, EdMasterBrowser::EVT_New)
  EVT_BUTTON(ID_Open, EdMasterBrowser::EVT_Open)
  EVT_MENU(ID_Open, EdMasterBrowser::EVT_Open)
  EVT_BUTTON(ID_Import, EdMasterBrowser::EVT_Import)
  EVT_MENU(ID_Import, EdMasterBrowser::EVT_Import)
  EVT_BUTTON(ID_Export, EdMasterBrowser::EVT_Export)
  EVT_MENU(ID_Export, EdMasterBrowser::EVT_Export)
  EVT_NOTEBOOK_PAGE_CHANGED( ID_Ctrl, EdMasterBrowser::EVT_PageChanged )
wxEND_EVENT_TABLE()