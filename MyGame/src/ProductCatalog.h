#pragma once
#include <map>
#include <string>
#include <vector>
#include "Product.h"

/// All products of the store, grouped by shelf category.
using ProductCatalog = std::map<ProductCategory, std::vector<Product>>;

/// Parses one "Name|Price|Category" line of Products.txt.
/// Returns false (and leaves outProduct untouched) when the line is malformed.
bool ParseProductLine(const std::string& line, Product& outProduct);

/// Reads the whole product file. Malformed lines are reported on stderr and skipped.
ProductCatalog LoadProductCatalog(const std::string& filename);

/// Products of one category, in file order. Empty when the category has none.
const std::vector<Product>& GetProducts(const ProductCatalog& catalog, ProductCategory category);
