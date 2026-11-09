#include "SupermarketMenuScreen.h"
#include "Transitions.h"
#include "UiHelpers.h"
#include <algorithm>
#include <string>

namespace
{
    constexpr float kCartPopupSeconds = 3.f;
    constexpr int kCartTextBaseSize = 32;
    constexpr int kCartTextMinSize = 20;
    constexpr int kCartTextFullSizeLines = 15; // Beyond this many lines the text shrinks

    /// "Name xQuantity" per line, for the cart popup.
    std::string CartSummary(const Cart& cart)
    {
        std::string summary;
        for (const CartItem& item : cart.GetItems())
        {
            summary += item.product.GetName() + " x" + std::to_string(item.quantity) + "\n";
        }
        return summary;
    }

    int CartTextSize(int lineCount)
    {
        int size = kCartTextBaseSize - std::max(0, lineCount - kCartTextFullSizeLines);
        return std::max(size, kCartTextMinSize);
    }
}

void RunSupermarketMenuScreen(sf::RenderWindow& window, AppState& appState, const sf::Font& font,
    TextureCache& textures, const Cart& cart)
{
    FadeIn(window, textures, AppState::Menu);

    sf::Sprite background = MakeSprite(textures, "Images/Menu.png");
    sf::Sprite cartCircle = MakeSprite(textures, "Images/ReceiptCircle.png", { -25.f, 0.f }, { 0.8f, 0.8f });
    sf::Sprite cartIcon = MakeSprite(textures, "Images/Kart.png", { 40.f, 100.f }, { 0.5f, 0.5f });
    sf::Sprite cartTip = MakeSprite(textures, "Images/Dialogue Box(Upside Down).png", { 50.f, 250.f }, { 0.8f, 0.8f });
    sf::Text cartTipText = MakeText(font, "See Cart?", sf::Color::Black);
    CenterTextInBox(cartTipText, cartTip);

    const sf::Texture& optionTexture = textures.Get("Images/InsideStoreOption.png");
    const sf::Vector2f optionScale(2.f, 2.f);
    Button searchButton(optionTexture, font, "Search Products?", { 1500.f, 100.f }, optionScale);
    Button payButton(optionTexture, font, "Pay Products?", { 1500.f, 250.f }, optionScale);
    Button leaveButton(optionTexture, font, "Leave SuperMarket?", { 1500.f, 400.f }, optionScale);

    sf::Sprite cartPopup = MakeSprite(textures, "Images/ItemList.png", { 500.f, 20.f }, { 2.f, 2.35f });
    sf::Text cartPopupText = MakeText(font, "", sf::Color::Black);
    sf::Clock cartPopupClock;
    bool showCartPopup = false;

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

        bool hoverSearch = searchButton.UpdateHover(mousePos);
        bool hoverPay = payButton.UpdateHover(mousePos);
        bool hoverLeave = leaveButton.UpdateHover(mousePos);
        bool hoverCart = IsMouseOverCircle(cartCircle, mousePos);

        window.clear();
        window.draw(background);
        searchButton.Draw(window);
        payButton.Draw(window);
        leaveButton.Draw(window);
        window.draw(cartCircle);
        window.draw(cartIcon);

        if (hoverCart)
        {
            window.draw(cartTip);
            window.draw(cartTipText);

            if (mouse.JustPressed())
            {
                cartPopupText.setString(CartSummary(cart));
                cartPopupText.setCharacterSize((unsigned)CartTextSize(cart.GetItemCount()));
                CenterTextInBox(cartPopupText, cartPopup);
                cartPopupClock.restart();
                showCartPopup = true;
            }
        }

        if (mouse.JustPressed())
        {
            if (hoverLeave)
            {
                FadeOut(window);
                appState = AppState::Start;
                return;
            }
            if (hoverSearch)
            {
                FadeOut(window);
                appState = AppState::Products;
                return;
            }
            if (hoverPay && !cart.IsEmpty())
            {
                FadeOut(window);
                appState = AppState::Cashier;
                return;
            }
        }

        if (showCartPopup)
        {
            if (cartPopupClock.getElapsedTime().asSeconds() < kCartPopupSeconds)
            {
                window.draw(cartPopup);
                window.draw(cartPopupText);
            }
            else
            {
                showCartPopup = false;
            }
        }

        mouse.EndFrame();
        window.display();
    }
}
