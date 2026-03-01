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
    template<typename... Args>
    T& emplace(Entity e, Args&&... args) {
        auto [it, inserted] = components.emplace(
            e,
            T{ std::forward<Args>(args)... }
        );
        return it->second;
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

template<typename... Ts>
class ComponentView {
public:
    explicit ComponentView(std::unordered_map<Entity, Ts>*... stores)
        : stores(stores...) {
    }

    struct Iterator {
        using Primary = std::tuple_element_t<0, std::tuple<Ts...>>;
        using Iter = typename std::unordered_map<Entity, Primary>::iterator;

        Iter it{};
        Iter end{};
        std::tuple<std::unordered_map<Entity, Ts>*...> stores;

        Iterator() = default;

        Iterator(Iter begin, Iter e, decltype(stores) s)
            : it(begin), end(e), stores(s)
        {
            advance();
        }

        void advance() {
            while (it != end && !hasAll(it->first))
                ++it;
        }

        bool hasAll(Entity e) {
            return std::apply(
                [&](auto*... s) {
                    return (... && (s && (s->find(e) != s->end())));
                },
                stores
            );
        }

        Iterator& operator++() {
            ++it;
            advance();
            return *this;
        }

        auto operator*() -> std::tuple<Entity, Ts&...> {
            Entity e = it->first;
            return std::tuple<Entity, Ts&...>(e, get<Ts>(e)...);
        }

        bool operator!=(const Iterator& other) const {
            return it != other.it;
        }

        template<typename T>
        T& get(Entity e) {
            return std::get<std::unordered_map<Entity, T>*>(stores)->at(e);
        }
    };

    Iterator begin() {
        auto* primary = std::get<0>(stores);
        if (!primary)
            return end();

        Iterator it{ primary->begin(), primary->end(), stores };
        it.advance();
        return it;
    }

    Iterator end() {
        auto* primary = std::get<0>(stores);
        if (!primary) return Iterator(); // default-constructed
        return Iterator{ primary->end(), primary->end(), stores };
    }

private:
    std::tuple<std::unordered_map<Entity, Ts>*...> stores;
};

class World {
public:
    Entity createEntity();
    void destroyEntity(Entity entity);

    template<typename T, typename... Args>
    T& addComponent(Entity entity, Args&&... args) {
        return getStore<T>().emplace(
            entity,
            std::forward<Args>(args)...
        );
    }

    template<typename T>
    void removeComponent(Entity entity) {
        if (auto* store = tryGetStore<T>())
            store->remove(entity);
    }

    template<typename T>
    T* getComponent(Entity entity) {
        if (auto* store = tryGetStore<T>())
            return store->get(entity);
        return nullptr;
    }

    template<typename... Ts>
    ComponentView<Ts...> view() {
        return ComponentView<Ts...>(
            (tryGetStore<Ts>() ? &tryGetStore<Ts>()->data() : nullptr)...
        );
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

    template<typename T>
    ComponentStore<T>* tryGetStore() {
        auto it = stores.find(std::type_index(typeid(T)));
        if (it == stores.end())
            return nullptr;
        return static_cast<ComponentStore<T>*>(it->second.get());
    }
    std::unordered_map<
        std::type_index,
        std::unique_ptr<IComponentStore>
    > stores;

    Entity nextEntity{ 0 };

};

