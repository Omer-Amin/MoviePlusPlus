#pragma once

#include <SDL2/SDL.h>
#include <functional>
#include "camera.h"
#include "scene.h"
#include <vector>


inline const int M_FOREVER = -1;

struct Movie
{
    Camera camera;
    SDL_Event event;

    std::function<void(SDL_Event&)> events = [&](SDL_Event& e) {};
    std::function<void(Movie&)> beforeTick = [&](Movie& movie) {};

    unsigned int fps;
    float timestep;
    bool playing = false;
    size_t activeScene = 0;

    bool infiniteLoop = false;
    int completedCycles = 0;
    int cycles = 1;

    float playSpeed = 1.0f;

    std::vector<std::vector<Scene>> scenes;
    std::function<void(Movie&)> frame;

    float accumulatedTime = 0.0f;
    Uint32 previousTime = SDL_GetTicks();

    Movie(Camera camera, unsigned int fps) : camera(camera), fps(fps)
    {
    	timestep = 1000.0f / (static_cast<float>(fps) * playSpeed);
    }

    std::function<void(Movie&)> renderMethod = [&](Movie& movie) {
        camera.clear(M_BLACK);
        for (auto& scene : movie.scenes[movie.activeScene]) {
            scene.renderMethod(scene, movie.camera);
        }
        camera.render();
    };

    bool play() {
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
    
    void compose(std::vector<std::vector<Scene>> vScenes) {
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
    
    void setCycles(int cys)
    {
        if (cys == M_FOREVER)
        {
            infiniteLoop = true;
        } else {
            cycles = cys;
        }
    };
};
