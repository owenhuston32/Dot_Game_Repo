#include <iostream>
#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include "GuiManager.h"
#include "InputManager.h"
#include "GameStateManager.h"
#include "RenderSystem.h"
#include "VelocitySystem.h"
#include "World.h"
#include "SpawnSystem.h"


int main() {

    World world = World();

    sf::RenderWindow window(sf::VideoMode({ 1280, 720 }), "Dot Game");

    GameState gameState;
    gameState.clearAndPush(Screen::MainMenu);

    tgui::Gui gui{ window };
	GuiManager guiManager{ world, gui, window, gameState };


    InputManager inputManager{ world, window };
    VelocitySystem velocitySystem{ world, window };
    RenderSystem renderSystem{ world, window};

    SpawnSystem spawnSystem{ world, window };
    spawnSystem.SpawnObjects();

    Entity cmd = world.createEntity();
    //world.addComponent<MoveCommand>(cmd, { e, dir, speed });

    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        while (auto optEvent = window.pollEvent())
        {
            const sf::Event& e = *optEvent;


            gui.handleEvent(e);

            if (e.is<sf::Event::Closed>())
                window.close();

            inputManager.handleInput(e, gameState);
        }

        // clear the window with black color
        window.clear(sf::Color::Black);


        renderSystem.render();
        velocitySystem.update();

        // draw everything here...
        /*window.draw();*/
        gui.draw();

        // end the current frame
        window.display();
    }

}