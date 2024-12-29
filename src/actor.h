#pragma once

#include "action.h"
#include "algebra.h"
#include "camera.h"
#include "vec2.h"
#include <vector>
#include <functional>

#define mainAction script[0][0]

struct Actor
{
    std::vector<Vec2> vertices;
    std::vector<Vec2> localVertices;
    Vec2 position;
    Vec2 angle = { 0, 0 };
    Vec2 scale = { 1, 1 };
    size_t vertNum;
    Script script;

    std::function<void(Actor&, Camera&)> renderMethod = [&](Actor& actor, Camera& camera) {
        if (vertNum > 1)
        {
            camera.setColor(M_WHITE);
            camera.poly(actor.vertices);
        }
    };

    Actor(std::vector<Vec2> vertices) : vertices(vertices) {
        vertNum = vertices.size();
        if (vertNum == 0) {
            vertices = {Vec2()};
            vertNum = 1;
        }
        
        if (vertNum == 1)
        {
            position = vertices[0];
            localVertices = {Vec2(0, 0)};
        } else 
        {
            position = Algebra::centroid(vertices);
            localVertices = Algebra::relativeVertices(vertices, position);
        }
    }

    void read(Script);
    void setPosition(Vec2);
    void afterTick(Actor&);
    Actor reverse();
};