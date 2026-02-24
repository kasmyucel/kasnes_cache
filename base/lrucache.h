#ifndef LRUCACHE_H
#define LRUCACHE_H

#include "base/node.h"
#include "base/data.h"
#include <mutex>
#include <unordered_map>

class LRUCache
{
public:
    LRUCache();
    LRUCache(int capacity);

    bool get(int key, Data& outValue);
    bool put(int key, Data value);

    bool add(Node *node);
    bool remove(Node *node);

private:
    int capacity;
    std::unordered_map<int, Node*> cacheMap;
    Node *head;
    Node *tail;

    mutable std::mutex cacheMutex;
};

#endif // LRUCACHE_H
