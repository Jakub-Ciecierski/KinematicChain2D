#ifndef PROJECT_EXAMPLE_GUI_H
#define PROJECT_EXAMPLE_GUI_H

#include "gui/gui.h"

#include <memory>

class RobotSimulation;
class RobotSolution;
struct ImVec4;

namespace ifx{
class EngineGUI;
class SceneContainer;
class PhysicsSimulation;
}

class ExampleGUI : public ifx::GUI{
public:

    ExampleGUI(GLFWwindow* window,
               std::shared_ptr<ifx::SceneContainer> scene,
               std::shared_ptr<RobotSimulation> robot_simulation,
               std::shared_ptr<ifx::PhysicsSimulation> physics_simulation);
    ~ExampleGUI();

    virtual void Render() override;
private:
    void RenderWindow();

    void RenderMode();

    void RenderEditorMode();
    void RenderObstacle();
    void RenderRobotProperties();

    void RenderPathFinder();
    void RenderSolutions();
    void RenderSetSolution(RobotSolution& solution,
                           ImVec4& color, std::string text,
                           int start_or_end);
    void RenderComputePath();
    void RenderChosenSolutions();

    void RenderConfigurationSpace();
    void RenderConfigurationSpacePlot();

    std::shared_ptr<ifx::EngineGUI> engine_gui_;
    std::shared_ptr<RobotSimulation> robot_simulation_;

};


#endif //PROJECT_EXAMPLE_GUI_H
