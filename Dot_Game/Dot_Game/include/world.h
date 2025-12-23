#pragma once

#include <unordered_map>
#include <typeindex>
#include <memory>

using Entity = std::uint32_t;

class IComponentStore {
public:
    virtual ~IComponentStore() = default;
    virtual void remove(Entity) = 0;
};

template<typename T>
class ComponentStore : public IComponentStore {
public:
    void add(Entity e, const T& component) {
        components[e] = component;
    }

    void remove(Entity e) override {
        components.erase(e);
    }

    T* get(Entity e) {
        auto it = components.find(e);
        return it != components.end() ? &it->second : nullptr;
    }

    auto& data() { return components; }
    const auto& data() const { return components; }

private:
    std::unordered_map<Entity, T> components;
};

template<typename T>
class ComponentView {
public:
    ComponentView(std::unordered_map<Entity, T>& data) : data(data) {}

    auto begin() { return data.begin(); }
    auto end() { return data.end(); }

private:
    std::unordered_map<Entity, T>& data;
};

class World {
public:
    Entity createEntity();
    void destroyEntity(Entity entity);

    template<typename T>
    void addComponent(Entity entity, const T& component) {
        getStore<T>().add(entity, component);
    }

    template<typename T>
    void removeComponent(Entity entity) {
        getStore<T>().remove(entity);
    }

    template<typename T>
    T* getComponent(Entity entity) {
        return getStore<T>().get(entity);
    }

    template<typename T>
    ComponentView<T> view() {
        return ComponentView<T>(getStore<T>().data());
    }

private:
    template<typename T>
    ComponentStore<T>& getStore() {
        auto type = std::type_index(typeid(T));

        auto [it, inserted] = stores.try_emplace(
            type,
            std::make_unique<ComponentStore<T>>()
        );

        return *static_cast<ComponentStore<T>*>(it->second.get());
    }

    std::unordered_map<
        std::type_index,
        std::unique_ptr<IComponentStore>
    > stores;

    Entity nextEntity{ 0 };
};