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

//========================================================================
// Engine-Interaction - Game Config

	void LoadPackages( const wxArrayString& Paths );
	int GamePromptHandler( TArray<char*>* Names );
	extern int sm_SelectedGame;

	wxString GetGameDir(); //Return absolute path of game directory.

//========================================================================
// Engine-Interaction - Ticking

	void DoTick( wxIdleEvent& event );
	extern double CurrentTime;
	extern double LastTime;

//========================================================================
// Static Resource variables
	extern wxBitmap g_bmpNew;
	extern wxBitmap g_bmpDir;
	extern wxBitmap g_bmpSave;
	extern wxBitmap g_bmpPackage;
	extern wxBitmap g_bmpClass;
	extern wxBitmap g_bmpSound;
	extern wxBitmap g_bmpMusic;
	extern wxBitmap g_bmpTexture;
	extern wxBitmap g_bmpMesh;
	extern wxBitmap g_bmpMisc;

	extern wxIcon g_icoNew;
	extern wxIcon g_icoDir;
	extern wxIcon g_icoSave;
	extern wxIcon g_icoPackage;
	extern wxIcon g_icoClass;
	extern wxIcon g_icoSound;
	extern wxIcon g_icoMusic;
	extern wxIcon g_icoTexture;
	extern wxIcon g_icoMesh;
	extern wxIcon g_icoMisc;

	void g_IcoInit(); //Init resources

//subdir suffix for different package types.
static const wxString gc_SubDir_U = "/System";
static const wxString gc_SubDir_UAX = "/Sounds";
static const wxString gc_SubDir_UMX = "/Music";
static const wxString gc_SubDir_UTX = "/Textures";
static const wxString gc_SubDir_UNR = "/Maps";
static const wxString gc_SubDir_USM = "/Meshes";
static const wxString gc_SubDir_USA = "/Save";

//========================================================================
// UObject Utility Functions

	extern UObject* g_LastMenuObject; //Used to pass the Object in question from event
	//to these functions

	void PlayObject( TArray<UObject*> Objects );
	void EditObject( TArray<UObject*> Objects );
	void ObjectExport( TArray<UObject*> Objects );
	void ObjectProperties( TArray<UObject*> Objects );

//========================================================================
// UObject Utility Classes

	enum
	{
		ID_ObjectActivate,
		ID_ObjectEdit,
		ID_ObjectExport,
		ID_ObjectProperties
	};

	struct UObjectClientData : public wxClientData
	{
	public:
		UObjectClientData( UObject* Obj, u32 Group = 0 ) : m_Object(Obj), m_Group(Group) {}
		inline UObject* GetObject() { return m_Object; }
		inline u32 GetGroup() { return m_Group; }

	private:
		UObject* m_Object;
		u32 m_Group;

	};

	class UObjectContextMenu : public wxMenu
	{
	public:
		UObjectContextMenu( wxWindow* Wnd, TArray<UObject*> Objects );

		void EVT_ObjectActivate( wxCommandEvent& event );
		void EVT_ObjectEdit( wxCommandEvent& event );
		void EVT_ObjectExport( wxCommandEvent& event );
		void EVT_ObjectProperties( wxCommandEvent& event );

	protected:
		TArray<UObject*> m_Objects;

		wxDECLARE_EVENT_TABLE();
	};

	class UObjectExportDialog : public wxDialog
	{
	public:
		UObjectExportDialog( UObject* Obj );
	};

	class UObjectImportDialog : public wxDialog
	{
	public:
		UObjectImportDialog();
	};

	//Prompt for starting new map/package/etc. from editorframe.
	class UObjectNewDialog : public wxDialog
	{
	public:
		UObjectNewDialog();
	};

	//========================================================================
	// "Renders" or otherwise previews a UObject in some way.
	class UObjectPreviewWindow : public wxWindow
	{
	public:
		UObjectPreviewWindow( wxWindow* Parent );
	};

	//========================================================================
	// EdUPackageCtrl - Control used to "Select" a UPackage.
	class EdUPackageCtrl : public wxComboBox
	{
	public:
		EdUPackageCtrl( wxWindow* Parent );

		void PackageListUpdate();
		UPackage* GetSelectedPackage();

	private:

	};

};