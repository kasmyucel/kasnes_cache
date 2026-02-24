#ifndef DATASERVICE_H
#define DATASERVICE_H

#include "base/lrucache.h"
#include "network/dbpool.h"

class DataService
{

public:
    DataService(LRUCache& c, DBPool& db);
    bool getData(int key, Data& result);

private:
    LRUCache& cache;
    DBPool& dbPool;

};

#endif // DATASERVICE_H
