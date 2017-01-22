#include "robot_simulation.h"

#include <object/game_object.h>
#include <game/scene_container.h>
#include <graphics/factory/render_object_factory.h>
#include <robot_scene.h>

RobotSimulation::RobotSimulation(std::shared_ptr<ifx::SceneContainer> scene) :
    scene_(scene){
    robot_scene_ = std::shared_ptr<RobotScene>(new RobotScene(scene_));
}

RobotSimulation::~RobotSimulation(){}

void RobotSimulation::Update(){
    if(!UpdateTime())
        return;
    robot_scene_->Update();
}