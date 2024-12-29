#pragma once

#include "action.h"
#include "actor.h"
#include "easing.h"
#include "scene.h"
#include "director.h"
#include <vector>

struct Writer
{
    static Scene wait(float duration)
    {
        Action delay(M_DELAY, duration);
        Actor waiter = Director::getActorWithScript(Actor({Vec2()}), {{delay}});
        return Scene({waiter});
    }

    static Scene morph(std::vector<Vec2> vertsA, std::vector<Vec2> vertsB, float duration, easingFunction easing)
    {
        if (vertsA.size() < vertsB.size())
        {
            std::vector<Vec2> newVertsA = vertsA;
            int i = vertsA.size() - 1;
            while(newVertsA.size() != vertsB.size())
            {
                i = i % newVertsA.size();
                int j = (i + 1) % newVertsA.size();
                newVertsA.insert(newVertsA.begin() + j, (newVertsA[i] + newVertsA[j]) / 2);
                i+=2;
            }
            vertsA = newVertsA;
        }
        else if (vertsA.size() > vertsB.size())
        {
            std::vector<Vec2> newVertsB = vertsB;
            int i = vertsB.size() - 1;
            while(newVertsB.size() != vertsA.size())
            {
                i = i % newVertsB.size();
                int j = (i + 1) % newVertsB.size();
                newVertsB.insert(newVertsB.begin() + j, (newVertsB[i] + newVertsB[j]) / 2);
                i+=2;
            }
            vertsB = newVertsB;
        }

        std::vector<Actor> actors;
        for (int i = 0; i < vertsA.size(); i++)
        {
            Action translateVertex(M_TRANSLATE, {Action::arg(vertsA[i]), Action::arg(vertsB[i])}, Action::arg({0, 0}), duration, easing);
            actors.push_back(
                Director::getActorWithScript(Actor({vertsA[i]}), {{translateVertex}})
            );
        }

        Scene scene(actors);

        scene.renderMethod = [&](Scene& scene, Camera& camera)
        {
            camera.setColor(M_WHITE);
            std::vector<Vec2> morpher;
            for (auto& actor : scene.actors)
            {
                morpher.push_back(actor.position);
            }
            camera.poly(morpher);
        };

        return scene;
    }

    static Scene draw(std::vector<Vec2> drawPath, float duration, easingFunction easing)
    {
   	    Action draw(M_TRANSLATE, Action::getPath(drawPath), Action::arg(0, 0), duration, easing);
   	    Scene drawSketch(Director::cloneAppendAction(Actor({}), draw));
   	    drawSketch.renderMethod = [&](Scene& scene, Camera& camera)
   	    {
   	        camera.setColor(M_WHITE);
   	        auto& action = scene.actors[0].script[0][0];
   			std::vector<Vec2> drawnSection = Algebra::getInterpolatedSet(action.path, action.easing(action.completion));
   	        camera.poly(drawnSection, false);
   	    };

   	    return drawSketch;
    }
};
