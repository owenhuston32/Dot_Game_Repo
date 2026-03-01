#include "Renderer.h"
#include <iostream>
#include <SFML/Graphics.hpp>


Renderer::Renderer(const Position& pos, const Sprite& sprite) 
	: _pos(pos), _sprite(sprite) {

}

const Sprite& Renderer::getSprite() {
	return _sprite;
}

const Position& Renderer::getPosition() {
	return _pos;
}
