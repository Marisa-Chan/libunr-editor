#include "Browsers.h"

wxIcon EdBrowser::m_icoPackage = wxIcon(wxT("res/bitmap/PackageBrowser.png"));
wxIcon EdBrowser::m_icoClass = wxIcon(wxT("res/bitmap/ClassBrowser.png"));
wxIcon EdBrowser::m_icoAudio = wxIcon(wxT("res/bitmap/AudioBrowser.png"));
wxIcon EdBrowser::m_icoMusic = wxIcon(wxT("res/bitmap/MusicBrowser.png"));
wxIcon EdBrowser::m_icoGraphics = wxIcon(wxT("res/bitmap/GraphicsBrowser.png"));
wxIcon EdBrowser::m_icoMesh = wxIcon(wxT("res/bitmap/MeshBrowser.png"));
    
EdBrowser::EdBrowser( int BrowserFlags, bool bDock ) : EdToolFrame(bDock), m_BrowserFlags(BrowserFlags)
{
    //Default Viewmode per browser mode.
    if( m_BrowserFlags == BRWFLG_Class )
        m_ViewMode = VIEW_List;
    else if( m_BrowserFlags == BRWFLG_Audio )
        m_ViewMode = VIEW_List;
    else if( m_BrowserFlags == BRWFLG_Music )
        m_ViewMode = VIEW_List;
    else if( m_BrowserFlags == BRWFLG_Graphics )
        m_ViewMode = VIEW_Thumbnail;
    else if( m_BrowserFlags == BRWFLG_Mesh )
        m_ViewMode = VIEW_Thumbnail;
    else
        m_ViewMode = VIEW_Raw;
        
    wxMenu* menuFile = new wxMenu();
    
    m_MenuFile = new wxMenu();
    
        m_MenuFile->Append(ID_BrowserNew, "&New...",
					"Create a new Package");
        m_MenuFile->Append(ID_BrowserOpen, "&Open...",
					"Load a new Package");
        m_MenuFile->Append(ID_BrowserSave, "&Save... ",
					"Save modified Packages");
        
        m_MenuFile->AppendSeparator();
        
        m_MenuFile->Append(ID_BrowserImport, "&Import... ",
					"Import Assets into Package");
        m_MenuFile->Append(ID_BrowserExport, "&Export... ",
					"Export Objects from package");
					
	    m_MenuFile->AppendSeparator();
	    
	    m_MenuFile->Append(wxID_EXIT, "&Exit...", "");
					
    m_MenuViewMode = new wxMenu();
    
        m_MenuViewMode->AppendCheckItem(ID_BrowserViewMode_Raw, "Object View", "");
        m_MenuViewMode->AppendCheckItem(ID_BrowserViewMode_Thumbnail, "Tile View", "");
        m_MenuViewMode->AppendCheckItem(ID_BrowserViewMode_List, "List View", "");
        
        m_MenuViewMode->Check( ID_BrowserViewMode_Raw, m_ViewMode == VIEW_Raw );
        m_MenuViewMode->Check( ID_BrowserViewMode_Thumbnail, m_ViewMode == VIEW_Thumbnail );
        m_MenuViewMode->Check( ID_BrowserViewMode_List, m_ViewMode == VIEW_List );
        
        m_MenuViewMode->AppendSeparator();
        
        m_MenuViewMode->AppendCheckItem( ID_BrowserViewMode_Preview, "Display Preview", "" );
        
        m_MenuViewMode->Check( ID_BrowserViewMode_Preview, m_bPreview );
        
        m_MenuViewMode->AppendSeparator();
        
        m_MenuViewMode->AppendCheckItem( ID_BrowserViewMode_Class, "Classes", "" );
        m_MenuViewMode->AppendCheckItem( ID_BrowserViewMode_Audio, "Audio", "" );
        m_MenuViewMode->AppendCheckItem( ID_BrowserViewMode_Music, "Music", "" );
        m_MenuViewMode->AppendCheckItem( ID_BrowserViewMode_Graphics, "Graphics", "" );
        m_MenuViewMode->AppendCheckItem( ID_BrowserViewMode_Mesh, "Meshes", "" );
        
    m_MenuView = new wxMenu();
    
        m_MenuView->AppendCheckItem( ID_BrowserDock, "Dock", "" );
        m_MenuView->Check( ID_BrowserDock, true ); //TODO: Dock Not yet implemented
        
        m_MenuView->AppendSeparator();
        
        m_MenuView->AppendSubMenu( m_MenuViewMode, "View Layout", "" );
        
    m_MenuBar = new wxMenuBar();
				
        m_MenuBar->Append( m_MenuFile, "&File" );
        m_MenuBar->Append( m_MenuView, "&View" );
    
    SetMenuBar( m_MenuBar );
    
    Centre();
    
    SetMinSize( wxSize(512,384) );
    
    //Window Area - Contains All the contents of the Browser Window besides the menu bar.
        //Options Bar
        //Main Splitter
            //View Splitter
            //Package List Window
            
    m_WindowArea = new wxBoxSizer( wxVERTICAL );
    
        //Options Bar - Quick-Select view options
        m_OptionsBar = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxSize( -1, 32 ), wxBORDER_SIMPLE );
        
            wxBoxSizer* optionsSizer = new wxBoxSizer( wxHORIZONTAL );
            
            wxString choiceStrAry[3] = { "Object", "Tile", "List" };
            m_ViewModeChoice = new wxChoice( m_OptionsBar, ID_BrowserViewMode_Choice, wxDefaultPosition, 
                wxDefaultSize, 3, choiceStrAry, 0, wxDefaultValidator, "View Mode" );
            m_ViewModeChoice->SetSelection( m_ViewMode );
            
            m_ViewCheck_Class = new wxCheckBox( m_OptionsBar, ID_BrowserViewMode_Class, "Class" );
            m_ViewCheck_Audio = new wxCheckBox( m_OptionsBar, ID_BrowserViewMode_Audio, "Audio" );
            m_ViewCheck_Music = new wxCheckBox( m_OptionsBar, ID_BrowserViewMode_Music, "Music" );
            m_ViewCheck_Graphics = new wxCheckBox( m_OptionsBar, ID_BrowserViewMode_Graphics, "Graphics" );
            m_ViewCheck_Mesh = new wxCheckBox( m_OptionsBar, ID_BrowserViewMode_Mesh, "Mesh" );
            m_ViewCheck_Package = new wxCheckBox( m_OptionsBar, ID_BrowserViewMode_Package, "All" );
            
            optionsSizer->Add( m_ViewModeChoice, 0, wxALIGN_LEFT );
            optionsSizer->Add( m_ViewCheck_Class, 0, wxALIGN_LEFT );
            optionsSizer->Add( m_ViewCheck_Audio, 0, wxALIGN_LEFT );
            optionsSizer->Add( m_ViewCheck_Music, 0, wxALIGN_LEFT );
            optionsSizer->Add( m_ViewCheck_Graphics, 0, wxALIGN_LEFT );
            optionsSizer->Add( m_ViewCheck_Mesh, 0, wxALIGN_LEFT );
            optionsSizer->Add( m_ViewCheck_Package, 0, wxALIGN_LEFT );
                
            m_OptionsBar->SetSizer( optionsSizer );
            
        m_WindowArea->Add( m_OptionsBar, 0, wxALIGN_TOP | wxEXPAND );
        
        //Main Splitter
        m_MainSplitter = new wxSplitterWindow( this, wxID_ANY, wxDefaultPosition, wxSize( 500,330 ) );
        m_MainSplitter->SetMinSize( wxSize(500,330) );
        
        m_WindowArea->Add( m_MainSplitter, 1, wxALIGN_BOTTOM | wxEXPAND );
    
            //View Splitter - View and Preview window split.
            m_ViewSplitter = new wxSplitterWindow( m_MainSplitter, wxID_ANY, wxDefaultPosition, wxSize( -1, 360 ) );
            m_ViewSplitter->SetMinSize( wxSize(500,100) );

                //View and Preview Windows
                m_ViewWindow = new wxPanel( m_ViewSplitter, -1 );
                m_PreviewWindow = new wxPanel( m_ViewSplitter, -1 );
                
            m_ViewSplitter->SplitVertically( m_ViewWindow, m_PreviewWindow );
            m_ViewSplitter->SetMinimumPaneSize( 50 );
            m_ViewSplitter->SetSashPosition( 250 );
        
            //Packages List Window
            m_PackagesList = new wxCheckListBox( m_MainSplitter, -1 ); 
            m_PackagesList->SetMinSize( wxSize(500,32) );
        
        m_MainSplitter->SplitHorizontally( m_ViewSplitter, m_PackagesList );
        m_MainSplitter->SetMinimumPaneSize( 50 );
        m_MainSplitter->SetSashPosition( 400 );
        m_MainSplitter->SetSashGravity( 1.0 );
    
    SetSizer(m_WindowArea);
    
    update();

    Show(true);
}

void EdBrowser::UpdatePackageList()
{
    wxArrayString strAry;
    
    for( size_t i = 0; i<EdEditor::GetPackages()->Size(); i++ )
    {
        strAry.Add( (*EdEditor::GetPackages())[i]->GetPackageName() );
    }
    
    m_PackagesList->Set( strAry );
}

void EdBrowser::OnExit( wxCommandEvent& event )
{
    Close(true);
}

void EdBrowser::EVT_BrowserOpen( wxCommandEvent& event )
{
    wxFileDialog openFileDialog( this, "Open UPackage", "", "", "UE Package Files (*.u, *.utx, *.uax\
        , *.umx, *.usm)|*.u;*.u;*.utx;*.uax;*.umx;*.usm", wxFD_OPEN|wxFD_FILE_MUST_EXIST|wxFD_MULTIPLE );
    
    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return;
        
    wxArrayString filePaths;
    
    openFileDialog.GetPaths( filePaths );
    
    EdEditor::LoadPackages( filePaths );
    
    UpdatePackageList(); //TODO: Find a way to do this for all Browser instances instead.
}

void EdBrowser::EVT_ViewChoice( wxCommandEvent& event )
{
    m_ViewMode = static_cast<EBrowserViewMode>(event.GetSelection());
    
    update();
}

void EdBrowser::EVT_BrowserViewMode_Raw( wxCommandEvent& event )
{
    m_ViewMode = VIEW_Raw;
    
    update();
}

void EdBrowser::EVT_BrowserViewMode_Thumbnail( wxCommandEvent& event )
{
    m_ViewMode = VIEW_Thumbnail;
    
    update();
}

void EdBrowser::EVT_BrowserViewMode_List( wxCommandEvent& event )
{
    m_ViewMode = VIEW_List;
    
    update();
}

void EdBrowser::EVT_BrowserViewMode_Preview( wxCommandEvent& event )
{
    m_bPreview = !m_bPreview;
    
    update();
}

void EdBrowser::EVT_BrowserViewMode_Class( wxCommandEvent& event )
{
    m_BrowserFlags ^= BRWFLG_Class;
    
    update();
}

void EdBrowser::EVT_BrowserViewMode_Audio( wxCommandEvent& event )
{
    m_BrowserFlags ^= BRWFLG_Audio;
    
    update();
}

void EdBrowser::EVT_BrowserViewMode_Music( wxCommandEvent& event )
{
    m_BrowserFlags ^= BRWFLG_Music;
    
    update();
}

void EdBrowser::EVT_BrowserViewMode_Graphics( wxCommandEvent& event )
{
    m_BrowserFlags ^= BRWFLG_Graphics;
    
    update();
}

void EdBrowser::EVT_BrowserViewMode_Mesh( wxCommandEvent& event )
{
    m_BrowserFlags ^= BRWFLG_Mesh;
    
    update();
}

void EdBrowser::EVT_BrowserViewMode_Package( wxCommandEvent& event )
{
    m_BrowserFlags = BRWFLG_Package;
    
    update();
}

void EdBrowser::EVT_BrowserDock( wxCommandEvent& event )
{
}

void EdBrowser::update()
{
    //ViewMode Checkboxes
    m_MenuViewMode->Check( ID_BrowserViewMode_Raw, m_ViewMode == VIEW_Raw );
    m_MenuViewMode->Check( ID_BrowserViewMode_Thumbnail, m_ViewMode == VIEW_Thumbnail );
    m_MenuViewMode->Check( ID_BrowserViewMode_List, m_ViewMode == VIEW_List );
    
    m_MenuViewMode->Check( ID_BrowserViewMode_Class, m_BrowserFlags & BRWFLG_Class );
    m_MenuViewMode->Check( ID_BrowserViewMode_Audio, m_BrowserFlags & BRWFLG_Audio );
    m_MenuViewMode->Check( ID_BrowserViewMode_Music, m_BrowserFlags & BRWFLG_Music );
    m_MenuViewMode->Check( ID_BrowserViewMode_Graphics, m_BrowserFlags & BRWFLG_Graphics );
    m_MenuViewMode->Check( ID_BrowserViewMode_Mesh, m_BrowserFlags & BRWFLG_Mesh );
    m_ViewCheck_Class->SetValue( m_BrowserFlags & BRWFLG_Class );
    m_ViewCheck_Audio->SetValue( m_BrowserFlags & BRWFLG_Audio );
    m_ViewCheck_Music->SetValue( m_BrowserFlags & BRWFLG_Music );
    m_ViewCheck_Graphics->SetValue( m_BrowserFlags & BRWFLG_Graphics );
    m_ViewCheck_Mesh->SetValue( m_BrowserFlags & BRWFLG_Mesh );
    m_ViewCheck_Package->SetValue( m_BrowserFlags == BRWFLG_Package );
    
    m_MenuViewMode->Check( ID_BrowserViewMode_Preview, m_bPreview );
    
    m_ViewModeChoice->SetSelection( m_ViewMode );
    
    //Window Label/Icon
    if( m_BrowserFlags == BRWFLG_Class )
    {
        SetLabel(wxString("Class Browser"));
        SetIcon(m_icoClass);
    }
    else if( m_BrowserFlags == BRWFLG_Audio )
    {
        SetLabel(wxString("Audio Browser"));    
        SetIcon(m_icoAudio);
    }
    else if( m_BrowserFlags == BRWFLG_Music )
    {
        SetLabel(wxString("Music Browser"));    
        SetIcon(m_icoMusic);
    }
    else if( m_BrowserFlags == BRWFLG_Graphics )
    {
        SetLabel(wxString("Graphics Browser")); 
        SetIcon(m_icoGraphics);   
    }
    else if( m_BrowserFlags == BRWFLG_Mesh )
    {
        SetLabel(wxString("Mesh Browser"));
        SetIcon(m_icoMesh);
    }
    else
    {
        SetLabel(wxString("Package Browser"));
        SetIcon(m_icoPackage);
    }
}

wxBEGIN_EVENT_TABLE(EdBrowser, wxFrame)
    EVT_CHOICE(ID_BrowserViewMode_Choice,   EdBrowser::EVT_ViewChoice )
    EVT_MENU(wxID_EXIT, EdBrowser::OnExit)
    EVT_MENU(ID_BrowserNew,   EdBrowser::EVT_BrowserNew)
    EVT_MENU(ID_BrowserOpen,   EdBrowser::EVT_BrowserOpen)
    EVT_MENU(ID_BrowserSave,   EdBrowser::EVT_BrowserSave)
    EVT_MENU(ID_BrowserImport,   EdBrowser::EVT_BrowserImport)
    EVT_MENU(ID_BrowserExport,   EdBrowser::EVT_BrowserExport)
    EVT_MENU(ID_BrowserViewMode_Raw,   EdBrowser::EVT_BrowserViewMode_Raw)
    EVT_MENU(ID_BrowserViewMode_Thumbnail,   EdBrowser::EVT_BrowserViewMode_Thumbnail)
    EVT_MENU(ID_BrowserViewMode_List,   EdBrowser::EVT_BrowserViewMode_List)
    EVT_MENU(ID_BrowserViewMode_Preview,   EdBrowser::EVT_BrowserViewMode_Preview)
    EVT_MENU(ID_BrowserViewMode_Class,   EdBrowser::EVT_BrowserViewMode_Class)
    EVT_CHECKBOX(ID_BrowserViewMode_Class, EdBrowser::EVT_BrowserViewMode_Class)
    EVT_MENU(ID_BrowserViewMode_Audio,   EdBrowser::EVT_BrowserViewMode_Audio)
    EVT_CHECKBOX(ID_BrowserViewMode_Audio, EdBrowser::EVT_BrowserViewMode_Audio)
    EVT_MENU(ID_BrowserViewMode_Music,   EdBrowser::EVT_BrowserViewMode_Music)
    EVT_CHECKBOX(ID_BrowserViewMode_Music, EdBrowser::EVT_BrowserViewMode_Music)
    EVT_MENU(ID_BrowserViewMode_Graphics,   EdBrowser::EVT_BrowserViewMode_Graphics)
    EVT_CHECKBOX(ID_BrowserViewMode_Graphics, EdBrowser::EVT_BrowserViewMode_Graphics)
    EVT_MENU(ID_BrowserViewMode_Mesh,   EdBrowser::EVT_BrowserViewMode_Mesh)
    EVT_CHECKBOX(ID_BrowserViewMode_Mesh, EdBrowser::EVT_BrowserViewMode_Mesh)
    EVT_CHECKBOX(ID_BrowserViewMode_Package, EdBrowser::EVT_BrowserViewMode_Package)
    EVT_MENU(ID_BrowserDock,   EdBrowser::EVT_BrowserDock)
wxEND_EVENT_TABLE()
