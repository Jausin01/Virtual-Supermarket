#pragma once
#include <SFML/Graphics.hpp>
#include "AppState.h"
#include "TextureCache.h"

/// Store front. "Enter" goes to the store menu (Menu), the receipt icon to the
/// saved receipts (Receipts) and "Exit" closes the window.
/// `fadeIn` is false only for the very first screen of the program.
void RunStartScreen(sf::RenderWindow& window, AppState& appState, const sf::Font& font,
    TextureCache& textures, bool fadeIn);
