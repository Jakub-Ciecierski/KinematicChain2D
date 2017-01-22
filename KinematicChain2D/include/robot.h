#ifndef PROJECT_ROBOT_H
#define PROJECT_ROBOT_H

#include <vector>
#include <memory>
#include <math/math_ifx.h>

namespace ifx{
class GameObject;
class SceneContainer;
}

struct RobotMiddlePoint{
    glm::vec2 p1;
    glm::vec2 p2;
};

struct RobotSolution{
    glm::vec2 p0;
    glm::vec2 p1;
    glm::vec2 p2;

    float alpha1;
    float alpha2;

    bool available = true;
};

struct Obstacle;

/**
 * All angles in degrees.
 */
class Robot {
public:
    Robot(std::shared_ptr<ifx::SceneContainer> scene);
    ~Robot();

    float* l1(){return &l1_;}
    float* l2(){return &l2_;}

    RobotSolution& solution1(){return solution1_;}
    RobotSolution& solution2(){return solution2_;}

    int solution_count(){return solution_count_;}

    void SolveInverse(const glm::vec2& p2);
    void SolveInverseWithConstraints(
            const glm::vec2& p2,
            const std::vector<std::shared_ptr<Obstacle>>& obstacles);

    void SolveDirect();
    RobotSolution SolveDirect(float alpha1, float alpha2);

    void SolveConstrains(
            RobotSolution& solution,
            const std::vector<std::shared_ptr<Obstacle>>& obstacles);
private:
    void UpdateGameObject();

    void SolveMiddlePoint(const glm::vec2& p0,
                          const glm::vec2& p2);

    float l1_;
    float l2_;

    RobotSolution solution1_;
    RobotSolution solution2_;
    int solution_count_;

    std::shared_ptr<ifx::GameObject> game_object_;
    std::shared_ptr<ifx::SceneContainer> scene_;
};


#endif //PROJECT_ROBOT_H

