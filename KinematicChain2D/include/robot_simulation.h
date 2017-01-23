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
class PathFinder;
class RobotSolution;
struct Node;

enum class SimulationMode{
    EDITOR, PATH_FINDER
};

class RobotSimulation : public ifx::Simulation {
public:
    RobotSimulation(std::shared_ptr<ifx::SceneContainer> scene);
    ~RobotSimulation();

    std::shared_ptr<RobotScene> robot_scene(){return robot_scene_;}
    std::shared_ptr<PathFinder> path_finder(){return path_finder_;}
    SimulationMode mode(){return mode_;}
    void mode(SimulationMode mode){mode_ = mode;}

    void Update() override;

    void StartPathFinder();

    void SetStartSolution(RobotSolution& solution);
    void SetEndSolution(RobotSolution& solution);

    RobotSolution GetStartSolution();
    RobotSolution GetEndSolution();

private:
    void UpdatePath();

    std::shared_ptr<RobotScene> robot_scene_;
    std::shared_ptr<PathFinder> path_finder_;
    std::shared_ptr<ifx::SceneContainer> scene_;

    std::vector<Node> path_;
    int current_path_index_;

    SimulationMode mode_;
};

#endif //PROJECT_HODOGRAPH_SIMULATION_H
