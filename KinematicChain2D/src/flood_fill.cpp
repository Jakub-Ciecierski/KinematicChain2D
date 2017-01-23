#include "flood_fill.h"

#include <algorithm>
#include <iostream>

FloodFill::FloodFill() :
        OBSTACLE(-1),
        TARGET_COLOR(0),
        size(180+180){}
FloodFill::~FloodFill(){}

std::vector<Node> FloodFill::Compute(std::vector<std::vector<int>>& data,
                                     Node start, Node end){
    std::queue<Node> Q;
    Q.push(start);
    int color = 1;
    data[start.i][start.j] = color;
    counter = 0;
    while(!Q.empty()){
        Node current_node = Q.front();
        Q.pop();

        MaybeAddToQueue(Q, data, GetLeft(current_node), current_node);
        MaybeAddToQueue(Q, data, GetRight(current_node), current_node);
        MaybeAddToQueue(Q, data, GetUp(current_node), current_node);
        MaybeAddToQueue(Q, data, GetDown(current_node), current_node);
    }
    std::cout << "start: " << start.i << ", " << start.j << std::endl;
    std::cout << "end:   " << end.i << ", " << end.j << std::endl;

    std::cout << "counter: " << counter <<  std::endl;
    std::cout << "should be: " << size*size << std::endl;

    return ComputeShortestPath(data, start, end);
}

void FloodFill::MaybeAddToQueue(std::queue<Node>& Q,
                                std::vector<std::vector<int>>& data,
                                Node new_node,
                                Node current_node){
    int current_color = data[current_node.i][current_node.j];

    if(data[new_node.i][new_node.j] == TARGET_COLOR){
        data[new_node.i][new_node.j] = current_color+1;
        Q.push(new_node);
        counter++;
    }
}

Node FloodFill::GetLeft(Node n){
    Node left;
    left.i = n.i - 1;
    left.j = n.j;
    if(left.i < 0)
        left.i = size-1;

    return left;
}
Node FloodFill::GetRight(Node n){
    Node right;
    right.i = n.i + 1;
    right.j = n.j;
    if(right.i > size-1)
        right.i = 0;

    return right;
}

Node FloodFill::GetUp(Node n){
    Node up;
    up.i = n.i;
    up.j = n.j - 1;
    if(up.j < 0)
        up.j = size-1;

    return up;
}
Node FloodFill::GetDown(Node n){
    Node down;
    down.i = n.i;
    down.j = n.j + 1;
    if(down.j > size-1)
        down.j = 0;

    return down;
}

std::vector<Node> FloodFill::ComputeShortestPath(
        std::vector<std::vector<int>>& data,
        Node start, Node end){
    std::vector<Node> path;
    if(data[end.i][end.j] == TARGET_COLOR)
        return path;

    Node current_node = end;
    path.push_back(current_node);

    do {
        std::vector<Node> neighbours{GetLeft(current_node),
                                     GetRight(current_node),
                                     GetUp(current_node),
                                     GetDown(current_node)};

        current_node = GetMinimumNode(data, neighbours);

        path.push_back(current_node);
    }while(!(start == current_node));

    std::reverse(path.begin(), path.end());

    return path;
}

Node FloodFill::GetMinimumNode(std::vector<std::vector<int>>& data,
                               std::vector<Node>& nodes){
    int min_color = 999999;
    int min_index = 0;
    for(int i = 0; i < nodes.size(); i++){
        Node n = nodes[i];
        int color = data[n.i][n.j];
        if(color == OBSTACLE)
            continue;

        if(min_color > color){
            min_color = color;
            min_index = i;
        }
    }
    return nodes[min_index];
}