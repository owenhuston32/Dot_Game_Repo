#pragma once
#include "world.h"
#include "TransformComponents.h"
#include "GameStateManager.h"
#include <SFML/Graphics.hpp>
#include <json.hpp>
#include <iostream>
#include <fstream>
#include <string>

using json = nlohmann::json;

class InputManager {

private:
    World& _world;
    sf::RenderWindow& _window;
    json _bindings;

    enum class Action
    {
        MoveLeft,
        MoveRight,
        Jump,
        Escape,
        Unknown
    };

    struct JoystickAxisBinding
    {
        sf::Joystick::Axis axis;
        float direction; // -1 or +1
        Action action;
    };

    std::unordered_map<sf::Keyboard::Key, Action> _keyToAction;
    std::unordered_map<unsigned int, Action> _controllerButtonToAction;
    std::vector<JoystickAxisBinding> _controllerAxes;


public:

    explicit InputManager(World& world, sf::RenderWindow& window);

    void loadBindings();

    void overrideBindings();

    std::string actionToString(InputManager::Action action);

    InputManager::Action stringToAction(const std::string& str);

    sf::Keyboard::Key stringToKey(const std::string& str);

    void onActionPressed(InputManager::Action action, GameState& gameState);

    void onActionReleased(InputManager::Action action, GameState& gameState);

    void onActionHeld(InputManager::Action action, GameState& gameState);

    void handleInput(const sf::Event& event, GameState& gameState);

    void update(GameState& gameState);
};