#include <wx/wx.h>
#include "core/lsys/lsys.h"
#include "gui/main_gui.h"
#include "core/model.h"
#include "gui/controller.h"

// wxIMPLEMENT_APP(MainGUI);
wxIMPLEMENT_APP_NO_MAIN(MainGUI);

int main(int argc, char *argv[]) {
    // LSys::LSystem model;
    Model model;
    Controller controller(&model);
    MainGUI *gui = new MainGUI();
    gui->set_model(&model);
    gui->set_controller(&controller);

    model.lsys.add_element(LSys::Element::with_data("F", 300, LSys::Constant));
    model.lsys.add_element(LSys::Element::with_data("+", 60, LSys::Constant));
    model.lsys.add_element(LSys::Element::with_data("-", -60, LSys::Constant));
    model.lsys.set_axiom("F - - F - - F");
    model.lsys.add_rule("F", "F + F - - F + F");
    // model.lsys.apply(2);

    return wxEntry(argc, argv);
}