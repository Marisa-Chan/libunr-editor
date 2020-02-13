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
* EdEditor.cpp
*
* written by Jesse 'Hyzoran' Kowalik
*========================================================================
*/

#include "EdEditor.h"
#include "EdGamePrompt.h"
#include "EdBrowsers.h"

wxFrame* EdEditor::sm_MainFrame = NULL;
int EdEditor::sm_SelectedGame;
double EdEditor::CurrentTime;
double EdEditor::LastTime;

wxBitmap EdEditor::g_bmpNew;
wxBitmap EdEditor::g_bmpDir;
wxBitmap EdEditor::g_bmpSave;
wxBitmap EdEditor::g_bmpPackage;
wxBitmap EdEditor::g_bmpClass;
wxBitmap EdEditor::g_bmpSound;
wxBitmap EdEditor::g_bmpMusic;
wxBitmap EdEditor::g_bmpGraphics;
wxBitmap EdEditor::g_bmpMesh;

wxIcon EdEditor::g_icoNew;
wxIcon EdEditor::g_icoDir;
wxIcon EdEditor::g_icoSave;
wxIcon EdEditor::g_icoPackage;
wxIcon EdEditor::g_icoClass;
wxIcon EdEditor::g_icoSound;
wxIcon EdEditor::g_icoMusic;
wxIcon EdEditor::g_icoGraphics;
wxIcon EdEditor::g_icoMesh;

void EdEditor::LoadPackages( const wxArrayString& Paths )
{
    for ( size_t i = 0; i < Paths.GetCount(); i++ )
    {
        UPackage* p = UPackage::StaticLoadPackage( Paths[i], false );
        p->LoadEditableTypes();
    }

    TArray<void*> localThreads;
    TArray<EdToolFrame*>* tools = EdToolFrame::GetTools();

    //Notify tools that Packages and Objects were updated, by running their Update functions on new threads.
    for ( size_t i = 0; i < tools->Size(); i++ )
    {
        localThreads.PushBack( GSystem->RunThread( EdBrowser::StaticThreadObjectUpdate, (*tools)[i] ) );
    }

    //Wait for all tools to finish updating the new object list.
    for (size_t i = 0; i < localThreads.Size(); i++)
    {
        GSystem->JoinThread( localThreads[i] );
    }
}

int EdEditor::GamePromptHandler( TArray<char*>* Names )
{
    int selected = -1;

    EdGamePrompt gamePrompt( Names, selected );
    gamePrompt.ShowModal();

    sm_SelectedGame = selected;

    return selected;
}

wxString EdEditor::GetGameDir()
{
    return wxString( GGameConfig->ReadString( "Game", "Path", sm_SelectedGame ) );
}

void EdEditor::DoTick( wxIdleEvent& event )
{
    CurrentTime = USystem::GetSeconds();

    double DeltaTime = CurrentTime - LastTime;
    GEngine->Tick(DeltaTime);

    LastTime = CurrentTime;

    event.RequestMore();
}

void EdEditor::g_IcoInit()
{
    g_bmpNew = wxBitmap(wxT("res/bitmap/New.png"), wxBITMAP_TYPE_PNG);
    g_bmpDir = wxBitmap(wxT("res/bitmap/Open.png"), wxBITMAP_TYPE_PNG);
    g_bmpSave = wxBitmap(wxT("res/bitmap/Save.png"), wxBITMAP_TYPE_PNG);
    g_bmpPackage = wxBitmap(wxT("res/bitmap/PackageBrowser.png"), wxBITMAP_TYPE_PNG);
    g_bmpClass = wxBitmap(wxT("res/bitmap/ClassBrowser.png"), wxBITMAP_TYPE_PNG);
    g_bmpSound = wxBitmap(wxT("res/bitmap/SoundBrowser.png"), wxBITMAP_TYPE_PNG);
    g_bmpMusic = wxBitmap(wxT("res/bitmap/MusicBrowser.png"), wxBITMAP_TYPE_PNG);
    g_bmpGraphics = wxBitmap(wxT("res/bitmap/GraphicsBrowser.png"), wxBITMAP_TYPE_PNG);
    g_bmpMesh = wxBitmap(wxT("res/bitmap/MeshBrowser.png"), wxBITMAP_TYPE_PNG);

    g_icoNew.CopyFromBitmap(g_bmpNew);
    g_icoDir.CopyFromBitmap(g_bmpDir);
    g_icoSave.CopyFromBitmap(g_bmpSave);
    g_icoPackage.CopyFromBitmap(g_bmpPackage);
    g_icoClass.CopyFromBitmap(g_bmpClass);
    g_icoSound.CopyFromBitmap(g_bmpSound);
    g_icoMusic.CopyFromBitmap(g_bmpMusic);
    g_icoGraphics.CopyFromBitmap(g_bmpGraphics);
    g_icoMesh.CopyFromBitmap(g_bmpMesh);
}