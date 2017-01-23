#ifndef PROJECT_FLOOD_FILL_H
#define PROJECT_FLOOD_FILL_H

#include <vector>
#include <queue>

/**
 * Assume:
 *  Target          == 0
 *  Obstacle        == -1
 *  Filled Pixels   == i++ (i = [1,n])
 *
 * Source: https://en.wikipedia.org/wiki/Flood_fill
 *
 * Flood-fill (node, target-color, replacement-color):
  1. If target-color is equal to replacement-color, return.
  2. If color of node is not equal to target-color, return.
  3. Set Q to the empty queue.
  4. Add node to the end of Q.
  5. While Q is not empty:
  6.     Set n equal to the first element of Q.
  7.     Remove first element from Q.
  9.     Set the color of n to replacement-color.
 10.     Add west node to end of Q if color of west is equal to target-color.
 11.     Add east node to end of Q if color of east is equal to target-color.
 12.     Add north node to end of Q if color of north is equal to target-color.
 13.     Add south node to end of Q if color of south is equal to target-color.
 14. Return.
 */

struct Node{
    int i;
    int j;

    bool operator==(const Node& n1){
        return (n1.i == i && n1.j == j);
    }
};

class FloodFill {
public:
    FloodFill();
    ~FloodFill();

    std::vector<Node> Compute(std::vector<std::vector<int>>& data,
                              Node start, Node end);
private:
    void MaybeAddToQueue(std::queue<Node>& Q,
                         std::vector<std::vector<int>>& data,
                         Node new_node,
                         Node current_node);

    Node GetLeft(Node n);
    Node GetRight(Node n);
    Node GetUp(Node n);
    Node GetDown(Node n);

    std::vector<Node> ComputeShortestPath(std::vector<std::vector<int>>& data,
                                          Node start, Node end);
    Node GetMinimumNode(std::vector<std::vector<int>>& data,
                        std::vector<Node>& nodes);

    const int OBSTACLE;
    const int TARGET_COLOR;
    const int size;

    int counter;
};


#endif //PROJECT_FLOOD_FILL_H
