#ifndef PROJECT_ROBOT_SCENE_H
#define PROJECT_ROBOT_SCENE_H

#include <vector>
#include <memory>

namespace ifx{
class GameObject;
class SceneContainer;
}

struct Obstacle;
class Robot;
class ConfigurationSpace;

class RobotScene {
public:

    RobotScene(std::shared_ptr<ifx::SceneContainer> scene_);
    ~RobotScene();

    std::shared_ptr<ConfigurationSpace> configuration_space(){
        return configuration_space_;
    }
    std::shared_ptr<Robot> robot(){return robot_;}

    void Update();

    void AddObstacle();

    void ComputeConfigurationSpace();
private:
    void SynchronizeRemovedObstacles();
    void Apply2DConstaints();

    std::shared_ptr<ConfigurationSpace> configuration_space_;
    std::shared_ptr<Robot> robot_;
    std::vector<std::shared_ptr<Obstacle>> obstacles_;

    std::shared_ptr<ifx::GameObject> effector_position_;

    std::shared_ptr<ifx::SceneContainer> scene_;
};


#endif //PROJECT_ROBOT_SCENE_H
