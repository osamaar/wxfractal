#include <wx/wx.h>
#include "../core/model.h"
#include "controller.h"

class MainGUI: public wxApp {
public:
    void set_model(Model *model);
    void set_controller(Controller *controller);
    virtual bool OnInit();
private:
    Model *m_model;
    Controller *m_controller;
};
