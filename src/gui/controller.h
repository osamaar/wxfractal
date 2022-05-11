#pragma once

#include "../common/observer.h"
#include <string>
#include "../core/model.h"

struct MainFrameController {
    Model *model;
    Observer<int> apply_observer;
    Observer<Model::LSysParam> param_observer;
    Observer<std::string> export_dxf_observer;

    MainFrameController(Model *model);
    void apply(int n);
    void set_parameters(Model::LSysParam param);
    void export_dxf(std::string filename);
};

struct AppController {
    Model *model;
    MainFrameController main_frame_controller;

    AppController(Model *model);
};
