#include "Cart.h"
#include <algorithm>

namespace
{
    std::vector<CartItem>::iterator FindByName(std::vector<CartItem>& items, const std::string& name)
    {
        return std::find_if(items.begin(), items.end(),
            [&name](const CartItem& item) { return item.product.GetName() == name; });
    }
}

void Cart::AddItem(const Product& product, int quantity)
{
    auto it = FindByName(_items, product.GetName());
    if (it != _items.end())
    {
        it->quantity += quantity;
        return;
    }

    _items.push_back({ product, quantity });
}

void Cart::RemoveItemByName(const std::string& name)
{
    auto it = FindByName(_items, name);
    if (it == _items.end())
    {
        return;
    }

    if (it->quantity > 1)
    {
        it->quantity -= 1;
    }
    else
    {
        _items.erase(it);
    }
}

void Cart::Clear()
{
    _items.clear();
}

float Cart::GetTotal() const
{
    float total = 0.f;
    for (const CartItem& item : _items)
    {
        total += item.product.GetPrice() * item.quantity;
    }
    return total;
}
