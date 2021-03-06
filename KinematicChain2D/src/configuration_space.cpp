#include "configuration_space.h"

#include <robot.h>
#include <obstacle.h>
#include <flood_fill.h>
#include <algorithm>

#include <graphics/shaders/textures/texture.h>
#include <graphics/factory/texture_factory.h>
#include <iostream>

const int CONFIG_AVAILABLE = 0;
const int CONFIG_NOT_AVAILABLE = -1;

ConfigurationSpace::ConfigurationSpace() :
    width_(180+180), height_(180+180){
    available_color_ = glm::vec3(0, 0, 0);
    not_available_color_ = glm::vec3(255, 0, 0);
    texture_ = ifx::Texture2D::MakeTexture2DEmpty(
            ifx::NO_FILEPATH,
            ifx::TextureTypes::DIFFUSE,
            ifx::TextureInternalFormat::RGB,
            ifx::TexturePixelType::UNSIGNED_BYTE,
            width_, height_);

    InitData();
}

ConfigurationSpace::~ConfigurationSpace(){}

void ConfigurationSpace::Compute(
        const std::vector<std::shared_ptr<Obstacle>>& obstacles,
        std::shared_ptr<Robot> robot){
    data_.clear();
    texture_data_.clear();

    for(int i = 0; i < width_; i++){
        std::vector<int> row;
        for(int j = 0; j < height_; j++){
            RobotSolution solution = robot->SolveDirect((float)i, (float)j);
            robot->SolveConstrains(solution, obstacles);
            if(solution.available){
                texture_data_.push_back(available_color_.x);
                texture_data_.push_back(available_color_.y);
                texture_data_.push_back(available_color_.z);
                row.push_back(CONFIG_AVAILABLE);
            }
            else{
                texture_data_.push_back(not_available_color_.x);
                texture_data_.push_back(not_available_color_.y);
                texture_data_.push_back(not_available_color_.z);
                row.push_back(CONFIG_NOT_AVAILABLE);
            }
        }
        data_.push_back(row);
    }
    texture_->InitData(texture_data_.data(), width_, height_);
}

void ConfigurationSpace::UpdateTexture(std::vector<Node> path){
    if(path.size() == 0)
        return;
    Node end_node = path[path.size()-1];
    int max_color = data_[end_node.i][end_node.j];

    texture_data_.clear();

    for(int i = 0; i < width_; i++){
        for(int j = 0; j < height_; j++){
            Node current{i,j};
            if(std::find(path.begin(), path.end(), current) != path.end()){
                int color = data_[i][j];
                float gradient = (float)color / (float)max_color;
                float actual_color = 255.0f * gradient;

                texture_data_.push_back(0);
                texture_data_.push_back(actual_color);
                texture_data_.push_back(0);

            }else{
                if(data_[i][j] == CONFIG_NOT_AVAILABLE){
                    texture_data_.push_back(not_available_color_.x);
                    texture_data_.push_back(not_available_color_.y);
                    texture_data_.push_back(not_available_color_.z);
                }
                else{
                    texture_data_.push_back(available_color_.x);
                    texture_data_.push_back(available_color_.y);
                    texture_data_.push_back(available_color_.z);
                }
            }

        }
    }
    texture_->InitData(texture_data_.data(), width_, height_);
}

void ConfigurationSpace::InitData(){
    data_.clear();
    texture_data_.clear();

    for(int i = 0; i < width_; i++){
        std::vector<int> row;
        for(int j = 0; j < height_; j++){
            texture_data_.push_back(available_color_.x);
            texture_data_.push_back(available_color_.y);
            texture_data_.push_back(available_color_.z);
            row.push_back(CONFIG_AVAILABLE);
        }
        data_.push_back(row);
    }
    texture_->InitData(texture_data_.data(), width_, height_);
}