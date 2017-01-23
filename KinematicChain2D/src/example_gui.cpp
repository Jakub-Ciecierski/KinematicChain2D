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
    RenderWindow();
    ImGui::End();

    ImGui::Render();
}

void ExampleGUI::RenderWindow(){
    if(ImGui::TreeNode("Editor")) {
        RenderEditorMode();
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Path Finder")) {
        RenderPathFinder();
        ImGui::TreePop();
    }

    //RenderMode();

    if(ImGui::TreeNode("Configuration Space")){
        RenderConfigurationSpace();
        ImGui::TreePop();
    }
}

void ExampleGUI::RenderMode(){
    static int e = 0;
    const int EDITOR = 0;
    const int PATH_FINDER = 1;

    if(ImGui::TreeNode("Simulation Mode")){
        ImGui::RadioButton("Editor", &e, EDITOR); ImGui::SameLine();
        ImGui::RadioButton("Path Finder", &e, PATH_FINDER);
        ImGui::TreePop();
    }

    if(e == EDITOR){
        robot_simulation_->mode(SimulationMode::EDITOR);
    }else if(e == PATH_FINDER){
        robot_simulation_->mode(SimulationMode::PATH_FINDER);
    }
}

void ExampleGUI::RenderEditorMode(){
    RenderObstacle();

    if(ImGui::TreeNode("Robot")){
        RenderRobotProperties();
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

void ExampleGUI::RenderPathFinder(){
    RenderSolutions();
    RenderComputePath();
    RenderChosenSolutions();
}

void ExampleGUI::RenderSolutions(){
    static int e = 0;
    const int START = 0;
    const int END = 1;
    ImGui::RadioButton("Start", &e, START); ImGui::SameLine();
    ImGui::RadioButton("End", &e, END);

    auto& solution1 = robot_simulation_->robot_scene()->robot()->solution1();
    auto& solution2 = robot_simulation_->robot_scene()->robot()->solution2();

    ImVec4 color1(0, 1, 0, 1);
    ImVec4 color2(0, 0, 1, 1);
    RenderSetSolution(solution1, color1, "First solution", e);
    RenderSetSolution(solution2, color2, "Second solution", e);

    if(!solution1.available && !solution2.available){
        ImGui::Text("No Solution available");
    }
}

void ExampleGUI::RenderSetSolution(RobotSolution& solution,
                                   ImVec4& color, std::string text,
                                   int start_or_end){
    const int START = 0;
    const int END = 1;
    if(solution.available){
        ImGui::PushStyleColor(ImGuiCol_Button, color);
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 0, 0, 1));
        if(ImGui::Button(text.c_str())){
            if(start_or_end == START){
                robot_simulation_->SetStartSolution(solution);
            }else if (start_or_end == END){
                robot_simulation_->SetEndSolution(solution);
            }
        }
        ImGui::PopStyleColor(2);
    }
}

void ExampleGUI::RenderComputePath(){
    if(ImGui::Button("Compute Path")){
        robot_simulation_->StartPathFinder();
    }
}

void ExampleGUI::RenderChosenSolutions(){
    float start_a1 = robot_simulation_->GetStartSolution().alpha1;
    float start_a2 = robot_simulation_->GetStartSolution().alpha2;

    float end_a1 = robot_simulation_->GetEndSolution().alpha1;
    float end_a2 = robot_simulation_->GetEndSolution().alpha2;

    static float v1[2];
    v1[0] = start_a1;
    v1[1] = start_a2;

    static float v2[2];
    v2[0] = end_a1;
    v2[1] = end_a2;

    ImGui::InputFloat2("Start: ", v1);
    ImGui::InputFloat2("End: ", v2);

}