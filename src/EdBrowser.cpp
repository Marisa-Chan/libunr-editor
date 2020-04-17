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
#pragma once

#include "EdBrowsers.h"

#define C_BUTTONSIZE 30
#define C_BUTTONCOLOUR wxColour( 130, 130, 130 )

EdBrowser::EdBrowser( wxString Title, bool bStartDocked )
	: EdToolFrame( EdEditor::sm_MainFrame, Title, bStartDocked )
{
	m_MenuFile = new wxMenu();
		m_MenuFile->Append( ID_New, "New...", "Make new Package." );
		m_MenuFile->Append( ID_Open, "Open...", "Load existing package." );
		m_MenuFile->AppendSeparator();
		m_MenuFile->Append( ID_Exit , "Exit...", "" );

	m_MenuBar = new wxMenuBar();
		m_MenuBar->Append( m_MenuFile, "File" );

	SetMenuBar( m_MenuBar );

	m_VSizer = new wxBoxSizer( wxVERTICAL );
		SetSizer( m_VSizer );

	m_HeaderPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxSize( -1, 32 ), wxBORDER_SIMPLE );
		m_HeaderSizer = new wxBoxSizer( wxHORIZONTAL );
		m_HeaderPanel->SetSizer( m_HeaderSizer );
		m_VSizer->Add( m_HeaderPanel, 0, wxEXPAND );

		wxBitmapButton* button_New = new wxBitmapButton( m_HeaderPanel, ID_New, EdEditor::g_bmpNew, wxDefaultPosition, wxSize( C_BUTTONSIZE, C_BUTTONSIZE ) );
		m_HeaderSizer->Add(button_New, 0, wxALIGN_CENTRE_VERTICAL);

		wxBitmapButton* button_Open = new wxBitmapButton( m_HeaderPanel, ID_Open, EdEditor::g_bmpDir, wxDefaultPosition, wxSize( C_BUTTONSIZE, C_BUTTONSIZE ) );
		m_HeaderSizer->Add(button_Open, 0, wxALIGN_CENTRE_VERTICAL);
}

EdBrowser::~EdBrowser()
{
}

ThreadReturnType EdBrowser::StaticThreadObjectUpdate( void* Browser )
{
	((EdBrowser*)Browser)->ObjectUpdate();

	return THREAD_SUCCESS;
}

void EdBrowser::PackageSelect( UPackage* Package )
{
	if( m_PackageCtrl == NULL )
		return;

	if( Package == NULL )
	{
		m_PackageCtrl->SetSelection( wxNOT_FOUND );
	}

	for( size_t i = 0; i<m_PackageCtrl->GetCount(); i++ )
	{
		UPackage* currentPackage = (UPackage*)((( EdEditor::UObjectClientData* )( m_PackageCtrl->GetClientObject( i ) ))->GetObject());

		if( currentPackage == Package )
		{
			m_PackageCtrl->SetSelection( i );
			return;
		}
	}

	m_PackageCtrl->SetSelection( wxNOT_FOUND );
}

UPackage* EdBrowser::GetSelectedPackage()
{
	if( m_PackageCtrl == NULL )
		return NULL;

	int select = m_PackageCtrl->GetSelection();

	if( select == wxNOT_FOUND )
		return NULL;

	return (UPackage*)((( EdEditor::UObjectClientData* )( m_PackageCtrl->GetClientObject( select ) ))->GetObject());
}

void EdBrowser::PackageListUpdate()
{
	if( m_PackageCtrl == NULL )
		return;

	UPackage* selected = GetSelectedPackage();

	m_PackageCtrl->Clear();

	for( size_t i = 0; i<UPackage::GetLoadedPackages()->Size(); i++ )
	{
		UPackage* currentPackage = (*(UPackage::GetLoadedPackages()))[i];

		m_PackageCtrl->Append( currentPackage->Name.Data(), new EdEditor::UObjectClientData( currentPackage ) );
	}

	PackageSelect( selected );
}

void EdBrowser::EVT_New( wxCommandEvent& event )
{

}

void EdBrowser::EVT_Open( wxCommandEvent& event )
{
	wxString subdir = EdEditor::GetGameDir() + wxString( m_dirPath );

	wxFileDialog openFileDialog( this, "Open Unreal Package", subdir, "", "UE Package Files (*.u, *.utx, *.uax\
    , *.umx, *.usm, *.unr)|*.u;*.u;*.utx;*.uax;*.umx;*.usm;*.unr", wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_MULTIPLE );

	if ( openFileDialog.ShowModal() == wxID_CANCEL )
		return;

	wxArrayString filePaths;
	openFileDialog.GetPaths( filePaths );
	EdEditor::LoadPackages( filePaths );
}

void EdBrowser::EVT_Import( wxCommandEvent& event )
{
}

void EdBrowser::EVT_Export( wxCommandEvent& event )
{
	new EdEditor::UObjectExportDialog( ((EdEditor::UObjectClientData*)( event.GetClientObject() ))->GetObject() );
}

wxBEGIN_EVENT_TABLE(EdBrowser, EdToolFrame)
	EVT_BUTTON(ID_New, EdBrowser::EVT_New)
	EVT_MENU(ID_New, EdBrowser::EVT_New)
	EVT_BUTTON(ID_Open, EdBrowser::EVT_Open)
	EVT_MENU(ID_Open, EdBrowser::EVT_Open)
	EVT_BUTTON(ID_Import, EdBrowser::EVT_Import)
	EVT_MENU(ID_Import, EdBrowser::EVT_Import)
	EVT_BUTTON(ID_Export, EdBrowser::EVT_Export)
	EVT_MENU(ID_Export, EdBrowser::EVT_Export)
wxEND_EVENT_TABLE()
