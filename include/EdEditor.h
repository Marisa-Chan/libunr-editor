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
* EdEditor.h - Editor Core
*
* written by Jesse 'Hyzoran' Kowalik
*========================================================================
*/
#pragma once

#include <wx/wxprec.h>
#include <wx/wx.h>
#include <wx/aui/aui.h>
#include <wx/statline.h>
#include <wx/display.h>

#include <libunr.h>

namespace EdEditor
{

extern wxFrame* sm_MainFrame;

//Libunr interaction
void LoadPackages( const wxArrayString& Paths );
int GamePromptHandler( TArray<char*>* Names );
	extern int sm_SelectedGame;

wxString GetGameDir(); //Return absolute path of game directory.

//Engine stuff

void DoTick( wxIdleEvent& event );
	extern double CurrentTime;
	extern double LastTime;

//Static Resource references
extern wxBitmap g_bmpNew;
extern wxBitmap g_bmpDir;
extern wxBitmap g_bmpSave;
extern wxBitmap g_bmpPackage;
extern wxBitmap g_bmpClass;
extern wxBitmap g_bmpSound;
extern wxBitmap g_bmpMusic;
extern wxBitmap g_bmpGraphics;
extern wxBitmap g_bmpMesh;

extern wxIcon g_icoNew;
extern wxIcon g_icoDir;
extern wxIcon g_icoSave;
extern wxIcon g_icoPackage;
extern wxIcon g_icoClass;
extern wxIcon g_icoSound;
extern wxIcon g_icoMusic;
extern wxIcon g_icoGraphics;
extern wxIcon g_icoMesh;

void g_IcoInit();

//subdir suffix for different package types.
static const wxString gc_SubDir_U = "/System";
static const wxString gc_SubDir_UAX = "/Sounds";
static const wxString gc_SubDir_UMX = "/Music";
static const wxString gc_SubDir_UTX = "/Textures";
static const wxString gc_SubDir_UNR = "/Maps";
static const wxString gc_SubDir_USM = "/Meshes";
static const wxString gc_SubDir_USA = "/Save";

//========================================================================
// UObject Utility Classes

struct UObjectClientData : public wxClientData
{
public:
	inline UObject* GetObject() { return m_Object; }

private:
	UObject* m_Object;

};

class UObjectContextMenu : public wxMenu
{
public:
	UObjectContextMenu( UObject* Obj );
};

class UObjectExportDialog : public wxDialog
{
public:
	UObjectExportDialog( UObject* Obj );
};

class UObjectImportDialog : public wxDialog
{
public:
	UObjectImportDialog( wxString& Path );
};

//Prompt for starting new map/package/etc. from editorframe.
class UObjectNewDialog : public wxDialog
{
public:
	UObjectNewDialog();
};

};