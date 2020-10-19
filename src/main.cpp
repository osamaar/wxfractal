#include <wx/wx.h>
#include "core/lsys/lsys.h"
#include "gui/main_gui.h"
#include "core/model.h"
#include "gui/controller.h"
#include <cstdio>

// wxIMPLEMENT_APP(MainGUI);
wxIMPLEMENT_APP_NO_MAIN(MainGUI);

struct TerminalView {
    Model *model;
    Observer<int> observer;

    TerminalView() : observer(&TerminalView::level_changed, this) { }

    void level_changed(int n) {
        printf("==================================\n");
        printf("Iterations: %d\n", n);
        printf("Token count: %zu\n", model->lsys.sequence.size());
        printf("Sequence: %s\n\n", model->lsys.as_string().c_str());
    }

    void set_model(Model *m) {
        model = m;
        model->change_notifier.add_observer(observer);
    }
};

int main(int argc, char *argv[]) {
    // LSys::LSystem model;
    Model model;
    MainGUI *gui = new MainGUI();
    AppController controller(&model);

    // TerminalView tview;
    // tview.set_model(&model);

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