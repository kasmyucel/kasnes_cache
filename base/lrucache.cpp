#include "lrucache.h"

LRUCache::LRUCache() {}

LRUCache::LRUCache(int capacity)
{
    this->capacity = capacity;
    this->head = new Node(-1, Data("null_product", -1, -1));
    this->tail = new Node(-1, Data("null_product", -1, -1));

    head->next = tail;
    tail->prev = head;
    head->prev = nullptr;
    tail->next = nullptr;

}


bool LRUCache::get(int key, Data& outValue)
{
    std::lock_guard<std::mutex> lock(cacheMutex);

    if(this->cacheMap.find(key) == this->cacheMap.end())
        return false;

    Node *node = cacheMap[key];
    remove(node);
    add(node);


    outValue = node->value;

    return true;
}

bool LRUCache::put(int key, Data value)
{
    std::lock_guard<std::mutex> lock(cacheMutex);

    if(this->cacheMap.find(key) != cacheMap.end()) // key found
    {
        Node *oldNode = cacheMap[key];
        remove(oldNode);
        delete oldNode;
    }

    Node *node = new Node(key, value);

    if(node != NULL)
    {
        cacheMap[key] = node;
        add(node);

        if (cacheMap.size() > capacity) {
            Node *nodeToDelete = tail->prev;
            remove(nodeToDelete);
            cacheMap.erase(nodeToDelete->key);
            delete nodeToDelete;
        }

        return true;
    }
    return false;

}

bool LRUCache::add(Node *node)
{
    // insert the node between head and previous last used node
    if(node != NULL)
    {
        Node *nextNode = head->next;
        head->next = node;
        node->prev = head;
        node->next = nextNode;
        nextNode->prev = node;
        return true;
    }
    return false;
}

bool LRUCache::remove(Node *node)
{
    if(node != NULL)
    {
        Node *prevNode = node->prev;
        Node *nextNode = node->next;
        prevNode->next = nextNode;
        nextNode->prev = prevNode;
        return true;
    }
    return false;

}
