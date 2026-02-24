#ifndef DATA_H
#define DATA_H

#include <string>
class Data
{
public:
    Data();
    Data(std::string productName_ , double price_, int id_);
    std::string getProductName();
    double getPrice();
    int getId();

public:
    char productName[100];
    double price ;
    int id;

};

#endif // DATA_H
