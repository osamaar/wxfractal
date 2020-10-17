#pragma once

#include "../core/model.h"
#include "../common/observer.h"

struct MainFrameController {
    Model *model;
    Observer<int> level_observer;

    MainFrameController(Model *model);
    void set_level(int n);
};

struct AppController {
    Model *model;
    MainFrameController main_frame_controller;

    AppController(Model *model);
};
