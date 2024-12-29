#pragma once

#include "action.h"
#include "actor.h"
#include "scene.h"
#include <vector>

struct Modifier
{
    std::vector<Actor> modifiers;
    std::function<void(Scene&, Modifier&)> apply = [&](Scene& scene, Modifier& modifier) {};
};

struct Director
{
    static Actor getActorWithScript(Actor actor, Script script)
    {
        for (auto& actions : script)
        {
            for (auto& action : actions)
            {
                action.jumpTo(actor);
            }
        }
        actor.script = script;

        return actor;
    }

    static void appendAction(Actor& actor, Action action)
    {
        // action.jumpTo(actor, 0);
        actor.script.push_back({action}); // TODO: pls fix
    }

    static Actor cloneAppendAction(Actor actor, Action action)
    {
        // action.jumpTo(actor, 0);
        actor.script.push_back({action}); // TODO: pls fix

        return actor;
    }

    static void sceneAppendAction(Scene& scene, Action action)
    {
        for (auto& actor : scene.actors)
        {
            Director::appendAction(actor, action);
        }
    }

    // TODO: script index parameter
    static Modifier getScenePathModifier(Scene& scene, Action action)
    {
        Modifier newModifier;

        std::vector<Actor> modifierActors;
        for (auto& actor : scene.actors)
        {
            Actor singleActor(actor.script[0][0].path);
            Director::appendAction(singleActor, action);
            modifierActors.push_back(singleActor);
        }

        newModifier.modifiers = modifierActors;

        newModifier.apply = [&](Scene& scene, Modifier& modifier) {
            for (size_t i = 0; i < scene.actors.size(); i++)
            {
                auto& morphActor = scene.actors[i].script[0][0];
                auto& pathModifier = modifier.modifiers[i];

                pathModifier.script[0][0].jumpTo(pathModifier, morphActor.completion);
                morphActor.path = pathModifier.vertices;
            }
        };

        return newModifier;
    }
};