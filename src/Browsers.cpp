#include "Browsers.h"

EdBrowser::EdBrowser( EBrowserFlags BrowserFlags, const wxString& Title, const wxPoint& 
Pos, const wxSize& Size ) : EdToolFrame(false), m_BrowserFlags(BrowserFlags)
{
    if( m_BrowserFlags == BRWFLG_Package ) //If using classes, prefer list mode..
        m_ViewMode = VIEW_Raw;
    else if( m_BrowserFlags == BRWFLG_Class ) //If using classes, prefer list mode..
        m_ViewMode = VIEW_List;
    else
        m_ViewMode = VIEW_Thumbnail;
    
    wxMenu* menuFile = new wxMenu();
    
    if( m_MenuBar != NULL )
        delete m_MenuBar;
    
    if( m_MenuFile != NULL)
        delete m_MenuFile;
    
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
        
    if( m_MenuView != NULL)
        delete m_MenuView;
    
    m_MenuViewMode = new wxMenu();
    
        m_MenuViewMode->AppendCheckItem(ID_BrowserViewMode_Raw, "Object View", "");
        m_MenuViewMode->AppendCheckItem(ID_BrowserViewMode_Thumbnail, "Tile View", "");
        m_MenuViewMode->AppendCheckItem(ID_BrowserViewMode_List, "List View", "");
        
        if( m_ViewMode == VIEW_Raw )
            m_MenuViewMode->Check( ID_BrowserViewMode_Raw, true );
        else
            m_MenuViewMode->Check( ID_BrowserViewMode_Raw, false );
        
        if( m_ViewMode == VIEW_Thumbnail )
            m_MenuViewMode->Check( ID_BrowserViewMode_Thumbnail, true );
        else
            m_MenuViewMode->Check( ID_BrowserViewMode_Thumbnail, false );
        
        if( m_ViewMode == VIEW_List )
            m_MenuViewMode->Check( ID_BrowserViewMode_List, true );
        else
            m_MenuViewMode->Check( ID_BrowserViewMode_List, false );
        
        m_MenuViewMode->AppendSeparator();
        
        m_MenuViewMode->AppendCheckItem( ID_BrowserViewMode_Preview, "Display Preview", "" );
        
        if( m_bPreview )
            m_MenuViewMode->Check( ID_BrowserViewMode_Preview, true );
        else
            m_MenuViewMode->Check( ID_BrowserViewMode_Preview, false );
        
        m_MenuViewMode->AppendSeparator();
        
        m_MenuViewMode->AppendCheckItem( ID_BrowserViewMode_Class, "Classes", "" );
        m_MenuViewMode->AppendCheckItem( ID_BrowserViewMode_Audio, "Audio", "" );
        m_MenuViewMode->AppendCheckItem( ID_BrowserViewMode_Music, "Music", "" );
        m_MenuViewMode->AppendCheckItem( ID_BrowserViewMode_Graphics, "Graphics", "" );
        m_MenuViewMode->AppendCheckItem( ID_BrowserViewMode_Mesh, "Meshes", "" );
        
        if( m_BrowserFlags & BRWFLG_Class )
            m_MenuViewMode->Check( ID_BrowserViewMode_Class, true );
        else
            m_MenuViewMode->Check( ID_BrowserViewMode_Class, false );
        
        if( m_BrowserFlags & BRWFLG_Audio )
            m_MenuViewMode->Check( ID_BrowserViewMode_Audio, true );
        else
            m_MenuViewMode->Check( ID_BrowserViewMode_Audio, false );
        
        if( m_BrowserFlags & BRWFLG_Music )
            m_MenuViewMode->Check( ID_BrowserViewMode_Music, true );
        else
            m_MenuViewMode->Check( ID_BrowserViewMode_Music, false );
        
        if( m_BrowserFlags & BRWFLG_Graphics )
            m_MenuViewMode->Check( ID_BrowserViewMode_Graphics, true );
        else
            m_MenuViewMode->Check( ID_BrowserViewMode_Graphics, false );
        
        if( m_BrowserFlags & BRWFLG_Mesh)
            m_MenuViewMode->Check( ID_BrowserViewMode_Mesh, true );
        else
            m_MenuViewMode->Check( ID_BrowserViewMode_Mesh, false );
    
    m_MenuView = new wxMenu();
    
        m_MenuView->AppendCheckItem( ID_BrowserDock, "Dock", "" );
        
        m_MenuView->AppendSeparator();
        
        m_MenuView->AppendSubMenu( m_MenuViewMode, "View Layout", "" );
        
    m_MenuBar = new wxMenuBar();
				
        m_MenuBar->Append( m_MenuFile, "&File" );
        m_MenuBar->Append( m_MenuView, "&View" );
    
    SetMenuBar( m_MenuBar );
    
    //TODO: Package Bar 
    //TODO: Group Bar
    
    this->Show(true);
}

void EdBrowser::renderMenu() //Update Browser Menu
{
    
}

void EdBrowser::renderView()
{
}

// ID bind
wxBEGIN_EVENT_TABLE(EdBrowser, EdBrowser)
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
    EVT_MENU(ID_BrowserViewMode_Audio,   EdBrowser::EVT_BrowserViewMode_Audio)
    EVT_MENU(ID_BrowserViewMode_Music,   EdBrowser::EVT_BrowserViewMode_Music)
    EVT_MENU(ID_BrowserViewMode_Graphics,   EdBrowser::EVT_BrowserViewMode_Graphics)
    EVT_MENU(ID_BrowserViewMode_Mesh,   EdBrowser::EVT_BrowserViewMode_Mesh)
    EVT_MENU(ID_BrowserDock,   EdBrowser::EVT_BrowserDock)
wxEND_EVENT_TABLE()
