#include "RenderSystem.h"

RenderSystem::RenderSystem(World& world, sf::RenderWindow& window) : _world(world), _window(window) {

}


std::shared_ptr<sf::Texture> RenderSystem::loadTexture(const std::string& path) {
    auto it = _textures.find(path);
    if (it != _textures.end()) return it->second;

    auto texture = std::make_shared<sf::Texture>();
    if (!texture->loadFromFile(path))
        throw std::runtime_error("Failed to load texture: " + path);

    _textures[path] = texture;
    return texture;
}

sf::Sprite RenderSystem::createSprite(const Renderer& r)
{
    auto tex = loadTexture(r._texturePath);
    sf::Sprite sprite(*tex);
    sprite.setTextureRect(r._textureRect);
    return sprite;
}

void RenderSystem::render() {

    for (auto [entity, renderer, transform] : _world.view<Renderer, Transform>()) {

        sf::Sprite sprite = createSprite(renderer);
        sprite.setPosition({ transform._pos.x, transform._pos.y });

        _window.draw(sprite);

    }
}