#pragma once
#include <unordered_map>
#include <typeindex>
#include <memory>
#include <cstdint>

using Entity = std::uint32_t;

// ----------------------
// Base Component Store
// ----------------------

class IComponentStore {
public:
    virtual ~IComponentStore() = default;
    virtual void remove(Entity entity) = 0;
};

// ----------------------
// Templated Component Store
// ----------------------

template<typename T>
class ComponentStore : public IComponentStore {
public:
    void add(Entity entity, T component) {
        components[entity] = component;
    }

    void remove(Entity entity) override {
        components.erase(entity);
    }

    T* get(Entity entity) {
        auto it = components.find(entity);
        return (it != components.end()) ? &it->second : nullptr;
    }

    const std::unordered_map<Entity, T>& data() const {
        return components;
    }

private:
    std::unordered_map<Entity, T> components;
};

// ----------------------
// World
// ----------------------

class World {
public:
    Entity createEntity();
    void destroyEntity(Entity entity);

    template<typename T>
    void addComponent(Entity entity, T component);

    template<typename T>
    void removeComponent(Entity entity);

    template<typename T>
    T* getComponent(Entity entity);

    template<typename T>
    ComponentStore<T>& getStore();

private:
    Entity nextEntity = 1;
    std::unordered_map<std::type_index, std::unique_ptr<IComponentStore>> stores;
};