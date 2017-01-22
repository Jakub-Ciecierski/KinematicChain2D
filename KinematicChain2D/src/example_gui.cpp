#include <graphics/model/model.h>
#include <graphics/factory/render_object_factory.h>

#include "example_gui.h"

#include <engine_gui/factory/engine_gui_factory.h>
#include <engine_gui/engine_gui.h>
#include <gui/imgui/imgui.h>
#include <physics/bullet_extensions/btFractureDynamicsWorld.h>
#include <physics/simulations/bullet_physics_simulation.h>
#include <robot_simulation.h>
#include <robot_scene.h>
#include <robot.h>
#include <configuration_space.h>




ExampleGUI::ExampleGUI(GLFWwindow* window,
                       std::shared_ptr<ifx::SceneContainer> scene,
                       std::shared_ptr<RobotSimulation> robot_simulation,
                       std::shared_ptr<ifx::PhysicsSimulation> physics_simulation) :
        ifx::GUI(window),
        robot_simulation_(robot_simulation){
    engine_gui_ = ifx::EngineGUIFactory().CreateEngineGUI(scene,
                                                          physics_simulation);
}
ExampleGUI::~ExampleGUI(){}

void ExampleGUI::Render(){
    NewFrame();

    engine_gui_->Render();

    ImGui::Begin("Robot");
    RenderEditorMode();
    ImGui::End();

    ImGui::Render();
}

void ExampleGUI::RenderEditorMode(){

    if(ImGui::TreeNode("Editor")){
        RenderObstacle();
        ImGui::TreePop();
    }
    if(ImGui::TreeNode("Robot")){
        RenderRobotProperties();
        ImGui::TreePop();
    }
    if(ImGui::TreeNode("Configuration Space")){
        RenderConfigurationSpace();
        ImGui::TreePop();
    }

}
void ExampleGUI::RenderObstacle(){
    if(ImGui::Button("Add Obstacle")){
        robot_simulation_->robot_scene()->AddObstacle();
    }
}

void ExampleGUI::RenderRobotProperties() {
    ImGui::SliderFloat("l1",
                       robot_simulation_->robot_scene()->robot()->l1(),
                       0.1, 10);
    ImGui::SliderFloat("l2",
                       robot_simulation_->robot_scene()->robot()->l2(),
                       0.1, 10);

    ImGui::SliderFloat("alpha1_1",
                       &robot_simulation_->robot_scene()->
                               robot()->solution1().alpha1,
                       0, 360);
    ImGui::SliderFloat("alpha1_2",
                       &robot_simulation_->robot_scene()->
                               robot()->solution1().alpha2,
                       0, 360);
    ImGui::SliderFloat("alpha2_1",
                       &robot_simulation_->robot_scene()->
                               robot()->solution2().alpha1,
                       0, 360);
    ImGui::SliderFloat("alpha2_2",
                       &robot_simulation_->robot_scene()->
                               robot()->solution2().alpha2,
                       0, 360);
}

void ExampleGUI::RenderConfigurationSpace(){
    if(ImGui::Button("Compute"))
        robot_simulation_->robot_scene()->ComputeConfigurationSpace();

    RenderConfigurationSpacePlot();
}

void ExampleGUI::RenderConfigurationSpacePlot(){
    ImTextureID tex_id = (void*)robot_simulation_->robot_scene()
            ->configuration_space()->texture()->id();

    ImGui::Image(tex_id,ImVec2(360, 360));
}