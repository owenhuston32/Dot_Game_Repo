#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>

struct Position {
    float x;
    float y;
};

struct Transform {
    sf::Transform _transform;

    explicit Transform(const sf::Transform& transform)
        : _transform(transform) {};
};
