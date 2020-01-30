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
 * EdGamePrompt.h - Game Prompt and configuration frontend.
 *
 * written by Jesse 'Hyzoran' Kowalik
 *========================================================================
*/
#pragma once

#include "EdConfigFrame.h"

class EdGamePrompt : public wxDialog
{
public: 
    EdGamePrompt( TArray<char*>* Names, int& OutGameIndex );

    void Refresh();
    void Select( int Choice );
    void OnEnable();

//Events
    enum
    {
      ID_Select,
      ID_Edit,
      ID_Cancel,
      ID_ListBox
    };

//Event Handlers

    void EVT_Select( wxCommandEvent& event );
    void EVT_Edit( wxCommandEvent& event );
    void EVT_ListBox( wxCommandEvent& event );
    void OnCancel( wxCommandEvent& event );

private:
  wxBoxSizer* hsizer;
  wxListBox* m_Ctrl;
  int* m_OutGameIndex;
};