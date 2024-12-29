#include "headers.h"

void Movie::setCycles(int cys)
{
    if (cys == M_FOREVER)
    {
        infiniteLoop = true;
    } else {
        cycles = cys;
    }
}

bool Movie::play() {
    if (cycles == 0) return false;

    playing = true;
    while (playing) {
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = currentTime - previousTime;
        previousTime = currentTime;

        accumulatedTime += deltaTime;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                playing = false;
            }
            events(event);
        }

        while (accumulatedTime >= timestep) {
            if (activeScene < scenes.size()) {
                float minCompletion = 1.0f;
                for (auto& scene : scenes[activeScene]) {
                	beforeTick(*this);
                    float completion = scene.tick();
                    if (completion < minCompletion) minCompletion = completion;
                    // if (completion == 1 && !scene.) scene.onComplete(scene);
                }
                if (minCompletion == 1.0f) activeScene++;
            }
            accumulatedTime -= timestep;
        }

        if (activeScene < scenes.size()) {
            renderMethod(*this);
        } else {
            completedCycles++;
            if (infiniteLoop || completedCycles < cycles) {
                activeScene = 0;
                for (auto& vScene : scenes)
                {
                    for (auto& scene : vScene)
                    {
                        for (auto& actor : scene.actors)
                        {
                            for (auto& vAction : actor.script)
                            {
                                for (auto& action : vAction)
                                {
                                    action.jumpTo(actor);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    SDL_DestroyRenderer(camera.renderer);
    SDL_DestroyWindow(camera.window);
    SDL_Quit();

    return true;
};

void Movie::compose(std::vector<std::vector<Scene>> vScenes) {
    for (auto& scns : vScenes)
    {
        for (auto& scene : scns)
        {
            for (auto& actor : scene.actors)
            {
                for (auto& actions : actor.script)
                {
                    for (auto& action : actions)
                    {
                        action.setFPS(fps);
                        action.jumpTo(actor, 0.0f);
                    }
                }
            }
        }
    }
    scenes = vScenes;
}
