#include "CashierScreen.h"
#include "Money.h"
#include "Payment.h"
#include "ReceiptStore.h"
#include "TalkingMoment.h"
#include "Transitions.h"
#include "UiHelpers.h"
#include <cctype>
#include <string>

namespace
{
    constexpr size_t kMaxPaymentDigits = 6;
    constexpr size_t kMaxNameLength = 20;
    constexpr size_t kMaxPhoneLength = 11;
    constexpr size_t kMaxEmailLength = 50;

    constexpr char kBackspace = 8;
    constexpr char kDelete = 127;

    bool IsLetter(char c) { return std::isalpha((unsigned char)c) != 0; }
    bool IsDigit(char c) { return std::isdigit((unsigned char)c) != 0; }
    bool IsPrintable(char c) { return c >= 32 && c <= 126; }

    /// A line of text the player types: only accepted characters, limited length, backspace to erase.
    struct TextField
    {
        bool (*accepts)(char);
        size_t maxLength;
        std::string value;

        void HandleTyped(char c)
        {
            if (c == kBackspace || c == kDelete)
            {
                if (!value.empty())
                {
                    value.pop_back();
                }
            }
            else if (accepts(c) && value.size() < maxLength)
            {
                value += c;
            }
        }
    };

    /// The typed character, or 0 when it is not plain ASCII.
    char TypedAscii(const sf::Event::TextEntered& event)
    {
        return event.unicode < 128 ? (char)event.unicode : 0;
    }
}

void RunCashierScreen(sf::RenderWindow& window, AppState& appState, const sf::Font& font,
    TextureCache& textures, Cart& cart)
{
    FadeIn(window, textures, AppState::Cashier);

    float total = cart.GetTotal();
    bool isMember = false;
    bool receiptSaved = false;
    TalkingMoment moment = TalkingMoment::Introduction;

    TextField payment{ IsDigit, kMaxPaymentDigits, "" };
    TextField name{ IsLetter, kMaxNameLength, "" };
    TextField phone{ IsDigit, kMaxPhoneLength, "" };
    TextField email{ IsPrintable, kMaxEmailLength, "" };
    std::string paymentMessage; // What the cashier says about the amount typed so far

    // ---- Scenery and dialogue ----
    sf::Sprite background = MakeSprite(textures, "Images/Cashier.png");
    sf::Sprite dialogueBox = MakeSprite(textures, "Images/Dialogue Box.png", { 600.f, -75.f }, { 1.5f, 1.5f });
    sf::Text dialogue = MakeText(font, "", sf::Color::Black);
    auto say = [&](const std::string& line)
    {
        dialogue.setString(line);
        CenterTextInBox(dialogue, dialogueBox);
    };
    say("Welcome!\nYour Total is: " + FormatMoney(total) + "\nDo you have membership Card?");

    sf::Sprite yesSprite = MakeSprite(textures, "Images/Yes.png", { 700.f, 620.f }, { 0.3f, 0.3f });
    sf::Sprite noSprite = MakeSprite(textures, "Images/No.png", { 550.f, 620.f }, { 0.3f, 0.3f });

    // ---- Input box shared by all typing steps ----
    sf::RectangleShape inputBox(sf::Vector2f(400.f, 50.f));
    inputBox.setFillColor(sf::Color(255, 255, 255, 220));
    inputBox.setPosition({ 700.f, 600.f });

    sf::Text paymentText = MakeText(font, "", sf::Color::Black);
    paymentText.setCharacterSize(30);
    paymentText.setPosition(inputBox.getPosition());

    sf::Text fieldText = MakeText(font, "", sf::Color::Black);
    fieldText.setCharacterSize(28);
    fieldText.setPosition(inputBox.getPosition() + sf::Vector2f(10.f, 10.f));

    Button exitButton(textures.Get("Images/MenuOptions.png"), font, "Exit", { 842.f, 800.f }, { 0.5f, 0.5f });

    auto currentField = [&]() -> TextField*
    {
        switch (moment)
        {
        case TalkingMoment::PayMoment:       return &payment;
        case TalkingMoment::GetName:         return &name;
        case TalkingMoment::GetPhoneNumber:  return &phone;
        case TalkingMoment::GetEmail:        return &email;
        default:                             return nullptr;
        }
    };

    /// Shows the cashier's reaction to the amount typed so far.
    auto refreshPaymentMessage = [&]()
    {
        if (payment.value.empty())
        {
            paymentMessage.clear();
            return;
        }
        paymentMessage = DescribePayment(CheckPayment(total, std::stof(payment.value)));
    };

    auto sayPaymentPrompt = [&]()
    {
        if (!paymentMessage.empty())
        {
            say(paymentMessage);
        }
        else if (isMember)
        {
            say("Great!\nYour new total is: " + FormatMoney(total));
        }
        else
        {
            say("Your total is: " + FormatMoney(total) + "\nHow much will you pay?");
        }
    };

    MouseInput mouse;

    while (window.isOpen())
    {
        // ---- Typing ----
        while (auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }

            TextField* field = currentField();
            if (field == nullptr)
            {
                continue;
            }

            if (const auto* typed = event->getIf<sf::Event::TextEntered>())
            {
                field->HandleTyped(TypedAscii(*typed));
                if (moment == TalkingMoment::PayMoment)
                {
                    refreshPaymentMessage();
                }
            }

            const auto* key = event->getIf<sf::Event::KeyPressed>();
            if (key == nullptr || key->code != sf::Keyboard::Key::Enter || field->value.empty())
            {
                continue;
            }

            // Enter confirms the current step.
            switch (moment)
            {
            case TalkingMoment::PayMoment:
            {
                PaymentCheck check = CheckPayment(total, std::stof(payment.value));
                if (check.enough)
                {
                    moment = isMember ? TalkingMoment::GetName : TalkingMoment::GetReceipt;
                }
                else
                {
                    paymentMessage = DescribePayment(check);
                    payment.value.clear();
                }
                break;
            }
            case TalkingMoment::GetName:        moment = TalkingMoment::GetPhoneNumber; break;
            case TalkingMoment::GetPhoneNumber: moment = TalkingMoment::GetEmail; break;
            case TalkingMoment::GetEmail:       moment = TalkingMoment::GetReceipt; break;
            default: break;
            }

            switch (moment)
            {
            case TalkingMoment::PayMoment:       sayPaymentPrompt(); break;
            case TalkingMoment::GetName:         say("Could you tell me your name?"); break;
            case TalkingMoment::GetPhoneNumber:  say("Could you tell me \nyour Phone Number?"); break;
            case TalkingMoment::GetEmail:        say("Could you tell me your Email?"); break;
            case TalkingMoment::GetReceipt:      say("Here's your receipt!\nYou can see it outside the store!\nHave a nice day!"); break;
            default: break;
            }
        }

        mouse.Update(window);
        const sf::Vector2f mousePos = mouse.Position();

        // ---- Per-step logic that depends on the mouse ----
        if (moment == TalkingMoment::Introduction && mouse.JustReleased())
        {
            const bool clickedYes = IsHovered(yesSprite, mousePos);
            const bool clickedNo = IsHovered(noSprite, mousePos);
            if (clickedYes || clickedNo)
            {
                isMember = clickedYes;
                if (isMember)
                {
                    total = ApplyMemberDiscount(total);
                }
                moment = TalkingMoment::PayMoment;
                sayPaymentPrompt();
            }
        }

        if (moment == TalkingMoment::PayMoment)
        {
            // Keeps the prompt/result line in sync with what was typed.
            sayPaymentPrompt();
        }

        if (moment == TalkingMoment::GetReceipt && !receiptSaved)
        {
            Receipt receipt;
            receipt.customerName = isMember ? name.value : "Guest";
            receipt.phoneNumber = isMember ? phone.value : "None";
            receipt.email = isMember ? email.value : "None";
            receipt.totalPaid = total;
            for (const CartItem& item : cart.GetItems())
            {
                receipt.items.emplace_back(item.product.GetName(), item.quantity);
            }
            ReceiptStore::Save(receipt);
            receiptSaved = true;
        }

        // ---- Drawing ----
        window.clear();
        window.draw(background);
        window.draw(dialogueBox);
        window.draw(dialogue);

        switch (moment)
        {
        case TalkingMoment::Introduction:
            window.draw(yesSprite);
            window.draw(noSprite);
            break;

        case TalkingMoment::PayMoment:
            paymentText.setString("$" + payment.value);
            window.draw(inputBox);
            window.draw(paymentText);
            break;

        case TalkingMoment::GetName:
            fieldText.setString("Name: " + name.value);
            window.draw(inputBox);
            window.draw(fieldText);
            break;

        case TalkingMoment::GetPhoneNumber:
            fieldText.setString("Phone: " + phone.value);
            window.draw(inputBox);
            window.draw(fieldText);
            break;

        case TalkingMoment::GetEmail:
            fieldText.setString("Email: " + email.value);
            window.draw(inputBox);
            window.draw(fieldText);
            break;

        case TalkingMoment::GetReceipt:
        {
            bool hoverExit = exitButton.UpdateHover(mousePos);
            exitButton.Draw(window);

            if (hoverExit && mouse.JustPressed())
            {
                cart.Clear();
                FadeOut(window);
                appState = AppState::Start;
                return;
            }
            break;
        }
        }

        mouse.EndFrame();
        window.display();
    }
}
