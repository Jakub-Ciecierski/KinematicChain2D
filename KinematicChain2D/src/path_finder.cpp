#include "path_finder.h"

#include <flood_fill.h>
#include <configuration_space.h>

PathFinder::PathFinder(
        std::shared_ptr<ConfigurationSpace> configuration_space) :
        configuration_space_(configuration_space){}
PathFinder::~PathFinder(){}

void PathFinder::SetStart(RobotSolution solution){
    start_solution_ = solution;
}
void PathFinder::SetEnd(RobotSolution solution){
    end_solution_ = solution;
}

std::vector<Node> PathFinder::ComputePath(){
    return StartFloodFill();
}

std::vector<Node> PathFinder::StartFloodFill(){
    FloodFill flood_fill;
    Node start{start_solution_.alpha1, start_solution_.alpha2};
    Node end{end_solution_.alpha1, end_solution_.alpha2};

    auto path = flood_fill.Compute(configuration_space_->data(), start, end);

    configuration_space_->UpdateTexture(path);

    return path;
}