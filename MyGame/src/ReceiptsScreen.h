#pragma once
#include <SFML/Graphics.hpp>
#include "AppState.h"
#include "TextureCache.h"

/// Lets the player read the saved receipts (up to five). Scroll with the mouse wheel.
/// Sets appState to Start when the player presses "Return".
void RunReceiptsScreen(sf::RenderWindow& window, AppState& appState, const sf::Font& font, TextureCache& textures);
