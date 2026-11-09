#include "Product.h"
#include "Money.h"

Product::Product(std::string name, float price, ProductCategory category)
    : _name(std::move(name)), _price(price), _category(category)
{
}

std::string Product::GetPrettyPrice() const
{
    return FormatMoney(_price);
}
