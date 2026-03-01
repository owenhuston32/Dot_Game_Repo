#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>

struct Sprite {
    std::string textureFilePath;
};

struct Renderer {
    const sf::IntRect _textureRect;
    const std::string _texturePath;

    explicit Renderer(const std::string& texturePath, const sf::IntRect& textureRect)
        : _texturePath(texturePath), _textureRect(textureRect) {
    };
};