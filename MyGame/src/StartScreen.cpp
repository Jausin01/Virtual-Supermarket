#include "StartScreen.h"
#include "Transitions.h"
#include "UiHelpers.h"

void RunStartScreen(sf::RenderWindow& window, AppState& appState, const sf::Font& font,
    TextureCache& textures, bool fadeIn)
{
    if (fadeIn)
    {
        FadeIn(window, textures, AppState::Start);
    }

    sf::Sprite background = MakeSprite(textures, "Images/StoreFront.png");
    sf::Sprite receiptCircle = MakeSprite(textures, "Images/ReceiptCircle.png", { 0.f, 50.f }, { 0.5f, 0.5f });
    sf::Sprite receiptIcon = MakeSprite(textures, "Images/Receipt.png", { 60.f, 75.f }, { 0.3f, 0.3f });
    sf::Sprite receiptTip = MakeSprite(textures, "Images/Dialogue Box(Upside Down).png", { 40.f, 200.f }, { 0.8f, 0.8f });
    sf::Text receiptTipText = MakeText(font, "See Receipts?", sf::Color::Black);
    CenterTextInBox(receiptTipText, receiptTip);

    const sf::Texture& buttonTexture = textures.Get("Images/MenuOptions.png");
    Button enterButton(buttonTexture, font, "Enter", { 842.f, 650.f }, { 0.5f, 0.5f });
    Button exitButton(buttonTexture, font, "Exit", { 842.f, 800.f }, { 0.5f, 0.5f });

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

        bool hoverEnter = enterButton.UpdateHover(mousePos);
        bool hoverExit = exitButton.UpdateHover(mousePos);
        bool hoverReceipts = IsMouseOverCircle(receiptCircle, mousePos);

        window.clear();
        window.draw(background);
        window.draw(receiptCircle);
        window.draw(receiptIcon);
        enterButton.Draw(window);
        exitButton.Draw(window);

        if (hoverReceipts)
        {
            window.draw(receiptTip);
            window.draw(receiptTipText);
        }

        if (mouse.JustPressed())
        {
            if (hoverReceipts)
            {
                FadeOut(window);
                appState = AppState::Receipts;
                return;
            }
            if (hoverExit)
            {
                window.close();
            }
            if (hoverEnter)
            {
                FadeOut(window);
                appState = AppState::Menu;
                return;
            }
        }

        mouse.EndFrame();
        window.display();
    }
}
