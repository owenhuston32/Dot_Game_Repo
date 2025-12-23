#include <render_system.h>

void RenderSystem::render(World& world) {
    auto& positions = world.getStore<Position>().data();

    for (auto& [entity, position] : positions) {
        auto* sprite = world.getComponent<Sprite>(entity);
        if (!sprite) continue;

        // Render entity
        drawSprite(position, *sprite);
    }
}