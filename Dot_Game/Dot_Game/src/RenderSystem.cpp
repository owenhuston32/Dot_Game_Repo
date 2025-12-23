#include "RenderSystem.h"

RenderSystem::RenderSystem(World& world, Renderer& renderer) : _world(world), _renderer(renderer) {

}


void RenderSystem::render() {

    for (auto& [entity, position] : _world.view<Position>()) {
        //auto* sprite = _world.getComponent<Sprite>(entity);
        //if (!sprite) continue;

        // Render entity
        _renderer.drawSprite(position);
    }
}