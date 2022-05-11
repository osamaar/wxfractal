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
    wxSpinCtrl *m_level_input;
    wxSpinCtrlDouble *m_rot_plus_input;
    wxSpinCtrlDouble *m_rot_minus_input;
    wxSpinCtrlDouble *m_scale_input;
    wxSpinCtrlDouble *m_forward_step_input;
    wxTextCtrl *m_axiom_input;
    wxTextCtrl *m_rules_input;
    wxTextCtrl *m_fsyn_input;
    // Observer<int> m_model_observer;
    Observer<Model::LSysParam> m_model_observer;
    Observable<int> m_apply_notifier;
    // Observable<int> m_level_notifier;
    Observable<Model::LSysParam> m_param_notifier;
    Observable<std::string> m_export_dxf_notifier;

    void init_menus();
    // void update_lsys(int n);
    void update_lsys(Model::LSysParam param);
    void notify_param_observers();

    // void on_hello(wxCommandEvent& event);
    void on_export_dxf(wxCommandEvent& event);
    void on_exit(wxCommandEvent& event);
    void on_about(wxCommandEvent& event);
    void on_apply_button_pressed(wxCommandEvent& event);
    void on_level_input(wxSpinEvent& event);
    void on_rot_plus_input(wxSpinDoubleEvent& event);
    void on_rot_minus_input(wxSpinDoubleEvent& event);
    void on_scale_input(wxSpinDoubleEvent& event);
    void on_forward_step_input(wxSpinDoubleEvent& event);
    void on_axiom_input(wxCommandEvent& event);
    void on_rules_input(wxCommandEvent& event);
    void on_fsyn_input(wxCommandEvent& event);

    wxDECLARE_EVENT_TABLE();
};
