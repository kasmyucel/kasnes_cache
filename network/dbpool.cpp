#include "dbpool.h"

DBPool::DBPool(int size) : poolSize(size)
{
    for (int i = 0; i < size; ++i) {
        connections.push(new DBConnection(i));
    }
}
