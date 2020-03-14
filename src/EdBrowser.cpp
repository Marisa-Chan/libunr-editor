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
* EdBrowser.cpp
*
* written by Jesse 'Hyzoran' Kowalik
*========================================================================
*/

#include "EdBrowsers.h"

EdBrowser::EdBrowser( wxString Title, bool bStartDocked )
	: EdToolFrame( EdEditor::sm_MainFrame, Title, bStartDocked )
{
	m_MenuFile = new wxMenu( "File" );
		m_MenuFile->Append( ID_New, "New...", "Make new Package." );
		m_MenuFile->Append( ID_Open, "Open...", "Load existing package." );

	m_MenuBar = new wxMenuBar();
		m_MenuBar->Append( m_MenuFile, "File" );

	SetMenuBar( m_MenuBar );

	m_VSizer = new wxBoxSizer( wxVERTICAL );
		SetSizer( m_VSizer );

	m_HeaderPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxSize( -1, 64 ) );
		m_HeaderSizer = new wxBoxSizer( wxHORIZONTAL );
		m_HeaderPanel->SetSizer( m_HeaderSizer );
		m_VSizer->Add( m_HeaderPanel, 1, wxEXPAND );
}

EdBrowser::~EdBrowser()
{
}

ThreadReturnType EdBrowser::StaticThreadObjectUpdate( void* Browser )
{
	((EdBrowser*)Browser)->ObjectUpdate();

	return THREAD_SUCCESS;
}

void EdBrowser::EVT_Import( wxCommandEvent& event )
{
	new EdEditor::UObjectImportDialog( ((EdEditor::UObjectClientData*)( event.GetClientObject() ))->GetObject() );
}

void EdBrowser::EVT_Export( wxCommandEvent& event )
{
	new EdEditor::UObjectExportDialog( ((EdEditor::UObjectClientData*)( event.GetClientObject() ))->GetObject() );
}


