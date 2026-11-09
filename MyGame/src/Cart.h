#pragma once
#include <string>
#include <vector>
#include "Product.h"

/// One line of the cart: a product and how many of it were added.
struct CartItem
{
    Product product;
    int quantity;
};

/// The player's shopping cart. Adding a product that is already in the cart
/// increases its quantity instead of creating a second line.
class Cart
{
public:
    void AddItem(const Product& product, int quantity);

    /// Removes one unit of the product; the line disappears when it reaches zero.
    void RemoveItemByName(const std::string& name);

    void Clear();

    bool IsEmpty() const { return _items.empty(); }

    /// Number of different products (cart lines), not the total quantity.
    int GetItemCount() const { return (int)_items.size(); }

    const std::vector<CartItem>& GetItems() const { return _items; }

    float GetTotal() const;

private:
    std::vector<CartItem> _items;
};
