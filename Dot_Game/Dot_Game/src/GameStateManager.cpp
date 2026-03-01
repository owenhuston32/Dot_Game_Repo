#include "InputManager.h"

InputManager::InputManager(World& world, sf::RenderWindow& window) : _world(world), _window(window) {
    loadBindings();
}



std::string InputManager::actionToString(InputManager::Action action)
{
    switch (action)
    {
    case Action::MoveLeft:  return "MoveLeft";
    case Action::MoveRight: return "MoveRight";
    case Action::Jump:      return "Jump";
    }
    return "Unknown";
}

InputManager::Action InputManager::stringToAction(const std::string& str)
{
    if (str == "MoveLeft")  return Action::MoveLeft;
    if (str == "MoveRight") return Action::MoveRight;
    if (str == "Jump")      return Action::Jump;

    return Action::Unknown;
}


sf::Keyboard::Key InputManager::stringToKey(const std::string& str)
{
    // First, handle named keys
    if (str == "Space")       return sf::Keyboard::Key::Space;
    if (str == "Enter")       return sf::Keyboard::Key::Enter;
    if (str == "Left")        return sf::Keyboard::Key::Left;
    if (str == "Right")       return sf::Keyboard::Key::Right;
    if (str == "Up")          return sf::Keyboard::Key::Up;
    if (str == "Down")        return sf::Keyboard::Key::Down;
    if (str == "Escape")      return sf::Keyboard::Key::Escape;
    if (str == "Backspace")   return sf::Keyboard::Key::Backspace;
    if (str == "LeftShift")   return sf::Keyboard::Key::LShift;
    if (str == "RightShift")  return sf::Keyboard::Key::RShift;
    if (str == "LeftControl") return sf::Keyboard::Key::LControl;
    if (str == "RightControl")return sf::Keyboard::Key::RControl;
    if (str == "a") return sf::Keyboard::Key::A;
    if (str == "b") return sf::Keyboard::Key::B;
    if (str == "c") return sf::Keyboard::Key::C;
    if (str == "d") return sf::Keyboard::Key::D;

    return sf::Keyboard::Key::Unknown;
}


void InputManager::loadBindings()
{
    std::ifstream file("./Resources/Json/Bindings.json");
    if (!file.is_open())
    {
        std::cerr << "Failed to open Bindings.json\n";
        return;
    }

    nlohmann::json json;
    file >> json;

    const auto& bindings = json["bindings"];

    // --- Keyboard ---
    if (bindings.contains("keyboard"))
    {
        for (const auto& binding : bindings["keyboard"])
        {
            std::string keyStr = binding["key"].get<std::string>();
            sf::Keyboard::Key k = stringToKey(keyStr);

            Action action = stringToAction(binding["action"].get<std::string>());
            _keyToAction[k] = action;
        }
    }

    // --- Joystick buttons ---
    if (bindings.contains("joystick") && bindings["joystick"].contains("buttons"))
    {
        for (const auto& binding : bindings["joystick"]["buttons"])
        {
            //unsigned int button = binding["button"].get<unsigned int>();
            //Action action = stringToAction(binding["action"].get<std::string>());
            //_joystickButtons[button] = action;
        }
    }

    // --- Joystick axes ---
    if (bindings.contains("joystick") && bindings["joystick"].contains("axes"))
    {
        for (const auto& binding : bindings["joystick"]["axes"])
        {
            //sf::Joystick::Axis axis = stringToAxis(binding["axis"].get<std::string>());
            //float direction = binding["direction"].get<float>(); // -1 or 1
            //Action action = stringToAction(binding["action"].get<std::string>());

            //_joystickAxes.push_back({ axis, direction, action });
        }
    }
}

void InputManager::overrideBindings() {
    std::ofstream file("./Resources/Json/Bindings.json");

    if (!file.is_open()) {
        std::cerr << "Failed to open Bindinds.json\n";
        return;
    }

    file << _bindings.dump(4);

}

void InputManager::onActionPressed(InputManager::Action action) {




    std::cout << "button pressed: " << actionToString(action) << std::endl;

}

void InputManager::onActionReleased(InputManager::Action action) {
    std::cout << "button released: " << actionToString(action) << std::endl;
}

void InputManager::onActionHeld(InputManager::Action action) {
    std::cout << "button held: " << actionToString(action) << std::endl;
}

void InputManager::handleInput(const sf::Event& event)
{
    if (const auto* e = event.getIf<sf::Event::KeyPressed>())
    {
        auto it = _keyToAction.find(e->code);
        if (it != _keyToAction.end())
            onActionPressed(it->second);
    }
    else if (const auto* e = event.getIf<sf::Event::KeyReleased>())
    {
        auto it = _keyToAction.find(e->code);
        if (it != _keyToAction.end())
            onActionReleased(it->second);
    }

    if (const auto* e = event.getIf<sf::Event::JoystickButtonPressed>())
    {
        if (e->joystickId == 0)
        {
            if (auto it = _controllerButtonToAction.find(e->button);
                it != _controllerButtonToAction.end())
                onActionPressed(it->second);
        }
    }
}

void InputManager::update() {


    for (const auto& binding : _controllerAxes)
    {
        float value = sf::Joystick::getAxisPosition(0, binding.axis);

        if (value * binding.direction > 50.f)
            onActionHeld(binding.action);
    }

}