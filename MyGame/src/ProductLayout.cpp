#include "ProductLayout.h"
#include <iterator>

namespace
{
    const ProductSpriteSpec kBreakFastSprites[] =
    {
        { "Images/Products/BreakFast/Bacon.png", 0.6f, 25.f, 25.f },
        { "Images/Products/BreakFast/Egg.png", 0.6f, 25.f, 45.f },
        { "Images/Products/BreakFast/Pankakes.png", 0.7f, 35.f, 45.f },
        { "Images/Products/BreakFast/SmilyBread.png", 0.2f, 25.f, 27.f },
    };

    const ProductSpriteSpec kDrinksSprites[] =
    {
        { "Images/Products/Drinks/Coke Mojito.png", 0.8f, 40.f, 35.f },
        { "Images/Products/Drinks/Tequila Sunrise.png", 0.45f, 35.f, 25.f },
        { "Images/Products/Drinks/Coke.png", 0.4f, 45.f, 25.f },
        { "Images/Products/Drinks/OrangeFanta.png", 0.4f, 50.f, 30.f },
        { "Images/Products/Drinks/Sprite.png", 0.15f, 50.f, 35.f },
    };

    const ProductSpriteSpec kFruitsSprites[] =
    {
        { "Images/Products/Fruits/Apple.png", 1.f, 25.f, 35.f },
        { "Images/Products/Fruits/Berry.png", 1.2f, 15.f, 25.f },
        { "Images/Products/Fruits/GrapeFruit.png", 0.7f, 35.f, 35.f },
        { "Images/Products/Fruits/Watermelon.png", 0.7f, 30.f, 50.f },
    };

    const ProductSpriteSpec kIngredientsSprites[] =
    {
        { "Images/Products/Ingredients/Butter.png", 1.f, 25.f, 45.f },
        { "Images/Products/Ingredients/Cheese.png", 1.5f, 40.f, 35.f },
        { "Images/Products/Ingredients/RawSteak.png", 1.5f, 35.f, 45.f },
        { "Images/Products/Ingredients/Wheat.png", 1.f, 30.f, 25.f },
    };

    const ProductSpriteSpec kMealSprites[] =
    {
        { "Images/Products/Meal/Hamburguer.png", 0.7f, 45.f, 40.f },
        { "Images/Products/Meal/Omurice.png", 0.5f, 30.f, 25.f },
        { "Images/Products/Meal/Steak.png", 0.5f, 30.f, 25.f },
        { "Images/Products/Meal/Suspicious Steak.png", 0.5f, 30.f, 35.f },
    };

    const ProductSpriteSpec kPowerUpsSprites[] =
    {
        { "Images/Products/PowerUps/1-Up Mushroom.png", 1.f, 45.f, 45.f },
        { "Images/Products/PowerUps/AcornShroom.png", 1.f, 35.f, 35.f },
        { "Images/Products/PowerUps/BeeShroom.png", 1.f, 40.f, 45.f },
        { "Images/Products/PowerUps/BellShroom.png", 1.f, 40.f, 50.f },
        { "Images/Products/PowerUps/BoomerangueFlower.png", 1.f, 40.f, 50.f },
        { "Images/Products/PowerUps/CherryFlower.png", 1.f, 45.f, 50.f },
        { "Images/Products/PowerUps/CloudFlower.png", 1.f, 35.f, 50.f },
        { "Images/Products/PowerUps/FireFlower.png", 1.f, 40.f, 50.f },
        { "Images/Products/PowerUps/GhostShroom.png", 1.f, 40.f, 50.f },
        { "Images/Products/PowerUps/GoldenFireFlower.png", 1.f, 40.f, 45.f },
        { "Images/Products/PowerUps/IceFlower.png", 1.f, 35.f, 45.f },
        { "Images/Products/PowerUps/MetalShroom.png", 1.f, 40.f, 45.f },
        { "Images/Products/PowerUps/Mushroom.png", 1.f, 45.f, 50.f },
        { "Images/Products/PowerUps/PenguinSuit.png", 1.f, 40.f, 45.f },
        { "Images/Products/PowerUps/PropellerShroom.png", 1.f, 40.f, 40.f },
        { "Images/Products/PowerUps/RockShroom.png", 1.f, 40.f, 45.f },
        { "Images/Products/PowerUps/SmallShroom.png", 1.f, 45.f, 50.f },
        { "Images/Products/PowerUps/YellowMushroom.png", 0.5f, 40.f, 50.f },
    };

    const ProductSpriteSpec kSweetsSprites[] =
    {
        { "Images/Products/Sweets/Cake.png", 0.3f, 30.f, 45.f },
        { "Images/Products/Sweets/Candy.png", 0.7f, 35.f, 35.f },
        { "Images/Products/Sweets/ChocolateBar.png", 0.35f, 25.f, 25.f },
        { "Images/Products/Sweets/IceCream.png", 0.6f, 35.f, 45.f },
        { "Images/Products/Sweets/Popsicle.png", 0.35f, 30.f, 25.f },
        { "Images/Products/Sweets/Pudding.png", 0.6f, 40.f, 35.f },
    };
    template <size_t N>
    std::vector<ProductSpriteSpec> ToVector(const ProductSpriteSpec (&specs)[N])
    {
        return std::vector<ProductSpriteSpec>(std::begin(specs), std::end(specs));
    }
}

const std::vector<ProductSpriteSpec>& GetSpriteSpecs(ProductCategory category)
{
    static const std::vector<ProductSpriteSpec> breakFast   = ToVector(kBreakFastSprites);
    static const std::vector<ProductSpriteSpec> drinks      = ToVector(kDrinksSprites);
    static const std::vector<ProductSpriteSpec> fruits      = ToVector(kFruitsSprites);
    static const std::vector<ProductSpriteSpec> ingredients = ToVector(kIngredientsSprites);
    static const std::vector<ProductSpriteSpec> meal        = ToVector(kMealSprites);
    static const std::vector<ProductSpriteSpec> powerUps    = ToVector(kPowerUpsSprites);
    static const std::vector<ProductSpriteSpec> sweets      = ToVector(kSweetsSprites);
    static const std::vector<ProductSpriteSpec> none;

    switch (category)
    {
    case ProductCategory::BreakFast:   return breakFast;
    case ProductCategory::Drinks:      return drinks;
    case ProductCategory::Fruits:      return fruits;
    case ProductCategory::Ingredients: return ingredients;
    case ProductCategory::Meal:        return meal;
    case ProductCategory::PowerUps:    return powerUps;
    case ProductCategory::Sweets:      return sweets;
    default:                           return none;
    }
}
