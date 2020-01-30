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

#include "EdMain.h"

class EdConfigFrame : public EdToolFrame
{
public:
    EdConfigFrame( FConfig* Config = NULL, wxString Section = "", bool bDock = false, EdGamePrompt* ModalWindow ); //If Empty Section string, then read all ini sections.

    //Multi-purpose class to store config-entry references in memory, as well as "Fake" ones, to be written.
    struct ConfigDataItem
    {
    public:
      ConfigDataItem( FConfig* Config, FConfigCategory* Category = NULL, FConfigEntry* Entry = NULL, int Index = -1, ConfigDataItem* Parent = NULL );
      ConfigDataItem( wxString ConfigStr = "", wxString CategoryStr = "", wxString EntryStr = "", ConfigDataItem* Parent = NULL );

      wxString GetDataName();
      bool WriteConfig(); //If it is fake, it will become non-fake.

      FConfig* m_Config = NULL;
      FConfigCategory* m_Category = NULL;
      FConfigEntry* m_Entry = NULL;
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

      void EVT_CheckList( wxCommandEvent& event );
      void EVT_Confirm( wxCommandEvent& event );

    private:
      TArray<wxTreeListItem> m_Items;
      TArray<wxTreeListItem>* m_OutSelected;
      wxCheckListBox* m_Ctrl;
    };

    void Apply(); //Commit config edits.
    void Refresh(); //Re-read ini and render. Nessecary when ini changes are applied.
      void RecurseEntry( wxTreeListItem ParentItem, FConfigEntry* Entry, ConfigDataItem BaseData ); //Recursively add an entry and its components.

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

    void EVT_Apply( wxCommandEvent& event );
    void EVT_Refresh( wxCommandEvent& event );
    void EVT_ObjectMenu( wxCommandEvent& event );
    void EVT_ObjectPlay( wxCommandEvent& event );
    void EVT_ObjectOpen( wxCommandEvent& event );
    virtual void OnExit( wxCommandEvent& event );

protected:
    FConfigCategory* m_Category;
    FConfig* m_Config;
    TArray<wxTreeListItem> m_Changed; //List of ConfigDataItems that have been changed.
    wxTreeListCtrl* m_Ctrl;

};