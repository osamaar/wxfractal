#include <cmath>
#include "koch.h"
#include "../common/vec2.h"

using namespace std;

void add_edge(
    vector<Vec2<double>> &v,
    int n,
    Vec2<double> p0,
    Vec2<double> p1
) {
    if(n > 0) {
        Vec2<double> p2, p3, p4;
        //TODO
    } else {
        v.push_back(p0);
        v.push_back(p1);
    }
}

void Koch::update(int n) {
    m_level = n;
    points.clear();

    double z = .25*sqrt(3.);

    Vec2<double> p0 {-z, .25};
    Vec2<double> p1 {z, .25};
    Vec2<double> p2 {0, -.5};

    add_edge(points, n, p0, p1);
    add_edge(points, n, p1, p2);
    add_edge(points, n, p2, p0);
}