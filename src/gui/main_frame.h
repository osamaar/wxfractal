#include <wx/wx.h>
#include <wx/spinctrl.h>
#include "../core/model.h"
#include "controller.h"

class DrawPanel;

class MainFrame: public wxFrame {
public:
    MainFrame(Model *model, Controller *controller);
private:
    Model *m_model;
    Controller *m_controller;
    DrawPanel *m_draw_area;
    wxSpinCtrl *m_spinner;

    void init_menus();
    void on_hello(wxCommandEvent& event);
    void on_exit(wxCommandEvent& event);
    void on_about(wxCommandEvent& event);
    void on_level_changed(wxSpinEvent& event);

    wxDECLARE_EVENT_TABLE();
};
