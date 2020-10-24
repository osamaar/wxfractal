#include <wx/wx.h>
#include <wx/spinctrl.h>
#include "../core/model.h"
#include "controller.h"
#include <string>

class DrawPanel;

class MainFrame: public wxFrame {
public:
    MainFrame(Model *model, MainFrameController *controller);
    ~MainFrame();
private:
    Model *m_model;
    MainFrameController *m_controller;
    DrawPanel *m_draw_area;
    wxSpinCtrl *m_spinner;
    Observer<int> m_lsys_observer;
    Observable<int> m_level_notifier;

    void init_menus();
    void update_lsys(int n);

    void on_hello(wxCommandEvent& event);
    void on_exit(wxCommandEvent& event);
    void on_about(wxCommandEvent& event);
    void on_level_changed(wxSpinEvent& event);

    wxDECLARE_EVENT_TABLE();
};
