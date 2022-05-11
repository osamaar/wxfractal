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
    Observer<Model::LSysParam> observer;

    TerminalView() : observer(&TerminalView::level_changed, this) { }

    void level_changed(Model::LSysParam param) {
        printf("==================================\n");
        printf("Iterations: %d\n", param.data.level);
        printf("Token count: %zu\n", model->lsys.sequence.size());
        printf("Sequence: %s\n\n", model->lsys.sequence_as_string().c_str());
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

    model.init_lsys();
    model.lsys.set_axiom("F - - F - - F");
    model.lsys.add_rule("F", "F + F - - F + F");

    Model::LSysParam param;
    param.data.level = 0;
    param.data.axiom = "F - - F - - F";
    param.data.rules = "F: F + F - - F + F";
    param.vis.rot_plus = 60.;
    param.vis.rot_minus = -60.;
    param.vis.forward_step = 300.;
    param.vis.scale = 1.;

    model.set_param(param);

    // model.lsys.apply(2);

    return wxEntry(argc, argv);
}