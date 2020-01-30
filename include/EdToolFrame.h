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
 * EdToolFrame.h - Editor tool objects - root class.
 *
 * written by Jesse 'Hyzoran' Kowalik
 *========================================================================
*/
#pragma once

#include "EdMain.h"

#define C_MINTOOLSIZE_X 512
#define C_MINTOOLSIZE_Y 384

class EdToolFrame : public wxFrame
(
public:
    EdToolFrame( wxString& Title = "", bool bStartDocked = false, EdGamePrompt* ModalWindow );
    ~EdToolFrame();

    static wxSize DefaultFrameSize();

  //Event Handlers
    virtual void OnExit( wxCommandEvent& event );

protected:
    wxWindow* m_ModalWindow;
    bool m_bDocked;
    int m_MyID;
);