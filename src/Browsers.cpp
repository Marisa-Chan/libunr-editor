/*===========================================================================*\
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
 * Browsers.cpp - View browser implementations
 * 
 * written by Jesse 'Hyzoran' Kowalik
*========================================================================
*/
#include "Browsers.h"
    
struct ObjectItemPair
{
    ObjectItemPair( UClass* inClass, wxTreeItemId inItem )
    {
        Class = inClass;
        Item = inItem;
    }
        
    UClass* Class;
    wxTreeItemId Item;
};

EdBrowser::EdBrowser( int BrowserFlags, bool bDock ) 
    : EdToolFrame(bDock), m_BrowserFlags(BrowserFlags)
{
    m_ToolType = TOOL_Browser;    
    
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
    else if( m_BrowserFlags == BRWFLG_Level )
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
        
        m_MenuViewMode->AppendCheckItem( ID_BrowserShowPackage, "Objects Show Package", "" );
        
        
        m_MenuViewMode->AppendSeparator();
        
        m_MenuViewMode->AppendCheckItem( ID_BrowserSortMode_Alpha, "Sort Alphanumeric", "" );
        
        m_MenuViewMode->AppendCheckItem( ID_BrowserSortMode_Package, "Sort by Package Alphanumeric", "" );
        
        m_MenuViewMode->AppendSeparator();
        
        m_MenuViewMode->AppendCheckItem( ID_BrowserSortAscending, "Sort Ascending", "" );
        
        m_MenuViewMode->AppendSeparator();
        m_MenuViewMode->AppendCheckItem( ID_BrowserViewMode_Class, "Classes", "" );
        m_MenuViewMode->AppendCheckItem( ID_BrowserViewMode_Audio, "Audio", "" );
        m_MenuViewMode->AppendCheckItem( ID_BrowserViewMode_Music, "Music", "" );
        m_MenuViewMode->AppendCheckItem( ID_BrowserViewMode_Graphics, "Graphics", "" );
        m_MenuViewMode->AppendCheckItem( ID_BrowserViewMode_Mesh, "Meshes", "" );
        m_MenuViewMode->AppendCheckItem( ID_BrowserViewMode_Level, "Levels", "" );
        
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
    
    SetMinSize( wxSize(600,384) );
    
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
            m_ViewCheck_Level = new wxCheckBox( m_OptionsBar, ID_BrowserViewMode_Level, "Levels" );
            m_ViewCheck_Package = new wxCheckBox( m_OptionsBar, ID_BrowserViewMode_Package, "All" );
            
            optionsSizer->Add( m_ViewModeChoice, 0, wxALIGN_LEFT );
            optionsSizer->Add( m_ViewCheck_Class, 0, wxALIGN_LEFT );
            optionsSizer->Add( m_ViewCheck_Audio, 0, wxALIGN_LEFT );
            optionsSizer->Add( m_ViewCheck_Music, 0, wxALIGN_LEFT );
            optionsSizer->Add( m_ViewCheck_Graphics, 0, wxALIGN_LEFT );
            optionsSizer->Add( m_ViewCheck_Mesh, 0, wxALIGN_LEFT );
            optionsSizer->Add( m_ViewCheck_Level, 0, wxALIGN_LEFT );
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
            
                m_View_Object = new wxTreeCtrl( m_ViewSplitter, 
                    ID_BrowserObjectMode, wxDefaultPosition, wxSize( -1, -1 ) );
                m_View_List = new wxTreeCtrl( m_ViewSplitter, 
                    ID_BrowserListMode, wxDefaultPosition, wxSize( -1, -1 ), wxTR_HIDE_ROOT | wxTR_HAS_BUTTONS | wxTR_MULTIPLE );
                m_View_Tile = new wxListCtrl( m_ViewSplitter, ID_BrowserTileMode, 
                    wxDefaultPosition, wxSize( -1, -1 ), wxLC_ICON );
                    
                m_PreviewWindow = new wxPanel( m_ViewSplitter, -1 );
                
            m_ViewSplitter->SplitVertically( m_View_Object, m_PreviewWindow );
            OldWindow = m_View_Object;
            m_ViewSplitter->SetMinimumPaneSize( 250 );
            m_ViewSplitter->SetSashPosition( m_ViewSplitter->GetSize().GetWidth()*0.8 );
            m_ViewSplitter->SetSashGravity( 0.8 );
        
            //Packages List Window
            m_PackagesList = new wxCheckListBox( m_MainSplitter, -1 ); 
            m_PackagesList->SetMinSize( wxSize(500,128) );
        
        m_MainSplitter->SetMinimumPaneSize( 128 );
        m_MainSplitter->SetSashPosition( m_MainSplitter->GetSize().GetHeight()*0.8 );
        m_MainSplitter->SetSashGravity( 0.8 );
        m_MainSplitter->SplitHorizontally( m_ViewSplitter, m_PackagesList );
    
    SetSizer(m_WindowArea);
    
    wxArrayString strAry;
    
    for( size_t i = 0; i<UPackage::GetLoadedPackages()->Size(); i++ )
    {
        strAry.Add( (*UPackage::GetLoadedPackages())[i]->Name.Data() );
    }
    
    UpdatePackageList( strAry );
    
    update();

    Show(true);
}

void EdBrowser::UpdatePackageList( const wxArrayString& NewList )
{
    //TODO: Remember selections on Package changes.
    
    m_PackagesList->Set( NewList );
    
    for( size_t i = 0; i<m_PackagesList->GetCount(); i++ )
    {
        m_PackagesList->Check( i, true );
    }
    
    update();
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

void EdBrowser::EVT_BrowserViewMode_Level( wxCommandEvent& event )
{
    m_BrowserFlags ^= BRWFLG_Level;
    
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

void EdBrowser::EVT_BrowserShowPackage( wxCommandEvent& event )
{
    m_bShowPackage = !m_bShowPackage;
    
    update();
}

void EdBrowser::EVT_BrowserSortAscending( wxCommandEvent& event )
{
    m_bSortAscending = !m_bSortAscending;
    
    update();
}

void EdBrowser::EVT_BrowserSortMode_Alpha( wxCommandEvent& event )
{
    m_SortMode = SORT_Alpha;
    
    update();
}

void EdBrowser::EVT_BrowserSortMode_Package( wxCommandEvent& event )
{
    m_SortMode = SORT_AlphaPackage;
    
    update();
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
    m_MenuViewMode->Check( ID_BrowserViewMode_Level, m_BrowserFlags & BRWFLG_Level );
    m_ViewCheck_Class->SetValue( m_BrowserFlags & BRWFLG_Class );
    m_ViewCheck_Audio->SetValue( m_BrowserFlags & BRWFLG_Audio );
    m_ViewCheck_Music->SetValue( m_BrowserFlags & BRWFLG_Music );
    m_ViewCheck_Graphics->SetValue( m_BrowserFlags & BRWFLG_Graphics );
    m_ViewCheck_Mesh->SetValue( m_BrowserFlags & BRWFLG_Mesh );
    m_ViewCheck_Level->SetValue( m_BrowserFlags & BRWFLG_Level );
    m_ViewCheck_Package->SetValue( m_BrowserFlags == BRWFLG_Package );
    
    
    m_MenuViewMode->Check( ID_BrowserShowPackage, m_bShowPackage );
    m_MenuViewMode->Check( ID_BrowserSortMode_Alpha, m_SortMode == SORT_Alpha );
    m_MenuViewMode->Check( ID_BrowserSortMode_Package, m_SortMode == SORT_AlphaPackage );
    m_MenuViewMode->Check( ID_BrowserSortAscending, m_bSortAscending );
    
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
    else if( m_BrowserFlags == BRWFLG_Level )
    {
        SetLabel(wxString("Level Browser"));
        SetIcon(m_icoMesh);
    }
    else
    {
        SetLabel(wxString("Package Browser"));
        SetIcon(m_icoPackage);
    }
    
    //Actual rendering stuff.
        
    //Object View
    if( m_ViewMode == VIEW_Raw )
    {
        m_View_Object->Show(true);
        m_View_List->Show(false);
        m_View_Tile->Show(false);
        
        objectUpdate();
        
        m_ViewSplitter->ReplaceWindow( OldWindow, m_View_Object );
        OldWindow = m_View_Object;
    }
    
    //List View 
    if( m_ViewMode == VIEW_List )
    {
        m_View_Object->Show(false);
        m_View_List->Show(true);
        m_View_Tile->Show(false);
            
        listUpdate();
        
        m_ViewSplitter->ReplaceWindow( OldWindow, m_View_List );
        OldWindow = m_View_List;
    }
    
    //Tile View 
    if( m_ViewMode == VIEW_Thumbnail )
    {
        m_View_Object->Show(false);
        m_View_List->Show(false);
        m_View_Tile->Show(true);
        
        tileUpdate();
        
        m_ViewSplitter->ReplaceWindow( OldWindow, m_View_Tile );
        OldWindow = m_View_Tile;
    }
    
    //Render Preview?
    
    m_PreviewWindow->Show( m_bPreview );
    
    if( !m_bPreview )
        m_ViewSplitter->Unsplit( m_PreviewWindow );
    else if( !m_ViewSplitter->IsSplit() )
    {
        m_ViewSplitter->SplitVertically( OldWindow, m_PreviewWindow, 
            m_ViewSplitter->GetSize().GetWidth()*0.8 );
    }
    
    m_ViewSplitter->UpdateSize();
        
}

void EdBrowser::objectUpdate()
{
    m_PackagesList->Enable();
}

void EdBrowser::listUpdate()
{
    m_View_List->DeleteAllItems();
    
    if( m_bTreeView )
        m_PackagesList->Disable(); //Tree view doesnt support package filtering.
    else
        m_PackagesList->Enable();
        
    //Class Items
Classes:
    
    if( !m_BrowserFlags & BRWFLG_Class )
        goto Audio;
        
    if( m_bTreeView ) //Tree view for classes
    {
        TArray<ObjectItemPair> parents;
        TArray<ObjectItemPair> newParents;
        bool bBuildTree = true;
        
        wxTreeItemId root = m_View_List->AddRoot(""); //Dummy root
        
        parents.PushBack( ObjectItemPair( UObject::StaticClass(), 
            addTreeItem( UObject::StaticClass(), root ) ) );
            
            
        wxTreeItemId object = parents[0].Item; //remember object to expand it later
        
        while( bBuildTree ) 
        {
            bBuildTree = false;
            
            for( size_t i = 0; i < UObject::ClassPool.Size(); i++ )
            {
                UClass* currentClass = UObject::ClassPool[i];
                currentClass = currentClass;
                
                for( size_t j = 0; j < parents.Size(); j++ )
                {
                    if( currentClass->SuperClass == parents[j].Class )
                    {
                        bBuildTree = true;
                        newParents.PushBack( ObjectItemPair( currentClass, 
                            addTreeItem( currentClass, parents[j].Item ) ) );
                            
                        break;
                    }
                }
            }
            
           //Sort
           for( size_t j = 0; j < parents.Size(); j++ )
           {
                m_View_List->SortChildren( parents[j].Item );
           }
           
           parents.Clear();
           parents = newParents;
           newParents.Clear();
        }
        
        //Automatically expand Object 
        m_View_List->Expand( object );
        
       return;
    }
    else //List view for classes
    {
    }
Audio:
    if( !m_BrowserFlags & BRWFLG_Audio )
        goto Music;
Music:
    if( !m_BrowserFlags & BRWFLG_Music )
        goto Graphics;
Graphics:
    if( !m_BrowserFlags & BRWFLG_Graphics )
        goto Mesh;
Mesh:
    if( !m_BrowserFlags & BRWFLG_Mesh )
        goto Level;
Level:
    if( !m_BrowserFlags & BRWFLG_Level )
        goto Finish;
Finish:
    return;
}

wxTreeItemId EdBrowser::addTreeItem( UClass* Class, wxTreeItemId Parent )
{  
    if( m_bShowPackage )
    {
        return m_View_List->AppendItem( Parent, wxString( Class->Name.Data() ) + wxString(" (") +
                wxString( Class->Pkg->Name.Data() ) + wxString(") ") );
    }
    else
    {
        return m_View_List->AppendItem( Parent, wxString( Class->Name.Data() ) );
    }
}

void EdBrowser::tileUpdate()
{
    m_PackagesList->Enable();
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
    EVT_MENU(ID_BrowserShowPackage, EdBrowser::EVT_BrowserShowPackage  )
    EVT_MENU(ID_BrowserSortAscending, EdBrowser::EVT_BrowserSortAscending  )
    EVT_MENU(ID_BrowserSortMode_Alpha, EdBrowser::EVT_BrowserSortMode_Alpha  )
    EVT_MENU(ID_BrowserSortMode_Package, EdBrowser::EVT_BrowserSortMode_Package  )
wxEND_EVENT_TABLE()
