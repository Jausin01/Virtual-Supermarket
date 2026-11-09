#pragma once
#include <vector>
#include "ProductCategory.h"

/// Where and how big the picture of one product is drawn inside its shelf slot.
struct ProductSpriteSpec
{
    const char* file;   // Image path
    float scale;        // Uniform scale applied to the image
    float offsetX;      // Distance from the slot's top-left corner
    float offsetY;
};

/// Picture settings of a category. The n-th entry belongs to the n-th product
/// of that category in Products.txt, so both lists must keep the same order.
const std::vector<ProductSpriteSpec>& GetSpriteSpecs(ProductCategory category);
