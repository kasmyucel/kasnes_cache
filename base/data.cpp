#include "data.h"
#include <cstring>
Data::Data()
{
    std::memset(this->productName, 0, sizeof(this->productName));
    std::string defaultName = "John Doe Wallet";
    std::strncpy(this->productName, defaultName.c_str(), sizeof(this->productName) - 1);

    this->id = -1;
    this->price = -1;
}
Data::Data(std::string productName_, double price_, int id_):
        price(price_),
        id(id_)
{
    std::memset(this->productName, 0, sizeof(this->productName));
    std::strncpy(this->productName, productName_.c_str(), sizeof(this->productName) - 1);
}

int Data::getId()
{
    return id;
}
std::string Data::getProductName()
{
    return productName ;
}
double Data::getPrice()
{
    return price;
}
