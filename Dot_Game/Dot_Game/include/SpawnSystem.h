#pragma once
#include "world.h"
#include "TransformComponents.h"
#include <SFML/Graphics.hpp>
#include <json.hpp>
#include <iostream>
#include <fstream>
#include <string>

using json = nlohmann::json;

class SpawnSystem {
public:
    void SpawnObjects();

    explicit SpawnSystem(World& world, sf::RenderWindow& window);

private:
    World& _world;
    sf::RenderWindow& _window;
};