#include "Transitions.h"
#include <optional>

namespace
{
    /// Background image shown behind the fade-in of each screen.
    const char* BackgroundFor(AppState screen)
    {
        switch (screen)
        {
        case AppState::Start:    return "Images/StoreFront.png";
        case AppState::Menu:     return "Images/Menu.png";
        case AppState::Products: return "Images/InsideSupermarket.png";
        case AppState::Receipts: return "Images/ReceiptBackground.png";
        case AppState::Cashier:  return "Images/Cashier.png";
        default:                 return nullptr;
        }
    }
}

void FadeOut(sf::RenderWindow& window, float duration)
{
    sf::RectangleShape fadeRect(sf::Vector2f(window.getSize()));
    sf::Clock clock;
    float alpha = 0.f;

    while (alpha < 255.f)
    {
        alpha += 255.f * clock.restart().asSeconds() / duration;
        if (alpha > 255.f)
        {
            alpha = 255.f;
        }

        // The last frame is drawn again, so the black layers build up over it.
        fadeRect.setFillColor(sf::Color(0, 0, 0, (unsigned char)alpha));
        window.draw(fadeRect);
        window.display();
    }
}

void FadeIn(sf::RenderWindow& window, TextureCache& textures, AppState screen, float duration)
{
    const char* backgroundPath = BackgroundFor(screen);
    std::optional<sf::Sprite> background;
    if (backgroundPath != nullptr)
    {
        background.emplace(textures.Get(backgroundPath));
    }

    sf::RectangleShape fadeRect(sf::Vector2f(window.getSize()));
    sf::Clock clock;
    float alpha = 255.f;

    while (alpha > 0.f)
    {
        alpha -= 255.f * clock.restart().asSeconds() / duration;
        if (alpha < 0.f)
        {
            alpha = 0.f;
        }

        window.clear();
        if (background)
        {
            window.draw(*background);
        }

        fadeRect.setFillColor(sf::Color(0, 0, 0, (unsigned char)alpha));
        window.draw(fadeRect);
        window.display();
    }
}
