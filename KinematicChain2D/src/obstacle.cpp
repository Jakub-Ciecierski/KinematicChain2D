#include "obstacle.h"

#include <object/game_object.h>

void Obstacle::Update(){
    const float a_half = 1.0f;
    auto& middle = game_object->getPosition();
    auto& scale = game_object->getScale();

    float scaled_x_length_half = a_half * scale.x;
    float scaled_y_length_half = a_half * scale.y;

    min_x = middle.x - (scaled_x_length_half);
    max_x = middle.x + (scaled_x_length_half);

    min_y = middle.y - (scaled_y_length_half);
    max_y = middle.y + (scaled_y_length_half);
}

bool Obstacle::Collides(const glm::vec2& p0, const glm::vec2& p1) const{
    float x1 = p0.x;
    float y1 = p0.y;
    float x2 = p1.x;
    float y2 = p1.y;
    // Completely outside.
    if ((x1 <= min_x && x2 <= min_x) || (y1 <= min_y && y2 <= min_y)
        || (x1 >= max_x && x2 >= max_x) || (y1 >= max_y && y2 >= max_y))
        return false;

    float m = (y2 - y1) / (x2 - x1);

    float y = m * (min_x - x1) + y1;
    if (y > min_y && y < max_y) return true;

    y = m * (max_x - x1) + y1;
    if (y > min_y && y < max_y) return true;

    float x = (min_y - y1) / m + x1;
    if (x > min_x && x < max_x) return true;

    x = (max_y - y1) / m + x1;
    if (x > min_x && x < max_x) return true;

    return false;
}