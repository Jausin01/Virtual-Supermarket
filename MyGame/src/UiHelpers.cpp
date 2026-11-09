#include "UiHelpers.h"
#include <cmath>
#include <iostream>

bool LoadFont(sf::Font& font, const std::string& path)
{
    if (!font.openFromFile(path))
    {
        std::cerr << "ERROR::COULD NOT READ FILE " << path << "!!!" << std::endl;
        return false;
    }
    return true;
}

void CenterTextInBox(sf::Text& text, const sf::Sprite& box)
{
    sf::FloatRect textBounds = text.getLocalBounds();
    text.setOrigin({ textBounds.position.x + textBounds.size.x / 2.f,
                     textBounds.position.y + textBounds.size.y / 2.f });

    sf::FloatRect boxBounds = box.getGlobalBounds();
    text.setPosition({ boxBounds.position.x + boxBounds.size.x / 2.f,
                       boxBounds.position.y + boxBounds.size.y / 2.f });
}

bool IsHovered(const sf::Sprite& sprite, sf::Vector2f mousePos)
{
    return sprite.getGlobalBounds().contains(mousePos);
}

bool IsMouseOverCircle(const sf::Sprite& sprite, sf::Vector2f mousePos)
{
    sf::FloatRect bounds = sprite.getGlobalBounds();

    sf::Vector2f center = { bounds.position.x + bounds.size.x / 2.f,
                            bounds.position.y + bounds.size.y / 2.f };
    float radius = bounds.size.x / 2.f;

    float dx = mousePos.x - center.x;
    float dy = mousePos.y - center.y;
    return std::sqrt(dx * dx + dy * dy) <= radius;
}

sf::Text MakeText(const sf::Font& font, const std::string& string, sf::Color color)
{
    sf::Text text(font);
    text.setString(string);
    text.setFillColor(color);
    return text;
}

sf::Sprite MakeSprite(TextureCache& textures, const std::string& path,
    sf::Vector2f position, sf::Vector2f scale)
{
    sf::Sprite sprite(textures.Get(path));
    sprite.setPosition(position);
    sprite.setScale(scale);
    return sprite;
}

void MouseInput::Update(const sf::RenderWindow& window)
{
    sf::Vector2i mouse = sf::Mouse::getPosition(window);
    _position = { (float)mouse.x, (float)mouse.y };
    _isDown = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
}

Button::Button(const sf::Texture& texture, const sf::Font& font, const std::string& label,
    sf::Vector2f position, sf::Vector2f scale)
    : _sprite(texture), _label(font)
{
    _sprite.setPosition(position);
    _sprite.setScale(scale);
    SetLabel(label);
}

bool Button::UpdateHover(sf::Vector2f mousePos)
{
    bool hovered = IsHovered(_sprite, mousePos);
    _label.setFillColor(hovered ? sf::Color::Red : sf::Color::White);
    return hovered;
}

void Button::SetEnabledLook(bool enabled)
{
    if (enabled)
    {
        _sprite.setColor(sf::Color::White);
        _label.setFillColor(sf::Color::Black);
    }
    else
    {
        _sprite.setColor(sf::Color(150, 150, 150, 200));
        _label.setFillColor(sf::Color(150, 150, 150));
    }
}

void Button::SetLabel(const std::string& label)
{
    _label.setString(label);
    CenterTextInBox(_label, _sprite);
}

void Button::Draw(sf::RenderTarget& target) const
{
    target.draw(_sprite);
    target.draw(_label);
}
