#pragma once
#include <SFML/Graphics.hpp>
#include "AppState.h"
#include "TextureCache.h"

/// Fades the screen to black over `duration` seconds.
void FadeOut(sf::RenderWindow& window, float duration = 1.f);

/// Fades from black to the background of the given screen over `duration` seconds.
void FadeIn(sf::RenderWindow& window, TextureCache& textures, AppState screen, float duration = 1.f);
