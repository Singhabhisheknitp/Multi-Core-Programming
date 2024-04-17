#ifndef NODE_H
#define NODE_H

class Node {
public:
    int data;
    std::atomic<Node*> next;
    Node(int data);
};

#endif // NODE_H