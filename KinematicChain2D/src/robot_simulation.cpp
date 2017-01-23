#include "robot_simulation.h"

#include <object/game_object.h>
#include <game/scene_container.h>
#include <graphics/factory/render_object_factory.h>
#include <robot_scene.h>
#include <path_finder.h>
#include <flood_fill.h>
#include <iostream>

RobotSimulation::RobotSimulation(std::shared_ptr<ifx::SceneContainer> scene) :
    scene_(scene),
    mode_(SimulationMode::EDITOR){
    robot_scene_ = std::shared_ptr<RobotScene>(new RobotScene(scene_));
    path_finder_ = std::shared_ptr<PathFinder>(new PathFinder(
            robot_scene_->configuration_space()));
}

RobotSimulation::~RobotSimulation(){}

void RobotSimulation::Update(){
    if(!UpdateTime())
        return;
    if(mode_ == SimulationMode::EDITOR)
        robot_scene_->Update();
    else if(mode_ == SimulationMode::PATH_FINDER){
        UpdatePath();
    }
}

void RobotSimulation::StartPathFinder(){
    robot_scene()->ComputeConfigurationSpace();

    mode_ = SimulationMode::PATH_FINDER;
    path_ = path_finder_->ComputePath();
    current_path_index_ = 0;
}

void RobotSimulation::SetStartSolution(RobotSolution& solution){
    path_finder_->SetStart(solution);
}
void RobotSimulation::SetEndSolution(RobotSolution& solution){
    path_finder_->SetEnd(solution);
}

RobotSolution RobotSimulation::GetStartSolution(){
    return path_finder_->start_solution();
}
RobotSolution RobotSimulation::GetEndSolution(){
    return path_finder_->end_solution();
}

void RobotSimulation::UpdatePath(){
    if(current_path_index_ >= path_.size()){
        mode_ = SimulationMode::EDITOR;
        return;
    }
    Node node = path_[current_path_index_];
    std::cout << node.i << ", " << node.j << std::endl;
    robot_scene()->robot()->SolveDirectAndUpdate(node.i, node.j);
    current_path_index_++;

    // Update Effector
    auto& solution1 = robot_scene()->robot()->solution1();
    auto& pos = solution1.p2;
    robot_scene()->effector_position()->moveTo(glm::vec3(pos.x, pos.y, 0));
}