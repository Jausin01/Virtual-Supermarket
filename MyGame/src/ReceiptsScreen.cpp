#include "ReceiptsScreen.h"
#include "ReceiptStore.h"
#include "Transitions.h"
#include "UiHelpers.h"
#include <algorithm>
#include <vector>

namespace
{
    constexpr float kWindowWidth = 1920.f;
    constexpr float kWindowHeight = 1080.f;

    constexpr float kScrollSpeed = 20.f;
    constexpr float kTextMarginX = 50.f;
    constexpr float kTextMarginY = 30.f;
    constexpr unsigned kTextSize = 24;

    constexpr float kSlotButtonY = 50.f;
    constexpr float kSlotButtonFirstX = 100.f;
    constexpr float kSlotButtonSpacing = 344.f;
    constexpr float kButtonScale = 0.5f;

    /// A view that only shows `area`, so scrolled text never leaves the receipt paper.
    sf::View ClipView(const sf::FloatRect& area)
    {
        sf::View view(area);
        view.setViewport(sf::FloatRect({ area.position.x / kWindowWidth, area.position.y / kWindowHeight },
                                       { area.size.x / kWindowWidth, area.size.y / kWindowHeight }));
        return view;
    }
}

void RunReceiptsScreen(sf::RenderWindow& window, AppState& appState, const sf::Font& font, TextureCache& textures)
{
    FadeIn(window, textures, AppState::Receipts);

    sf::Sprite background = MakeSprite(textures, "Images/ReceiptBackground.png");
    sf::Sprite paper = MakeSprite(textures, "Images/ReceiptContent.png", { 650.f, 150.f }, { 1.5f, 1.5f });
    const sf::Texture& buttonTexture = textures.Get("Images/MenuOptions.png");

    Button returnButton(buttonTexture, font, "Return", { 1500.f, 950.f }, { kButtonScale, kButtonScale });

    std::vector<Button> slotButtons;
    bool slotHasReceipt[ReceiptStore::kSlotCount];
    for (int slot = 0; slot < ReceiptStore::kSlotCount; ++slot)
    {
        slotButtons.emplace_back(buttonTexture, font, "Receipt " + std::to_string(slot + 1),
            sf::Vector2f(kSlotButtonFirstX + kSlotButtonSpacing * (float)slot, kSlotButtonY),
            sf::Vector2f(kButtonScale, kButtonScale));
        slotHasReceipt[slot] = ReceiptStore::SlotHasContent(slot);
    }

    sf::Text receiptText = MakeText(font, "", sf::Color::Black);
    receiptText.setCharacterSize(kTextSize);
    bool receiptSelected = false;

    const sf::FloatRect paperBounds = paper.getGlobalBounds();
    float scrollOffset = 0.f;
    float maxScroll = 0.f;

    MouseInput mouse;

    while (window.isOpen())
    {
        while (auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }

            if (const auto* scroll = event->getIf<sf::Event::MouseWheelScrolled>())
            {
                if (scroll->wheel == sf::Mouse::Wheel::Vertical)
                {
                    scrollOffset = std::clamp(scrollOffset - scroll->delta * kScrollSpeed, 0.f, maxScroll);
                }
            }
        }

        mouse.Update(window);
        const sf::Vector2f mousePos = mouse.Position();

        bool hoverReturn = returnButton.UpdateHover(mousePos);
        std::vector<bool> hoverSlot;
        for (int slot = 0; slot < ReceiptStore::kSlotCount; ++slot)
        {
            hoverSlot.push_back(slotButtons[slot].UpdateHover(mousePos));
            slotButtons[slot].SetEnabledLook(slotHasReceipt[slot]); // Slots without a receipt look greyed out
        }

        window.clear();
        window.draw(background);
        for (const Button& button : slotButtons)
        {
            button.Draw(window);
        }
        window.draw(paper);
        returnButton.Draw(window);

        if (mouse.JustPressed())
        {
            for (int slot = 0; slot < ReceiptStore::kSlotCount; ++slot)
            {
                if (hoverSlot[slot] && slotHasReceipt[slot])
                {
                    receiptText.setString(ReceiptStore::ReadSlot(slot));
                    receiptSelected = true;
                    scrollOffset = 0.f;
                }
            }

            if (hoverReturn)
            {
                FadeOut(window);
                appState = AppState::Start;
                return;
            }
        }

        if (receiptSelected)
        {
            // The scroll range is how much taller the text is than the paper.
            const sf::FloatRect textBounds = receiptText.getLocalBounds();
            const float textHeight = textBounds.position.y + textBounds.size.y;
            const float visibleHeight = paperBounds.size.y - 2.f * kTextMarginY;
            maxScroll = std::max(0.f, textHeight - visibleHeight);
            scrollOffset = std::min(scrollOffset, maxScroll);

            receiptText.setPosition({ paperBounds.position.x + kTextMarginX,
                                      paperBounds.position.y + kTextMarginY - scrollOffset });

            window.setView(ClipView(paperBounds));
            window.draw(receiptText);
            window.setView(window.getDefaultView());
        }

        mouse.EndFrame();
        window.display();
    }
}
