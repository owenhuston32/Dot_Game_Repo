#pragma once
#include "world.h"
#include "TransformComponents.h"
#include "GameStateManager.h"
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <json.hpp>

using json = nlohmann::json;

#include <unordered_map>

class GuiManager {

private:
    World& _world;
    tgui::Gui& _gui;
    sf::RenderWindow& _window;
	GameState& _gameState;
    bool applyLayout(tgui::Widget::Ptr widget, const json& node);

    // map of Screen enum (as int) to root panel/widget
    std::unordered_map<int, tgui::Panel::Ptr> _screens;

    // load a screen UI from JSON and register it under screen enum
    bool loadScreenFromJson(const std::string& path, Screen screen);

    // show a specific screen (hide others)
    void setActiveScreen(Screen screen);

    // track currently visible screen
    int _activeScreen = -1;

    // id of registered listener on GameState
    size_t _listenerId = 0;

public:

    explicit GuiManager(World& world, tgui::Gui& gui, sf::RenderWindow& window, GameState& gameState);

};