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
 * SoundBrowser.h - Sound browser definition
 *
 * written by Jesse 'Hyzoran' Kowalik & Adam 'Xaleros' Smith
 *========================================================================
*/

#include "EditorMain.h"
#include "SoundBrowser.h"
#include <Engine/USound.h>

EdSoundBrowser::EdSoundBrowser( bool bDock, wxSize Size )
  : EdBrowser( USound::StaticClass(), bDock, Size )
{
  SetLabel( wxString( "Sound Browser" ) );
  SetIcon( EdEditorFrame::sm_icoSound );
  
  ConstructPackageButtons();

  // TODO: Support multiple view types for browsers that could make use of them
  ListConstruct();
}

EdSoundBrowser::~EdSoundBrowser()
{
}

void EdSoundBrowser::Update()
{
  ListUpdate();
}