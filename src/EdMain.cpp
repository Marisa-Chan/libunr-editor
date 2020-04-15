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
 * EdMain.cpp
 *
 * written by Jesse 'Hyzoran' Kowalik
 *========================================================================
*/

#include "EdMain.h"
#include <Editor/UEditorEngine.h>

#define C_BUTTONSIZE 30
#define C_BUTTONCOLOUR wxColour( 130, 130, 130 )

EdEditorFrame::EdEditorFrame(const wxString& Title, const wxPoint& Pos, const wxSize& Size)
    : wxFrame(NULL, wxID_ANY, Title, Pos, Size)
{
    //Init interface
    wxMenu* menuFile = new wxMenu();

    menuFile->Append(ID_New, "&New... \tCTRL+N",
        "Start a new map/Package");
    menuFile->Append(ID_Open, "&Open... \tCTRL+O",
        "Open an existing map or package");
    menuFile->Append(ID_Save, "&Save All... \tCTRL+O",
        "Save all modified maps/packages");

    menuFile->AppendSeparator();

    menuFile->Append(ID_Import, "&Import... \tCTRL+O+I",
        "Import an asset into a package");

    menuFile->Append(ID_Export, "&Export... \tCTRL+O+E",
        "Export Contents of Package/Map");

    menuFile->AppendSeparator();

    menuFile->Append(wxID_EXIT, "&Exit...",
        "Close the editor");

    wxMenu* menuView = new wxMenu();

    menuView->Append(ID_Preferences, "&Preferences...", "Configure the editor");

    menuView->AppendSeparator();

    menuView->Append(ID_BrowserPackage, "&Package Browser...", "Open a Package browser instance");
    menuView->Append(ID_BrowserClass, "&Class Browser...", "Open an Class browser instance");
    menuView->Append(ID_BrowserSound, "&Sound Browser...", "Open a Sound browser instance");
    menuView->Append(ID_BrowserMusic, "&Music Browser...", "Open a Music browser instance");
    menuView->Append(ID_BrowserGraphics, "&Graphics Browser...", "Open a Graphics browser instance");
    menuView->Append(ID_BrowserMesh, "&Mesh Browser...", "Open a mesh browser instance");

    menuView->AppendSeparator();

    menuView->Append(ID_ViewLog, "&Show Log...", "Show me the console fax");
    menuView->Append(ID_ActiveTools, "&Show Tools...", "Show all running tools");

    wxMenu* menuTools = new wxMenu();

    menuTools->Append(ID_MapEditor, "&Map Editor...", "New editor tab.");
    menuTools->Append(ID_MeshEditor, "&Mesh Editor...", "Used for advanced brush-building, quick-prefabs, 2d-shape editor, etc.");

    wxMenu* menuHelp = new wxMenu();

    menuHelp->Append(ID_Manual, "&Editor Manual...", "Documentation shipped with the editor");
    menuHelp->Append(wxID_ABOUT, "&About OpenUE...", "About OpenUE");

    wxMenuBar* menuBar = new wxMenuBar();

    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuView, "&View");
    menuBar->Append(menuTools, "&Tools");
    menuBar->Append(menuHelp, "&Help");

    SetMenuBar(menuBar);

    wxBoxSizer* contentSizer = new wxBoxSizer(wxVERTICAL);

    wxPanel* toolBar = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(-1, 40), wxBORDER_RAISED);

    contentSizer->Add(toolBar, 0, wxALIGN_TOP | wxALIGN_LEFT | wxEXPAND);

    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);

    wxBitmapButton* button_New = new wxBitmapButton(toolBar, ID_New, EdEditor::g_bmpNew, wxDefaultPosition, wxSize(C_BUTTONSIZE, C_BUTTONSIZE));
    buttonSizer->Add(button_New, 0, wxALIGN_CENTRE_VERTICAL);

    wxBitmapButton* button_Open = new wxBitmapButton(toolBar, ID_Open, EdEditor::g_bmpDir, wxDefaultPosition, wxSize(C_BUTTONSIZE, C_BUTTONSIZE));
    buttonSizer->Add(button_Open, 0, wxALIGN_CENTRE_VERTICAL);

    wxBitmapButton* button_Save = new wxBitmapButton(toolBar, ID_Save, EdEditor::g_bmpSave, wxDefaultPosition, wxSize(C_BUTTONSIZE, C_BUTTONSIZE));
    buttonSizer->Add(button_Save, 0, wxALIGN_CENTRE_VERTICAL);

    buttonSizer->AddSpacer(C_BUTTONSIZE);

    wxBitmapButton* button_PackageBrowser = new wxBitmapButton(toolBar, ID_BrowserPackage, EdEditor::g_bmpPackage, wxDefaultPosition, wxSize(C_BUTTONSIZE, C_BUTTONSIZE));
    buttonSizer->Add(button_PackageBrowser, 0, wxALIGN_CENTRE_VERTICAL);

    wxBitmapButton* button_ClassBrowser = new wxBitmapButton(toolBar, ID_BrowserClass, EdEditor::g_bmpClass, wxDefaultPosition, wxSize(C_BUTTONSIZE, C_BUTTONSIZE));
    buttonSizer->Add(button_ClassBrowser, 0, wxALIGN_CENTRE_VERTICAL);

    wxBitmapButton* button_SoundBrowser = new wxBitmapButton(toolBar, ID_BrowserSound, EdEditor::g_bmpSound, wxDefaultPosition, wxSize(C_BUTTONSIZE, C_BUTTONSIZE));
    buttonSizer->Add(button_SoundBrowser, 0, wxALIGN_CENTRE_VERTICAL);

    wxBitmapButton* button_MusicBrowser = new wxBitmapButton(toolBar, ID_BrowserMusic, EdEditor::g_bmpMusic, wxDefaultPosition, wxSize(C_BUTTONSIZE, C_BUTTONSIZE));
    buttonSizer->Add(button_MusicBrowser, 0, wxALIGN_CENTRE_VERTICAL);

    wxBitmapButton* button_GraphicsBrowser = new wxBitmapButton(toolBar, ID_BrowserGraphics, EdEditor::g_bmpGraphics, wxDefaultPosition, wxSize(C_BUTTONSIZE, C_BUTTONSIZE));
    buttonSizer->Add(button_GraphicsBrowser, 0, wxALIGN_CENTRE_VERTICAL);

    wxBitmapButton* button_MeshBrowser = new wxBitmapButton(toolBar, ID_BrowserMesh, EdEditor::g_bmpMesh, wxDefaultPosition, wxSize(C_BUTTONSIZE, C_BUTTONSIZE));
    buttonSizer->Add(button_MeshBrowser, 0, wxALIGN_CENTRE_VERTICAL);

    toolBar->SetSizer(buttonSizer);

    wxPanel* tabBar = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(-1, 24), wxBORDER_NONE);
    contentSizer->Add(tabBar, 0, wxALIGN_TOP | wxEXPAND);

    wxBoxSizer* contentSizer2 = new wxBoxSizer(wxHORIZONTAL);
    contentSizer->Add(contentSizer2, 1, wxEXPAND);

    wxPanel* toolBar2 = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(80, -1), wxBORDER_RAISED);
    contentSizer2->Add(toolBar2, 0, wxALIGN_LEFT | wxEXPAND);

    wxPanel* backGround = new wxPanel(this);
    backGround->SetBackgroundColour(wxColour(80, 80, 80));
    contentSizer2->Add(backGround, 1, wxEXPAND);

    SetSizer(contentSizer);

    CreateStatusBar();
    SetStatusText("Welcome to OpenUEd!");
}

EdEditorFrame::~EdEditorFrame()
{
}

void EdEditorFrame::OnExit(wxCommandEvent& event)
{
    Close(true);
}

void EdEditorFrame::OnAbout(wxCommandEvent& event)
{
    new EdAbout( this );
}

void EdEditorFrame::EVT_New(wxCommandEvent& event)
{
    event.Skip();
}

void EdEditorFrame::EVT_Open(wxCommandEvent& event)
{
    event.Skip();
}

void EdEditorFrame::EVT_Save(wxCommandEvent& event)
{
    event.Skip();
}

void EdEditorFrame::EVT_Import(wxCommandEvent& event)
{
    event.Skip();
}

void EdEditorFrame::EVT_Export(wxCommandEvent& event)
{
    event.Skip();
}

void EdEditorFrame::EVT_Preferences(wxCommandEvent& event)
{
    event.Skip();
}

void EdEditorFrame::EVT_BrowserPackage(wxCommandEvent& event)
{
    //new EdPackageBrowser();
    event.Skip();
}

void EdEditorFrame::EVT_BrowserClass(wxCommandEvent& event)
{
    new EdClassBrowser( UObject::StaticClass() );
    event.Skip();
}

void EdEditorFrame::EVT_BrowserSound(wxCommandEvent& event)
{
  TArray<UClass*> classes;
  classes.PushBack( USound::StaticClass() );

  new EdObjectBrowser( classes, false );
  event.Skip();
}

void EdEditorFrame::EVT_BrowserMusic(wxCommandEvent& event)
{
  TArray<UClass*> classes;
  classes.PushBack( UMusic::StaticClass() );

  new EdObjectBrowser( classes, false );
  event.Skip();
}

void EdEditorFrame::EVT_BrowserTexture(wxCommandEvent& event)
{
  TArray<UClass*> classes;
  classes.PushBack( UTexture::StaticClass() );

  new EdObjectBrowser( classes, false );
  event.Skip();
}

void EdEditorFrame::EVT_BrowserMesh(wxCommandEvent& event)
{
  TArray<UClass*> classes;
  classes.PushBack( UModel::StaticClass() );

  new EdObjectBrowser( classes, false );
  event.Skip();
}

void EdEditorFrame::EVT_ViewLog(wxCommandEvent& event)
{
    event.Skip();
}

void EdEditorFrame::EVT_ActiveTools(wxCommandEvent& event)
{
    event.Skip();
}

void EdEditorFrame::EVT_MapEditor(wxCommandEvent& event)
{
    event.Skip();
}

void EdEditorFrame::EVT_MeshEditor(wxCommandEvent& event)
{
    event.Skip();
}

void EdEditorFrame::EVT_Manual(wxCommandEvent& event)
{
    event.Skip();
}

bool EditorApp::OnInit()
{
    EdEditor::sm_MainFrame = NULL;

    wxInitAllImageHandlers();

    EdEditor::g_IcoInit();

    if (LibunrInit( EdEditor::GamePromptHandler, NULL, true ) == false)
        return false;

    UPackage* EditorPkg = UPackage::StaticLoadPackage("Editor");

    if (!EditorPkg)
    {
        GLogf(LOG_CRIT, "Required package 'Editor' is missing");
        return false;
    }

    UClass* EditorEngineClass = (UClass*)UObject::StaticLoadObject(EditorPkg, "EditorEngine", UClass::StaticClass(), NULL, true);
    if (!EditorEngineClass)
    {
        GLogf(LOG_CRIT, "Failed to load EditorEngine class");
        return false;
    }

    GEngine = (UEngine*)EditorEngineClass->CreateObject();
    if (!GEngine->Init())
    {
        GLogf(LOG_CRIT, "Editor Engine init failed");
        return false;
    }

    for (size_t i = 0; i < UPackage::GetLoadedPackages()->Size(); i++)
    {
      if( (*UPackage::GetLoadedPackages())[i] == NULL )
        continue;

      (*UPackage::GetLoadedPackages())[i]->LoadEditableTypes();
    }

    // Start ticking loop
    EdEditor::LastTime = USystem::GetSeconds();
    Connect(wxID_ANY, wxEVT_IDLE, wxIdleEventHandler( EdEditorFrame::DoTick ) );

    EdEditor::sm_MainFrame = new EdEditorFrame("libunr-editor", wxPoint(-1, -1),
        wxSize(wxSystemSettings::GetMetric(wxSYS_SCREEN_X), wxSystemSettings::GetMetric(wxSYS_SCREEN_Y)));

    EdEditor::sm_MainFrame->Maximize();
    EdEditor::sm_MainFrame->Show(true);

    return true;
}

EdAbout::EdAbout( wxWindow* Parent )
    : wxFrame( Parent, -1, "About", wxDefaultPosition, wxSize( 420, 400 ) )
{
    wxBoxSizer* ContentSizer = new wxBoxSizer( wxVERTICAL );

    wxStaticText* aboutlibunr = new wxStaticText( this, -1, "LIBUNR\nCopyright (C) 2018-2019 Adam Smith", wxDefaultPosition, wxDefaultSize );
    ContentSizer->Add( aboutlibunr, 0, wxALL | wxALIGN_CENTRE_HORIZONTAL, 2 );

    wxStaticText* aboutlibunr2 = new wxStaticText( this, -1, "Open Source Re-Implementation of Unreal Engine 1", wxDefaultPosition, wxDefaultSize );
    ContentSizer->Add( aboutlibunr2, 0, wxALL | wxALIGN_CENTRE_HORIZONTAL, 2 );
    aboutlibunr2->Wrap( 400 );

    wxTextCtrl* libunrlink = new wxTextCtrl ( this, -1, "https://bitbucket.org/Xaleros/libunr/src/master/", wxDefaultPosition, wxSize( 400, 20 ), wxTE_READONLY );
    ContentSizer->Add( libunrlink, 0, wxALL | wxALIGN_CENTRE_HORIZONTAL, 2 );

    wxStaticText* aboutlibunreditor = new wxStaticText( this, -1, "LIBUNR-EDITOR\nCopyright (C) 2018-2019 Adam Smith\nWritten by Jesse \"Hyzoran\" Kowalik", wxDefaultPosition, wxDefaultSize );
    ContentSizer->Add( aboutlibunreditor, 0, wxALL | wxALIGN_CENTRE_HORIZONTAL, 2 );

    wxStaticText* aboutlibunreditor2 = new wxStaticText( this, -1, "Graphical Frontend and Development suite for libunr", wxDefaultPosition, wxDefaultSize );
    ContentSizer->Add( aboutlibunreditor2, 0, wxALL | wxALIGN_CENTRE_HORIZONTAL, 2 );
    aboutlibunreditor2->Wrap( 400 );

    wxTextCtrl* openuelink = new wxTextCtrl( this, -1, "https://bitbucket.org/Xaleros/OpenUE/", wxDefaultPosition, wxSize( 400, 20 ), wxTE_READONLY );
    ContentSizer->Add( openuelink, 0, wxALL | wxALIGN_CENTRE_HORIZONTAL, 2 );

    wxPanel* padding = new wxPanel( this );
    ContentSizer->Add( padding, 1, wxALL, 0 );

    wxButton* closeb = new wxButton( this, wxID_OK, "Ok" );
    ContentSizer->Add( closeb, 0, wxALIGN_BOTTOM | wxALIGN_CENTRE_HORIZONTAL );

    Connect( wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( EdAbout::OnClose ) );

    SetSizer( ContentSizer );

    closeb->SetFocus();
    Centre();

    SetMinSize( wxSize( 420, 400 ) );
    Show( true );
}

void EdAbout::OnClose( wxCommandEvent& event )
{
    Close( true );
}

wxBEGIN_EVENT_TABLE(EdEditorFrame, wxFrame)
    EVT_MENU(wxID_EXIT, EdEditorFrame::OnExit)
    EVT_MENU(wxID_ABOUT, EdEditorFrame::OnAbout)
    EVT_MENU(ID_New, EdEditorFrame::EVT_New)
    EVT_MENU(ID_Open, EdEditorFrame::EVT_Open)
    EVT_MENU(ID_Save, EdEditorFrame::EVT_Save)
    EVT_MENU(ID_Import, EdEditorFrame::EVT_Import)
    EVT_MENU(ID_Export, EdEditorFrame::EVT_Export)
    EVT_MENU(ID_Preferences, EdEditorFrame::EVT_Preferences)
    EVT_MENU(ID_BrowserPackage, EdEditorFrame::EVT_BrowserPackage)
    EVT_BUTTON(ID_BrowserPackage, EdEditorFrame::EVT_BrowserPackage)
    EVT_MENU(ID_BrowserClass, EdEditorFrame::EVT_BrowserClass)
    EVT_BUTTON(ID_BrowserClass, EdEditorFrame::EVT_BrowserClass)
    EVT_MENU(ID_BrowserSound, EdEditorFrame::EVT_BrowserSound)
    EVT_BUTTON(ID_BrowserSound, EdEditorFrame::EVT_BrowserSound)
    EVT_MENU(ID_BrowserMusic, EdEditorFrame::EVT_BrowserMusic)
    EVT_BUTTON(ID_BrowserMusic, EdEditorFrame::EVT_BrowserMusic)
    EVT_MENU(ID_BrowserGraphics, EdEditorFrame::EVT_BrowserTexture)
    EVT_BUTTON(ID_BrowserGraphics, EdEditorFrame::EVT_BrowserTexture)
    EVT_MENU(ID_BrowserMesh, EdEditorFrame::EVT_BrowserMesh)
    EVT_BUTTON(ID_BrowserMesh, EdEditorFrame::EVT_BrowserMesh)
    EVT_MENU(ID_ViewLog, EdEditorFrame::EVT_ViewLog)
    EVT_MENU(ID_ActiveTools, EdEditorFrame::EVT_ActiveTools)
    EVT_MENU(ID_MapEditor, EdEditorFrame::EVT_MapEditor)
    EVT_MENU(ID_MeshEditor, EdEditorFrame::EVT_MeshEditor)
    EVT_MENU(ID_Manual, EdEditorFrame::EVT_Manual)
wxEND_EVENT_TABLE()

wxIMPLEMENT_APP(EditorApp);
