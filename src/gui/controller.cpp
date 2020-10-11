#include "controller.h"

Controller::Controller(Model *model)
    : model(model)
    , level_observer(&Controller::change_level, this)
{

}

void Controller::change_level(int n) {
    model->lsys.apply(n);
    model->subject.notify();
}