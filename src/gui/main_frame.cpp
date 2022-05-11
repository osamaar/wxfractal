#include <wx/spinctrl.h>
#include "main_frame.h"
#include "draw_panel.h"

enum {
    // ID_Hello = 1001,
    ID_EXPORT_DXF = 1001,
    ID_BTN_APPLY,
    ID_SPIN_DUMMY,
    ID_OUTLINE
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
    // EVT_MENU(ID_Hello,   MainFrame::on_hello)
    EVT_MENU(ID_EXPORT_DXF,   MainFrame::on_export_dxf)
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
    , m_model_observer(&MainFrame::update_lsys, this)
    , m_apply_notifier()
    , m_param_notifier()
    , m_export_dxf_notifier()
{
    init_menus();

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

    auto apply_button = new wxButton(outline, wxID_ANY, "Apply");
    outline_expander->Add(apply_button, 0, wxALL | wxEXPAND, 15);
    apply_button->Bind(wxEVT_BUTTON, &MainFrame::on_apply_button_pressed, this);
    // apply_button->SetDefault();

    wxFlexGridSizer *outline_sizer = new wxFlexGridSizer(2, wxSize(10, 10));
    outline_expander->Add(outline_sizer, 1, wxALL | wxEXPAND, 15);
    // outline_sizer->Add(20, 20);
    // outline_sizer->Add(20, 20);

    m_level_input = new wxSpinCtrl(outline);
    // outline_sizer->Add(m_spinner, 0, wxALL, 20);
    add_input_field(m_level_input, "Iterations", outline, outline_sizer);
    m_level_input->Bind(wxEVT_SPINCTRL, &MainFrame::on_level_input, this);

    m_rot_plus_input = new_spin_double_default(outline, 0., 9999., 90., .1);
    add_input_field(m_rot_plus_input, "Rotation (+)", outline, outline_sizer);
    m_rot_plus_input->Bind(wxEVT_SPINCTRLDOUBLE, &MainFrame::on_rot_plus_input, this);

    m_rot_minus_input = new_spin_double_default(outline, -9999., 0., -90., .1);
    add_input_field(m_rot_minus_input, "Rotation (-)", outline, outline_sizer);
    m_rot_minus_input->Bind(wxEVT_SPINCTRLDOUBLE, &MainFrame::on_rot_minus_input, this);

    m_scale_input = new_spin_double_default(outline, .01, 9999., 1., .01);
    add_input_field(m_scale_input, "Scale Factor", outline, outline_sizer);
    m_scale_input->Bind(wxEVT_SPINCTRLDOUBLE, &MainFrame::on_scale_input, this);

    m_forward_step_input = new_spin_double_default(outline, .1, 9999., 1., .1);
    add_input_field(m_forward_step_input, "Forward Step", outline, outline_sizer);
    m_forward_step_input->Bind(wxEVT_SPINCTRLDOUBLE, &MainFrame::on_forward_step_input, this);

    m_axiom_input = new wxTextCtrl(outline, -1, "FF++FF-", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
    add_input_field(m_axiom_input, "Axiom", outline, outline_sizer);
    m_axiom_input->Bind(wxEVT_TEXT, &MainFrame::on_axiom_input, this);

    m_rules_input = new wxTextCtrl(outline, -1, "F : FF+++FF++FF", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
    add_input_field(m_rules_input, "Rules", outline, outline_sizer);
    m_rules_input->Bind(wxEVT_TEXT, &MainFrame::on_rules_input, this);

    m_fsyn_input = new wxTextCtrl(outline, -1, "", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
    add_input_field(m_fsyn_input, "Forward Synonyms", outline, outline_sizer);
    m_rules_input->Bind(wxEVT_TEXT, &MainFrame::on_fsyn_input, this);

    outline_sizer->AddGrowableCol(1, 1);

    Layout();

    CreateStatusBar();
    // SetStatusText( "Welcome to wxWidgets!" );

    m_draw_area = lpanel;

    m_model->change_notifier.add_observer(m_model_observer);

    // m_level_notifier.add_observer(controller->level_observer);
    // m_level_notifier.notify(m_level_input->GetValue());

    m_param_notifier.add_observer(controller->param_observer);
    // notify_param_observers();

    m_apply_notifier.add_observer(controller->apply_observer);
    m_apply_notifier.notify(0);

    m_export_dxf_notifier.add_observer(controller->export_dxf_observer);
}

MainFrame::~MainFrame() {
    m_model->change_notifier.remove_observer(m_model_observer);
    // m_level_notifier.remove_observer(m_controller->level_observer);
    m_param_notifier.remove_observer(m_controller->param_observer);
}

void MainFrame::init_menus() {
    wxMenu *menu_file = new wxMenu;
    // menu_file->Append(ID_Hello, "&Hello...\tCtrl-H",
    //                  "Help string shown in status bar for this menu item");
    menu_file->Append(ID_EXPORT_DXF, "&Export DWF...\tCtrl-E",
                     "Export resulting drawing in DWF vector format.");
    menu_file->AppendSeparator();
    menu_file->Append(wxID_EXIT);

    wxMenu *menu_help = new wxMenu;
    menu_help->Append(wxID_ABOUT);

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menu_file, "&File");
    menuBar->Append(menu_help, "&Help");

    SetMenuBar( menuBar );
}

// void MainFrame::update_lsys(int n) {
void MainFrame::update_lsys(Model::LSysParam param) {
    m_level_input->SetValue(param.data.level);
    m_rot_plus_input->SetValue(param.vis.rot_plus);
    m_rot_minus_input->SetValue(param.vis.rot_minus);
    m_scale_input->SetValue(param.vis.scale);
    m_forward_step_input->SetValue(param.vis.forward_step);
    m_axiom_input->ChangeValue(param.data.axiom);
    m_rules_input->ChangeValue(param.data.rules);
}

void MainFrame::notify_param_observers() {
    Model::LSysParam param;

    param.data.level = m_level_input->GetValue();
    param.data.axiom = m_axiom_input->GetValue();
    param.data.rules = m_rules_input->GetValue();
    param.vis.rot_plus = m_rot_plus_input->GetValue();
    param.vis.rot_minus = m_rot_minus_input->GetValue();
    param.vis.scale = m_scale_input->GetValue();
    param.vis.forward_step = m_forward_step_input->GetValue();
    param.vis.forward_synonyms = m_fsyn_input->GetValue();

    m_param_notifier.notify(param);
}

void MainFrame::on_exit(wxCommandEvent& event) {
    Close(true);
}

void MainFrame::on_about(wxCommandEvent& event) {
    wxMessageBox( "This is a wxWidgets' Hello world sample",
                  "About Hello World", wxOK | wxICON_INFORMATION );
}

// void MainFrame::on_hello(wxCommandEvent& event) {
//     wxLogMessage("Hello world from wxWidgets!zzz");
// }

void MainFrame::on_export_dxf(wxCommandEvent& event) {
    m_export_dxf_notifier.notify("DUMMY DXF FILE NAME");
}

void MainFrame::on_apply_button_pressed(wxCommandEvent& event) {
    notify_param_observers();
    m_apply_notifier.notify(0);
}

void MainFrame::on_level_input(wxSpinEvent& event) { 
    // m_level_notifier.notify(m_level_input->GetValue());
    // notify_param_observers();
}

void MainFrame::on_rot_plus_input(wxSpinDoubleEvent& event) {
    notify_param_observers();
}

void MainFrame::on_rot_minus_input(wxSpinDoubleEvent& event) {
    // notify_param_observers();
}

void MainFrame::on_scale_input(wxSpinDoubleEvent& event) {
    // notify_param_observers();
}

void MainFrame::on_forward_step_input(wxSpinDoubleEvent& event) {
    // notify_param_observers();
}

void MainFrame::on_axiom_input(wxCommandEvent& event) {
    // notify_param_observers();
}

void MainFrame::on_rules_input(wxCommandEvent& event) {
    // notify_param_observers();
}

void MainFrame::on_fsyn_input(wxCommandEvent& event) {
    // notify_param_observers();
}
