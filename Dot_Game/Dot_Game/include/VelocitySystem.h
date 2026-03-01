#pragma once
#include "world.h"
#include "TransformComponents.h"
#include <SFML/Graphics.hpp>

class VelocitySystem {
public:
    void update();

    explicit VelocitySystem(World& world, sf::RenderWindow& window);

private:
    World& _world;
    sf::RenderWindow& _window;
};