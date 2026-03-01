#include "GuiManager.h"
#include <fstream>
#include <sstream>
#include <iostream>

GuiManager::GuiManager(World& world, tgui::Gui& gui, sf::RenderWindow& window, GameState& gameState) : _world(world), _gui(gui), _window(window), _gameState(gameState) {


    // Preload known screens and keep them in sync with gameState
    loadScreenFromJson("resources/json/MainMenu.json", Screen::MainMenu);
    loadScreenFromJson("resources/json/PauseMenu.json", Screen::PauseMenu);
    loadScreenFromJson("resources/json/SettingsMenu.json", Screen::Settings);
    loadScreenFromJson("resources/json/BindingsMenu.json", Screen::Bindings);

    // Ensure UI matches initial game state
    if (!_gameState.isEmpty())
        _activeScreen = static_cast<int>(_gameState.top());
    else
        _activeScreen = static_cast<int>(Screen::MainMenu);
    setActiveScreen(static_cast<Screen>(_activeScreen));

    // Register listener so GuiManager is notified when GameState changes
    _listenerId = _gameState.addListener([this](Screen s) {
        setActiveScreen(s);
    });

}

bool GuiManager::applyLayout(tgui::Widget::Ptr widget, const json& node)
{
    bool applied = false;
    if (node.contains("size") && node["size"].is_array())
    {
        try {
            tgui::Layout sizeX(node["size"][0].get<std::string>());
            tgui::Layout sizeY(node["size"][1].get<std::string>());
            widget->setSize(sizeX, sizeY);
            applied = true;
        }
        catch (...) {}
    }

    if (node.contains("position") && node["position"].is_array())
    {
        try {
            tgui::Layout posX(node["position"][0].get<std::string>());
            tgui::Layout posY(node["position"][1].get<std::string>());
            widget->setPosition(posX, posY);
            applied = true;
        }
        catch (...) {}
    }

    return applied;
}

bool GuiManager::loadScreenFromJson(const std::string& path, Screen screen)
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        std::cerr << "GuiManager: failed to open " << path << std::endl;
        return false;
    }

    json root;
    try {
        file >> root;
    }
    catch (const std::exception& e)
    {
        std::cerr << "GuiManager: JSON parse error: " << e.what() << std::endl;
        return false;
    }

    // Panel
    auto menuPanel = tgui::Panel::create();
    if (root.contains("panel"))
    {
        const auto& p = root["panel"];
        applyLayout(menuPanel, p);
    }

    // register and hide initially
    _screens[static_cast<int>(screen)] = menuPanel;
    menuPanel->setVisible(false);
    _gui.add(menuPanel);

    // Title
    if (root.contains("title"))
    {
        const auto& t = root["title"];
        std::string text = t.value("text", std::string(""));
        auto title = tgui::Label::create(text);
        title->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);

        if (t.contains("textSizeDivisor"))
        {
            unsigned int div = t["textSizeDivisor"].get<unsigned int>();
            if (div > 0)
            {
                unsigned int titleSize = static_cast<unsigned int>(_window.getSize().y / div);
                title->setTextSize(titleSize);
            }
        }

        applyLayout(title, t);

        menuPanel->add(title);
    }

    // Buttons
    if (root.contains("buttons") && root["buttons"].is_array())
    {
        for (const auto& b : root["buttons"])
        {
            std::string text = b.value("text", std::string(""));
            std::string id = b.value("id", std::string(""));

            auto btn = tgui::Button::create(text);


            applyLayout(btn, b);

            menuPanel->add(btn, id);

            // Wire action
            if (b.contains("action") && b["action"].is_object())
            {
                const auto& act = b["action"];
                std::string type = act.value("type", std::string(""));

                if (type == "push_screen")
                {
                    std::string screenName = act.value("screen", std::string(""));
                    btn->onClick([this, screenName]() {
                        // convert name to enum
                        Screen target = Screen::Gameplay;
                        if (screenName == "Gameplay") target = Screen::Gameplay;
                        else if (screenName == "Settings") target = Screen::Settings;
                        else if (screenName == "MainMenu") target = Screen::MainMenu;

                        // update game state
                        if (target == Screen::Gameplay || target == Screen::MainMenu)
                        {
                            _gameState.clearAndPush(target);
                        }
                        else
                        {
                            _gameState.push(target);
                        }

                        // listener will call setActiveScreen, but also set immediately
                        setActiveScreen(target);
                    });
                }
                else if (type == "pop_screen")
                {
                    btn->onClick([this]() {
                        if (!_gameState.isEmpty())
                            _gameState.pop();
                        // listener will update UI; setActiveScreen called by listener
                    });
                }
                else if (type == "close_window")
                {
                    btn->onClick([this]() {
                        _window.close();
                    });
                }
            }
        }
    }

    // Controls (e.g., sliders) - settings screen
    if (root.contains("controls") && root["controls"].is_array())
    {
        for (const auto& c : root["controls"]) {
            std::string type = c.value("type", std::string(""));
            if (type == "slider")
            {
                std::string id = c.value("id", std::string(""));
                std::string labelText = c.value("label", std::string(""));

                auto label = tgui::Label::create(labelText);
                applyLayout(label, c);
                menuPanel->add(label);

                auto slider = tgui::Slider::create();
                int min = c.value("min", 0);
                int max = c.value("max", 100);
                int value = c.value("value", 0);
                slider->setMinimum(min);
                slider->setMaximum(max);
                slider->setValue(value);
                applyLayout(slider, c);
                menuPanel->add(slider, id);
            }
        }
    }

    return true;
}

void GuiManager::setActiveScreen(Screen screen)
{
    // hide all
    for (auto& kv : _screens)
    {
        if (kv.second)
            kv.second->setVisible(false);
    }

    auto it = _screens.find(static_cast<int>(screen));
    if (it != _screens.end() && it->second)
        it->second->setVisible(true);

    _activeScreen = static_cast<int>(screen);
}