#include "World.h"

Entity World::createEntity() {
    return nextEntity++;
}

void World::destroyEntity(Entity entity) {
    for (auto& [_, store] : stores) {
        store->remove(entity);
    }
}