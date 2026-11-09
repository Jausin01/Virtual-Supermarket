#include "ProductCatalog.h"
#include <fstream>
#include <iostream>
#include <sstream>

namespace
{
    std::string Trim(const std::string& text)
    {
        const char* whitespace = " \t\r\n";
        size_t first = text.find_first_not_of(whitespace);
        if (first == std::string::npos)
        {
            return "";
        }
        size_t last = text.find_last_not_of(whitespace);
        return text.substr(first, last - first + 1);
    }

    bool TryParseCategory(const std::string& text, ProductCategory& outCategory)
    {
        static const std::map<std::string, ProductCategory> categories =
        {
            { "PowerUp",     ProductCategory::PowerUps },
            { "BreakFast",   ProductCategory::BreakFast },
            { "Drinks",      ProductCategory::Drinks },
            { "Fruits",      ProductCategory::Fruits },
            { "Ingredients", ProductCategory::Ingredients },
            { "Meal",        ProductCategory::Meal },
            { "Sweets",      ProductCategory::Sweets },
        };

        auto it = categories.find(text);
        if (it == categories.end())
        {
            return false;
        }
        outCategory = it->second;
        return true;
    }
}

bool ParseProductLine(const std::string& line, Product& outProduct)
{
    std::stringstream stream(line);
    std::string name, priceText, categoryText;

    if (!std::getline(stream, name, '|') ||
        !std::getline(stream, priceText, '|') ||
        !std::getline(stream, categoryText))
    {
        return false;
    }

    name = Trim(name);
    priceText = Trim(priceText);
    categoryText = Trim(categoryText);

    float price = 0.f;
    try
    {
        price = std::stof(priceText);
    }
    catch (const std::exception&)
    {
        return false;
    }

    ProductCategory category;
    if (name.empty() || !TryParseCategory(categoryText, category))
    {
        return false;
    }

    outProduct = Product(name, price, category);
    return true;
}

ProductCatalog LoadProductCatalog(const std::string& filename)
{
    ProductCatalog catalog;

    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Failed to open product file: " << filename << "\n";
        return catalog;
    }

    std::string line;
    while (std::getline(file, line))
    {
        if (Trim(line).empty())
        {
            continue;
        }

        Product product("", 0.f, ProductCategory::None);
        if (!ParseProductLine(line, product))
        {
            std::cerr << "Skipping malformed product line: " << line << "\n";
            continue;
        }

        catalog[product.GetCategory()].push_back(product);
    }

    return catalog;
}

const std::vector<Product>& GetProducts(const ProductCatalog& catalog, ProductCategory category)
{
    static const std::vector<Product> empty;
    auto it = catalog.find(category);
    return it != catalog.end() ? it->second : empty;
}
