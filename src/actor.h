#pragma once

#include "action.h"
#include "camera.h"
#include "vec2.h"
#include <vector>
#include <functional>

#define mainAction script[0][0]

struct Actor
{
    std::vector<Vec2> vertices;
    std::vector<Vec2> localVertices;
    size_t vertNum;
    
    Vec2 position;
    
    Vec2 angle = { 0, 0 };
    Vec2 scale = { 1, 1 };

	RGBA strokeColor = M_WHITE;
	RGBA fillColor = M_CLEAR;

    Script script;
    
    std::function<void(Actor&)> afterTick = [&](Actor& actor) {};

    std::function<void(Actor&, Camera&)> renderMethod = [&](Actor& actor, Camera& camera) {
        if (vertNum > 1)
        {
            camera.setColor(actor.fillColor);
            camera.polyFill(actor.vertices);

            camera.setColor(actor.strokeColor);
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

    void prepare(Script acts)
    {
        for (auto& actions : acts)
        {
            for (auto& action : actions)
            {
                action.jumpTo(*this);
            }
        }
        script = acts;
    }
    
    void setPosition(Vec2 newPos)
    {
        const Vec2 deltaPosition = newPos - position;
        position = newPos;
        for (auto& vertex : vertices)
            vertex = vertex + deltaPosition;
    }
    
    
    Actor reverse()
    {
        Actor reversed = *this;
        for (auto& actions : reversed.script)
        {
            for (auto& action : actions)
            {
                action = action.reverse();
                action.jumpTo(reversed);
            }
            // std::reverse(actions.begin(), actions.end());
        }
        std::reverse(reversed.script.begin(), reversed.script.end());
    
        return reversed;
    }
};
