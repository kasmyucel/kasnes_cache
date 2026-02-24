#include "node.h"

Node::Node() {}

Node::Node(int k, Data v)
{
    key = k;
    value = v;
    next = nullptr;
    prev = nullptr;
}
