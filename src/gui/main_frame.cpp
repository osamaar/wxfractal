#include <wx/spinctrl.h>
#include "main_frame.h"
#include "draw_panel.h"

enum {
    ID_Hello = 1
};

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_MENU(ID_Hello,   MainFrame::on_hello)
    EVT_MENU(wxID_EXIT,  MainFrame::on_exit)
    EVT_MENU(wxID_ABOUT, MainFrame::on_about)
wxEND_EVENT_TABLE()

MainFrame::MainFrame(Model *model, Controller *controller)
    : wxFrame(
        NULL,
        wxID_ANY,
        "Hello World",
        wxPoint(-1, -1),
        wxSize(960, 520)
    )
    , m_model(model)
    , m_controller(controller)
{
    wxBoxSizer *hsizer = new wxBoxSizer(wxHORIZONTAL);
    // wxBoxSizer *vsizer = new wxBoxSizer(wxVERTICAL);
    // hsizer->Add(vsizer, wxEXPAND);

    DrawPanel *lpanel = new DrawPanel(this, model);
    lpanel->SetBackgroundColour(wxColour(32, 32, 32, 255));
    // lpanel->SetMinSize(wxSize(200, -1));
    hsizer->Add(lpanel, 3, wxEXPAND, 5);

    wxPanel *rpanel = new wxPanel(this);
    rpanel->SetBackgroundColour(wxColour(200, 200, 200, 255));
    rpanel->SetMinSize(wxSize(200, -1));
    m_spinner = new wxSpinCtrl(rpanel);
    m_spinner->Bind(wxEVT_SPINCTRL, &MainFrame::on_level_changed, this);
    hsizer->Add(rpanel, 1, wxEXPAND, 5);

    SetSizer(hsizer);

    init_menus();
    CreateStatusBar();
    SetStatusText( "Welcome to wxWidgets!" );

    m_draw_area = lpanel;

    m_controller->change_level(m_spinner->GetValue());
}

void MainFrame::init_menus() {
    wxMenu *menuFile = new wxMenu;
    menuFile->Append(ID_Hello, "&Hello...\tCtrl-H",
                     "Help string shown in status bar for this menu item");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);

    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append( menuFile, "&File" );
    menuBar->Append( menuHelp, "&Help" );

    SetMenuBar( menuBar );
}

void MainFrame::on_exit(wxCommandEvent& event) {
    Close(true);
}

void MainFrame::on_about(wxCommandEvent& event) {
    wxMessageBox( "This is a wxWidgets' Hello world sample",
                  "About Hello World", wxOK | wxICON_INFORMATION );
}

void MainFrame::on_hello(wxCommandEvent& event) {
    wxLogMessage("Hello world from wxWidgets!");
}

void MainFrame::on_level_changed(wxSpinEvent& event) { 
    m_controller->change_level(m_spinner->GetValue());
}