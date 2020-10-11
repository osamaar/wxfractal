#include <vector>
#include "../common/vec2.h"

class Koch {
public:
    std::vector<Vec2<double>> points;

    Koch() : Koch(0) { }
    Koch(int n) { update(n); }
    void update(int n);
    int level() { return m_level; }
private:
    int m_level;
};