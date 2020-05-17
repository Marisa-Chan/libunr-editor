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

#define C_BUTTONSIZE 30
#define C_BUTTONCOLOUR wxColour( 130, 130, 130 )

EdBrowser::EdBrowser( wxWindow* Parent )
	: wxWindow( Parent, wxID_ANY )
{
	m_VSizer = new wxBoxSizer( wxVERTICAL );
		SetSizer( m_VSizer );

	m_HeaderPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxSize( -1, 32 ), wxBORDER_SIMPLE );
	m_HeaderSizer = new wxBoxSizer( wxHORIZONTAL );
	m_HeaderPanel->SetSizer( m_HeaderSizer );
	m_VSizer->Add( m_HeaderPanel, 0, wxEXPAND );

	m_PackageCtrl = new EdEditor::EdUPackageCtrl( m_HeaderPanel );
	m_HeaderSizer->Add( m_PackageCtrl, 0, 0, 4 );

	m_CheckFilterPackage = new wxCheckBox( m_HeaderPanel, EdToolFrame::ID_bFilterPackageCheck, wxString("Filter Package?") );
	m_CheckFilterPackage->SetValue( false );
	m_HeaderSizer->Add( m_CheckFilterPackage, 0 );
}

EdBrowser::~EdBrowser()
{
}

void EdBrowser::EVT_FilterPackage( wxCommandEvent& event )
{
	if( m_PackageCtrl == NULL )
		return;

	ObjectUpdate( false );
}

void EdBrowser::EVT_FilterPackageCtrl( wxCommandEvent& event )
{
	if( m_PackageCtrl == NULL )
		return;

	if( !m_CheckFilterPackage->GetValue() )
		return;

	ObjectUpdate( false );
}

wxBEGIN_EVENT_TABLE(EdBrowser, wxWindow)
	EVT_CHECKBOX( EdToolFrame::ID_bFilterPackageCheck, EdBrowser::EVT_FilterPackage )
	EVT_COMBOBOX( EdToolFrame::ID_PackageCtrl, EdBrowser::EVT_FilterPackageCtrl )
wxEND_EVENT_TABLE()
