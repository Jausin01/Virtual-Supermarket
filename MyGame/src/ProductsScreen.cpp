#include "ProductsScreen.h"
#include "ProductLayout.h"
#include "Transitions.h"
#include "UiHelpers.h"
#include <algorithm>
#include <map>
#include <memory>

namespace
{
    // ---- Layout -------------------------------------------------------------
    constexpr int kSlotsPerPage = 9;
    constexpr int kSlotColumns = 3;
    constexpr float kSlotFirstX = 50.f;
    constexpr float kSlotFirstY = 50.f;
    constexpr float kSlotSpacing = 200.f;
    constexpr float kSlotScale = 0.3f;

    constexpr float kButtonX = 1500.f;
    constexpr float kButtonScale = 2.f;
    constexpr float kModeButtonY = 850.f;
    constexpr float kReturnButtonY = 950.f;

    constexpr float kInfoBoxScale = 0.8f;
    constexpr float kInfoBoxOffsetX = -75.f; // Info box follows the mouse, shifted to the left

    struct CategoryButtonInfo
    {
        ProductCategory category;
        const char* label;
        float y;
    };

    const CategoryButtonInfo kCategoryButtons[] =
    {
        { ProductCategory::BreakFast,   "BreakFast",   100.f },
        { ProductCategory::Drinks,      "Drinks",      200.f },
        { ProductCategory::Fruits,      "Fruits",      300.f },
        { ProductCategory::Ingredients, "Ingredients", 400.f },
        { ProductCategory::Meal,        "Meal",        500.f },
        { ProductCategory::PowerUps,    "Power-Ups",   600.f },
        { ProductCategory::Sweets,      "Sweets",      700.f },
    };

    sf::Vector2f SlotPosition(int slot)
    {
        return { kSlotFirstX + kSlotSpacing * (float)(slot % kSlotColumns),
                 kSlotFirstY + kSlotSpacing * (float)(slot / kSlotColumns) };
    }

    int PageCount(size_t productCount)
    {
        return std::max(1, ((int)productCount + kSlotsPerPage - 1) / kSlotsPerPage);
    }

    // ---- Per-category display data -----------------------------------------

    /// A product on a shelf: its picture and the text shown when hovered.
    struct ShelfItem
    {
        sf::Sprite picture;
        sf::Text info;
    };

    /// Builds the pictures and info texts of a category, in Products.txt order.
    /// Item n sits in slot (n % 9) of page (n / 9).
    std::vector<ShelfItem> BuildShelf(ProductCategory category, const ProductCatalog& catalog,
        const sf::Font& font, TextureCache& textures)
    {
        std::vector<ShelfItem> shelf;
        const std::vector<Product>& products = GetProducts(catalog, category);
        const std::vector<ProductSpriteSpec>& specs = GetSpriteSpecs(category);

        for (size_t i = 0; i < products.size() && i < specs.size(); ++i)
        {
            const ProductSpriteSpec& spec = specs[i];
            sf::Vector2f slot = SlotPosition((int)i % kSlotsPerPage);

            ShelfItem item{ MakeSprite(textures, spec.file), sf::Text(font) };
            item.picture.setScale({ spec.scale, spec.scale });
            item.picture.setPosition({ slot.x + spec.offsetX, slot.y + spec.offsetY });

            item.info.setString(products[i].GetName() + "\n" + products[i].GetPrettyPrice());
            item.info.setFillColor(sf::Color::Black);

            shelf.push_back(std::move(item));
        }
        return shelf;
    }
}

void RunProductsScreen(sf::RenderWindow& window, AppState& appState, const sf::Font& font,
    TextureCache& textures, const ProductCatalog& catalog, Cart& cart)
{
    FadeIn(window, textures, AppState::Products);

    // Pictures and texts of every category are prepared once, before the loop.
    std::map<ProductCategory, std::vector<ShelfItem>> shelves;
    for (const CategoryButtonInfo& info : kCategoryButtons)
    {
        shelves[info.category] = BuildShelf(info.category, catalog, font, textures);
    }

    sf::Sprite background = MakeSprite(textures, "Images/InsideSupermarket.png");

    const sf::Texture& optionTexture = textures.Get("Images/InsideStoreOption.png");
    std::vector<Button> categoryButtons;
    for (const CategoryButtonInfo& info : kCategoryButtons)
    {
        categoryButtons.emplace_back(optionTexture, font, info.label,
            sf::Vector2f(kButtonX, info.y), sf::Vector2f(kButtonScale, kButtonScale));
    }
    Button modeButton(optionTexture, font, "Getting MODE",
        { kButtonX, kModeButtonY }, { kButtonScale, kButtonScale });
    Button returnButton(optionTexture, font, "Return",
        { kButtonX, kReturnButtonY }, { kButtonScale, kButtonScale });

    // The nine empty shelf slots (placeholders) that products are drawn on.
    std::vector<sf::Sprite> slots;
    for (int i = 0; i < kSlotsPerPage; ++i)
    {
        slots.push_back(MakeSprite(textures, "Images/ItemPlaceHolder.png",
            SlotPosition(i), { kSlotScale, kSlotScale }));
    }

    // Page button, only used by categories with more than nine products.
    sf::Sprite nextPageButton = MakeSprite(textures, "Images/NextPageButton.png", { 450.f, 650.f }, { 0.55f, 0.5f });
    sf::Sprite previousPageButton = MakeSprite(textures, "Images/NextPageButtonReverse.png", { 450.f, 650.f }, { 0.55f, 0.5f });
    sf::Text pageButtonHint = MakeText(font, "Turn Page?", sf::Color::Black);

    // Box that follows the mouse and shows the name and price of the hovered product.
    sf::Sprite infoBox = MakeSprite(textures, "Images/Dialogue Box(Upside Down).png",
        { 0.f, 0.f }, { kInfoBoxScale, kInfoBoxScale });

    ProductCategory currentCategory = ProductCategory::None;
    bool returningMode = false; // false: clicking adds to the cart, true: removes from it
    int page = 0;
    MouseInput mouse;

    while (window.isOpen())
    {
        while (auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        mouse.Update(window);
        const sf::Vector2f mousePos = mouse.Position();

        // ---- Menu buttons ----
        std::vector<bool> categoryHovered;
        for (Button& button : categoryButtons)
        {
            categoryHovered.push_back(button.UpdateHover(mousePos));
        }
        bool hoverMode = modeButton.UpdateHover(mousePos);
        bool hoverReturn = returnButton.UpdateHover(mousePos);

        window.clear();
        window.draw(background);
        for (const Button& button : categoryButtons)
        {
            button.Draw(window);
        }
        modeButton.Draw(window);
        returnButton.Draw(window);

        infoBox.setPosition({ mousePos.x + kInfoBoxOffsetX, mousePos.y });

        if (mouse.JustPressed())
        {
            for (size_t i = 0; i < categoryButtons.size(); ++i)
            {
                if (categoryHovered[i])
                {
                    currentCategory = kCategoryButtons[i].category;
                }
            }

            if (hoverMode)
            {
                returningMode = !returningMode;
                modeButton.SetLabel(returningMode ? "Returning MODE" : "Getting MODE");
                modeButton.UpdateHover(mousePos);
            }

            if (hoverReturn)
            {
                FadeOut(window);
                appState = AppState::Menu;
                return;
            }
        }

        // ---- Products of the selected category ----
        if (currentCategory != ProductCategory::None)
        {
            const std::vector<Product>& products = GetProducts(catalog, currentCategory);
            std::vector<ShelfItem>& shelf = shelves[currentCategory];

            const int pageCount = PageCount(shelf.size());
            const int shownPage = std::min(page, pageCount - 1);
            const int firstIndex = shownPage * kSlotsPerPage;
            const int lastIndex = std::min((int)shelf.size(), firstIndex + kSlotsPerPage);

            // Empty shelf slots, then the product pictures on top of them.
            for (int index = firstIndex; index < lastIndex; ++index)
            {
                window.draw(slots[index - firstIndex]);
            }
            for (int index = firstIndex; index < lastIndex; ++index)
            {
                window.draw(shelf[index].picture);
            }

            // Page button (only when the category has more than one page).
            if (pageCount > 1)
            {
                if (IsHovered(nextPageButton, mousePos))
                {
                    CenterTextInBox(pageButtonHint, infoBox);
                    window.draw(infoBox);
                    window.draw(pageButtonHint);

                    if (mouse.JustPressed())
                    {
                        page = (shownPage + 1) % pageCount;
                    }
                }
                window.draw(shownPage < pageCount - 1 ? nextPageButton : previousPageButton);
            }

            // Hover info and click on a product.
            for (int index = firstIndex; index < lastIndex; ++index)
            {
                if (!IsHovered(slots[index - firstIndex], mousePos))
                {
                    continue;
                }

                CenterTextInBox(shelf[index].info, infoBox);
                window.draw(infoBox);
                window.draw(shelf[index].info);

                if (mouse.JustPressed())
                {
                    if (returningMode)
                    {
                        cart.RemoveItemByName(products[index].GetName());
                    }
                    else
                    {
                        cart.AddItem(products[index], 1);
                    }
                }
            }
        }

        mouse.EndFrame();
        window.display();
    }
}
