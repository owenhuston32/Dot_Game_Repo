#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>

struct Position {
    float x;
    float y;
    float z;
};

struct Rotation {
    float rotation;
};

struct Scale {
    float scale;
};

struct Transform {
    Position _pos;
    Rotation _rot;
    Scale _scale;


    explicit Transform(const Position& pos, const Rotation& rot, const Scale& scale)
        : _pos(pos), _rot(rot), _scale(scale) {};
};