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
 * ObjectViewer.h - Object Properties Tool, view and edit Objects.
 *
 * written by Jesse 'Hyzoran' Kowalik
 *========================================================================
*/
#pragma once

#include <wx/dataview.h>
#include <wx/splitter.h>
#include "EditorMain.h"

//NOTE: Design highly tentative as I figure out the nature of wxDataViewCtrl

//Control for rendering/editing a particular property.
//-Implements the actual node inheritence and expansion
//-Implements actual property rendering, editing them, other actions.
//-Rendering/Updating children updates all sub-children.

class wxEdPropertyViewCtrl;

class wxEdPropertyWindow : public wxControl
{
public:
  wxEdPropertyWindow( UObject* Property, wxEdPropertyWindow* parent );
  wxEdPropertyWindow( UObject* Property, wxEdPropertyViewCtrl* parent );

  void PropertyUpdate( bool bReadOnly = true ); //Update/Render property and its children recursively.

  bool AddChild( wxEdPropertyWindow* Child );

private:
  bool bExpanded = false;
  UObject* m_Property;
  TArray<wxEdPropertyWindow> m_Children;
};

//Control for browsing the properties of a UObject, by managing a tree of wxEdPropertyWindow nodes that handle rendering/editing property values.
//-Tracks root properties( those that have m_Obj as their m_Obj )
//-Descends into children properties and renders/updates them as nessecary.
//-Only real UI element is the scrollbar and window in which all wxEdPropertyWindows render.
class wxEdPropertyViewCtrl : public wxDataViewCtrl
{
public:
  wxEdPropertyViewCtrl( UObject* Obj, wxWindow* parent, bool bReadOnly = true, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = 0 );

  void TreeUpdate( wxEdPropertyWindow* ToUpdate = NULL ); //Update/render all children of a property, if none given, update ALL properties.

  void SetReadOnly( bool Value ); //Set this object to be read only.

private:
  UObject* m_Obj;
  TArray<wxEdPropertyWindow> m_Properties;
  bool m_bReadOnly;
};

//Actual wxFrame for viewing various properties of an Object, or even rendering a preview of it.
//--Planning to divide between the Properties view and some sort of convenience window, maybe such as rendering a selected property
//in a relevant way, or maybe selecting a property to isolate its own sub-tree in another wxEdPropertyCtrl instance.
class EdObjectViewer : public EdToolFrame
{
public:
  EdObjectViewer( UObject* Obj, bool bStartDocked = false, wxSize Size = GetFrameSize() );

  void PackagesAdded( size_t PackageStartIndex );
  void PackagesRemoved();
  void ObjectsAdded();
  void ObjectsRemoved();

private:
  UObject* m_Obj;
  wxSplitterWindow* m_MainFrame;
  wxEdPropertyViewCtrl* m_PropertyCtrl;
};