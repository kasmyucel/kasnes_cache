#ifndef NODE_H
#define NODE_H
#include "base/data.h"

class Node
{
public:
    Node();
    Node(int k, Data v);

public:
    int key;
    Data value;
    Node *next;
    Node *prev;
};

#endif // NODE_H
