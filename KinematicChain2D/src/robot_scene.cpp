#include <graphics/factory/program_factory.h>
#include <graphics/factory/model_factory.h>
#include <object/render_object.h>
#include <graphics/factory/texture_factory.h>
#include "robot_scene.h"
#include "robot.h"

#include <game/scene_container.h>
#include <object/game_object.h>
#include <obstacle.h>

#include <configuration_space.h>

RobotScene::RobotScene(std::shared_ptr<ifx::SceneContainer> scene) :
        scene_(scene){
    effector_position_
            = std::shared_ptr<ifx::GameObject>(new ifx::GameObject());
    effector_position_->moveTo(glm::vec3(0,2,0));
    scene_->Add(effector_position_);

    configuration_space_ = std::shared_ptr<ConfigurationSpace>(
            new ConfigurationSpace());

    robot_ = std::shared_ptr<Robot>(new Robot(scene));
}
RobotScene::~RobotScene(){}

void RobotScene::Update(){
    SynchronizeRemovedObstacles();

    for(auto& obstacle : obstacles_)
        obstacle->Update();

    Apply2DConstaints();

    auto& effector_position = effector_position_->getPosition();
    glm::vec2 position(effector_position.x, effector_position.y);

    robot_->SolveInverseWithConstraints(position, obstacles_);
}

void RobotScene::AddObstacle(){
    std::shared_ptr<Program> program = ifx::ProgramFactory().LoadMainProgram();
    std::shared_ptr<ifx::Model> model = ifx::ModelFactory::LoadFloorModel();

    auto material = std::shared_ptr<ifx::Material>(new ifx::Material());
    const glm::vec3 color(255,0,0);
    material->diffuse = ifx::TextureFactory().CreateSolidColorTexture(
            color, ifx::TextureTypes::DIFFUSE);
    material->specular = ifx::TextureFactory().CreateSolidColorTexture(
            color, ifx::TextureTypes::SPECULAR);
    model->getMesh(0)->material(material);

    auto render_object
            = std::shared_ptr<ifx::RenderObject>(
                    new ifx::RenderObject(ObjectID(0), model));
    render_object->addProgram(program);
    auto obstacle = std::shared_ptr<Obstacle>(new Obstacle());
    obstacle->game_object
            = std::shared_ptr<ifx::GameObject>(new ifx::GameObject());
    obstacle->game_object->Add(render_object);

    obstacles_.push_back(obstacle);
    scene_->Add(obstacle->game_object);
}

void RobotScene::SynchronizeRemovedObstacles(){
    std::vector<std::shared_ptr<Obstacle>> keep_obstacles;
    for(auto& obstacle : obstacles_){
        if(scene_->Exists(obstacle->game_object))
            keep_obstacles.push_back(obstacle);
    }
    obstacles_ = keep_obstacles;
}

void RobotScene::Apply2DConstaints(){
    for(auto& obstacle : obstacles_){
        auto& position = obstacle->game_object->getPosition();
        obstacle->game_object->moveTo(glm::vec3(position.x, position.y, 0));
        obstacle->game_object->rotateTo(glm::vec3(0, 0, 0));
    }
}

void RobotScene::ComputeConfigurationSpace(){
    configuration_space_->Compute(obstacles_, robot_);
}