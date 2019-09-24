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
 * Browsers.cpp - View browser implementations
 * 
 * written by Jesse 'Hyzoran' Kowalik
*========================================================================
*/

#include "Browsers.h"

struct wxObjectItemData : public wxTreeItemData
{
    wxObjectItemData( UObject* Obj ) : m_Obj(Obj)
    {}
    
    UObject* m_Obj = NULL;
};

EdBrowser::EdBrowser( EBrowserType BrowserType, bool bDock ) 
    : EdToolFrame(bDock), m_Mode(BrowserType)
{
    m_ToolType = TOOL_Browser;    
    
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
        
    m_MenuBar = new wxMenuBar();
				
        m_MenuBar->Append( m_MenuFile, "&File" );
    
    SetMenuBar( m_MenuBar );
    
    SetMinSize( wxSize(600,384) );
    
    m_WindowAreaSizer = new wxBoxSizer( wxVERTICAL );
    
        m_OptionsBar = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxSize( -1, 32 ), wxBORDER_SIMPLE );
            
            wxBoxSizer* optionsSizer = new wxBoxSizer( wxHORIZONTAL );
            
            m_Check_Dock = new wxCheckBox( m_OptionsBar, ID_Browser_Dock, "Dock" );
            optionsSizer->Add( m_Check_Dock, 0, wxALIGN_LEFT );
            
            m_OptionsBar->SetSizer( optionsSizer );
            
        m_WindowAreaSizer->Add( m_OptionsBar, 0, wxALIGN_TOP | wxEXPAND );
    
    switch( BrowserType )
    {
        case( BRWFLG_Class ):
        {
            SetLabel(wxString("Class Browser"));
            SetIcon(m_icoClass);
            goto ListContruct;
        }
        
        case( BRWFLG_Audio ):
        {
            SetLabel(wxString("Audio Browser"));    
            SetIcon(m_icoAudio);
            goto ListContruct;
        }
        
        case( BRWFLG_Music ):
        {
            SetLabel(wxString("Music Browser"));    
            SetIcon(m_icoMusic);
            goto ListContruct;
        }
        
        case( BRWFLG_Texture ):
        {
            SetLabel(wxString("Texture Browser")); 
            SetIcon(m_icoGraphics); 
            goto TileContruct;
        }
        
        case( BRWFLG_Mesh ):
        {
            SetLabel(wxString("Mesh Browser"));
            SetIcon(m_icoMesh);
            goto TileContruct;
        }
        
        case( BRWFLG_Level ):
        {
            SetLabel(wxString("Level Browser"));
            SetIcon(m_icoMesh);
            goto TileContruct;
        }
        
        case( BRWFLG_Package ):
        {
            SetLabel(wxString("Package Browser"));
            SetIcon(m_icoPackage);
            goto ObjectConstruct;
        }
    }
    
ObjectConstruct:
    
    goto Finish;
    
ListContruct:

    if( m_Mode == BRWFLG_Class )
    {
        m_Check_ShowPackage = new wxCheckBox( m_OptionsBar, ID_Browser_ShowPackage, "Show Package" );
        optionsSizer->Add( m_Check_ShowPackage, 0, wxALIGN_LEFT );
    }

     //Main Splitter
    m_MainSplitter = new wxSplitterWindow( this, wxID_ANY, wxDefaultPosition, wxSize( 500,330 ) );
    m_MainSplitter->SetMinSize( wxSize(500,384) );
    
    m_WindowAreaSizer->Add( m_MainSplitter, 1, wxALIGN_BOTTOM | wxEXPAND );
    
        m_ListView = new wxTreeCtrl( m_MainSplitter, ID_ClassTree, wxDefaultPosition, wxDefaultSize,
            wxTR_MULTIPLE | wxTR_HIDE_ROOT | wxTR_HAS_BUTTONS );
        m_ListView->SetMinSize( wxSize(500,256) );
    
        //Packages List Window
        m_PackagesList = new wxCheckListBox( m_MainSplitter, ID_PackageList, wxDefaultPosition,
            wxDefaultSize, 0, NULL, wxLB_SORT ); 
        m_PackagesList->SetMinSize( wxSize(500,128) );
    
    m_MainSplitter->SetMinimumPaneSize( 128 );
    m_MainSplitter->SetSashPosition( m_MainSplitter->GetSize().GetHeight()*0.8 );
    m_MainSplitter->SetSashGravity( 0.8 );
    m_MainSplitter->SplitHorizontally( m_ListView, m_PackagesList );
    
    goto Finish;

TileContruct:

    goto Finish;
    
Finish:

    SetSizer(m_WindowAreaSizer);
    
    //Initialize Packagelist with all loaded Packages.
    SYS_NewPackages( 0 );
    SYS_NewObjects( 0 );
    
    Show(true);

}

void EdBrowser::SYS_NewPackages( size_t PackageStartIndex )
{
    for( size_t i = PackageStartIndex; i<(*UPackage::GetLoadedPackages()).Size(); i++ )
    {
        m_PackagesList->Append( (*UPackage::GetLoadedPackages())[i]->Name.Data(),
            (*UPackage::GetLoadedPackages())[i] );
    }
    
    update();
}

void EdBrowser::SYS_NewObjects( size_t ObjectStartIndex )
{
    switch( m_Mode )
    {
        case( BRWFLG_Class ):
        {
            classUpdate( ObjectStartIndex );
            break;
        }
        
        case( BRWFLG_Audio ):
        {
            listUpdate( ObjectStartIndex );
            break;
        }
        
        case( BRWFLG_Music ):
        {
            listUpdate( ObjectStartIndex );
            break;
        }
        
        case( BRWFLG_Texture ):
        {
            tileUpdate( ObjectStartIndex );
            break;
        }
        
        case( BRWFLG_Mesh ):
        {
            tileUpdate( ObjectStartIndex );
            break;
        }
        
        case( BRWFLG_Level ):
        {
            tileUpdate( ObjectStartIndex );
            break;
        }
        
        case( BRWFLG_Package ):
        {
            objectUpdate( ObjectStartIndex );
            break;
        }
    }
        
    update();
}

void EdBrowser::SYS_PackagesRemoved()
{
    m_PackagesList->Clear();
    
    for( size_t i = 0; i<(*UPackage::GetLoadedPackages()).Size(); i++ )
    {
        m_PackagesList->Append( (*UPackage::GetLoadedPackages())[i]->Name.Data(),
            (*UPackage::GetLoadedPackages())[i] );
    }
    
    update();
}

void EdBrowser::SYS_ObjectsRemoved()
{
    switch( m_Mode )
    {
        case( BRWFLG_Class ):
        {
            classUpdate( 0 );
            break;
        }
        
        case( BRWFLG_Audio ):
        {
            listUpdate( 0 );
            break;
        }
        
        case( BRWFLG_Music ):
        {
            listUpdate( 0 );
            break;
        }
        
        case( BRWFLG_Texture ):
        {
            tileUpdate( 0 );
            break;
        }
        
        case( BRWFLG_Mesh ):
        {
            tileUpdate( 0 );
            break;
        }
        
        case( BRWFLG_Level ):
        {
            tileUpdate( 0 );
            break;
        }
        
        case( BRWFLG_Package ):
        {
            objectUpdate( 0 );
            break;
        }
    }
    
    update();
}

void EdBrowser::OnExit( wxCommandEvent& event )
{
    Close(true);
}

void EdBrowser::EVT_BrowserNew( wxCommandEvent& event )
{
}

void EdBrowser::EVT_BrowserOpen( wxCommandEvent& event )
{
    wxFileDialog openFileDialog( this, "Open UPackage", "", "", "UE Package Files (*.u, *.utx, *.uax\
        , *.umx, *.usm, *.unr)|*.u;*.u;*.utx;*.uax;*.umx;*.usm;*.unr", wxFD_OPEN|wxFD_FILE_MUST_EXIST|wxFD_MULTIPLE );
    
    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return;
        
    wxArrayString filePaths;
    
    openFileDialog.GetPaths( filePaths );
    
    EdEditor::LoadPackages( filePaths );
}

void EdBrowser::EVT_BrowserSave( wxCommandEvent& event )
{
}

void EdBrowser::EVT_BrowserImport( wxCommandEvent& event )
{
}

void EdBrowser::EVT_BrowserExport( wxCommandEvent& event )
{
}

void EdBrowser::EVT_PackageList( wxCommandEvent& event )
{
    update();
}

void EdBrowser::EVT_Browser_Dock( wxCommandEvent& event )
{
    m_bDocked = !m_bDocked;
    
    update();
}

void EdBrowser::EVT_Browser_ShowPackage( wxCommandEvent& event )
{
    m_bShowPackage = !m_bShowPackage;
    
    classUpdate(0);
    update();
}

void EdBrowser::EVT_ClassTree( wxCommandEvent& event )
{
}

void EdBrowser::update()
{
    //Update Checkboxes
    if( m_Check_ShowPackage != NULL )
        m_Check_ShowPackage->SetValue( m_bShowPackage );
        
    m_Check_Dock->SetValue( m_bDocked );
}

void EdBrowser::objectUpdate( size_t ObjectStartIndex )
{
}

void EdBrowser::classUpdate( size_t ObjectStartIndex )
{
    TArray<UClass*> expandedObjects; //Previously Expanded UObjects.
    TArray<wxTreeItemId> toExpand;
    
    if(m_ListView->GetCount() > 0)
        recurseExpandPopulate( expandedObjects, m_ListView->GetRootItem() );
    
    //Clear previous state.
    m_ListView->DeleteAllItems();
    
    wxTreeItemId root = m_ListView->AddRoot(""); //Dummy root
    
    //We track previous populated parent classes so that new classes be subbed below them.
    TArray<wxTreeItemId> parents;
    TArray<wxTreeItemId> newParents;
    
    //Add Object class.
    parents.PushBack( addTreeItem( root, UObject::StaticClass() ) );
    
    wxTreeItemId object = parents[0];
    
    bool bBuildTree = true;
    
    while( bBuildTree ) 
    {
        bBuildTree = false;
        
        //For each Object in the classpool, we compare it against each parent, if true, then 
        //nest it beneath the parent and add it to the list of new parents.
        //If we ever find a class to add, we tell the loop to proceed again.
        for( size_t i = 0; i < UObject::ClassPool.Size(); i++ )
        {
            UClass* currentClass = UObject::ClassPool[i];
            currentClass = currentClass;
            
            for( size_t j = 0; j < parents.Size(); j++ )
            {
                if( currentClass->SuperClass == ((wxObjectItemData*)m_ListView->GetItemData(parents[j]))->m_Obj )
                {
                    bBuildTree = true;
                    newParents.PushBack( addTreeItem( parents[j], currentClass ) );
                    
                    //Check if added Object was expanded before
                    for( size_t k = 0; k<expandedObjects.Size(); k++ )
                    {
                        if( expandedObjects[k] == currentClass )
                        {
                            toExpand.PushBack( newParents.Back() );
                        }
                    }
                        
                    break;
                }
            }
        }
        
       //Sort Alphanumeric
       for( size_t j = 0; j < parents.Size(); j++ )
       {
            m_ListView->SortChildren( parents[j] );
       }
       
       parents.Clear();
       parents = newParents;
       newParents.Clear();
    }
    
    m_ListView->Expand( object );
    
    //Remember Expanded
    for( size_t i = 0; i<toExpand.Size(); i++ )
    {
        m_ListView->Expand( toExpand[i] );
    }
}

void EdBrowser::recurseExpandPopulate( TArray<UClass*>& ExpandedAry, wxTreeItemId Parent, 
    wxTreeItemIdValue Cookie )
{
    wxTreeItemId id;

    if ( !Cookie )
        id = m_ListView->GetFirstChild(Parent, Cookie);
    else
        id = m_ListView->GetNextChild(Parent, Cookie);

    if ( !id.IsOk() )
        return;
        
    if( m_ListView->IsExpanded( id ) )
    {
        ExpandedAry.PushBack( (UClass*)((wxObjectItemData*)m_ListView->GetItemData(id))->m_Obj );
    }

    if (m_ListView->ItemHasChildren(id))
        recurseExpandPopulate(ExpandedAry, id);

    recurseExpandPopulate(ExpandedAry, Parent, Cookie);
}

wxTreeItemId EdBrowser::addTreeItem( wxTreeItemId Parent, UClass* Obj )
{
    if( m_bShowPackage )
    {
        return m_ListView->AppendItem( Parent, wxString(Obj->Name.Data())
            + wxString(" (") + wxString( Obj->Pkg->Name.Data() ) + wxString(") "), 
            -1, -1, new wxObjectItemData(Obj) );
    }
    else
    {
        return m_ListView->AppendItem( Parent, Obj->Name.Data(), -1, -1,
            new wxObjectItemData(Obj) );
    }
}

void EdBrowser::listUpdate( size_t ObjectStartIndex )
{
}

void EdBrowser::tileUpdate( size_t ObjectStartIndex )
{
}

wxBEGIN_EVENT_TABLE(EdBrowser, wxFrame)
    EVT_MENU(wxID_EXIT, EdBrowser::OnExit)
    EVT_MENU(ID_BrowserNew,   EdBrowser::EVT_BrowserNew)
    EVT_MENU(ID_BrowserOpen,   EdBrowser::EVT_BrowserOpen)
    EVT_MENU(ID_BrowserSave,   EdBrowser::EVT_BrowserSave)
    EVT_MENU(ID_BrowserImport,   EdBrowser::EVT_BrowserImport)
    EVT_MENU(ID_BrowserExport,   EdBrowser::EVT_BrowserExport)
    EVT_CHECKBOX(ID_Browser_ShowPackage, EdBrowser::EVT_Browser_ShowPackage)
    EVT_CHECKBOX(ID_Browser_Dock,   EdBrowser::EVT_Browser_Dock)
    EVT_CHECKLISTBOX( ID_PackageList, EdBrowser::EVT_PackageList )
wxEND_EVENT_TABLE()
