#ifndef PROJECT_PATH_FINDER_H
#define PROJECT_PATH_FINDER_H

#include <robot.h>

#include <memory>

class ConfigurationSpace;
struct Node;

class PathFinder {
public:

    PathFinder(std::shared_ptr<ConfigurationSpace> configuration_space);
    ~PathFinder();

    RobotSolution start_solution(){return start_solution_;}
    RobotSolution end_solution(){return end_solution_;}

    void SetStart(RobotSolution solution);
    void SetEnd(RobotSolution solution);

    std::vector<Node> ComputePath();

private:
    std::vector<Node> StartFloodFill();

    std::shared_ptr<ConfigurationSpace> configuration_space_;

    RobotSolution start_solution_;
    RobotSolution end_solution_;
};


#endif //PROJECT_PATH_FINDER_H
