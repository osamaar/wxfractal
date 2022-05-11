#include "controller.h"
#include "main_frame.h"
#include "../core/model.h"

MainFrameController::MainFrameController(Model *model)
    : model(model)
    , apply_observer(&MainFrameController::apply, this)
    , param_observer(&MainFrameController::set_parameters, this)
    , export_dxf_observer(&MainFrameController::export_dxf, this)
{

}

void MainFrameController::apply(int n) {
    model->apply();
}

void MainFrameController::set_parameters(Model::LSysParam param) {
    model->set_param(param);
}

void MainFrameController::export_dxf(std::string filename) {
    wxLogMessage("EXPORT");
}

AppController::AppController(Model *model)
    : model(model)
    , main_frame_controller(model)
{

}