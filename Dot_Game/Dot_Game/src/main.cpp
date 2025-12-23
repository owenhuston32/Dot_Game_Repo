#include <iostream>
#include <SFML/Graphics.hpp>
#include "RenderSystem.h"
#include "RenderComponents.h"
#include "World.h"
#include "Renderer.h"


int main() {

    World world = World();

    Renderer renderer = Renderer();

    RenderSystem renderSystem = RenderSystem(world, renderer);

    Entity player = world.createEntity();

    world.addComponent(player, Position{ 10, 10 });

    renderSystem.render();

    // create the window
    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "My window");

    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        while (const std::optional event = window.pollEvent())
        {
            // "close requested" event: we close the window
            if (event->is<sf::Event::Closed>())
                window.close();
        }


        renderSystem.render();

        // clear the window with black color
        window.clear(sf::Color::White);

        // draw everything here...
        // window.draw(...);

        // end the current frame
        window.display();
    }

}