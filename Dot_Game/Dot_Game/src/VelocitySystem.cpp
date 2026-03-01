#include "VelocitySystem.h"

VelocitySystem::VelocitySystem(World& world, sf::RenderWindow& window) : _world(world), _window(window) {

}


void VelocitySystem::update() {

    for (auto [entity, transform] : _world.view<Transform>()) {

        //transform._pos.x = transform._pos.x + 1;

    }
}