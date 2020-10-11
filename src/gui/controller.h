#pragma once

#include "../core/model.h"
#include "../common/observer.h"

struct Controller {
    Model *model;
    Observer<int> level_observer;

    Controller(Model *model);
    void change_level(int n);
};
