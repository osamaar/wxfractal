#include <wx/spinctrl.h>
#include "main_frame.h"
#include "draw_panel.h"

enum {
    ID_Hello = 1
};

//////////////////////
// Helper functions
void add_input_field(
    wxWindow *input_widget,
    std::string title,
    wxWindow *parent,
    wxSizer *sizer
) {
    wxStaticText *title_widget = new wxStaticText(parent, -1, title);
    sizer->Add(title_widget, 0, wxALIGN_RIGHT);
    sizer->Add(input_widget, 1, wxEXPAND);
}

// void add_input_int(
//     wxWindow *input_widget,
//     std::string title,
//     wxWindow *parent,
//     wxSizer *sizer
// ) {

// }

wxSpinCtrlDouble *new_spin_double_default(wxWindow *parent, double min, double max, double init, double inc) {
    return new wxSpinCtrlDouble(
        parent, -1, wxEmptyString, wxDefaultPosition,
        wxDefaultSize, wxSP_ARROW_KEYS, 
        min, max, init, inc);
}

//////////////////////

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_MENU(ID_Hello,   MainFrame::on_hello)
    EVT_MENU(wxID_EXIT,  MainFrame::on_exit)
    EVT_MENU(wxID_ABOUT, MainFrame::on_about)
wxEND_EVENT_TABLE()

MainFrame::MainFrame(Model *model, MainFrameController *controller)
    : wxFrame(
        NULL,
        wxID_ANY,
        "Hello World",
        wxPoint(-1, -1),
        wxSize(960, 520)
    )
    , m_model(model)
    , m_controller(controller)
    , m_lsys_observer(&MainFrame::update_lsys, this)
    , m_level_notifier()
{
    // create widget
    // add to parent sizer
    // create inner sizer
    // set inner sizer
    // configure widget

    wxBoxSizer *hsizer = new wxBoxSizer(wxHORIZONTAL);
    SetSizer(hsizer);

    DrawPanel *lpanel = new DrawPanel(this, model);
    hsizer->Add(lpanel, 3, wxEXPAND, 5);
    lpanel->SetBackgroundColour(wxColour(32, 32, 32, 255));
    // lpanel->SetMinSize(wxSize(200, -1));

    auto *rpanel = new wxScrolledWindow(this);
    hsizer->Add(rpanel, 1, wxEXPAND, 5);
    wxBoxSizer *rpanel_sizer = new wxBoxSizer(wxVERTICAL);
    rpanel->SetScrollRate(0, 5);
    rpanel->SetSizer(rpanel_sizer);
    rpanel->SetBackgroundColour(wxColour(200, 200, 200, 255));
    // rpanel->SetMinSize(wxSize(250, -1));

    wxStaticBox *outline = new wxStaticBox(rpanel, wxID_ANY, "Parameters");
    rpanel_sizer->Add(outline, 1, wxEXPAND | wxALL, 10);
    wxBoxSizer *outline_expander = new wxBoxSizer(wxVERTICAL);
    outline->SetSizer(outline_expander);
    outline_expander->Add(0, 15);       // top spacer

    wxFlexGridSizer *outline_sizer = new wxFlexGridSizer(2, wxSize(10, 10));
    outline_expander->Add(outline_sizer, 1, wxALL | wxEXPAND, 15);
    // outline_sizer->Add(20, 20);
    // outline_sizer->Add(20, 20);

    m_spinner = new wxSpinCtrl(outline);
    // outline_sizer->Add(m_spinner, 0, wxALL, 20);
    add_input_field(m_spinner, "Iterations", outline, outline_sizer);
    m_spinner->Bind(wxEVT_SPINCTRL, &MainFrame::on_level_changed, this);

    auto angle_plus_widget = new_spin_double_default(outline, 0., 9999., 90., .1);
    add_input_field(angle_plus_widget, "Rotation (+)", outline, outline_sizer);

    auto angle_minus_widget = new_spin_double_default(outline, -9999., 0., -90., .1);
    add_input_field(angle_minus_widget, "Rotation (-)", outline, outline_sizer);

    auto scale_widget = new_spin_double_default(outline, .01, 9999., 1., .01);
    add_input_field(scale_widget, "Scale Factor", outline, outline_sizer);

    auto dist_widget = new_spin_double_default(outline, .1, 9999., 1., .1);
    add_input_field(dist_widget, "Forward Step", outline, outline_sizer);

    auto axiom_widget = new wxTextCtrl(outline, -1, "FF++FF-", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
    add_input_field(axiom_widget, "Axiom", outline, outline_sizer);

    auto rules_widget = new wxTextCtrl(outline, -1, "F : FF+++FF++FF", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
    add_input_field(rules_widget, "Rules", outline, outline_sizer);

    outline_sizer->AddGrowableCol(1, 1);

    Layout();

    init_menus();
    CreateStatusBar();
    SetStatusText( "Welcome to wxWidgets!" );

    m_draw_area = lpanel;

    m_model->change_notifier.add_observer(m_lsys_observer);

    m_level_notifier.add_observer(controller->level_observer);
    m_level_notifier.notify(m_spinner->GetValue());
}

MainFrame::~MainFrame() {
    m_model->change_notifier.remove_observer(m_lsys_observer);
    m_level_notifier.remove_observer(m_controller->level_observer);
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

void MainFrame::update_lsys(int n) {
    m_spinner->SetValue(n);
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
    m_level_notifier.notify(m_spinner->GetValue());
}