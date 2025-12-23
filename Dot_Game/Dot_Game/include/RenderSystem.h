#pragma once
#include "world.h"
#include "RenderComponents.h"

#include "Renderer.h"

class RenderSystem {
public:
    void render();

    RenderSystem(World& world, Renderer& renderer);

private:
    World& _world;
    Renderer& _renderer;
};