#pragma once
#include <SFML/Graphics.hpp>
#include "AppState.h"
#include "Cart.h"
#include "TextureCache.h"

/// Checkout: the cashier asks about the membership card (10% off), takes the
/// payment, collects the member's details and saves the receipt.
/// Empties the cart and sets appState to Start when the player leaves.
void RunCashierScreen(sf::RenderWindow& window, AppState& appState, const sf::Font& font,
    TextureCache& textures, Cart& cart);
