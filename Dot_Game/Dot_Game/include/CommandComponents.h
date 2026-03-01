#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>

struct Move {
	std::uint32_t entity;
	float dir;
	float speed;
};