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
 * EditorMain.cpp - Main editor frame
 *
 * written by Jesse 'Hyzoran' Kowalik
 *========================================================================
*/

#include "EditorMain.h"
#include "Browser.h"
#include "ClassBrowser.h"
#include "LevelBrowser.h"
#include "MeshBrowser.h"
#include "MusicBrowser.h"
#include "PackageBrowser.h"
#include "SoundBrowser.h"
#include "TextureBrowser.h"
#include <Editor/UEditorEngine.h>

int EditorApp::sm_SelectedGame;

EdEditorFrame* EdEditorFrame::sm_Frame;
TArray<EdToolFrame*> EdEditorFrame::sm_ToolArray;
size_t EdEditorFrame::sm_EmptySlots;

wxBitmap EdEditorFrame::sm_bmpNew;
wxBitmap EdEditorFrame::sm_bmpDir;
wxBitmap EdEditorFrame::sm_bmpSave;
wxBitmap EdEditorFrame::sm_bmpPackage;
wxBitmap EdEditorFrame::sm_bmpClass;
wxBitmap EdEditorFrame::sm_bmpSound;
wxBitmap EdEditorFrame::sm_bmpMusic;
wxBitmap EdEditorFrame::sm_bmpGraphics;
wxBitmap EdEditorFrame::sm_bmpMesh;
wxIcon EdEditorFrame::sm_icoNew;
wxIcon EdEditorFrame::sm_icoDir;
wxIcon EdEditorFrame::sm_icoSave;
wxIcon EdEditorFrame::sm_icoPackage;
wxIcon EdEditorFrame::sm_icoClass;
wxIcon EdEditorFrame::sm_icoSound;
wxIcon EdEditorFrame::sm_icoMusic;
wxIcon EdEditorFrame::sm_icoGraphics;
wxIcon EdEditorFrame::sm_icoMesh;
wxString EdEditorFrame::csm_SubDir_U;
wxString EdEditorFrame::csm_SubDir_UAX;
wxString EdEditorFrame::csm_SubDir_UMX;
wxString EdEditorFrame::csm_SubDir_UTX;
wxString EdEditorFrame::csm_SubDir_UNR;
wxString EdEditorFrame::csm_SubDir_USM;
wxString EdEditorFrame::csm_SubDir_USA;

EdEditorFrame::EdEditorFrame( const wxString& Title, const wxPoint& Pos, const wxSize& Size )
  : wxFrame( NULL, wxID_ANY, Title, Pos, Size )
{
  sm_Frame = this;

  //Init interface
  wxMenu* menuFile = new wxMenu();

  menuFile->Append( ID_New, "&New... \tCTRL+N",
    "Start a new map/Package" );
  menuFile->Append( ID_Open, "&Open... \tCTRL+O",
    "Open an existing map or package" );
  menuFile->Append( ID_Save, "&Save All... \tCTRL+O",
    "Save all modified maps/packages" );

  menuFile->AppendSeparator();

  menuFile->Append( ID_Import, "&Import... \tCTRL+O+I",
    "Import an asset into a package" );

  menuFile->Append( ID_Export, "&Export... \tCTRL+O+E",
    "Export Contents of Package/Map" );

  menuFile->AppendSeparator();

  menuFile->Append( wxID_EXIT, "&Exit...",
    "Close the editor" );

  wxMenu* menuView = new wxMenu();

  menuView->Append( ID_Preferences, "&Preferences...", "Configure the editor" );

  menuView->AppendSeparator();

  menuView->Append( ID_BrowserPackage, "&Package Browser...", "Open a Package browser instance" );
  menuView->Append( ID_BrowserClass, "&Class Browser...", "Open an Class browser instance" );
  menuView->Append( ID_BrowserSound, "&Sound Browser...", "Open a Sound browser instance" );
  menuView->Append( ID_BrowserMusic, "&Music Browser...", "Open a Music browser instance" );
  menuView->Append( ID_BrowserGraphics, "&Graphics Browser...", "Open a Graphics browser instance" );
  menuView->Append( ID_BrowserMesh, "&Mesh Browser...", "Open a mesh browser instance" );

  menuView->AppendSeparator();

  menuView->Append( ID_ViewLog, "&Show Log...", "Show me the console fax" );
  menuView->Append( ID_ActiveTools, "&Show Tools...", "Show all running tools" );

  wxMenu* menuTools = new wxMenu();

  menuTools->Append( ID_MapEditor, "&Map Editor...", "New editor tab." );
  menuTools->Append( ID_MeshEditor, "&Mesh Editor...", "Used for advanced brush-building, quick-prefabs, 2d-shape editor, etc." );

  wxMenu* menuHelp = new wxMenu();

  menuHelp->Append( ID_Manual, "&Editor Manual...", "Documentation shipped with the editor" );
  menuHelp->Append( wxID_ABOUT, "&About OpenUE...", "About OpenUE" );

  wxMenuBar* menuBar = new wxMenuBar();

  menuBar->Append( menuFile, "&File" );
  menuBar->Append( menuView, "&View" );
  menuBar->Append( menuTools, "&Tools" );
  menuBar->Append( menuHelp, "&Help" );

  SetMenuBar( menuBar );

  wxBoxSizer* contentSizer = new wxBoxSizer( wxVERTICAL );

  wxPanel* toolBar = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxSize( -1, 40 ), wxBORDER_RAISED );

  contentSizer->Add( toolBar, 0, wxALIGN_TOP | wxALIGN_LEFT | wxEXPAND );

  wxBoxSizer* buttonSizer = new wxBoxSizer( wxHORIZONTAL );

  wxBitmapButton* button_NewMap = new wxBitmapButton( toolBar, ID_New, EdEditorFrame::sm_bmpNew, wxDefaultPosition, wxSize( C_BUTTONSIZE, C_BUTTONSIZE ) );
  buttonSizer->Add( button_NewMap, 0, wxALIGN_CENTRE_VERTICAL );

  wxBitmapButton* button_OpenMap = new wxBitmapButton( toolBar, ID_Open, EdEditorFrame::sm_bmpDir, wxDefaultPosition, wxSize( C_BUTTONSIZE, C_BUTTONSIZE ) );
  buttonSizer->Add( button_OpenMap, 0, wxALIGN_CENTRE_VERTICAL );

  wxBitmapButton* button_Save = new wxBitmapButton( toolBar, ID_Save, EdEditorFrame::sm_bmpSave, wxDefaultPosition, wxSize( C_BUTTONSIZE, C_BUTTONSIZE ) );
  buttonSizer->Add( button_Save, 0, wxALIGN_CENTRE_VERTICAL );

  buttonSizer->AddSpacer( C_BUTTONSIZE );

  wxBitmapButton* button_PackageBrowser = new wxBitmapButton( toolBar, ID_BrowserPackage, EdEditorFrame::sm_bmpPackage, wxDefaultPosition, wxSize( C_BUTTONSIZE, C_BUTTONSIZE ) );
  buttonSizer->Add( button_PackageBrowser, 0, wxALIGN_CENTRE_VERTICAL );

  wxBitmapButton* button_ClassBrowser = new wxBitmapButton( toolBar, ID_BrowserClass, EdEditorFrame::sm_bmpClass, wxDefaultPosition, wxSize( C_BUTTONSIZE, C_BUTTONSIZE ) );
  buttonSizer->Add( button_ClassBrowser, 0, wxALIGN_CENTRE_VERTICAL );

  wxBitmapButton* button_SoundBrowser = new wxBitmapButton( toolBar, ID_BrowserSound, EdEditorFrame::sm_bmpSound, wxDefaultPosition, wxSize( C_BUTTONSIZE, C_BUTTONSIZE ) );
  buttonSizer->Add( button_SoundBrowser, 0, wxALIGN_CENTRE_VERTICAL );

  wxBitmapButton* button_MusicBrowser = new wxBitmapButton( toolBar, ID_BrowserMusic, EdEditorFrame::sm_bmpMusic, wxDefaultPosition, wxSize( C_BUTTONSIZE, C_BUTTONSIZE ) );
  buttonSizer->Add( button_MusicBrowser, 0, wxALIGN_CENTRE_VERTICAL );

  wxBitmapButton* button_GraphicsBrowser = new wxBitmapButton( toolBar, ID_BrowserGraphics, EdEditorFrame::sm_bmpGraphics, wxDefaultPosition, wxSize( C_BUTTONSIZE, C_BUTTONSIZE ) );
  buttonSizer->Add( button_GraphicsBrowser, 0, wxALIGN_CENTRE_VERTICAL );

  wxBitmapButton* button_MeshBrowser = new wxBitmapButton( toolBar, ID_BrowserMesh, EdEditorFrame::sm_bmpMesh, wxDefaultPosition, wxSize( C_BUTTONSIZE, C_BUTTONSIZE ) );
  buttonSizer->Add( button_MeshBrowser, 0, wxALIGN_CENTRE_VERTICAL );

  toolBar->SetSizer( buttonSizer );

  wxPanel* tabBar = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxSize( -1, 24 ), wxBORDER_NONE );
  contentSizer->Add( tabBar, 0, wxALIGN_TOP | wxEXPAND );

  wxBoxSizer* contentSizer2 = new wxBoxSizer( wxHORIZONTAL );
  contentSizer->Add( contentSizer2, 1, wxEXPAND );

  wxPanel* toolBar2 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxSize( 80, -1 ), wxBORDER_RAISED );
  contentSizer2->Add( toolBar2, 0, wxALIGN_LEFT | wxEXPAND );

  wxPanel* backGround = new wxPanel( this );
  backGround->SetBackgroundColour( wxColour( 80, 80, 80 ) );
  contentSizer2->Add( backGround, 1, wxEXPAND );

  SetSizer( contentSizer );

  CreateStatusBar();
  SetStatusText( "Welcome to OpenUEd!" );
}

EdEditorFrame::~EdEditorFrame()
{
}

size_t EdEditorFrame::RegisterTool( EdToolFrame* Tool )
{
  if ( sm_EmptySlots > 0 ) //A slot was freed earlier, find and use that slot.
  {
    for ( size_t i = 0; i < sm_ToolArray.Size(); i++ )
    {
      if ( sm_ToolArray[i] == NULL )
      {
        sm_ToolArray[i] = Tool; //Manual cast workaround for wx class.
        sm_EmptySlots--; //We used a slot.
        return i;
      }
    }
    //None found? Something went wrong.
    GLogf( LOG_WARN, "EdEditorFrame::RegTool() : sm_EmptySlots > 0, but none found in sm_ToolArray!" );
    GLogf( LOG_WARN, "Possible memory corruption and/or leak. Pushing new tool to end of array..." );
  }
  sm_ToolArray.PushBack( (EdToolFrame*)Tool );
  return sm_ToolArray.Size() - 1;
}

bool EdEditorFrame::UnregisterTool( size_t id )
{
  if ( sm_ToolArray[id] != NULL )
  {
    sm_ToolArray[id] = NULL;
    sm_EmptySlots++;
    return true;
  }
  GLogf( LOG_WARN, "EdEditor::UnregisterTool() : Unregistered tool that does not exist in sm_ToolArray!" );
  return false;
}

EdEditorFrame* EdEditorFrame::GetMainFrame()
{
  return sm_Frame;
}

void EdEditorFrame::LoadPackages( const wxArrayString& Paths )
{
  size_t packageStart = (*UPackage::GetLoadedPackages()).Size(); //Index of new packages to load.
  size_t objectStart = UObject::GetGlobalObjectPool()->Size(); //Index of new objects to load.

  for ( size_t i = 0; i < Paths.GetCount(); i++ )
  {
    UPackage* p = UPackage::StaticLoadPackage( Paths[i], false );
    p->LoadEditableTypes();
  }

  // Check to make sure packages were actually loaded.
  if ( (*UPackage::GetLoadedPackages()).Size() > packageStart )
  {
    for ( size_t i = 0; i < sm_ToolArray.Size(); i++ )
    {
      if ( sm_ToolArray[i] != NULL )
        sm_ToolArray[i]->PackagesAdded( packageStart );
    }
  }

  // Check to make sure objects were actually loaded.
  if ( UObject::GetGlobalObjectPool()->Size() > objectStart )
  {
    for ( size_t i = 0; i < sm_ToolArray.Size(); i++ )
    {
      if ( sm_ToolArray[i] != NULL )
        sm_ToolArray[i]->ObjectsAdded();
    }
  }
}

void EdEditorFrame::ObjectMenu( UObject* Obj )
{
  lastMenuObject = Obj;

  //TODO: Does a popped-up menu get automatically destroyed when closed? If not, memory leak. :<
  wxMenu* Popup = new wxMenu();

  //Object has audio properties
  if ( Obj->IsA( USound::StaticClass() ) || Obj->IsA( UMusic::StaticClass() ) )
  {
    Popup->Append( ID_ObjectActivate, "Play" );
  }

  //Object is graphical
  if ( Obj->IsA( UPrimitive::StaticClass() ) || Obj->IsA( UTexture::StaticClass() ) )
  {
    Popup->Append( ID_ObjectActivate, "View" );
  }

  //Object can be "Opened" in another tool.
  if ( Obj->IsA( ULevel::StaticClass() ) || Obj->IsA( UModel::StaticClass() ) || Obj->IsA( UStaticMesh::StaticClass() ) )
  {
    Popup->Append( ID_ObjectOpen, "Open" );
  }

  Popup->AppendSeparator();

  //Exporting
  Popup->Append( ID_ObjectExport, "Export Object..." );

  Popup->AppendSeparator();
  Popup->Append( ID_ObjectProperties, "Properties" );

  PopupMenu( Popup );
}

void EdEditorFrame::ObjectActivate( UObject* Obj )
{
  FVector Ass;
  Ass.X = 0;
  Ass.Y = 0;
  Ass.Z = 0;

  //Sounds
  if ( Obj->IsA( USound::StaticClass() ) )
  {
    GEngine->Audio->PlaySound( NULL, (USound*)Obj, Ass, 1, 1, 1 );
  }

  //Music
  if ( Obj->IsA( UMusic::StaticClass() ) )
  {
    //TODO: Playsound code here.
  }
}

void EdEditorFrame::EVT_ObjectActivate( wxCommandEvent& event )
{
  ObjectActivate( lastMenuObject );
}

void EdEditorFrame::ObjectProperties( UObject* Obj )
{
  if ( Obj == NULL )
  {
    return;
  }

  new EdObjectViewer( lastMenuObject );
}

void EdEditorFrame::EVT_ObjectProperties( wxCommandEvent& event )
{
  ObjectProperties( lastMenuObject );
  event.Skip();
}

wxString EdEditorFrame::GetGameDir()
{
  return wxString( GGameConfig->ReadString( "Game", "Path", EditorApp::sm_SelectedGame ) );
}

void EdEditorFrame::OnExit( wxCommandEvent& event )
{
  Close( true );
}

void EdEditorFrame::OnAbout( wxCommandEvent& event )
{
  if ( !EdAbout::IsOpened() )
    new EdAbout( this );
}

void EdEditorFrame::EVT_New( wxCommandEvent& event ) 
{
  event.Skip();
}

void EdEditorFrame::EVT_Open( wxCommandEvent& event ) 
{
  event.Skip();
}

void EdEditorFrame::EVT_Save( wxCommandEvent& event ) 
{
  event.Skip();
}

void EdEditorFrame::EVT_Import( wxCommandEvent& event ) 
{
  event.Skip();
}

void EdEditorFrame::EVT_Export( wxCommandEvent& event ) 
{
  event.Skip();
}

void EdEditorFrame::EVT_Preferences( wxCommandEvent& event ) 
{
  event.Skip();
}

void EdEditorFrame::EVT_BrowserPackage( wxCommandEvent& event )
{
  new EdPackageBrowser();
  event.Skip();
}

void EdEditorFrame::EVT_BrowserClass( wxCommandEvent& event )
{
  new EdClassBrowser();
  event.Skip();
}

void EdEditorFrame::EVT_BrowserSound( wxCommandEvent& event )
{
  new EdSoundBrowser();
  event.Skip();
}

void EdEditorFrame::EVT_BrowserMusic( wxCommandEvent& event )
{
  new EdMusicBrowser();
  event.Skip();
}

void EdEditorFrame::EVT_BrowserTexture( wxCommandEvent& event )
{
  new EdTextureBrowser();
  event.Skip();
}

void EdEditorFrame::EVT_BrowserMesh( wxCommandEvent& event )
{
  new EdMeshBrowser();
  event.Skip();
}

void EdEditorFrame::EVT_ViewLog( wxCommandEvent& event ) 
{
  event.Skip();
}

void EdEditorFrame::EVT_ActiveTools( wxCommandEvent& event ) 
{
  event.Skip();
}

void EdEditorFrame::EVT_MapEditor( wxCommandEvent& event ) 
{
  event.Skip();
}

void EdEditorFrame::EVT_MeshEditor( wxCommandEvent& event ) 
{
  event.Skip();
}

void EdEditorFrame::EVT_Manual( wxCommandEvent& event ) 
{
  event.Skip();
}

EdGamePrompt::EdGamePrompt( TArray<char*>* Names )
  : wxDialog( NULL, wxID_ANY, "Select Game", wxDefaultPosition, wxSize( 300, 300 ) )
{
  wxBoxSizer* vsizer = new wxBoxSizer( wxVERTICAL );

  m_GameSize = Names->Size();
  m_Names = Names;
  wxString* strNames = new wxString[m_GameSize];

  for ( size_t i = 0; i < m_GameSize; i++ )
    strNames[i] = wxString::FromAscii( (*Names)[i] );

  m_ChoiceBox = new wxListBox( this, ID_GameChoice, wxDefaultPosition, wxSize( 300, -1 ), m_GameSize, strNames, wxLB_SINGLE | wxLB_NEEDED_SB );

  vsizer->Add( m_ChoiceBox, 1, wxEXPAND, 64 );

  wxPanel* endPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize );

  wxBoxSizer* hsizer = new wxBoxSizer( wxHORIZONTAL );
  wxButton* confirmB = new wxButton( endPanel, ID_NewGameChoice, "Add" );
  wxButton* cancelB = new wxButton( endPanel, wxID_CANCEL, "Cancel" );

  hsizer->Add( confirmB, 0, 0 );
  hsizer->AddStretchSpacer();
  hsizer->Add( cancelB, 0, 0 );
  endPanel->SetSizer( hsizer );

  vsizer->Add( endPanel, 0, 0, 8 );

  CentreOnScreen();
  SetSizer( vsizer );
  delete[] strNames;
}

int EdGamePrompt::GetSelected()
{
  return m_GameSelect;
}

void EdGamePrompt::EVT_ChoiceSelect( wxCommandEvent& event )
{
  m_GameSelect = event.GetSelection();
  EndModal( 0 );
}

void EdGamePrompt::EVT_NewGame( wxCommandEvent& event )
{
  EdNewGameDialog newGamePrompt( m_GameSize, &m_GameSelect );

  if ( newGamePrompt.ShowModal() != wxID_CANCEL )
  {
    m_GameSelect = m_GameSize;
    EndModal( 0 );
  }

  event.Skip();
}

EdNewGameDialog::EdNewGameDialog( size_t NewIndex, int* ChoiceOut )
  : wxDialog( NULL, wxID_ANY, "Add Game" )
{
  m_GameSize = NewIndex;

  wxBoxSizer* vsizer = new wxBoxSizer( wxVERTICAL );

  wxStaticText* labelName = new wxStaticText( this, wxID_ANY, "Name:" );
  vsizer->Add( labelName, wxALIGN_LEFT );

  m_NameField = new wxTextCtrl( this, wxID_ANY, "", wxDefaultPosition, wxSize( 300, -1 ) );
  vsizer->Add( m_NameField, wxALIGN_LEFT );

  wxStaticText* exeName = new wxStaticText( this, wxID_ANY, "Executable Name:" );
  vsizer->Add( exeName, wxALIGN_LEFT );

  m_ExeField = new wxTextCtrl( this, wxID_ANY, "", wxDefaultPosition, wxSize( 300, -1 ) );
  vsizer->Add( m_ExeField, wxALIGN_LEFT );

  wxStaticText* pathName = new wxStaticText( this, wxID_ANY, "Directory:" );
  vsizer->Add( pathName, wxALIGN_LEFT );

  wxBoxSizer* hsizer = new wxBoxSizer( wxHORIZONTAL );
  wxPanel* pathPanel = new wxPanel( this );

  m_PathField = new wxTextCtrl( pathPanel, wxID_ANY, "", wxDefaultPosition, wxSize( 300, -1 ) );
  hsizer->Add( m_PathField, wxALIGN_LEFT );

  wxButton* pathB = new wxButton( pathPanel, ID_DirDialog, "..." );
  hsizer->Add( pathB, wxALIGN_LEFT );

  pathPanel->SetSizer( hsizer );

  vsizer->Add( pathPanel, wxALIGN_LEFT );
  vsizer->AddSpacer( 4 );

  wxBoxSizer* hsizer2 = new wxBoxSizer( wxHORIZONTAL );
  wxPanel* bPanel = new wxPanel( this );

  wxButton* confirmB = new wxButton( bPanel, ID_Confirm, "Confirm" );
  hsizer2->Add( confirmB, wxALIGN_LEFT );
  wxButton* cancelB = new wxButton( bPanel, wxID_CANCEL, "Cancel" );
  hsizer2->Add( cancelB, wxALIGN_LEFT );

  bPanel->SetSizer( hsizer2 );

  vsizer->Add( bPanel, wxALIGN_LEFT );

  SetSizer( vsizer );
}

void EdNewGameDialog::EVT_Confirm( wxCommandEvent& event )
{
  //TODO: Verify directory

  GLibunrConfig->WriteString( "Game", "Name", m_NameField->GetLineText( 0 ), m_GameSize );
  GLibunrConfig->WriteString( "Game", "Exec", m_ExeField->GetLineText( 0 ), m_GameSize );
  GLibunrConfig->WriteString( "Game", "Path", m_PathField->GetLineText( 0 ), m_GameSize );

  GLibunrConfig->Save();

  EndModal( 0 );
}

void EdNewGameDialog::EVT_DirDialog( wxCommandEvent& event )
{
  wxDirDialog file( this, "Game Directory", "", wxDD_DIR_MUST_EXIST );
  if ( file.ShowModal() == wxID_CANCEL )
    return;
  else
    m_PathField->ChangeValue( file.GetPath() );
}

int EditorApp::GamePromptHandler( TArray<char*>* Names )
{
  EdGamePrompt gamePrompt( Names );
  gamePrompt.ShowModal();

  EditorApp::sm_SelectedGame = gamePrompt.GetSelected();

  return EditorApp::sm_SelectedGame;
}

bool EditorApp::OnInit()
{
  wxInitAllImageHandlers();

  EdEditorFrame::csm_SubDir_U = "/System";
  EdEditorFrame::csm_SubDir_UAX = "/Sounds";
  EdEditorFrame::csm_SubDir_UMX = "/Music";
  EdEditorFrame::csm_SubDir_UTX = "/Textures";
  EdEditorFrame::csm_SubDir_UNR = "/Maps";
  EdEditorFrame::csm_SubDir_USM = "/Meshes";
  EdEditorFrame::csm_SubDir_USA = "/Save";

  EdEditorFrame::sm_bmpNew = wxBitmap( wxT( "res/bitmap/New.png" ), wxBITMAP_TYPE_PNG );
  EdEditorFrame::sm_bmpDir = wxBitmap( wxT( "res/bitmap/Open.png" ), wxBITMAP_TYPE_PNG );
  EdEditorFrame::sm_bmpSave = wxBitmap( wxT( "res/bitmap/Save.png" ), wxBITMAP_TYPE_PNG );
  EdEditorFrame::sm_bmpPackage = wxBitmap( wxT( "res/bitmap/PackageBrowser.png" ), wxBITMAP_TYPE_PNG );
  EdEditorFrame::sm_bmpClass = wxBitmap( wxT( "res/bitmap/ClassBrowser.png" ), wxBITMAP_TYPE_PNG );
  EdEditorFrame::sm_bmpSound = wxBitmap( wxT( "res/bitmap/SoundBrowser.png" ), wxBITMAP_TYPE_PNG );
  EdEditorFrame::sm_bmpMusic = wxBitmap( wxT( "res/bitmap/MusicBrowser.png" ), wxBITMAP_TYPE_PNG );
  EdEditorFrame::sm_bmpGraphics = wxBitmap( wxT( "res/bitmap/GraphicsBrowser.png" ), wxBITMAP_TYPE_PNG );
  EdEditorFrame::sm_bmpMesh = wxBitmap( wxT( "res/bitmap/MeshBrowser.png" ), wxBITMAP_TYPE_PNG );

  EdEditorFrame::sm_icoNew.CopyFromBitmap( EdEditorFrame::sm_bmpNew );
  EdEditorFrame::sm_icoDir.CopyFromBitmap( EdEditorFrame::sm_bmpDir );
  EdEditorFrame::sm_icoSave.CopyFromBitmap( EdEditorFrame::sm_bmpSave );
  EdEditorFrame::sm_icoPackage.CopyFromBitmap( EdEditorFrame::sm_bmpPackage );
  EdEditorFrame::sm_icoClass.CopyFromBitmap( EdEditorFrame::sm_bmpClass );
  EdEditorFrame::sm_icoSound.CopyFromBitmap( EdEditorFrame::sm_bmpSound );
  EdEditorFrame::sm_icoMusic.CopyFromBitmap( EdEditorFrame::sm_bmpMusic );
  EdEditorFrame::sm_icoGraphics.CopyFromBitmap( EdEditorFrame::sm_bmpGraphics );
  EdEditorFrame::sm_icoMesh.CopyFromBitmap( EdEditorFrame::sm_bmpMesh );

  if ( LibunrInit( GamePromptHandler, NULL, true ) == false )
    return false;

  UPackage* EditorPkg = UPackage::StaticLoadPackage( "Editor" );
  if ( !EditorPkg )
  {
    GLogf( LOG_CRIT, "Required package 'Editor' is missing" );
    return false;
  }

  UClass* EditorEngineClass = (UClass*)UObject::StaticLoadObject( EditorPkg, "EditorEngine", UClass::StaticClass(), NULL, true );
  if ( !EditorEngineClass )
  {
    GLogf( LOG_CRIT, "Failed to load EditorEngine class" );
    return false;
  }

  GEngine = (UEngine*)EditorEngineClass->CreateObject();
  if ( !GEngine->Init() )
  {
    GLogf( LOG_CRIT, "Editor Engine init failed" );
    return false;
  }

  for ( size_t i = 0; i < UPackage::GetLoadedPackages()->Size(); i++ )
    (*UPackage::GetLoadedPackages())[i]->LoadEditableTypes();

  EdEditorFrame* frame = new EdEditorFrame( "libunr-editor", wxPoint( -1, -1 ),
    wxSize( wxSystemSettings::GetMetric( wxSYS_SCREEN_X ), wxSystemSettings::GetMetric( wxSYS_SCREEN_Y ) ) );

  frame->Maximize();
  frame->Show( true );
  return true;
}

wxBEGIN_EVENT_TABLE( EdEditorFrame, wxFrame )
  EVT_MENU( wxID_EXIT, EdEditorFrame::OnExit )
  EVT_MENU( wxID_ABOUT, EdEditorFrame::OnAbout )
  EVT_MENU( ID_New, EdEditorFrame::EVT_New )
  EVT_MENU( ID_Open, EdEditorFrame::EVT_Open )
  EVT_MENU( ID_Save, EdEditorFrame::EVT_Save )
  EVT_MENU( ID_Import, EdEditorFrame::EVT_Import )
  EVT_MENU( ID_Export, EdEditorFrame::EVT_Export )
  EVT_MENU( ID_Preferences, EdEditorFrame::EVT_Preferences )
  EVT_MENU( ID_BrowserPackage, EdEditorFrame::EVT_BrowserPackage )
  EVT_BUTTON( ID_BrowserPackage, EdEditorFrame::EVT_BrowserPackage )
  EVT_MENU( ID_BrowserClass, EdEditorFrame::EVT_BrowserClass )
  EVT_BUTTON( ID_BrowserClass, EdEditorFrame::EVT_BrowserClass )
  EVT_MENU( ID_BrowserSound, EdEditorFrame::EVT_BrowserSound )
  EVT_BUTTON( ID_BrowserSound, EdEditorFrame::EVT_BrowserSound )
  EVT_MENU( ID_BrowserMusic, EdEditorFrame::EVT_BrowserMusic )
  EVT_BUTTON( ID_BrowserMusic, EdEditorFrame::EVT_BrowserMusic )
  EVT_MENU( ID_BrowserGraphics, EdEditorFrame::EVT_BrowserTexture )
  EVT_BUTTON( ID_BrowserGraphics, EdEditorFrame::EVT_BrowserTexture )
  EVT_MENU( ID_BrowserMesh, EdEditorFrame::EVT_BrowserMesh )
  EVT_BUTTON( ID_BrowserMesh, EdEditorFrame::EVT_BrowserMesh )
  EVT_MENU( ID_ViewLog, EdEditorFrame::EVT_ViewLog )
  EVT_MENU( ID_ActiveTools, EdEditorFrame::EVT_ActiveTools )
  EVT_MENU( ID_MapEditor, EdEditorFrame::EVT_MapEditor )
  EVT_MENU( ID_MeshEditor, EdEditorFrame::EVT_MeshEditor )
  EVT_MENU( ID_Manual, EdEditorFrame::EVT_Manual )
  EVT_MENU( ID_ObjectProperties, EdEditorFrame::EVT_ObjectProperties )
  EVT_MENU( ID_ObjectActivate, EdEditorFrame::EVT_ObjectActivate )
wxEND_EVENT_TABLE()

wxBEGIN_EVENT_TABLE( EdGamePrompt, wxDialog )
  EVT_LISTBOX_DCLICK( ID_GameChoice, EdGamePrompt::EVT_ChoiceSelect )
  EVT_BUTTON( ID_NewGameChoice, EdGamePrompt::EVT_NewGame )
wxEND_EVENT_TABLE()

wxBEGIN_EVENT_TABLE( EdNewGameDialog, wxDialog )
  EVT_BUTTON( ID_DirDialog, EdNewGameDialog::EVT_DirDialog )
  EVT_BUTTON( ID_Confirm, EdNewGameDialog::EVT_Confirm )
wxEND_EVENT_TABLE()

wxIMPLEMENT_APP( EditorApp );
