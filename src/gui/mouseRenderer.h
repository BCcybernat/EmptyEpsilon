#ifndef MOUSE_RENDERER_H
#define MOUSE_RENDERER_H

#include "engine.h"

class MouseRenderer : public Renderable
{
public:
    bool visible;

    MouseRenderer();

    virtual void render(sf::RenderTarget& window);
    //virtual void update(float delta);
};

#endif//MOUSE_RENDERER_H
