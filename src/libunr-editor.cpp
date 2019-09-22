/*===========================================================================*\
|*  libunr-editor - An open source development suite for Unreal Engine 1     *|
|*  games and libunr                                                         *|
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
 * libunr-editor.cpp - Editor main.
 * 
 * written by Jesse 'Hyzoran' Kowalik
 *========================================================================
*/

#include "libunr-editor.h"

EdEditorFrame* EdEditorFrame::sm_Frame;
TArray<EdToolFrame*> EdEditorFrame::sm_ToolArray;
size_t EdEditorFrame::sm_EmptySlots;
TArray<UPackage*> EdEditorFrame::sm_Packages;

wxIcon EdEditorFrame::m_File;
wxIcon EdEditorFrame::m_Dir;
wxIcon EdEditorFrame::m_Save;

wxIcon EdBrowser::m_icoPackage;
wxIcon EdBrowser::m_icoClass;
wxIcon EdBrowser::m_icoAudio;
wxIcon EdBrowser::m_icoMusic;
wxIcon EdBrowser::m_icoGraphics;
wxIcon EdBrowser::m_icoMesh;

EdEditorFrame::EdEditorFrame( const wxString& Title, const wxPoint& Pos, const wxSize& Size )
    : wxFrame( NULL, wxID_ANY, Title, Pos, Size )
{
    sm_Frame = this;
    
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
        menuView->Append(ID_BrowserAudio, "&Audio Browser...", "Open an Audio browser instance");
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
        menuHelp->Append(wxID_ABOUT, "&About libunr...", "About LIBUNR");
        
    wxMenuBar* menuBar= new wxMenuBar();
        
        menuBar->Append( menuFile, "&File" );
        menuBar->Append( menuView, "&View" );
        menuBar->Append( menuTools, "&Tools" );
        menuBar->Append( menuHelp, "&Help" );
    
    SetMenuBar( menuBar );
    
    wxBoxSizer* contentSizer = new wxBoxSizer( wxVERTICAL );
    
    wxPanel* toolBar = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxSize( -1, 40 ), wxBORDER_RAISED );
    
    contentSizer->Add( toolBar, 0, wxALIGN_TOP | wxALIGN_LEFT | wxEXPAND );
    
    wxBoxSizer* buttonSizer = new wxBoxSizer( wxHORIZONTAL );
    
    #define C_BUTTONSIZE 34
    #define C_BUTTONCOLOUR wxColour( 130, 130, 130 )
    
    wxBitmapButton* button_NewMap = new wxBitmapButton( toolBar, ID_New, EdEditorFrame::m_File, wxDefaultPosition, wxSize( C_BUTTONSIZE,C_BUTTONSIZE ) );
    buttonSizer->Add( button_NewMap, 0, wxALIGN_CENTRE_VERTICAL );
    
    wxBitmapButton* button_OpenMap = new wxBitmapButton( toolBar, ID_Open, EdEditorFrame::m_Dir, wxDefaultPosition, wxSize( C_BUTTONSIZE,C_BUTTONSIZE ) );
    buttonSizer->Add( button_OpenMap, 0, wxALIGN_CENTRE_VERTICAL );
    
    wxBitmapButton* button_Save = new wxBitmapButton( toolBar, ID_Save, EdEditorFrame::m_Save, wxDefaultPosition, wxSize( C_BUTTONSIZE,C_BUTTONSIZE ) );
    buttonSizer->Add( button_Save, 0, wxALIGN_CENTRE_VERTICAL );
    
    buttonSizer->AddSpacer( C_BUTTONSIZE );
    
    wxBitmapButton* button_PackageBrowser = new wxBitmapButton( toolBar, ID_BrowserPackage, EdBrowser::m_icoPackage, wxDefaultPosition, wxSize( C_BUTTONSIZE,C_BUTTONSIZE ) );
    buttonSizer->Add( button_PackageBrowser, 0, wxALIGN_CENTRE_VERTICAL );
    
    wxBitmapButton* button_ClassBrowser = new wxBitmapButton( toolBar, ID_BrowserClass, EdBrowser::m_icoClass, wxDefaultPosition, wxSize( C_BUTTONSIZE,C_BUTTONSIZE ) );
    buttonSizer->Add( button_ClassBrowser, 0, wxALIGN_CENTRE_VERTICAL );
    
    wxBitmapButton* button_AudioBrowser = new wxBitmapButton( toolBar, ID_BrowserAudio, EdBrowser::m_icoAudio, wxDefaultPosition, wxSize( C_BUTTONSIZE,C_BUTTONSIZE ) );
    buttonSizer->Add( button_AudioBrowser, 0, wxALIGN_CENTRE_VERTICAL );
    
    wxBitmapButton* button_MusicBrowser = new wxBitmapButton( toolBar, ID_BrowserMusic, EdBrowser::m_icoMusic, wxDefaultPosition, wxSize( C_BUTTONSIZE,C_BUTTONSIZE ) );
    buttonSizer->Add( button_MusicBrowser, 0, wxALIGN_CENTRE_VERTICAL );
    
    wxBitmapButton* button_GraphicsBrowser = new wxBitmapButton( toolBar, ID_BrowserGraphics, EdBrowser::m_icoGraphics, wxDefaultPosition, wxSize( C_BUTTONSIZE,C_BUTTONSIZE ) );
    buttonSizer->Add( button_GraphicsBrowser, 0, wxALIGN_CENTRE_VERTICAL );
    
    wxBitmapButton* button_MeshBrowser = new wxBitmapButton( toolBar, ID_BrowserMesh, EdBrowser::m_icoMesh, wxDefaultPosition, wxSize( C_BUTTONSIZE,C_BUTTONSIZE ) );
    buttonSizer->Add( button_MeshBrowser, 0, wxALIGN_CENTRE_VERTICAL );
    
    toolBar->SetSizer(buttonSizer);
    
    wxPanel* tabBar = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxSize( -1, 24 ), wxBORDER_NONE );
    contentSizer->Add( tabBar, 0, wxALIGN_TOP | wxEXPAND );
    
    wxBoxSizer* contentSizer2 = new wxBoxSizer( wxHORIZONTAL );
    contentSizer->Add( contentSizer2, 1, wxEXPAND );
    
    wxPanel* toolBar2 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxSize( 80, -1 ), wxBORDER_RAISED );
    contentSizer2->Add( toolBar2, 0, wxALIGN_LEFT | wxEXPAND );
    
    wxPanel* backGround = new wxPanel( this );
    backGround->SetBackgroundColour( wxColour( 80, 80, 80 ) );
    contentSizer2->Add( backGround, 1, wxEXPAND );
    
    SetSizer(contentSizer);
    
    CreateStatusBar();
    SetStatusText( "Welcome to libunr!" );
}

EdEditorFrame::~EdEditorFrame()
{
    for( size_t i = 0; i < sm_ToolArray.Size(); i++ )
    {
        if( sm_ToolArray[i] != NULL )
        {
            delete sm_ToolArray[i];
        }
    }
}

bool EdEditorFrame::NewTool( const EdToolFrame* Tool )
{
    if( sm_EmptySlots > 0 ) //A slot was freed earlier, find and use that slot.
    {
        for( size_t i = 0; i < sm_ToolArray.Size(); i++ )
        {
            if( sm_ToolArray[i] == NULL )
            {
                sm_ToolArray[i] = (EdToolFrame*)Tool; //Manual cast workaround for wx class.
                sm_EmptySlots--; //We used a slot.
                return true;
            }
        }
        //None found? Something went wrong.
        Editor_Log( "Warning: EdEditorFrame::NewTool : sm_EmptySlots > 0, but none found in sm_ToolArray!\
         Possible memory corruption and/or leak.\nPushing new tool to end of array...") ;
    }
    sm_ToolArray.PushBack( (EdToolFrame*)Tool );
    return true;
}

bool EdEditorFrame::KillTool( const EdToolFrame* Tool )
{
    for( size_t i = 0; i < sm_ToolArray.Size(); i++ )
    {
        if( sm_ToolArray[i]==Tool )
        {
            sm_ToolArray[i] = NULL;
            sm_EmptySlots++;
            return true;
        }
        Editor_Log( "Error: EdEditor::UnRegTool() : Unregistered tool that does not exist in sm_ToolArray!") ;
        return false;
    }
}

void EdEditorFrame::Editor_Log( const wxString& Msg )
{
    std::cout << Msg;
}

EdEditorFrame* EdEditorFrame::GetFrame()
{
    return sm_Frame;
}

TArray<UPackage*>* EdEditorFrame::GetPackages()
{
    return &sm_Packages;
}

void EdEditorFrame::LoadPackages( const wxArrayString& Paths )
{
    for( size_t i = 0; i<Paths.GetCount(); i++ )
    {
        UPackage* p = UPackage::StaticLoadPackage( Paths[i], false );
        
        p->LoadEditableTypes();
    }
    
    wxArrayString strAry;
    
    for( size_t i = 0; i<UPackage::GetLoadedPackages()->Size(); i++ )
    {
        strAry.Add( (*UPackage::GetLoadedPackages())[i]->Name.Data() );
    }
    
    if( strAry.GetCount() > 0 )
    {
        for( size_t i = 0; i<sm_ToolArray.Size(); i++ )
        {
            if( sm_ToolArray[i]->m_ToolType == TOOL_Browser )
            {
                static_cast<EdBrowser*>( sm_ToolArray[i] )->UpdatePackageList( strAry );
            }
        }
    }
}

void EdEditorFrame::OnExit( wxCommandEvent& event )
{
    Close( true );
}

void EdEditorFrame::OnAbout( wxCommandEvent& event )
{
    if( m_bAboutUp )
        return;
    else
    {
        new EdAbout( this, &m_bAboutUp );
        m_bAboutUp = true;
    }
}

void EdEditorFrame::EVT_New( wxCommandEvent& event ){}

void EdEditorFrame::EVT_Open( wxCommandEvent& event ){}

void EdEditorFrame::EVT_Save( wxCommandEvent& event ){}

void EdEditorFrame::EVT_Import( wxCommandEvent& event ){}

void EdEditorFrame::EVT_Export( wxCommandEvent& event ){}

void EdEditorFrame::EVT_Preferences( wxCommandEvent& event ){}

void EdEditorFrame::EVT_BrowserPackage( wxCommandEvent& event )
{
    new EdBrowser( BRWFLG_Package, false );
}

void EdEditorFrame::EVT_BrowserClass( wxCommandEvent& event )
{
    new EdBrowser( BRWFLG_Class, false );
}

void EdEditorFrame::EVT_BrowserAudio( wxCommandEvent& event )
{
    new EdBrowser( BRWFLG_Audio, false );
}

void EdEditorFrame::EVT_BrowserMusic( wxCommandEvent& event )
{
    new EdBrowser( BRWFLG_Music, false );
}

void EdEditorFrame::EVT_BrowserGraphics( wxCommandEvent& event )
{
    new EdBrowser( BRWFLG_Graphics, false );
}

void EdEditorFrame::EVT_BrowserMesh( wxCommandEvent& event )
{
    new EdBrowser( BRWFLG_Mesh, false );
}

void EdEditorFrame::EVT_ViewLog( wxCommandEvent& event ){}

void EdEditorFrame::EVT_ActiveTools( wxCommandEvent& event ){}

void EdEditorFrame::EVT_MapEditor( wxCommandEvent& event ){}

void EdEditorFrame::EVT_MeshEditor( wxCommandEvent& event ){}

void EdEditorFrame::EVT_Manual( wxCommandEvent& event ){}

EdGamePrompt::EdGamePrompt( TArray<char*>* Names )
    : wxDialog( NULL, wxID_ANY, "Select Game", wxDefaultPosition, wxSize(420,400) )
{
    wxBoxSizer* vsizer = new wxBoxSizer( wxVERTICAL );
    
    m_GameSize = Names->Size();
    m_Names = Names;
    wxString* strNames = new wxString[m_GameSize];
    
    for( size_t i = 0; i<m_GameSize; i++ )
    {
        strNames[i] = wxString::FromAscii( (*Names)[i] );
    }
    
    m_ChoiceBox = new wxListBox( this, ID_GameChoice, wxDefaultPosition, wxSize(420,-1), m_GameSize, strNames, wxLB_SINGLE | wxLB_NEEDED_SB );
        
        vsizer->Add( m_ChoiceBox );
        
    wxPanel* endPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize );
    
        wxBoxSizer* hsizer = new wxBoxSizer( wxHORIZONTAL );
        wxButton* confirmB = new wxButton( endPanel, ID_NewGameChoice, "Add" );
        wxButton* cancelB = new wxButton( endPanel, wxID_CANCEL, "Cancel" );
        
        hsizer->Add( confirmB, 0, wxALIGN_LEFT, 4 );
        hsizer->Add( cancelB, 0, wxALIGN_LEFT, 4 );
        endPanel->SetSizer( hsizer );
        
        vsizer->Add( endPanel );
        
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
    EndModal(0);
}

void EdGamePrompt::EVT_NewGame( wxCommandEvent& event )
{
    EdNewGameDialog newGamePrompt( m_GameSize, &m_GameSelect );
    
    if( newGamePrompt.ShowModal() == wxID_CANCEL )
        return;
    else
    {
        m_GameSelect = m_GameSize;
        EndModal(0);
    }
}

EdNewGameDialog::EdNewGameDialog( size_t NewIndex, int* ChoiceOut )
    : wxDialog( NULL, wxID_ANY, "Add Game" )
{
    m_GameSize = NewIndex;
    
    wxBoxSizer* vsizer = new wxBoxSizer( wxVERTICAL );
    
    wxStaticText* labelName = new wxStaticText( this, wxID_ANY, "Name:" );
    vsizer->Add( labelName, wxALIGN_LEFT );
    
    m_NameField = new wxTextCtrl( this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize );
    vsizer->Add( m_NameField, wxALIGN_LEFT );
    
    wxStaticText* exeName = new wxStaticText( this, wxID_ANY, "Executable Name:" );
    vsizer->Add( exeName, wxALIGN_LEFT );
    
    m_ExeField =  new wxTextCtrl( this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize );
    vsizer->Add( m_ExeField, wxALIGN_LEFT );
    
    wxStaticText* pathName = new wxStaticText( this, wxID_ANY, "Directory:" );
    vsizer->Add( pathName, wxALIGN_LEFT );
    
    wxBoxSizer* hsizer = new wxBoxSizer( wxHORIZONTAL );
    wxPanel* pathPanel = new wxPanel( this );
        
        m_PathField =  new wxTextCtrl( pathPanel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize );
        hsizer->Add( m_PathField, wxALIGN_LEFT );
        
        wxButton* pathB = new wxButton( pathPanel, ID_DirDialog, "..." );
        hsizer->Add( pathB, wxALIGN_LEFT );
        
        pathPanel->SetSizer(hsizer);
    
    vsizer->Add( pathPanel, wxALIGN_LEFT );
    vsizer->AddSpacer( 4 );
    
    wxBoxSizer* hsizer2 = new wxBoxSizer( wxHORIZONTAL );
    wxPanel* bPanel = new wxPanel( this );
        
        wxButton* confirmB = new wxButton( bPanel, ID_Confirm, "Confirm" );
        hsizer2->Add( confirmB, wxALIGN_LEFT );
        wxButton* cancelB = new wxButton( bPanel, wxID_CANCEL, "Cancel" );
        hsizer2->Add( cancelB, wxALIGN_LEFT );
        
        bPanel->SetSizer(hsizer2);
        
    vsizer->Add( bPanel, wxALIGN_LEFT );
    
    SetSizer( vsizer );
}

void EdNewGameDialog::EVT_Confirm( wxCommandEvent& event )
{
    //TODO: Verify directory
    
    GLibunrConfig->WriteString( "Game", "Name", m_NameField->GetLineText(0), m_GameSize );
    GLibunrConfig->WriteString( "Game", "Exec", m_ExeField->GetLineText(0), m_GameSize );
    GLibunrConfig->WriteString( "Game", "Path", m_PathField->GetLineText(0), m_GameSize );
    
    GLibunrConfig->Save();
    
    EndModal(0);
    
}

void EdNewGameDialog::EVT_DirDialog( wxCommandEvent& event )
{
    wxDirDialog file( this, "Game Directory", "", wxDD_DIR_MUST_EXIST  );
    if( file.ShowModal() == wxID_CANCEL )
        return;
    else
        m_PathField->ChangeValue( file.GetPath() );
}

int WXAPP_EdEditor::GamePromptHandler( TArray<char*>* Names )
{
    EdGamePrompt gamePrompt( Names );
    
    gamePrompt.ShowModal();
    
    return gamePrompt.GetSelected();
}

bool WXAPP_EdEditor::OnInit()
{
    wxInitAllImageHandlers();

    EdEditorFrame::m_File = wxIcon(wxString("res/bitmap/New.png"));
    EdEditorFrame::m_Dir = wxIcon(wxT("res/bitmap/Open.png"));
    EdEditorFrame::m_Save = wxIcon(wxT("res/bitmap/Save.png"));

    EdBrowser::m_icoPackage = wxIcon(wxT("res/bitmap/PackageBrowser.png"));
    EdBrowser::m_icoClass = wxIcon(wxT("res/bitmap/ClassBrowser.png"));
    EdBrowser::m_icoAudio = wxIcon(wxT("res/bitmap/AudioBrowser.png"));
    EdBrowser::m_icoMusic = wxIcon(wxT("res/bitmap/MusicBrowser.png"));
    EdBrowser::m_icoGraphics = wxIcon(wxT("res/bitmap/GraphicsBrowser.png"));
    EdBrowser::m_icoMesh = wxIcon(wxT("res/bitmap/MeshBrowser.png"));
    
    if( LibunrInit( GamePromptHandler, NULL, true ) == false )
        return true;

    for( size_t i = 0; i<UPackage::GetLoadedPackages()->Size(); i++ )
    {
        (*UPackage::GetLoadedPackages())[i]->LoadEditableTypes();
    }
    
    EdEditorFrame* frame = new EdEditorFrame( "libunr-editor", wxPoint(-1,-1), 
        wxSize( wxSystemSettings::GetMetric ( wxSYS_SCREEN_X ), wxSystemSettings::GetMetric ( wxSYS_SCREEN_Y ) ) );
    frame->Show(true);
    return true;
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
    EVT_MENU(ID_BrowserAudio, EdEditorFrame::EVT_BrowserAudio)
    EVT_BUTTON(ID_BrowserAudio, EdEditorFrame::EVT_BrowserAudio)
    EVT_MENU(ID_BrowserMusic, EdEditorFrame::EVT_BrowserMusic)
    EVT_BUTTON(ID_BrowserMusic, EdEditorFrame::EVT_BrowserMusic)
    EVT_MENU(ID_BrowserGraphics, EdEditorFrame::EVT_BrowserGraphics)
    EVT_BUTTON(ID_BrowserGraphics, EdEditorFrame::EVT_BrowserGraphics)
    EVT_MENU(ID_BrowserMesh, EdEditorFrame::EVT_BrowserMesh)
    EVT_BUTTON(ID_BrowserMesh, EdEditorFrame::EVT_BrowserMesh)
    EVT_MENU(ID_ViewLog, EdEditorFrame::EVT_ViewLog)
    EVT_MENU(ID_ActiveTools, EdEditorFrame::EVT_ActiveTools)
    EVT_MENU(ID_MapEditor, EdEditorFrame::EVT_MapEditor)
    EVT_MENU(ID_MeshEditor, EdEditorFrame::EVT_MeshEditor)
    EVT_MENU(ID_Manual, EdEditorFrame::EVT_Manual)
wxEND_EVENT_TABLE()

wxBEGIN_EVENT_TABLE(EdGamePrompt, wxDialog)
    EVT_LISTBOX_DCLICK(ID_GameChoice, EdGamePrompt::EVT_ChoiceSelect)
    EVT_BUTTON(ID_NewGameChoice, EdGamePrompt::EVT_NewGame)
wxEND_EVENT_TABLE()

wxBEGIN_EVENT_TABLE(EdNewGameDialog, wxDialog)
    EVT_BUTTON(ID_DirDialog, EdNewGameDialog::EVT_DirDialog)
    EVT_BUTTON(ID_Confirm, EdNewGameDialog::EVT_Confirm)
wxEND_EVENT_TABLE()

wxIMPLEMENT_APP(WXAPP_EdEditor);
