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
 * Components.h - Various internal components of the editor
 * 
 * written by Jesse 'Hyzoran' Kowalik
 *========================================================================
*/

#pragma once

#include <wx/wxprec.h>
#include <wx/wx.h>
#include <wx/wrapsizer.h>

#define C_BUTTONSIZE 34
#define C_BUTTONCOLOUR wxColour( 130, 130, 130 )

#define C_TOOLBUTTONSIZE 28

enum
{
  TOOL_Generic = 0,
  TOOL_Browser = 1
};

class EdToolFrame : public wxFrame
{
public:
  EdToolFrame( bool bStartDocked = false, wxSize Size = GetFrameSize() );
  ~EdToolFrame();

  bool m_bDocked = false;
  int m_ToolType = TOOL_Generic;
  size_t m_MyID;
    
  virtual void OnExit( wxCommandEvent& event );

  static wxSize GetFrameSize(); //Return the default framesize for toolframe.
    
  //Sysytem notifications, called by EdEditorFrame for all tools when nessecary.
  virtual void PackagesAdded( size_t PackageStartIndex ) = 0;
  virtual void PackagesRemoved() = 0;
  virtual void ObjectsAdded() = 0;
  virtual void ObjectsRemoved() = 0;
};

class EdAbout : public wxFrame
{
public:
	EdAbout( wxWindow* parent );
	
	void OnClose( wxCommandEvent& event );
  
  static bool IsOpened();

private:
  static bool sm_IsOpen;
};
