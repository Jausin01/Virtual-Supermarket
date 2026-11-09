#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include <unordered_map>

/// Loads every image file only once and keeps it alive, so sprites can
/// safely hold a reference to their texture for the whole program.
class TextureCache
{
public:
    /// Returns the texture of a file. If the file cannot be read, an error is
    /// printed once and an empty texture is returned, so the game keeps running.
    const sf::Texture& Get(const std::string& path);

private:
    std::unordered_map<std::string, std::unique_ptr<sf::Texture>> _textures;
};
