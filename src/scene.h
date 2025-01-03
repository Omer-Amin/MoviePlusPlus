#pragma once

#include "action.h"
#include "actor.h"
#include "easing.h"
#include <vector>

struct Scene
{
    std::vector<Actor> actors;
    size_t actionIndex = 0;

    std::function<void(Scene&, Camera&)> renderMethod = [&](Scene& scene, Camera& camera) {
        for (auto& actor : scene.actors)
        {
            actor.renderMethod(actor, camera);
        }
    };

    std::function<void(Scene&)> onComplete = [&](Scene& scene) {};
    std::function<void(Scene&)> afterTick = [&](Scene& scene) {};

    Scene(std::vector<Actor> actors) : actors(actors) {};

    Scene(Actor actor) : actors({actor}) {};

    float tick()
    {
		float minCompletion = 1.0f;
		for (auto& actor : actors)
		{
			float completion = actor.tick();
			if (completion < minCompletion) minCompletion = completion;
		}

		afterTick(*this);
		if (minCompletion == 1) onComplete(*this);
		return minCompletion;
    }

    Scene reverse()
    {
        Scene reversed = *this;
        for (auto& actor : reversed.actors)
        {
            actor = actor.reverse();
        }
        return reversed;
    }

    void setEasing(easingFunction easing)
    {
        for (auto& actor : actors)
        {
            for (auto& actions : actor.script)
            {
                for (auto& action : actions)
                {
                    action.easing = easing;
                }
            }
        }
    }
};
