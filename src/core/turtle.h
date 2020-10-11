#pragma once

#include <vector>
#include "../common/vec2.h"

class Turtle {
public:
    struct Segment {
        Vec2<double> p0;
        Vec2<double> p1;
        Segment(Vec2<double> p0_, Vec2<double> p1_) : p0(p0_), p1(p1_) { }
    };

    void forward(double dist);
    void rotate(double angle);
    void push_state();
    void pop_state();
    void set_state(Vec2<double> position, double angle);
    std::vector<Segment>& trace();
private:
    struct State {
        Vec2<double> position;
        double angle;
        State() : position(0, 0), angle(0) { }
    } m_state;

    std::vector<State> m_state_stack;
    std::vector<Segment> m_trace;
};