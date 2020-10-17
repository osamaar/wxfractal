#pragma once

#include "../common/observer.h"
#include "lsys/lsys.h"

struct Model {
    Observable<int> change_notifier;
    LSys::LSystem lsys;

    void set_level(int n) {
        lsys.apply(n);
        change_notifier.set_data(n);
        change_notifier.notify();
    }
};