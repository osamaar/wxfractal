#include "controller.h"
#include "main_frame.h"

MainFrameController::MainFrameController(Model *model)
    : model(model)
    , level_observer(&MainFrameController::set_level, this)
{

}

void MainFrameController::set_level(int n) {
    model->set_level(n);
}

AppController::AppController(Model *model)
    : model(model)
    , main_frame_controller(model)
{

}