/*===========================================================================*\
|*  libunr - An open source Unreal Engine 1 implementation in library form   *|
|*  Copyright (C) 2018-2019  Adam W.E. Smith                                 *|
|*  libunr-editor - An open source development suite for Unreal Engine 1     *|
|*  games and libunr                                                         *|
|*  Copyright (C) 2018-2019  Adam W.E. Smith                                 *|
|*                                                                           *|
|*  This program is free software: you can redistribute it and/or modify     *|
|*  it under the terms of the GNU Affero General Public License as           *|
|*  published by the Free Software Foundation, either version 3 of the       *|
|*  License, or (at your option) any later version.                          *|
|*                                                                           *|
|*  This program is distributed in the hope that it will be useful,          *|
|*  but WITHOUT ANY WARRANTY; without even the implied warranty of           *|
|*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *|
|*  GNU Affero General Public License for more details.                      *|
|*                                                                           *|
|*  You should have received a copy of the GNU Affero General Public License *|
|*  along with this program.  If not, see <https://www.gnu.org/licenses/>.   *|
\*===========================================================================*/

/*========================================================================
 * libunr-editor.cpp - Editor main.
 * 
 * written by Jesse 'Hyzoran' Kowalik
 *========================================================================
*/

#include "private/libunr-editor.h"

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
        menuView->Append(ID_BrowserAudio, "&Audio Browser...", "Open an Audio browser\
instance");
        menuView->Append(ID_BrowserMusic, "&Music Browser...", "Open a Music browser\
instance");
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
    
    wxBitmap bitmap_NewMap( "res/bitmap/New.png", wxBITMAP_TYPE_PNG );
    wxBitmapButton* button_NewMap = new wxBitmapButton( toolBar, ID_New, bitmap_NewMap, wxDefaultPosition, wxSize( C_BUTTONSIZE,C_BUTTONSIZE ) );
    buttonSizer->Add( button_NewMap, 0, wxALIGN_CENTRE_VERTICAL );
    
    wxBitmap bitmap_OpenMap( "res/bitmap/Open.png", wxBITMAP_TYPE_PNG );
    wxBitmapButton* button_OpenMap = new wxBitmapButton( toolBar, ID_Open, bitmap_OpenMap, wxDefaultPosition, wxSize( C_BUTTONSIZE,C_BUTTONSIZE ) );
    buttonSizer->Add( button_OpenMap, 0, wxALIGN_CENTRE_VERTICAL );
    
    wxBitmap bitmap_Save( "res/bitmap/Save.png", wxBITMAP_TYPE_PNG );
    wxBitmapButton* button_Save = new wxBitmapButton( toolBar, ID_Save, bitmap_Save, wxDefaultPosition, wxSize( C_BUTTONSIZE,C_BUTTONSIZE ) );
    buttonSizer->Add( button_Save, 0, wxALIGN_CENTRE_VERTICAL );
    
    buttonSizer->AddSpacer( C_BUTTONSIZE );
    
    wxBitmap bitmap_PackageBrowser( "res/bitmap/PackageBrowser.png", wxBITMAP_TYPE_PNG );
    wxBitmapButton* button_PackageBrowser = new wxBitmapButton( toolBar, ID_BrowserPackage, bitmap_PackageBrowser, wxDefaultPosition, wxSize( C_BUTTONSIZE,C_BUTTONSIZE ) );
    buttonSizer->Add( button_PackageBrowser, 0, wxALIGN_CENTRE_VERTICAL );
    
    wxBitmap bitmap_ClassBrowser( "res/bitmap/ClassBrowser.png", wxBITMAP_TYPE_PNG );
    wxBitmapButton* button_ClassBrowser = new wxBitmapButton( toolBar, ID_BrowserClass, bitmap_ClassBrowser, wxDefaultPosition, wxSize( C_BUTTONSIZE,C_BUTTONSIZE ) );
    buttonSizer->Add( button_ClassBrowser, 0, wxALIGN_CENTRE_VERTICAL );
    
    wxBitmap bitmap_AudioBrowser( "res/bitmap/AudioBrowser.png", wxBITMAP_TYPE_PNG );
    wxBitmapButton* button_AudioBrowser = new wxBitmapButton( toolBar, ID_BrowserAudio, bitmap_AudioBrowser, wxDefaultPosition, wxSize( C_BUTTONSIZE,C_BUTTONSIZE ) );
    buttonSizer->Add( button_AudioBrowser, 0, wxALIGN_CENTRE_VERTICAL );
    
    wxBitmap bitmap_MusicBrowser( "res/bitmap/MusicBrowser.png", wxBITMAP_TYPE_PNG );
    wxBitmapButton* button_MusicBrowser = new wxBitmapButton( toolBar, ID_BrowserMusic, 
bitmap_MusicBrowser, wxDefaultPosition, wxSize( C_BUTTONSIZE,C_BUTTONSIZE ) );
    buttonSizer->Add( button_MusicBrowser, 0, wxALIGN_CENTRE_VERTICAL );
    
    wxBitmap bitmap_GraphicsBrowser( "res/bitmap/GraphicsBrowser.png", wxBITMAP_TYPE_PNG );
    wxBitmapButton* button_GraphicsBrowser = new wxBitmapButton( toolBar, ID_BrowserGraphics, bitmap_GraphicsBrowser, wxDefaultPosition, wxSize( C_BUTTONSIZE,C_BUTTONSIZE ) );
    buttonSizer->Add( button_GraphicsBrowser, 0, wxALIGN_CENTRE_VERTICAL );
    
    wxBitmap bitmap_MeshBrowser( "res/bitmap/MeshBrowser.png", wxBITMAP_TYPE_PNG );
    wxBitmapButton* button_MeshBrowser = new wxBitmapButton( toolBar, ID_BrowserMesh, bitmap_MeshBrowser, wxDefaultPosition, wxSize( C_BUTTONSIZE,C_BUTTONSIZE ) );
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
    
    this->SetSizer(contentSizer);
    
    CreateStatusBar();
    SetStatusText( "Welcome to libunr!" );
}

static bool EdEditorFrame::NewTool( const EdToolFrame* Tool )
{
    if( sm_EmptySlots > 0 ) //A slot was freed earlier, find and use that slot.
    {
        for( size_t i = 0; i++; i<sm_ToolArray.Size() )
        {
            if( sm_ToolArray[i] == NULL )
            {
                sm_ToolArray[i] = (EdToolFrame*)Tool; //Manual cast workaround for wx class.
                sm_EmptySlots--; //We used a slot.
                return true;
            }
        }
        //None found? Something went wrong.
        Editor_Log( "Warning: EdEditorFrame::NewTool : sm_EmptySlots > 0, but none found in sm_ToolArray! Possible memory corruption and/or leak.\nPushing new tool to end of array...") ;
    }
    sm_ToolArray.PushBack( (EdToolFrame*)Tool );
    return true;
}

static bool EdEditorFrame::KillTool( const EdToolFrame* Tool )
{
    for( size_t i; i++; i<sm_ToolArray.Size() )
    {
        if( sm_ToolArray[i]==Tool )
        {
            m_ToolArray[i] = NULL;
            sm_EmptySlots++;
            return true;
        }
        Editor_Log( "Error: EdEditor::UnRegTool() : Unregistered tool that does not exist in sm_ToolArray!") ;
        return false;
    }
}

static void Editor_Log( const wxString& Msg )
{
    std::cout << Msg;
}

static EdEditorFrame* GetFrame()
{
    return sm_Frame;
}

void EdEditorFrame::EVT_New( wxCommandEvent& event ){}

void EdEditorFrame::EVT_Open( wxCommandEvent& event ){}

void EdEditorFrame::EVT_Save( wxCommandEvent& event ){}

void EdEditorFrame::EVT_Import( wxCommandEvent& event ){}

void EdEditorFrame::EVT_Export( wxCommandEvent& event ){}

void EdEditorFrame::EVT_Preferences( wxCommandEvent& event ){}

void EdEditorFrame::EVT_BrowserPackage( wxCommandEvent& event )
{
}

void EdEditorFrame::EVT_BrowserClass( wxCommandEvent& event ){}

void EdEditorFrame::EVT_BrowserAudio( wxCommandEvent& event ){}

void EdEditorFrame::EVT_BrowserMusic( wxCommandEvent& event ){}

void EdEditorFrame::EVT_BrowserGraphics( wxCommandEvent& event ){}

void EdEditorFrame::EVT_BrowserMesh( wxCommandEvent& event ){}

void EdEditorFrame::EVT_ViewLog( wxCommandEvent& event ){}

void EdEditorFrame::EVT_ActiveTools( wxCommandEvent& event ){}

void EdEditorFrame::EVT_MapEditor( wxCommandEvent& event ){}

void EdEditorFrame::EVT_MeshEditor( wxCommandEvent& event ){}

void EdEditorFrame::EVT_Manual( wxCommandEvent& event ){}

bool WXAPP_EdEditor::OnInit()
{
    EdEditorFrame* frame = new EdEditorFrame( "libunr-editor", wxPoint(-1,-1), wxSize( wxSystemSettings::GetMetric ( wxSYS_SCREEN_X ), wxSystemSettings::GetMetric ( wxSYS_SCREEN_Y ) ) );
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
    EVT_MENU(ID_BrowserClass, EdEditorFrame::EVT_BrowserClass)
    EVT_MENU(ID_Audio, EdEditorFrame::EVT_BrowserAudio)
    EVT_MENU(ID_Music, EdEditorFrame::EVT_BrowserMusic)
    EVT_MENU(ID_Graphics, EdEditorFrame::EVT_BrowserGraphics)
    EVT_MENU(ID_Mesh, EdEditorFrame::EVT_BrowserMesh)
    EVT_MENU(ID_ViewLog, EdEditorFrame::EVT_ViewLog)
    EVT_MENU(ID_ActiveTools, EdEditorFrame::EVT_ActiveTools)
    EVT_MENU(ID_MapEditor, EdEditorFrame::EVT_MapEditor)
    EVT_MENU(ID_MeshEditor, EdEditorFrame::EVT_MeshEditor)
    EVT_MENU(ID_Manual, EdEditorFrame::EVT_Manual)
wxEND_EVENT_TABLE()

wxIMPLEMENT_APP(WXAPP_EdEditor)
