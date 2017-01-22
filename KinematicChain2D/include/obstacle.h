#ifndef PROJECT_OBSTACLE_H
#define PROJECT_OBSTACLE_H

#include <math/math_ifx.h>
#include <memory>

namespace ifx{
class GameObject;
}

struct Obstacle{
    // Assume it is a box 2x2.
    // Middle by GameObject.position
    // Scale by GameObject.scale.dim
    void Update();
    bool Collides(const glm::vec2& p0, const glm::vec2& p1) const;

    std::shared_ptr<ifx::GameObject> game_object;

    float min_x;
    float min_y;
    float max_x;
    float max_y;
};


#endif //PROJECT_OBSTACLE_H
