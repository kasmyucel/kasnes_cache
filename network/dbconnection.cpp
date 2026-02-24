#include "dbconnection.h"
#include <cstring>
#include <iostream>
DBConnection::DBConnection(int id)
    : connection(conn_str), connectionID(id)
{
    connectionStatus = connection.is_open();
}

bool DBConnection::isConnected()
{
    return this->connectionStatus;
}

int DBConnection::getID()
{
    return this->connectionID;
};

Data DBConnection::checkDB(int id)
{
    pqxx::work txn{this->connection};

    std::cerr << "Conn string: " << conn_str << std::endl;


    std::string_view query = "SELECT id, product_name, price FROM products WHERE id = $1 LIMIT 5";

    pqxx::result res = txn.exec(query, id);

    if (res.empty())
    {

        return Data("John Doe Wallet", -1, -1);
    }


    auto row = res[0];

    int productID = row["id"].as<int>();
    double productPrice = row["price"].as<double>();
    std::string name_str = row["product_name"].as<std::string>();

    txn.commit();

    Data result(name_str, productPrice, productID);

    return result;
}
