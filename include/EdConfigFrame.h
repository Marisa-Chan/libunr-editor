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
 * EdConfigFrame.h - ini editing frame.
 *
 * written by Jesse 'Hyzoran' Kowalik
 *========================================================================
*/
#pragma once

#include <wx/treelist.h>

#include "EdToolFrame.h"
#include "EdGamePrompt.h"

class EdConfigFrame : public EdToolFrame
{
public:
    EdConfigFrame( wxWindow* Parent = EdEditor::sm_MainFrame, FConfig* Config = NULL, wxString Section = "", bool bDock = false, EdGamePrompt* ModalPrompt = NULL ); //If Empty Section string, then read all ini sections.
    ~EdConfigFrame();

    void ObjectUpdate( bool m_bUpdatePackageList = true ) {} //ConfigFrame does not deal with Objects so do nothing.

    //Multi-purpose class to store config-entry references in memory, as well as "Fake" ones, to be written.
    struct ConfigDataItem : public wxClientData
    {
    public:
      ConfigDataItem( FConfig* Config, FConfig::FConfigCategory* Category = NULL, FConfig::FConfigEntry* Entry = NULL, int Index = -1, ConfigDataItem* Parent = NULL );
      ConfigDataItem( wxString ConfigStr = "", wxString CategoryStr = "", wxString EntryStr = "", ConfigDataItem* Parent = NULL );

      wxString GetDataName();
      bool WriteConfig(); //If it is fake, it will become non-fake.

      FConfig* m_Config = NULL;
      FConfig::FConfigCategory* m_Category = NULL;
      FConfig::FConfigEntry* m_Entry = NULL;
      int m_Index = -1;
      ConfigDataItem* m_Parent = NULL;
      bool m_bFake = false; //If this configdataitem is fake, it doesnt exist in ini yet.
      wxString m_ConfigStr = "";
      wxString m_CategoryStr = "";
      wxString m_EntryStr = "";
    };

    class EdConfigConfirm : public wxDialog
    {
    public:
      EdConfigConfirm( EdConfigFrame* Parent, TArray<wxTreeListItem>& Changed, wxTreeListCtrl* Ctrl, TArray<wxTreeListItem>& OutSelected );

    //Events
      enum
      {
        ID_Confirm
      };

      void EventCheckList( wxCommandEvent& event );
      void EventConfirm( wxCommandEvent& event );

    private:
      TArray<wxTreeListItem> m_Items;
      TArray<wxTreeListItem>* m_OutSelected;
      wxCheckListBox* m_Ctrl;
    };

    void Apply(); //Commit config edits.
    void ConfigRefresh(); //Re-read ini and render. Nessecary when ini changes are applied.
      void RecurseEntry( wxTreeListItem ParentItem, FConfig::FConfigEntry* Entry, ConfigDataItem BaseData ); //Recursively add an entry and its components.

    //Upcoming functions:
    //void NewEntry( ConfigDataItem Parent );

//Events
    enum
    {
        ID_Apply,
        ID_Refresh,
        ID_ObjectMenu,
        ID_ObjectPlay,
        ID_ObjectOpen
    };

    //Event Handlers

    void EventApply( wxCommandEvent& event );
    void EventRefresh( wxCommandEvent& event );
    void EventObjectMenu( wxCommandEvent& event );
    void EventObjectPlay( wxCommandEvent& event );
    void EventObjectOpen( wxCommandEvent& event );
    virtual void OnExit();

    wxDECLARE_EVENT_TABLE();

protected:
    EdGamePrompt* m_ModalPrompt = NULL;
    FConfig::FConfigCategory* m_Category;
    FConfig* m_Config;
    TArray< wxTreeListItem > m_Changed; //List of ConfigDataItems that have been changed.
    wxTreeListCtrl* m_Ctrl;

};