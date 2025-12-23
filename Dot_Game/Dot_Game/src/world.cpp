#include "world.h"

// ----------------------
// Entity Management
// ----------------------

Entity World::createEntity() {
    return nextEntity++;
}

void World::destroyEntity(Entity entity) {
    for (auto& [_, store] : stores) {
        store->remove(entity);
    }
}

// ----------------------
// Component Management
// ----------------------

template<typename T>
void World::addComponent(Entity entity, T component) {
    getStore<T>().add(entity, component);
}

template<typename T>
void World::removeComponent(Entity entity) {
    getStore<T>().remove(entity);
}

template<typename T>
T* World::getComponent(Entity entity) {
    return getStore<T>().get(entity);
}

template<typename T>
ComponentStore<T>& World::getStore() {
    auto type = std::type_index(typeid(T));

    auto it = stores.find(type);
    if (it == stores.end()) {
        auto store = std::make_unique<ComponentStore<T>>();
        auto* ptr = store.get();
        stores[type] = std::move(store);
        return *ptr;
    }

    return *static_cast<ComponentStore<T>*>(it->second.get());
}

// ----------------------
// Template Instantiations (example)
// ----------------------

template ComponentStore<int>& World::getStore<int>();
template void World::addComponent<int>(Entity, int);
template int* World::getComponent<int>(Entity);