#include "main_gui.h"
#include "main_frame.h"
#include "../core/model.h"
#include "controller.h"

bool MainGUI::OnInit() {
    auto mfctrl = &m_controller->main_frame_controller;

    MainFrame *frame = new MainFrame(m_model, mfctrl); 
    frame->Show(true);

    MainFrame *frame01 = new MainFrame(m_model, mfctrl);
    frame01->Show(true);

    return true;
}

void MainGUI::set_model(Model *model) {
    m_model = model;
}

void MainGUI::set_controller(AppController *controller) {
    m_controller = controller;
}