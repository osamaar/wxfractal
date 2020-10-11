#pragma once

#include "../common/observer.h"
#include "lsys/lsys.h"

struct Model {
    Subject<int> subject;
    LSys::LSystem lsys;
};