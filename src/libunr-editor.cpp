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

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <wx/statline.h>

#include <libunr/Util/TArray.h>

#include "Components.h"
wxFrame* g_EditorFrame;
bool (*g_funcRegTool)( const EdToolFrame* Tool );
bool (*g_funcUnRegTool)( const EdToolFrame* Tool );

#include "Browsers.h"

// ID Table
enum
{
	ID_New,
	ID_Open,
    ID_Save,
	ID_Import,
    ID_Export,
	ID_Preferences,
	ID_BrowserPackage,
	ID_BrowserClass,
	ID_BrowserAudio,
    ID_BrowserMusic,
	ID_BrowserGraphics,
	ID_BrowserMesh,
	ID_ViewLog,
	ID_ActiveTools,
	ID_MapEditor,
	ID_MeshEditor,
	ID_Manual
};

bool GRegTool( const EdToolFrame* Tool );
bool GUnRegTool( const EdToolFrame* Tool );

//Editor frame, where the real magic happens.
class EdEditor : public wxFrame
{
	public:
		EdEditor( const wxString& Title, const wxPoint& Pos, const wxSize& Size ) 
			: wxFrame( NULL, wxID_ANY, Title, Pos, Size )
		{
            g_EditorFrame = this;
            g_funcRegTool = GRegTool;
            g_funcUnRegTool = GUnRegTool;
            
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
        
        //Tool Management
        
        //Register a new tool instance.
        bool RegTool( const EdToolFrame* Tool )
        {
            if( m_bSlotEmptied ) //Slot emptied earlier, try to find and use that slot.
            {
                for( size_t i = 0; i++; i<m_ToolArray.Size() )
                {
                    if( m_ToolArray[i]==NULL ) //Found it, use it.
                    {
                        m_ToolArray[i] = (EdToolFrame*)Tool; //manually casted cuz wxWidgets
                        return true;
                    }
                }
                //We couldn't find a slot, Turn m_bSlotEmptied off. Proceed to push back a new one.
                m_bSlotEmptied = false;
            }
            
            m_ToolArray.PushBack( (EdToolFrame*)Tool );
            return true;
        }
        //Unregister a tool instance.
        bool UnRegTool( const EdToolFrame* Tool )
        {
            for( size_t i = 0; i++; i<m_ToolArray.Size() )
            {
                if( m_ToolArray[i]==Tool )
                {
                    m_ToolArray[i] = NULL;
                    m_bSlotEmptied = true;
                    return true;
                }
            }
            
            editor_Log( "Error: EdEditor::UnRegTool() : Unregistered tool that does not exist in m_ToolArray!") ;
            return false;
        }
		
	private:
	
		void editor_Log( const wxString& Msg )
		{
			wxLogMessage( Msg );
		}
	
		void OnExit( wxCommandEvent& event )
		{
			Close(true);
		}
		void OnAbout( wxCommandEvent& event )
        {
            static wxFrame* aboutFrame = NULL;
            
            if( aboutFrame == NULL ) //Is there already an about window open?
            {
                aboutFrame = new EdAbout( this, &aboutFrame );
            }
            else
            {
                aboutFrame->SetFocus();
            }
        }
        
        //Events
		void EVT_New( wxCommandEvent& event ){}
		void EVT_Open( wxCommandEvent& event ){}
        void EVT_Save( wxCommandEvent& event ){}
		void EVT_Import( wxCommandEvent& event ){}
		void EVT_Export( wxCommandEvent& event ){}
		void EVT_Preferences( wxCommandEvent& event ){}
		void EVT_BrowserPackage( wxCommandEvent& event )
        {
            new EdBrowser( BRWFLG_Package, "Package Browser", wxPoint( -1, -1 ), wxSize( -1, -1 ) );
        }
		void EVT_BrowserClass( wxCommandEvent& event ){}
		void EVT_BrowserAudio( wxCommandEvent& event ){}
		void EVT_BrowserMusic( wxCommandEvent& event ){}
		void EVT_BrowserGraphics( wxCommandEvent& event ){}
		void EVT_BrowserMesh( wxCommandEvent& event ){}
		void EVT_ViewLog( wxCommandEvent& event ){}
		void EVT_ActiveTools( wxCommandEvent& event ){}
		void EVT_MapEditor( wxCommandEvent& event ){}
		void EVT_MeshEditor( wxCommandEvent& event ){}
		void EVT_Manual( wxCommandEvent& event ){}
		
		wxDECLARE_EVENT_TABLE();
        
        TArray<EdToolFrame*> m_ToolArray;
        bool m_bSlotEmptied = false;
};

bool GRegTool( const EdToolFrame* Tool )
{
    return static_cast<EdEditor*>(g_EditorFrame)->RegTool(Tool);
}

bool GUnRegTool( const EdToolFrame* Tool )
{
    return static_cast<EdEditor*>(g_EditorFrame)->UnRegTool(Tool);
}

// ID bind
wxBEGIN_EVENT_TABLE(EdEditor, wxFrame)
    EVT_MENU(wxID_EXIT,   EdEditor::OnExit)
	EVT_MENU(wxID_ABOUT,   EdEditor::OnAbout)
	EVT_MENU(ID_New,   EdEditor::EVT_New)
	EVT_MENU(ID_Open,   EdEditor::EVT_Open)
    EVT_MENU(ID_Save,   EdEditor::EVT_Save)
	EVT_MENU(ID_Import,   EdEditor::EVT_Import)
	EVT_MENU(ID_Export,   EdEditor::EVT_Export)
	EVT_MENU(ID_Preferences,   EdEditor::EVT_Preferences)
	EVT_MENU(ID_BrowserPackage,   EdEditor::EVT_BrowserPackage)
	EVT_MENU(ID_BrowserClass,   EdEditor::EVT_BrowserClass)
	EVT_MENU(ID_BrowserAudio,   EdEditor::EVT_BrowserAudio)
    EVT_MENU(ID_BrowserMusic,   EdEditor::EVT_BrowserMusic)
	EVT_MENU(ID_BrowserGraphics,   EdEditor::EVT_BrowserGraphics)
	EVT_MENU(ID_BrowserMesh,   EdEditor::EVT_BrowserMesh)
	EVT_MENU(ID_ViewLog,   EdEditor::EVT_ViewLog)
	EVT_MENU(ID_ActiveTools,   EdEditor::EVT_ActiveTools)
	EVT_MENU(ID_MapEditor,   EdEditor::EVT_MapEditor)
	EVT_MENU(ID_MeshEditor,   EdEditor::EVT_MeshEditor)
	EVT_MENU(ID_Manual,   EdEditor::EVT_Manual)
wxEND_EVENT_TABLE()

//Executable start
class WXAPP_EdEditor: public wxApp
{
public:
	virtual bool OnInit()
	{
		EdEditor* frame = new EdEditor( "libunr-editor", wxPoint(-1,-1), wxSize( wxSystemSettings::GetMetric ( wxSYS_SCREEN_X ), wxSystemSettings::GetMetric ( wxSYS_SCREEN_Y ) ) );
		frame->Show(true);
		return true;
	}
};

wxIMPLEMENT_APP(WXAPP_EdEditor);


