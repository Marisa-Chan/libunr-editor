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
wxBitmap EdEditor::g_bmpTexture;
wxBitmap EdEditor::g_bmpMesh;
wxBitmap EdEditor::g_bmpMisc;

wxIcon EdEditor::g_icoNew;
wxIcon EdEditor::g_icoDir;
wxIcon EdEditor::g_icoSave;
wxIcon EdEditor::g_icoPackage;
wxIcon EdEditor::g_icoClass;
wxIcon EdEditor::g_icoSound;
wxIcon EdEditor::g_icoMusic;
wxIcon EdEditor::g_icoTexture;
wxIcon EdEditor::g_icoMesh;
wxIcon EdEditor::g_icoMisc;

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
    //Threaded Implementation
    for ( size_t i = 0; i < tools->Size(); i++ )
    {
        localThreads.PushBack( GSystem->RunThread( EdBrowser::StaticThreadObjectUpdate, (*tools)[i] ) );
    }

    //Wait for all tools to finish updating the new object list.
    for (size_t i = 0; i < localThreads.Size(); i++)
    {
        GSystem->JoinThread( localThreads[i] );
    }
    
    //Single Threaded implementation.
    /*
    for ( size_t i = 0; i < tools->Size(); i++ )
    {
      (*tools)[i]->ObjectUpdate();
    }
    */
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
  return wxString( GLibunrConfig->ReadString( "Game", "Path", sm_SelectedGame ) );
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
    g_bmpTexture = wxBitmap(wxT("res/bitmap/GraphicsBrowser.png"), wxBITMAP_TYPE_PNG);
    g_bmpMesh = wxBitmap(wxT("res/bitmap/MeshBrowser.png"), wxBITMAP_TYPE_PNG);
    g_bmpMisc = wxBitmap(wxT("res/bitmap/Misc.png"), wxBITMAP_TYPE_PNG);

    g_icoNew.CopyFromBitmap(g_bmpNew);
    g_icoDir.CopyFromBitmap(g_bmpDir);
    g_icoSave.CopyFromBitmap(g_bmpSave);
    g_icoPackage.CopyFromBitmap(g_bmpPackage);
    g_icoClass.CopyFromBitmap(g_bmpClass);
    g_icoSound.CopyFromBitmap(g_bmpSound);
    g_icoMusic.CopyFromBitmap(g_bmpMusic);
    g_icoTexture.CopyFromBitmap(g_bmpTexture);
    g_icoMesh.CopyFromBitmap(g_bmpMesh);
    g_icoMisc.CopyFromBitmap(g_bmpMisc);
}

UObject* EdEditor::g_LastMenuObject = NULL;

void EdEditor::PlayObject( TArray<UObject*> Objects )
{
  //Play Objects, only play the first object in the list though.
  if( ( Objects.Size() < 1 ) || ( Objects[0] == NULL ) )
    return;

  UObject* Obj = Objects[0];

  //USound and UMusic are audio files and played.
  if( Obj->IsA( USound::StaticClass() ) )
  {
    FVector ZeroVec;
    ZeroVec.X = 0;
    ZeroVec.Y = 0;
    ZeroVec.Z = 0;

    GEngine->Audio->PlaySound( NULL, (USound*)Obj, ZeroVec, 1, 1, 1 );
  }
  else if( Obj->IsA( UMusic::StaticClass() ) )
  {
    FVector ZeroVec;
    ZeroVec.X = 0;
    ZeroVec.Y = 0;
    ZeroVec.Z = 0;

    GEngine->Audio->PlayMusic( (UMusic*)Obj, 0, MTRAN_Instant );
  }
}

void EdEditor::EditObject( TArray<UObject*> Objects )
{
}

void EdEditor::ObjectExport( TArray<UObject*> Objects )
{
}

void EdEditor::ObjectProperties( TArray<UObject*> Objects )
{
}

EdEditor::UObjectContextMenu::UObjectContextMenu( wxWindow* Wnd, TArray<UObject*> Objects ) : m_Objects( Objects )
{
  if( Objects.IsEmpty() )
  {
    delete this;
    return;
  }

  Append( ID_ObjectActivate, "Play/Preview..." );
  Append( ID_ObjectEdit, "Edit..." );

  AppendSeparator();

  Append( ID_ObjectExport, "Export Object..." );

  AppendSeparator();

  Append( ID_ObjectProperties, "Properties..." );

  Wnd->PopupMenu( this );
}

void EdEditor::UObjectContextMenu::EVT_ObjectActivate( wxCommandEvent& event )
{
  EdEditor::PlayObject( m_Objects );
}

void EdEditor::UObjectContextMenu::EVT_ObjectEdit( wxCommandEvent& event )
{
  EdEditor::EditObject( m_Objects );
}

void EdEditor::UObjectContextMenu::EVT_ObjectExport( wxCommandEvent& event )
{
  EdEditor::ObjectExport( m_Objects );
}

void EdEditor::UObjectContextMenu::EVT_ObjectProperties( wxCommandEvent& event )
{
  EdEditor::ObjectProperties( m_Objects );
}

EdEditor::UObjectExportDialog::UObjectExportDialog(  UObject* Obj  )
{
}

EdEditor::UObjectImportDialog::UObjectImportDialog()
{
}

wxBEGIN_EVENT_TABLE( EdEditor::UObjectContextMenu, wxMenu )
  EVT_MENU( ID_ObjectActivate, EdEditor::UObjectContextMenu::EVT_ObjectActivate )
  EVT_MENU( ID_ObjectEdit, EdEditor::UObjectContextMenu::EVT_ObjectEdit )
  EVT_MENU( ID_ObjectExport, EdEditor::UObjectContextMenu::EVT_ObjectExport )
  EVT_MENU( ID_ObjectProperties, EdEditor::UObjectContextMenu::EVT_ObjectProperties )
wxEND_EVENT_TABLE()