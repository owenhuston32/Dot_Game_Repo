#include "SpawnSystem.h"
#include <functional>
#include <RenderComponents.h>
#include <EntityComponents.h>

SpawnSystem::SpawnSystem(World& world, sf::RenderWindow& window) : _world(world), _window(window) {

}


void SpawnSystem::SpawnObjects() {

    std::ifstream file("./resources/json/Spawns.json");
    if (!file.is_open()) {
        std::cerr << "Failed to open Spawns.json\n";
        return;
    }

    json data;
    file >> data;

    std::unordered_map<std::string, Entity> entityByName;

    using ComponentAdder = std::function<void(World&, Entity, const json&)>;

    std::unordered_map<std::string, ComponentAdder> componentRegistry;

    componentRegistry["Transform"] =
        [](World& world, Entity e, const json& data)
        {

            Position pos = {
                data["pos"]["x"].get<float>(),
                data["pos"]["y"].get<float>(),
                data["pos"]["z"].get<float>()
            };
            Rotation rot = { data["rot"].get<float>() };
            Scale scale = { data["scale"].get<float>()};

            world.addComponent<Transform>(e, Transform{pos, rot, scale});
        };

    componentRegistry["Renderer"] =
        [this](World& world, Entity e, const json& data)
        {
            const sf::IntRect textureRect = { 
                {data["rect"]["x"].get<int>(),
                 data["rect"]["y"].get<int>()},
                {data["rect"]["width"].get<int>(),
                data["rect"]["height"].get<int>()}};

            const std::string& texturePath = data["texture"].get<std::string>();
            world.addComponent<Renderer>(e, texturePath, textureRect);
        };

    componentRegistry["PlayerTag"] =
        [](World& world, Entity e, const json&)
        {
            world.addComponent<PlayerTag>(e);
        };

    componentRegistry["EnemyTag"] =
        [](World& world, Entity e, const json&)
        {
            world.addComponent<EnemyTag>(e);
        };



    for (const auto& entityJson : data["entities"])
    {
        Entity e = _world.createEntity();

        std::string id = entityJson["id"];
        entityByName[id] = e;

        for (auto& [componentName, componentData] :
            entityJson["components"].items())
        {

            componentRegistry.at(componentName)(
                _world,
                e,
                componentData
                );
        }
    }
}