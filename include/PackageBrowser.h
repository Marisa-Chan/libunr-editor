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
 * PackageBrowser.h - Package Browser code
 *
 * written by Jesse 'Hyzoran' Kowalik & Adam 'Xaleros' Smith
 *========================================================================
*/

#include "Browser.h"

class EdPackageBrowser : public EdBrowser
{
public:
  EdPackageBrowser( bool bDock = false, wxSize Size = EdToolFrame::GetFrameSize() );
  ~EdPackageBrowser();

  virtual void PackagesAdded( size_t PackageStartIndex );
  virtual void PackagesRemoved();

  virtual void ObjectsAdded();
  virtual void ObjectsRemoved();

protected:
  virtual void Update();

  UPackage* m_SelectedPackage = NULL;
  wxWindow* m_PackageHeader = NULL;
  wxListCtrl* m_PackageInfo = NULL;
  wxListCtrl* m_PackageFlags = NULL;
  wxListCtrl* m_NameTable = NULL;
  wxListCtrl* m_ExportTable = NULL;
  wxDataViewCtrl* m_ExportTree = NULL;
  wxListCtrl* m_ImportTable = NULL;
  wxDataViewCtrl* m_ImportTree = NULL;
  wxWindow* m_ViewPane = NULL; //Object preview tab
  wxSplitterWindow* m_MainSplitter = NULL;
  wxComboBox* m_PackagesList = NULL;
  wxNotebook* m_TabWindow = NULL;

private:
  void OnPackageList( wxCommandEvent& event );
  wxDECLARE_EVENT_TABLE();
};
