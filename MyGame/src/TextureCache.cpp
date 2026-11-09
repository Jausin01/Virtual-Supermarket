#include "TextureCache.h"
#include <iostream>

const sf::Texture& TextureCache::Get(const std::string& path)
{
    auto it = _textures.find(path);
    if (it != _textures.end())
    {
        return *it->second;
    }

    auto texture = std::make_unique<sf::Texture>();
    if (!texture->loadFromFile(path))
    {
        std::cerr << "ERROR::COULD NOT READ FILE " << path << "!!!" << std::endl;
    }

    const sf::Texture& result = *texture;
    _textures[path] = std::move(texture);
    return result;
}
