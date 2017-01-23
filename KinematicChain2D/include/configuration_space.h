#ifndef PROJECT_CONFIGURATION_SPACE_H
#define PROJECT_CONFIGURATION_SPACE_H

#include <vector>
#include <memory>
#include <math/math_ifx.h>

class Obstacle;
class Robot;
struct Node;

namespace ifx{
class Texture2D;
}

extern const int CONFIG_AVAILABLE;
extern const int CONFIG_NOT_AVAILABLE;

class ConfigurationSpace {
public:

    ConfigurationSpace();
    ~ConfigurationSpace();

    std::vector<std::vector<int>>& data(){return data_;}
    std::shared_ptr<ifx::Texture2D> texture(){return texture_;}

    void Compute(const std::vector<std::shared_ptr<Obstacle>>& obstacles,
                 std::shared_ptr<Robot> robot);

    void UpdateTexture(std::vector<Node> path);

private:
    void InitData();

    const int width_;
    const int height_;

    std::vector<std::vector<int>> data_;

    std::vector<unsigned char> texture_data_;
    std::shared_ptr<ifx::Texture2D> texture_;
    glm::vec3 available_color_;
    glm::vec3 not_available_color_;
};


#endif //PROJECT_CONFIGURATION_SPACE_H
