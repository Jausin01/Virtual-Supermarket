#pragma once
#include <SFML/Graphics.hpp>
#include "AppState.h"
#include "Cart.h"
#include "TextureCache.h"

/// Inside the store: choose to browse products, pay, or leave. Hovering the cart
/// icon and clicking it shows the cart contents for a few seconds.
/// Sets appState to Products, Cashier or Start.
void RunSupermarketMenuScreen(sf::RenderWindow& window, AppState& appState, const sf::Font& font,
    TextureCache& textures, const Cart& cart);
