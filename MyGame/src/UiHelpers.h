#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "TextureCache.h"

/// Loads a font file. Prints an error and returns false when it cannot be read.
bool LoadFont(sf::Font& font, const std::string& path);

/// Centers a text in the middle of a sprite.
void CenterTextInBox(sf::Text& text, const sf::Sprite& box);

/// True when the mouse position is inside the sprite.
bool IsHovered(const sf::Sprite& sprite, sf::Vector2f mousePos);

/// True when the mouse is inside the circle that fits the sprite (width = diameter).
bool IsMouseOverCircle(const sf::Sprite& sprite, sf::Vector2f mousePos);

/// Creates a text with a font, string and color (default character size).
sf::Text MakeText(const sf::Font& font, const std::string& string, sf::Color color = sf::Color::White);

/// Creates a sprite from a cached texture, already positioned and scaled.
sf::Sprite MakeSprite(TextureCache& textures, const std::string& path,
    sf::Vector2f position = { 0.f, 0.f }, sf::Vector2f scale = { 1.f, 1.f });

/// Tracks the left mouse button once per frame so screens can ask
/// "was it just pressed?" instead of keeping their own flags.
class MouseInput
{
public:
    /// Call once at the start of every frame.
    void Update(const sf::RenderWindow& window);

    /// Call once at the end of every frame.
    void EndFrame() { _wasDown = _isDown; }

    sf::Vector2f Position() const { return _position; }
    bool IsDown() const { return _isDown; }
    bool JustPressed() const { return _isDown && !_wasDown; }
    bool JustReleased() const { return !_isDown && _wasDown; }

private:
    sf::Vector2f _position;
    bool _isDown = false;
    bool _wasDown = false;
};

/// A sprite with a centered text label. The label turns red while hovered.
class Button
{
public:
    Button(const sf::Texture& texture, const sf::Font& font, const std::string& label,
        sf::Vector2f position, sf::Vector2f scale);

    /// Updates the hover color and returns whether the mouse is over the button.
    bool UpdateHover(sf::Vector2f mousePos);

    /// Greyed-out look for disabled buttons / normal look otherwise.
    void SetEnabledLook(bool enabled);

    void SetLabel(const std::string& label);

    void Draw(sf::RenderTarget& target) const;

    const sf::Sprite& GetSprite() const { return _sprite; }

private:
    sf::Sprite _sprite;
    sf::Text _label;
};
