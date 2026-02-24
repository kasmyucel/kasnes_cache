#include "dataservice.h"

DataService::DataService(LRUCache& c, DBPool& db) : cache(c), dbPool(db)
{

}

bool DataService::getData(int key, Data& result)
{


    if (cache.get(key, result)) {
        return true; // Cache HIT
    }

    {
        DBPool::ConnectionGuard dbConn(dbPool);

        Data found = dbConn->checkDB(key);

        if (found.id != -1) {
            result = found;
            cache.put(key, result);
            return true;
        }
    }
    return false;
}
