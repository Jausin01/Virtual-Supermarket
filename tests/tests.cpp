// Plain assert-style tests for the game logic that does not need a window.
#include <cmath>
#include <fstream>
#include <iostream>
#include "Cart.h"
#include "Money.h"
#include "Payment.h"
#include "ProductCatalog.h"

namespace
{
    int failures = 0;

    void Check(bool condition, const char* what)
    {
        if (!condition)
        {
            std::cerr << "FAILED: " << what << "\n";
            ++failures;
        }
    }

    bool Near(float a, float b) { return std::fabs(a - b) < 0.001f; }
}

int main()
{
    // ---- Money ----
    Check(FormatMoney(9.99f) == "$9.99", "FormatMoney 9.99");
    Check(FormatMoney(5.f) == "$5.00", "FormatMoney whole number");

    // ---- Payment ----
    Check(Near(ApplyMemberDiscount(100.f), 90.f), "member discount");
    {
        PaymentCheck exact = CheckPayment(12.33f, 12.33f);
        Check(exact.enough && exact.difference == 0.f, "exact payment");
        Check(DescribePayment(exact) == "Thank you for exact payment!", "exact message");

        PaymentCheck change = CheckPayment(12.33f, 20.f);
        Check(change.enough && Near(change.difference, 7.67f), "change is computed");
        Check(DescribePayment(change) == "Thank you!\nYour change is: $7.67", "change message");

        PaymentCheck missing = CheckPayment(12.33f, 5.f);
        Check(!missing.enough && Near(missing.difference, 7.33f), "missing amount");
        Check(DescribePayment(missing) == "Insufficient!\nStill missing: $7.33", "missing message");
    }

    // ---- Cart ----
    {
        Product apple("Apple", 1.20f, ProductCategory::Fruits);
        Product milk("Milk", 2.50f, ProductCategory::Drinks);

        Cart cart;
        Check(cart.IsEmpty(), "new cart is empty");

        cart.AddItem(apple, 1);
        cart.AddItem(apple, 1);
        cart.AddItem(milk, 1);
        Check(cart.GetItemCount() == 2, "same product shares one line");
        Check(cart.GetItems()[0].quantity == 2, "quantity increases");
        Check(Near(cart.GetTotal(), 4.90f), "cart total");

        cart.RemoveItemByName("Apple");
        Check(cart.GetItems()[0].quantity == 1, "remove takes one unit");
        cart.RemoveItemByName("Apple");
        Check(cart.GetItemCount() == 1, "line disappears at zero");
        cart.RemoveItemByName("Not in cart"); // must not crash
        Check(cart.GetItemCount() == 1, "removing an unknown product changes nothing");

        cart.Clear();
        Check(cart.IsEmpty(), "clear empties the cart");
    }

    // ---- Product catalog ----
    {
        Product product("x", 0.f, ProductCategory::None);
        Check(ParseProductLine("Apple|1.20|Fruits", product), "parse a good line");
        Check(product.GetName() == "Apple" && Near(product.GetPrice(), 1.2f) &&
              product.GetCategory() == ProductCategory::Fruits, "parsed fields");
        Check(!ParseProductLine("Broken line", product), "reject a line without separators");
        Check(!ParseProductLine("Apple|abc|Fruits", product), "reject a bad price");
        Check(!ParseProductLine("Apple|1.20|NoSuchCategory", product), "reject an unknown category");

        const std::string path = "catalog_test.txt";
        {
            std::ofstream file(path);
            file << "Apple|1.20|Fruits\nBad line\nMilk|2.50|Drinks\n";
        }
        ProductCatalog catalog = LoadProductCatalog(path);
        std::remove(path.c_str());
        Check(GetProducts(catalog, ProductCategory::Fruits).size() == 1, "catalog has the fruit");
        Check(GetProducts(catalog, ProductCategory::Drinks).size() == 1, "catalog has the drink");
        Check(GetProducts(catalog, ProductCategory::Sweets).empty(), "empty category is empty");
    }

    if (failures == 0)
    {
        std::cout << "All tests passed.\n";
    }
    return failures == 0 ? 0 : 1;
}
