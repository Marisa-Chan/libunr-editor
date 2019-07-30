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
 * Components.inc - Various internal components of the editor.
 * 
 * written by Jesse 'Hyzoran' Kowalik
 *========================================================================
*/
#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <wx/wrapsizer.h>

class editor_about : public wxFrame
{
private:
	wxFrame** m_Selfptr;
	
//NOTE: If changing any things around here, probably will have to account for new sizing.
public:
	editor_about( wxWindow* parent, wxFrame** ptr )
		: wxFrame( parent, -1, "About", wxDefaultPosition, wxSize(420,400) ), m_Selfptr(ptr)
		{
		
		wxBoxSizer* ContentSizer = new wxBoxSizer( wxVERTICAL );
		
		wxStaticText* aboutlibunr = new wxStaticText( this, -1, "LIBUNR\nCopyright (C) 2018-2019  Adam W.E. Smith", wxDefaultPosition, wxDefaultSize );
		ContentSizer->Add( aboutlibunr, 0, wxALL | wxALIGN_CENTRE_HORIZONTAL, 2 );
		
		wxStaticText* aboutlibunr2 = new wxStaticText( this, -1, "Open Source Re-Implementation of Unreal Engine 1", wxDefaultPosition, wxDefaultSize );
		ContentSizer->Add( aboutlibunr2, 0, wxALL | wxALIGN_CENTRE_HORIZONTAL, 2 );
		aboutlibunr2->Wrap( 400 );
		
		wxTextCtrl* libunrlink = new wxTextCtrl
		( this, -1, "https://bitbucket.org/Xaleros/libunr/src/master/", wxDefaultPosition, wxSize( 400, 20 ), wxTE_READONLY );
		ContentSizer->Add( libunrlink, 0, wxALL | wxALIGN_CENTRE_HORIZONTAL, 2 );
		
		
		wxStaticText* aboutlibunreditor = new wxStaticText( this, -1, "LIBUNR-EDITOR\nCopyright (C) 2018-2019  Adam W.E. Smith\nWritten by Jesse \"Hyzoran\" Kowalik", wxDefaultPosition, wxDefaultSize );
		ContentSizer->Add( aboutlibunreditor, 0, wxALL | wxALIGN_CENTRE_HORIZONTAL, 2 );
		
		wxStaticText* aboutlibunreditor2 = new wxStaticText( this, -1, "Graphical Frontend and Development suite for libunr", wxDefaultPosition, wxDefaultSize );
		ContentSizer->Add( aboutlibunreditor2, 0, wxALL | wxALIGN_CENTRE_HORIZONTAL, 2 );
		aboutlibunreditor2->Wrap( 400 );
		
		wxTextCtrl* libunrlink2 = new wxTextCtrl
		( this, -1, "https://bitbucket.org/Xaleros/libunr-editor/src/master/", wxDefaultPosition, wxSize( 400, 20 ), wxTE_READONLY );
		ContentSizer->Add( libunrlink2, 0, wxALL | wxALIGN_CENTRE_HORIZONTAL, 2 );
		
		wxPanel* padding = new wxPanel( this );
		ContentSizer->Add( padding, 1, wxALL, 0 );
		
		wxButton* closeb = new wxButton(this, wxID_OK , "Ok" );
		ContentSizer->Add( closeb, 0, wxALIGN_BOTTOM | wxALIGN_CENTRE_HORIZONTAL );
		
		Connect( wxID_OK , wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(editor_about::CloseAbout) );
		
		this->SetSizer(ContentSizer);
		
		closeb->SetFocus();
		Centre();
		
		SetMinSize( wxSize(420,400) );
		this->Show(true);
	}
	
	void CloseAbout( wxCommandEvent& event )
	{
		*m_Selfptr = NULL;
		Close(true);
	}
};