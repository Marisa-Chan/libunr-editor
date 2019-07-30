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

#include "Components.inc"

// ID Table
enum
{
	ID_NewMap,
	ID_OpenMap,
	ID_NewPackage,
	ID_OpenPackage,
	ID_ImportAsset,
	ID_Preferences,
	ID_BrowserPackage,
	ID_BrowserActor,
	ID_BrowserAudio,
	ID_BrowserGraphics,
	ID_BrowserMesh,
	ID_ViewLog,
	ID_ActiveTools,
	ID_MapEditor,
	ID_MeshEditor,
	ID_Manual
};

//Editor frame, where the real magic happens.
class libunr_editor: public wxFrame
{
	public:
		libunr_editor( const wxString& Title, const wxPoint& Pos, const wxSize& Size ) 
			: wxFrame( NULL, wxID_ANY, Title, Pos, Size )
		{
			//Init interface
			wxMenu* menuFile = new wxMenu();
			
				menuFile->Append(ID_NewMap, "&New Map... \tCTRL+N",
					"Start a new map in the map tool");
				menuFile->Append(ID_OpenMap, "&Open Map... \tCTRL+O",
					"Open an existing map using the map tool");
				
				menuFile->AppendSeparator();
				
				menuFile->Append(ID_NewPackage, "&New Package... \tCTRL+N+P",
					"Create a new package in the editor");
				menuFile->Append(ID_OpenPackage, "&Open Package... \tCTRL+O+P",
					"Load a package into the editor");
				
				menuFile->AppendSeparator();
				
				menuFile->Append(ID_ImportAsset, "&Import Asset... \tCTRL+O+I",
					"Import an asset into a package");
				
				menuFile->AppendSeparator();
				
				menuFile->Append(wxID_EXIT, "&Exit...",
					"Close the editor");
			
			wxMenu* menuView = new wxMenu();
			
				menuView->Append(ID_Preferences, "&Preferences...", "Configure the editor");
				
				menuView->AppendSeparator();
				
				menuView->Append(ID_BrowserPackage, "&Package Browser...", "Open a Package browser instance");
				menuView->Append(ID_BrowserActor, "&Actor Browser...", "Open an Actor browser instance");
				menuView->Append(ID_BrowserAudio, "&Audio Browser...", "Open an Audio browser instance");
				menuView->Append(ID_BrowserGraphics, "&Graphics Browser...", "Open a Graphics browser instance");
				menuView->Append(ID_BrowserMesh, "&Mesh Browser...", "Open a mesh browser instance");
				
				menuView->AppendSeparator();
				
				menuView->Append(ID_ViewLog, "&Show Log...", "Show me the console fax");
				menuView->Append(ID_ActiveTools, "&Show Tools...", "Show all running tools");
			
			wxMenu* menuTools = new wxMenu();
			
				menuTools->Append(ID_MapEditor, "&Map Editor...", "Open a map editor instance");
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
            
            wxToolBar *toolbar = CreateToolBar();
            
            wxBitmap bitmap_NewMap( "res/bitmap/New.png", wxBITMAP_TYPE_PNG );
            toolbar->AddTool( ID_NewMap, wxT("New Map"), bitmap_NewMap );
            toolbar->Realize();
            
            wxBitmap bitmap_OpenMap( "res/bitmap/Open.png", wxBITMAP_TYPE_PNG );
            toolbar->AddTool( ID_NewMap, wxT("New Map"), bitmap_OpenMap );
            toolbar->Realize();
            
            wxBitmap bitmap_SaveMap( "res/bitmap/Save.png", wxBITMAP_TYPE_PNG );
            toolbar->AddTool( ID_NewMap, wxT("New Map"), bitmap_SaveMap );
            toolbar->Realize();
            
            wxBitmap bitmap_PackageBrowser( "res/bitmap/PackageBrowser.png", wxBITMAP_TYPE_PNG );
            toolbar->AddTool( ID_NewMap, wxT("New Map"), bitmap_PackageBrowser );
            toolbar->Realize();
            
            wxBitmap bitmap_ActorBrowser( "res/bitmap/ActorBrowser.png", wxBITMAP_TYPE_PNG );
            toolbar->AddTool( ID_NewMap, wxT("New Map"), bitmap_ActorBrowser );
            toolbar->Realize();
            
            wxBitmap bitmap_AudioBrowser( "res/bitmap/AudioBrowser.png", wxBITMAP_TYPE_PNG );
            toolbar->AddTool( ID_NewMap, wxT("New Map"), bitmap_AudioBrowser );
            toolbar->Realize();
            
            wxBitmap bitmap_GraphicsBrowser( "res/bitmap/GraphicsBrowser.png", wxBITMAP_TYPE_PNG );
            toolbar->AddTool( ID_NewMap, wxT("New Map"), bitmap_GraphicsBrowser );
            toolbar->Realize();
            
            wxBitmap bitmap_MeshBrowser( "res/bitmap/MeshBrowser.png", wxBITMAP_TYPE_PNG );
            toolbar->AddTool( ID_NewMap, wxT("New Map"), bitmap_MeshBrowser );
            toolbar->Realize();
            
            Connect( ID_NewMap, wxEVT_COMMAND_TOOL_CLICKED, 
            wxCommandEventHandler(libunr_editor::EVT_NewMap) );
			
			CreateStatusBar();
			SetStatusText( "Welcome to libunr!" );
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
                aboutFrame = new editor_about( this, &aboutFrame );
            }
            else
            {
                aboutFrame->SetFocus();
            }
        }
		void EVT_NewMap( wxCommandEvent& event ){}
		void EVT_OpenMap( wxCommandEvent& event ){}
		void EVT_NewPackage( wxCommandEvent& event ){}
		void EVT_OpenPackage( wxCommandEvent& event ){}
		void EVT_ImportAsset( wxCommandEvent& event ){}
		void EVT_Preferences( wxCommandEvent& event ){}
		void EVT_BrowserPackage( wxCommandEvent& event ){}
		void EVT_BrowserActor( wxCommandEvent& event ){}
		void EVT_BrowserAudio( wxCommandEvent& event ){}
		void EVT_BrowserGraphics( wxCommandEvent& event ){}
		void EVT_BrowserMesh( wxCommandEvent& event ){}
		void EVT_ViewLog( wxCommandEvent& event ){}
		void EVT_ActiveTools( wxCommandEvent& event ){}
		void EVT_MapEditor( wxCommandEvent& event ){}
		void EVT_MeshEditor( wxCommandEvent& event ){}
		void EVT_Manual( wxCommandEvent& event ){}
		
		wxDECLARE_EVENT_TABLE();
};

// ID bind
wxBEGIN_EVENT_TABLE(libunr_editor, wxFrame)
    EVT_MENU(wxID_EXIT,   libunr_editor::OnExit)
	EVT_MENU(wxID_ABOUT,   libunr_editor::OnAbout)
	EVT_MENU(ID_NewMap,   libunr_editor::EVT_NewMap)
	EVT_MENU(ID_OpenMap,   libunr_editor::EVT_OpenMap)
	EVT_MENU(ID_NewPackage,   libunr_editor::EVT_NewPackage)
	EVT_MENU(ID_OpenPackage,   libunr_editor::EVT_OpenPackage)
	EVT_MENU(ID_ImportAsset,   libunr_editor::EVT_ImportAsset)
	EVT_MENU(ID_Preferences,   libunr_editor::EVT_Preferences)
	EVT_MENU(ID_BrowserPackage,   libunr_editor::EVT_BrowserPackage)
	EVT_MENU(ID_BrowserActor,   libunr_editor::EVT_BrowserActor)
	EVT_MENU(ID_BrowserAudio,   libunr_editor::EVT_BrowserAudio)
	EVT_MENU(ID_BrowserGraphics,   libunr_editor::EVT_BrowserGraphics)
	EVT_MENU(ID_BrowserMesh,   libunr_editor::EVT_BrowserMesh)
	EVT_MENU(ID_ViewLog,   libunr_editor::EVT_ViewLog)
	EVT_MENU(ID_ActiveTools,   libunr_editor::EVT_ActiveTools)
	EVT_MENU(ID_MapEditor,   libunr_editor::EVT_MapEditor)
	EVT_MENU(ID_MeshEditor,   libunr_editor::EVT_MeshEditor)
	EVT_MENU(ID_Manual,   libunr_editor::EVT_Manual)
wxEND_EVENT_TABLE()

//Executable start
class WXAPP_libunr_editor: public wxApp
{
public:
	virtual bool OnInit()
	{
		libunr_editor* frame = new libunr_editor( "libunr-editor", wxPoint(-1,-1), wxSize(-1,-1) );
		frame->Show(true);
		return true;
	}
};

wxIMPLEMENT_APP(WXAPP_libunr_editor);


