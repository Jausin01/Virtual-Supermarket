#pragma once
#include <string>
#include "ProductCategory.h"

/// One item that can be bought in the supermarket.
class Product
{
public:
    Product(std::string name, float price, ProductCategory category);

    const std::string& GetName() const { return _name; }
    float GetPrice() const { return _price; }
    ProductCategory GetCategory() const { return _category; }

    /// Price ready to display, e.g. "$9.99".
    std::string GetPrettyPrice() const;

private:
    std::string _name;
    float _price;
    ProductCategory _category;
};
