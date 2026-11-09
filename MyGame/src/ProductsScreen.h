#pragma once
#include <SFML/Graphics.hpp>
#include "AppState.h"
#include "Cart.h"
#include "ProductCatalog.h"
#include "TextureCache.h"

/// Shelf screen: the player picks a category, then clicks products to add them
/// to the cart (or to remove them, while "Returning MODE" is on).
/// Sets appState to Menu when the player presses "Return".
void RunProductsScreen(sf::RenderWindow& window, AppState& appState, const sf::Font& font,
    TextureCache& textures, const ProductCatalog& catalog, Cart& cart);
