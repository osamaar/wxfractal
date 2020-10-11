#include <cmath>
#include "turtle.h"

#define PI 3.14159265

double to_radians(double degrees) {
    return degrees*PI/180.;
}

void Turtle::forward(double dist) {
    using namespace std;

    auto new_pos = m_state.position;
    double radians = to_radians(m_state.angle);
    new_pos.x += dist*cos(radians);
    new_pos.y += dist*sin(radians);
    m_trace.emplace_back(m_state.position, new_pos);
    m_state.position = new_pos;
}

void Turtle::rotate(double angle) {
    m_state.angle += angle;
}

void Turtle::push_state() {
    m_state_stack.push_back(m_state);
}

void Turtle::pop_state() {
    m_state_stack.pop_back();
}

void Turtle::set_state(Vec2<double> position, double angle) {
    m_state.position = position;
    m_state.angle = angle;
}

std::vector<Turtle::Segment>& Turtle::trace() {
    return m_trace;
}