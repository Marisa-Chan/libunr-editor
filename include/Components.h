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

class EdToolFrame : public wxFrame
{
public:
      EdToolFrame( bool bStartDocked = false );
      
      ~EdToolFrame();
      
      bool m_bDocked = false;
      const wxString m_DefaultName = "Editor Tool";

};

 //Editor Interface, Do not alter!
extern wxFrame* g_EditorFrame;
extern bool (*g_funcRegTool)( const EdToolFrame* Tool );
extern bool (*g_funcUnRegTool)( const EdToolFrame* Tool );

class EdAbout : public wxFrame
{
private:
	wxFrame** m_Selfptr;
	
//NOTE: If changing any things around here, probably will have to account for new sizing.
public:
	EdAbout( wxWindow* parent, wxFrame** ptr );
	
	void CloseAbout( wxCommandEvent& event );
};
