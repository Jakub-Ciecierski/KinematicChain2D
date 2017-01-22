#ifndef PROJECT_HODOGRAPH_SIMULATION_H
#define PROJECT_HODOGRAPH_SIMULATION_H

#include <vr/simulation.h>

#include <math/math_ifx.h>

#include <random>
#include <memory>

namespace ifx{
class GameObject;
class SceneContainer;
}

class RobotScene;

class RobotSimulation : public ifx::Simulation {
public:

    RobotSimulation(std::shared_ptr<ifx::SceneContainer> scene);
    ~RobotSimulation();

    std::shared_ptr<RobotScene> robot_scene(){return robot_scene_;}

    void Update() override;

private:
    std::shared_ptr<RobotScene> robot_scene_;
    std::shared_ptr<ifx::SceneContainer> scene_;


};

#endif //PROJECT_HODOGRAPH_SIMULATION_H
