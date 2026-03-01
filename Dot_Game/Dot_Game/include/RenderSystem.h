#pragma once
#include "world.h"
#include "RenderComponents.h"
#include "TransformComponents.h"
#include <iostream>
#include <SFML/Graphics.hpp>

class RenderSystem {
public:

    std::shared_ptr<sf::Texture> loadTexture(const std::string& path);
    sf::Sprite createSprite(const Renderer& r);
    void render();

    explicit RenderSystem(World& world, sf::RenderWindow& window);

private:
    World& _world;
    sf::RenderWindow& _window;
    std::unordered_map<std::string, std::shared_ptr<sf::Texture>> _textures;
};