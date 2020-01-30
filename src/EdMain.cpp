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
 * EdMain.cpp
 *
 * written by Jesse 'Hyzoran' Kowalik
 *========================================================================
*/

#include "EdMain.h"
#include <Editor/UEditorEngine.h>

wxBitmap EdEditorFrame::sm_bmpNew;
wxBitmap EdEditorFrame::sm_bmpDir;
wxBitmap EdEditorFrame::sm_bmpSave;
wxBitmap EdEditorFrame::sm_bmpPackage;
wxBitmap EdEditorFrame::sm_bmpClass;
wxBitmap EdEditorFrame::sm_bmpSound;
wxBitmap EdEditorFrame::sm_bmpMusic;
wxBitmap EdEditorFrame::sm_bmpGraphics;
wxBitmap EdEditorFrame::sm_bmpMesh;
wxIcon EdEditorFrame::sm_icoNew;
wxIcon EdEditorFrame::sm_icoDir;
wxIcon EdEditorFrame::sm_icoSave;
wxIcon EdEditorFrame::sm_icoPackage;
wxIcon EdEditorFrame::sm_icoClass;
wxIcon EdEditorFrame::sm_icoSound;
wxIcon EdEditorFrame::sm_icoMusic;
wxIcon EdEditorFrame::sm_icoGraphics;
wxIcon EdEditorFrame::sm_icoMesh;
wxString EdEditorFrame::csm_SubDir_U;
wxString EdEditorFrame::csm_SubDir_UAX;
wxString EdEditorFrame::csm_SubDir_UMX;
wxString EdEditorFrame::csm_SubDir_UTX;
wxString EdEditorFrame::csm_SubDir_UNR;
wxString EdEditorFrame::csm_SubDir_USM;
wxString EdEditorFrame::csm_SubDir_USA;

EdEditorFrame::EdEditorFrame(const wxString& Title, const wxPoint& Pos, const wxSize& Size)
    : wxFrame(NULL, wxID_ANY, Title, Pos, Size)
{
    //Init interface
    wxMenu* menuFile = new wxMenu();

    menuFile->Append(ID_New, "&New... \tCTRL+N",
        "Start a new map/Package");
    menuFile->Append(ID_Open, "&Open... \tCTRL+O",
        "Open an existing map or package");
    menuFile->Append(ID_Save, "&Save All... \tCTRL+O",
        "Save all modified maps/packages");

    menuFile->AppendSeparator();

    menuFile->Append(ID_Import, "&Import... \tCTRL+O+I",
        "Import an asset into a package");

    menuFile->Append(ID_Export, "&Export... \tCTRL+O+E",
        "Export Contents of Package/Map");

    menuFile->AppendSeparator();

    menuFile->Append(wxID_EXIT, "&Exit...",
        "Close the editor");

    wxMenu* menuView = new wxMenu();

    menuView->Append(ID_Preferences, "&Preferences...", "Configure the editor");

    menuView->AppendSeparator();

    menuView->Append(ID_BrowserPackage, "&Package Browser...", "Open a Package browser instance");
    menuView->Append(ID_BrowserClass, "&Class Browser...", "Open an Class browser instance");
    menuView->Append(ID_BrowserSound, "&Sound Browser...", "Open a Sound browser instance");
    menuView->Append(ID_BrowserMusic, "&Music Browser...", "Open a Music browser instance");
    menuView->Append(ID_BrowserGraphics, "&Graphics Browser...", "Open a Graphics browser instance");
    menuView->Append(ID_BrowserMesh, "&Mesh Browser...", "Open a mesh browser instance");

    menuView->AppendSeparator();

    menuView->Append(ID_ViewLog, "&Show Log...", "Show me the console fax");
    menuView->Append(ID_ActiveTools, "&Show Tools...", "Show all running tools");

    wxMenu* menuTools = new wxMenu();

    menuTools->Append(ID_MapEditor, "&Map Editor...", "New editor tab.");
    menuTools->Append(ID_MeshEditor, "&Mesh Editor...", "Used for advanced brush-building, quick-prefabs, 2d-shape editor, etc.");

    wxMenu* menuHelp = new wxMenu();

    menuHelp->Append(ID_Manual, "&Editor Manual...", "Documentation shipped with the editor");
    menuHelp->Append(wxID_ABOUT, "&About OpenUE...", "About OpenUE");

    wxMenuBar* menuBar = new wxMenuBar();

    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuView, "&View");
    menuBar->Append(menuTools, "&Tools");
    menuBar->Append(menuHelp, "&Help");

    SetMenuBar(menuBar);

    wxBoxSizer* contentSizer = new wxBoxSizer(wxVERTICAL);

    wxPanel* toolBar = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(-1, 40), wxBORDER_RAISED);

    contentSizer->Add(toolBar, 0, wxALIGN_TOP | wxALIGN_LEFT | wxEXPAND);

    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);

    wxBitmapButton* button_NewMap = new wxBitmapButton(toolBar, ID_New, EdEditorFrame::sm_bmpNew, wxDefaultPosition, wxSize(C_BUTTONSIZE, C_BUTTONSIZE));
    buttonSizer->Add(button_NewMap, 0, wxALIGN_CENTRE_VERTICAL);

    wxBitmapButton* button_OpenMap = new wxBitmapButton(toolBar, ID_Open, EdEditorFrame::sm_bmpDir, wxDefaultPosition, wxSize(C_BUTTONSIZE, C_BUTTONSIZE));
    buttonSizer->Add(button_OpenMap, 0, wxALIGN_CENTRE_VERTICAL);

    wxBitmapButton* button_Save = new wxBitmapButton(toolBar, ID_Save, EdEditorFrame::sm_bmpSave, wxDefaultPosition, wxSize(C_BUTTONSIZE, C_BUTTONSIZE));
    buttonSizer->Add(button_Save, 0, wxALIGN_CENTRE_VERTICAL);

    buttonSizer->AddSpacer(C_BUTTONSIZE);

    wxBitmapButton* button_PackageBrowser = new wxBitmapButton(toolBar, ID_BrowserPackage, EdEditorFrame::sm_bmpPackage, wxDefaultPosition, wxSize(C_BUTTONSIZE, C_BUTTONSIZE));
    buttonSizer->Add(button_PackageBrowser, 0, wxALIGN_CENTRE_VERTICAL);

    wxBitmapButton* button_ClassBrowser = new wxBitmapButton(toolBar, ID_BrowserClass, EdEditorFrame::sm_bmpClass, wxDefaultPosition, wxSize(C_BUTTONSIZE, C_BUTTONSIZE));
    buttonSizer->Add(button_ClassBrowser, 0, wxALIGN_CENTRE_VERTICAL);

    wxBitmapButton* button_SoundBrowser = new wxBitmapButton(toolBar, ID_BrowserSound, EdEditorFrame::sm_bmpSound, wxDefaultPosition, wxSize(C_BUTTONSIZE, C_BUTTONSIZE));
    buttonSizer->Add(button_SoundBrowser, 0, wxALIGN_CENTRE_VERTICAL);

    wxBitmapButton* button_MusicBrowser = new wxBitmapButton(toolBar, ID_BrowserMusic, EdEditorFrame::sm_bmpMusic, wxDefaultPosition, wxSize(C_BUTTONSIZE, C_BUTTONSIZE));
    buttonSizer->Add(button_MusicBrowser, 0, wxALIGN_CENTRE_VERTICAL);

    wxBitmapButton* button_GraphicsBrowser = new wxBitmapButton(toolBar, ID_BrowserGraphics, EdEditorFrame::sm_bmpGraphics, wxDefaultPosition, wxSize(C_BUTTONSIZE, C_BUTTONSIZE));
    buttonSizer->Add(button_GraphicsBrowser, 0, wxALIGN_CENTRE_VERTICAL);

    wxBitmapButton* button_MeshBrowser = new wxBitmapButton(toolBar, ID_BrowserMesh, EdEditorFrame::sm_bmpMesh, wxDefaultPosition, wxSize(C_BUTTONSIZE, C_BUTTONSIZE));
    buttonSizer->Add(button_MeshBrowser, 0, wxALIGN_CENTRE_VERTICAL);

    toolBar->SetSizer(buttonSizer);

    wxPanel* tabBar = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(-1, 24), wxBORDER_NONE);
    contentSizer->Add(tabBar, 0, wxALIGN_TOP | wxEXPAND);

    wxBoxSizer* contentSizer2 = new wxBoxSizer(wxHORIZONTAL);
    contentSizer->Add(contentSizer2, 1, wxEXPAND);

    wxPanel* toolBar2 = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(80, -1), wxBORDER_RAISED);
    contentSizer2->Add(toolBar2, 0, wxALIGN_LEFT | wxEXPAND);

    wxPanel* backGround = new wxPanel(this);
    backGround->SetBackgroundColour(wxColour(80, 80, 80));
    contentSizer2->Add(backGround, 1, wxEXPAND);

    SetSizer(contentSizer);

    CreateStatusBar();
    SetStatusText("Welcome to OpenUEd!");
}

EdEditorFrame::~EdEditorFrame()
{
}

size_t EdEditorFrame::RegisterTool(EdToolFrame* Tool)
{
    if (sm_EmptySlots > 0) //A slot was freed earlier, find and use that slot.
    {
        for (size_t i = 0; i < sm_ToolArray.Size(); i++)
        {
            if (sm_ToolArray[i] == NULL)
            {
                m_ToolArray[i] = Tool;
                m_EmptySlots--; //We used a slot.
                return i;
            }
        }
        //None found? Something went wrong.
        GLogf(LOG_WARN, "EdEditorFrame::RegTool() : sm_EmptySlots > 0, but none found in sm_ToolArray!");
        GLogf(LOG_WARN, "Possible memory corruption... Pushing new tool to end of array...");
    }
    m_ToolArray.PushBack((EdToolFrame*)Tool);
    return sm_ToolArray.Size() - 1;
}

bool EdEditorFrame::UnregisterTool(size_t id)
{
    if (sm_ToolArray[id] != NULL)
    {
        m_ToolArray[id] = NULL;
        m_EmptySlots++;
        return true;
    }
    GLogf(LOG_WARN, "EdEditor::UnregisterTool() : Unregistered tool that does not exist in sm_ToolArray!");
    return false;
}

void EdEditorFrame::LoadPackages(const wxArrayString& Paths)
{
    for ( size_t i = 0; i < Paths.GetCount(); i++ )
    {
        UPackage* p = UPackage::StaticLoadPackage( Paths[i], false );
        p->LoadEditableTypes();
    }

    TArray<void*> localThreads;

    //Notify tools that Packages and Objects were updated, by running their Update functions on new threads.
    for ( size_t i = 0; i < m_ToolArray.Size(); i++ )
    {
        localThreads.PushBack( USystem::RunThread( &EdBrowser::ObjectUpdate( m_ToolArray[i] ), NULL ) );
    }

    //Wait for all tools to finish updating the new object list.
    for (size_t i = 0; i < m_localThreads.Size(); i++)
    {
        USystem::JoinThread( m_localThreads[i] );
    }
}

void EdEditorFrame::ObjectMenu(UObject* Obj)
{
    //TODO: Does a popped-up menu get automatically destroyed when closed? If not, memory leak. :<
    ObjectPopup* Popup = new wxObjectPopup( Obj );

    //Object has audio properties
    if (Obj->IsA(USound::StaticClass()) || Obj->IsA(UMusic::StaticClass()))
    {
        Popup->Append(ID_ObjectPlay, "Play");
    }

    Popup->Append(ID_ObjectOpen, "Open");

    Popup->AppendSeparator();

    //Exporting
    Popup->Append(ID_ObjectExport, "Export Object...");

    Popup->AppendSeparator();

    Popup->Append(ID_ObjectProperties, "Properties");

    PopupMenu(Popup);
}

void EdEditorFrame::ObjectPlay(UObject* Obj)
{
    FVector ZeroVec;
    ZeroVec.X = 0;
    ZeroVec.Y = 0;
    ZeroVec.Z = 0;

    //Sounds
    if (Obj->IsA(USound::StaticClass()))
        GEngine->Audio->PlaySound(NULL, (USound*)Obj, ZeroVec, 1, 1, 1);

    //Music
    if (Obj->IsA(UMusic::StaticClass()))
        GEngine->Audio->PlayMusic((UMusic*)Obj, 0, MTRAN_Instant);
}

void EdEditorFrame::ObjectProperties(UObject* Obj)
{
    if (Obj == NULL)
    {
        return;
    }

    new EdObjectViewer(lastMenuObject);
}

void EdEditorFrame::EVT_ObjectProperties(wxCommandEvent& event)
{
    ObjectProperties(lastMenuObject);
    event.Skip();
}

wxString EdEditorFrame::GetGameDir()
{
    return wxString(GGameConfig->ReadString("Game", "Path", EditorApp::sm_SelectedGame));
}

void EdEditorFrame::OnExit(wxCommandEvent& event)
{
    Close(true);
}

void EdEditorFrame::OnAbout(wxCommandEvent& event)
{
    new EdAbout();
}

void EdEditorFrame::EVT_New(wxCommandEvent& event)
{
    event.Skip();
}

void EdEditorFrame::EVT_Open(wxCommandEvent& event)
{
    event.Skip();
}

void EdEditorFrame::EVT_Save(wxCommandEvent& event)
{
    event.Skip();
}

void EdEditorFrame::EVT_Import(wxCommandEvent& event)
{
    event.Skip();
}

void EdEditorFrame::EVT_Export(wxCommandEvent& event)
{
    event.Skip();
}

void EdEditorFrame::EVT_Preferences(wxCommandEvent& event)
{
    event.Skip();
}

void EdEditorFrame::EVT_BrowserPackage(wxCommandEvent& event)
{
    new EdPackageBrowser();
    event.Skip();
}

void EdEditorFrame::EVT_BrowserClass(wxCommandEvent& event)
{
    new EdClassBrowser();
    event.Skip();
}

void EdEditorFrame::EVT_BrowserSound(wxCommandEvent& event)
{
    new EdSoundBrowser();
    event.Skip();
}

void EdEditorFrame::EVT_BrowserMusic(wxCommandEvent& event)
{
    new EdMusicBrowser();
    event.Skip();
}

void EdEditorFrame::EVT_BrowserTexture(wxCommandEvent& event)
{
    new EdTextureBrowser();
    event.Skip();
}

void EdEditorFrame::EVT_BrowserMesh(wxCommandEvent& event)
{
    new EdMeshBrowser();
    event.Skip();
}

void EdEditorFrame::EVT_ViewLog(wxCommandEvent& event)
{
    event.Skip();
}

void EdEditorFrame::EVT_ActiveTools(wxCommandEvent& event)
{
    event.Skip();
}

void EdEditorFrame::EVT_MapEditor(wxCommandEvent& event)
{
    event.Skip();
}

void EdEditorFrame::EVT_MeshEditor(wxCommandEvent& event)
{
    event.Skip();
}

void EdEditorFrame::EVT_Manual(wxCommandEvent& event)
{
    event.Skip();
}

int EditorApp::GamePromptHandler(TArray<char*>* Names)
{
    int selected = -1;

    EdGamePrompt gamePrompt( Names, selected );
    gamePrompt.ShowModal();

    return selected;
}

bool EditorApp::OnInit()
{
    g_MainFrame = NULL;

    wxInitAllImageHandlers();

    EdEditorFrame::csm_SubDir_U = "/System";
    EdEditorFrame::csm_SubDir_UAX = "/Sounds";
    EdEditorFrame::csm_SubDir_UMX = "/Music";
    EdEditorFrame::csm_SubDir_UTX = "/Textures";
    EdEditorFrame::csm_SubDir_UNR = "/Maps";
    EdEditorFrame::csm_SubDir_USM = "/Meshes";
    EdEditorFrame::csm_SubDir_USA = "/Save";

    EdEditorFrame::sm_bmpNew = wxBitmap(wxT("res/bitmap/New.png"), wxBITMAP_TYPE_PNG);
    EdEditorFrame::sm_bmpDir = wxBitmap(wxT("res/bitmap/Open.png"), wxBITMAP_TYPE_PNG);
    EdEditorFrame::sm_bmpSave = wxBitmap(wxT("res/bitmap/Save.png"), wxBITMAP_TYPE_PNG);
    EdEditorFrame::sm_bmpPackage = wxBitmap(wxT("res/bitmap/PackageBrowser.png"), wxBITMAP_TYPE_PNG);
    EdEditorFrame::sm_bmpClass = wxBitmap(wxT("res/bitmap/ClassBrowser.png"), wxBITMAP_TYPE_PNG);
    EdEditorFrame::sm_bmpSound = wxBitmap(wxT("res/bitmap/SoundBrowser.png"), wxBITMAP_TYPE_PNG);
    EdEditorFrame::sm_bmpMusic = wxBitmap(wxT("res/bitmap/MusicBrowser.png"), wxBITMAP_TYPE_PNG);
    EdEditorFrame::sm_bmpGraphics = wxBitmap(wxT("res/bitmap/GraphicsBrowser.png"), wxBITMAP_TYPE_PNG);
    EdEditorFrame::sm_bmpMesh = wxBitmap(wxT("res/bitmap/MeshBrowser.png"), wxBITMAP_TYPE_PNG);

    EdEditorFrame::sm_icoNew.CopyFromBitmap(EdEditorFrame::sm_bmpNew);
    EdEditorFrame::sm_icoDir.CopyFromBitmap(EdEditorFrame::sm_bmpDir);
    EdEditorFrame::sm_icoSave.CopyFromBitmap(EdEditorFrame::sm_bmpSave);
    EdEditorFrame::sm_icoPackage.CopyFromBitmap(EdEditorFrame::sm_bmpPackage);
    EdEditorFrame::sm_icoClass.CopyFromBitmap(EdEditorFrame::sm_bmpClass);
    EdEditorFrame::sm_icoSound.CopyFromBitmap(EdEditorFrame::sm_bmpSound);
    EdEditorFrame::sm_icoMusic.CopyFromBitmap(EdEditorFrame::sm_bmpMusic);
    EdEditorFrame::sm_icoGraphics.CopyFromBitmap(EdEditorFrame::sm_bmpGraphics);
    EdEditorFrame::sm_icoMesh.CopyFromBitmap(EdEditorFrame::sm_bmpMesh);

    if (LibunrInit(GamePromptHandler, NULL, true, false) == false)
        return false;

    UPackage* EditorPkg = UPackage::StaticLoadPackage("Editor");
    if (!EditorPkg)
    {
        GLogf(LOG_CRIT, "Required package 'Editor' is missing");
        return false;
    }

    UClass* EditorEngineClass = (UClass*)UObject::StaticLoadObject(EditorPkg, "EditorEngine", UClass::StaticClass(), NULL, true);
    if (!EditorEngineClass)
    {
        GLogf(LOG_CRIT, "Failed to load EditorEngine class");
        return false;
    }

    GEngine = (UEngine*)EditorEngineClass->CreateObject();
    if (!GEngine->Init())
    {
        GLogf(LOG_CRIT, "Editor Engine init failed");
        return false;
    }

    for (size_t i = 0; i < UPackage::GetLoadedPackages()->Size(); i++)
        (*UPackage::GetLoadedPackages())[i]->LoadEditableTypes();

    // Start ticking loop
    LastTime = USystem::GetSeconds();
    Connect(wxID_ANY, wxEVT_IDLE, wxIdleEventHandler(EditorApp::DoTick));

    g_MainFrame = new EdEditorFrame("libunr-editor", wxPoint(-1, -1),
        wxSize(wxSystemSettings::GetMetric(wxSYS_SCREEN_X), wxSystemSettings::GetMetric(wxSYS_SCREEN_Y)));

    g_MainFrame->Maximize();
    g_MainFrame->Show(true);

    return true;
}

void EditorApp::DoTick(wxIdleEvent& event)
{
    CurrentTime = USystem::GetSeconds();

    double DeltaTime = CurrentTime - LastTime;
    GEngine->Tick(DeltaTime);

    LastTime = CurrentTime;

    event.RequestMore();
}

wxBEGIN_EVENT_TABLE(EdEditorFrame, wxFrame)
    EVT_MENU(wxID_EXIT, EdEditorFrame::OnExit)
    EVT_MENU(wxID_ABOUT, EdEditorFrame::OnAbout)
    EVT_MENU(ID_New, EdEditorFrame::EVT_New)
    EVT_MENU(ID_Open, EdEditorFrame::EVT_Open)
    EVT_MENU(ID_Save, EdEditorFrame::EVT_Save)
    EVT_MENU(ID_Import, EdEditorFrame::EVT_Import)
    EVT_MENU(ID_Export, EdEditorFrame::EVT_Export)
    EVT_MENU(ID_Preferences, EdEditorFrame::EVT_Preferences)
    EVT_MENU(ID_BrowserPackage, EdEditorFrame::EVT_BrowserPackage)
    EVT_BUTTON(ID_BrowserPackage, EdEditorFrame::EVT_BrowserPackage)
    EVT_MENU(ID_BrowserClass, EdEditorFrame::EVT_BrowserClass)
    EVT_BUTTON(ID_BrowserClass, EdEditorFrame::EVT_BrowserClass)
    EVT_MENU(ID_BrowserSound, EdEditorFrame::EVT_BrowserSound)
    EVT_BUTTON(ID_BrowserSound, EdEditorFrame::EVT_BrowserSound)
    EVT_MENU(ID_BrowserMusic, EdEditorFrame::EVT_BrowserMusic)
    EVT_BUTTON(ID_BrowserMusic, EdEditorFrame::EVT_BrowserMusic)
    EVT_MENU(ID_BrowserGraphics, EdEditorFrame::EVT_BrowserTexture)
    EVT_BUTTON(ID_BrowserGraphics, EdEditorFrame::EVT_BrowserTexture)
    EVT_MENU(ID_BrowserMesh, EdEditorFrame::EVT_BrowserMesh)
    EVT_BUTTON(ID_BrowserMesh, EdEditorFrame::EVT_BrowserMesh)
    EVT_MENU(ID_ViewLog, EdEditorFrame::EVT_ViewLog)
    EVT_MENU(ID_ActiveTools, EdEditorFrame::EVT_ActiveTools)
    EVT_MENU(ID_MapEditor, EdEditorFrame::EVT_MapEditor)
    EVT_MENU(ID_MeshEditor, EdEditorFrame::EVT_MeshEditor)
    EVT_MENU(ID_Manual, EdEditorFrame::EVT_Manual)
wxEND_EVENT_TABLE()

wxIMPLEMENT_APP(EditorApp);
