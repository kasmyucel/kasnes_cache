#ifndef DBCONNECTION_H
#define DBCONNECTION_H

#include <pqxx/pqxx>
#include "settings.h"
#include "base/data.h"

class DBConnection
{
public:
    DBConnection(int id);

    bool isConnected();
    int getID();
    Data checkDB(int id);

private:
    std::string conn_str = "host=" DB_ADDRESS
        " port=" PORT
        " dbname=" DB_NAME
        " user=" DB_USER
        " password=" DB_PASS;

    pqxx::connection connection;

    bool connectionStatus;
    int connectionID;
};

#endif // DBCONNECTION_H
