#include "main_gui.h"
#include "main_frame.h"
#include "../core/model.h"
#include "controller.h"

bool MainGUI::OnInit() {
    MainFrame *frame = new MainFrame(m_model, m_controller);

    frame->Show(true);
    return true;
}

void MainGUI::set_model(Model *model) {
    m_model = model;
}

void MainGUI::set_controller(Controller *controller) {
    m_controller = controller;
}