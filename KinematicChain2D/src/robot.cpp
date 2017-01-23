#include "robot.h"

#include <obstacle.h>
#include <game/scene_container.h>
#include <object/game_object.h>
#include <graphics/factory/render_object_factory.h>
#include <iostream>

Robot::Robot(std::shared_ptr<ifx::SceneContainer> scene) :
        l1_(1),
        l2_(1),
        solution_count_(0),
        game_object_(nullptr),
        scene_(scene){
    solution1_.p0 = glm::vec2(0,0);
    solution1_.p2 = glm::vec2(0,2);
    solution1_.alpha1 = 0;
    solution1_.alpha2 = 0;

    solution2_.p0 = glm::vec2(0,0);
    solution2_.p2 = glm::vec2(0,2);
    solution2_.alpha1 = 0;
    solution2_.alpha2 = 0;
}

Robot::~Robot(){}

void Robot::SolveInverse(const glm::vec2& p2){
    solution1_.p2 = p2;
    solution2_.p2 = p2;

    SolveMiddlePoint(solution1_.p0, solution1_.p2);

    UpdateGameObject();
}

void Robot::SolveInverseWithConstraints(
        const glm::vec2& p2,
        const std::vector<std::shared_ptr<Obstacle>>& obstacles){
    solution1_.p2 = p2;
    solution2_.p2 = p2;

    SolveMiddlePoint(solution1_.p0, solution1_.p2);

    SolveConstrains(solution1_, obstacles);
    SolveConstrains(solution2_, obstacles);

    UpdateGameObject();
}

void Robot::SolveMiddlePoint(const glm::vec2& p0,
                             const glm::vec2& p2){
    // TODO corner cases.
    RobotMiddlePoint p1;

    auto x0 = p0.x;
    auto y0 = p0.y;
    auto x1 = p2.x;
    auto y1 = p2.y;

    auto r0 = l1_;
    auto r1 = l2_;

    p1.p1.x = (-(pow(r0,2)*x0) + pow(r1,2)*x0 + pow(x0,3) + pow(r0,2)
                                                                *x1 - pow(r1,2)*x1 - pow(x0,2)*x1 - x0*pow(x1,2) + pow(x1,3) + x0*pow(y0,2) + x1*pow(y0,2) - 2*x0*y0*y1 - 2*x1*y0*y1 + x0*pow(y1,2) +
     x1*pow(y1,2) - sqrt(-(pow(y0 - y1,2)*(pow(r0,4) + pow(-pow(r1,2) +
                                                                   pow(x0,2) - 2*x0*x1 + pow(x1,2) + pow(y0,2) - 2*y0*y1 + pow(y1,2),2) -
                                               2*pow(r0,2)*(pow(r1,2) +
                                                       pow(x0,2) - 2*x0*x1 + pow(x1,2) + pow(y0,2) - 2*y0*y1 + pow(y1,2))))))/(2.*(pow(x0,2) - 2*x0*x1 + pow(x1,2) + pow(y0 - y1,2)));

    p1.p1.y = (-(pow(r0,2)*pow(y0,2)) + pow(r1,2)*pow(y0,2) + pow(x0,2)*pow(y0,2) - 2*x0*x1*pow(y0,2) + pow(x1,2)*pow(y0,2) + pow(y0,4) + 2*pow(r0,2)*y0*y1 - 2*pow(r1,2)*y0*y1 - 2*pow(y0,3)*y1 -
               pow(r0,2)*pow(y1,2) + pow(r1,2)*pow(y1,2) - pow(x0,2)*pow(y1,2) + 2*x0*x1*pow(y1,2) - pow(x1,2)*pow(y1,2) + 2*y0*pow(y1,3) - pow(y1,4) +
               x0*sqrt(-(pow(y0 - y1,2)*(pow(r0,4) + pow(-pow(r1,2) + pow(x0,2) - 2*x0*x1 + pow(x1,2) + pow(y0,2) - 2*y0*y1 + pow(y1,2),2) -
                                           2*pow(r0,2)*(pow(r1,2) + pow(x0,2) - 2*x0*x1 + pow(x1,2) + pow(y0,2) - 2*y0*y1 + pow(y1,2))))) -
               x1*sqrt(-(pow(y0 - y1,2)*(pow(r0,4) + pow(-pow(r1,2) + pow(x0,2) - 2*x0*x1 + pow(x1,2) + pow(y0,2) - 2*y0*y1 + pow(y1,2),2) -
                                           2*pow(r0,2)*(pow(r1,2) + pow(x0,2)
                                                        - 2*x0*x1 + pow(x1,2) + pow(y0,2) - 2*y0*y1 + pow(y1,2))))))/(2.*(pow(x0,2) - 2*x0*x1 + pow(x1,2) + pow(y0 - y1,2))*(y0 - y1));

    p1.p2.x = (-(pow(r0,2)*x0) + pow(r1,2)*x0 + pow(x0,3) + pow(r0,2)*x1 - pow(r1,2)*x1 - pow(x0,2)*x1 - x0*pow(x1,2) + pow(x1,3) + x0*pow(y0,2) + x1*pow(y0,2) - 2*x0*y0*y1 - 2*x1*y0*y1 + x0*pow(y1,2) +
               x1*pow(y1,2) + sqrt(-(pow(y0 - y1,2)*(pow(r0,4) + pow(-pow(r1,2) + pow(x0,2) - 2*x0*x1 + pow(x1,2) + pow(y0,2) - 2*y0*y1 + pow(y1,2),2) -
                                                         2*pow(r0,2)*(pow
                                                                                (r1,2) + pow(x0,2) - 2*x0*x1 + pow(x1,2) + pow(y0,2) - 2*y0*y1 + pow(y1,2))))))/(2.*(pow(x0,2) - 2*x0*x1 + pow(x1,2) + pow(y0 - y1,2)));
    p1.p2.y = (-(pow(r0,2)*pow(y0,2)) + pow(r1,2)*pow(y0,2) + pow(x0,2)*pow(y0,2) - 2*x0*x1*pow(y0,2) + pow(x1,2)*pow(y0,2) + pow(y0,4) + 2*pow(r0,2)*y0*y1 - 2*pow(r1,2)*y0*y1 - 2*pow(y0,3)*y1 -
               pow(r0,2)*pow(y1,2) + pow(r1,2)*pow(y1,2) - pow(x0,2)*pow(y1,2) + 2*x0*x1*pow(y1,2) - pow(x1,2)*pow(y1,2) + 2*y0*pow(y1,3) - pow(y1,4) -
               x0*sqrt(-(pow(y0 - y1,2)*(pow(r0,4) + pow(-pow(r1,2) + pow(x0,2) - 2*x0*x1 + pow(x1,2) + pow(y0,2) - 2*y0*y1 + pow(y1,2),2) -
                                           2*pow(r0,2)*(pow(r1,2) + pow(x0,2) - 2*x0*x1 + pow(x1,2) + pow(y0,2) - 2*y0*y1 + pow(y1,2))))) +
               x1*sqrt(-(pow(y0 - y1,2)*(pow(r0,4) + pow(-pow(r1,2) + pow(x0,2) - 2*x0*x1 + pow(x1,2) + pow(y0,2) - 2*y0*y1 + pow(y1,2),2) -
                                           2*pow(r0,2)*(pow(r1,2) + pow
                                                                                (x0,2) - 2*x0*x1 + pow(x1,2) + pow(y0,2) - 2*y0*y1 + pow(y1,2))))))/(2.*(pow(x0,2) - 2*x0*x1 + pow(x1,2) + pow(y0 - y1,2))*(y0 - y1));

    solution1_.p1 = p1.p1;
    solution2_.p1 = p1.p2;

    // compute alphas
    glm::vec2 y_axis(0,1);
    glm::vec2 direction1_p1 = glm::normalize(p1.p1 - p0);
    glm::vec2 direction1_p2 = glm::normalize(p2 - p1.p1);

    float sina1 = (direction1_p1.x);
    float cosa1 = (direction1_p1.y);
    solution1_.alpha1 = atan2(sina1, cosa1);

    float sina2 = (direction1_p2.x);
    float cosa2 = (direction1_p2.y);
    solution1_.alpha2 = atan2(sina2, cosa2);


    glm::vec2 direction2_p1 = glm::normalize(p1.p2 - p0);
    glm::vec2 direction2_p2 = glm::normalize(p2 - p1.p2);
    float sina2_1 = (direction2_p1.x);
    float cosa2_1 = (direction2_p1.y);
    solution2_.alpha1 = atan2(sina2_1, cosa2_1);

    float sina2_2 = (direction2_p2.x);
    float cosa2_2 = (direction2_p2.y);
    solution2_.alpha2 = atan2(sina2_2, cosa2_2);

    /*
    solution1_.alpha1 = acos(glm::dot(glm::normalize(p1.p1 - p0), y_axis));
    solution1_.alpha2 = acos(glm::dot(glm::normalize(p2 - p1.p1), y_axis));

    solution2_.alpha1 = acos(glm::dot(glm::normalize(p1.p2 - p0), y_axis));
    solution2_.alpha2 = acos(glm::dot(glm::normalize(p2 - p1.p2), y_axis));
*/

    if(solution1_.alpha1 < 0)
        solution1_.alpha1 += 2*M_PI;
    if(solution1_.alpha2 < 0)
        solution1_.alpha2 += 2*M_PI;
    if(solution2_.alpha1 < 0)
        solution2_.alpha1 += 2*M_PI;
    if(solution2_.alpha2 < 0)
        solution2_.alpha2 += 2*M_PI;

    solution1_.alpha1 = glm::degrees(solution1_.alpha1);
    solution1_.alpha2 = glm::degrees(solution1_.alpha2);
    solution2_.alpha1 = glm::degrees(solution2_.alpha1);
    solution2_.alpha2 = glm::degrees(solution2_.alpha2);

    solution1_.available = true;
    solution2_.available = true;

    if(std::isnan(solution1_.alpha1) || std::isnan(solution1_.alpha2))
        solution1_.available = false;
    if(std::isnan(solution2_.alpha1) || std::isnan(solution2_.alpha2))
        solution2_.available = false;
}

void Robot::SolveConstrains(
        RobotSolution& solution,
        const std::vector<std::shared_ptr<Obstacle>>& obstacles){
    if(!solution.available) return;
    for(auto& obstacle : obstacles){
        if(obstacle->Collides(solution.p0, solution.p1))
            solution.available = false;
        if(obstacle->Collides(solution.p1, solution.p2))
            solution.available = false;
    }
}

void Robot::SolveDirectAndUpdate(){
    SolveDirectAndUpdate(solution1_.alpha1, solution1_.alpha2);
}

void Robot::SolveDirectAndUpdate(float alpha1, float alpha2){
    solution1_.p1.x
            = solution1_.p0.x + (l1_ * sin(glm::radians(alpha1)));
    solution1_.p1.y
            = solution1_.p0.y + (l1_ * cos(glm::radians(alpha1)));

    solution1_.p2.x
            = solution1_.p1.x + (l2_ * sin(glm::radians(alpha2)));
    solution1_.p2.y
            = solution1_.p1.y + (l2_ * cos(glm::radians(alpha2)));

    solution1_.alpha1 = alpha1;
    solution1_.alpha2 = alpha2;
    solution1_.available = true;

    solution2_ = solution1_;

    UpdateGameObject();
}

RobotSolution Robot::SolveDirect(float alpha1, float alpha2){
    RobotSolution solution;
    solution.p0 = solution1_.p0;

    solution.p1.x
            = solution.p0.x + (l1_ * sin(glm::radians(alpha1)));
    solution.p1.y
            = solution.p0.y + (l1_ * cos(glm::radians(alpha1)));

    solution.p2.x
            = solution.p1.x + (l2_ * sin(glm::radians(alpha2)));
    solution.p2.y
            = solution.p1.y + (l2_ * cos(glm::radians(alpha2)));

    return solution;
}

void Robot::UpdateGameObject(){
    if(game_object_)
        scene_->Remove(game_object_);
    game_object_ = std::shared_ptr<ifx::GameObject>(new ifx::GameObject());

    glm::vec3 p0 = glm::vec3(solution1_.p0.x, solution1_.p0.y, 0);
    glm::vec3 p1_1 = glm::vec3(solution1_.p1.x, solution1_.p1.y, 0);
    glm::vec3 p1_2 = glm::vec3(solution2_.p1.x, solution2_.p1.y, 0);
    glm::vec3 p2 = glm::vec3(solution1_.p2.x, solution1_.p2.y, 0);

    glm::vec3 color1(0, 255, 0);
    glm::vec3 color2(0, 0, 255);

    if(!solution1_.available){
        color1 = glm::vec3(120, 0, 0);
    }
    if(!solution2_.available){
        color2 = glm::vec3(120, 0, 0);
    }

    auto render_object1_1 = ifx::RenderObjectFactory().CreateLine(p0, p1_1,
                                                                color1);
    auto render_object1_2 = ifx::RenderObjectFactory().CreateLine(p1_1, p2,
                                                                color1);

    auto render_object2_1 = ifx::RenderObjectFactory().CreateLine(p0, p1_2,
                                                                color2);
    auto render_object2_2 = ifx::RenderObjectFactory().CreateLine(p1_2, p2,
                                                                color2);
/*
    if(solution1_.available){
        game_object_->Add(render_object1_1);
        game_object_->Add(render_object1_2);
    }

    if(solution2_.available){
        game_object_->Add(render_object2_1);
        game_object_->Add(render_object2_2);
    }
*/


    game_object_->Add(render_object1_1);
    game_object_->Add(render_object1_2);

    game_object_->Add(render_object2_1);
    game_object_->Add(render_object2_2);

    scene_->Add(game_object_);
}
