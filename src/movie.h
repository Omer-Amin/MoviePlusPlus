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

    std::vector<std::vector<Scene>> scenes;
    std::function<void(Movie&)> frame;

    float accumulatedTime = 0.0f;
    Uint32 previousTime = SDL_GetTicks();

    Movie(Camera camera, unsigned int fps) : camera(camera), fps(fps), timestep(1000.0f / fps) {}

    std::function<void(Movie&)> renderMethod = [&](Movie& movie) {
        camera.clear(M_BLACK);
        for (auto& scene : movie.scenes[movie.activeScene]) {
            scene.renderMethod(scene, movie.camera);
        }
        camera.render();
    };

    bool play();
    void compose(std::vector<std::vector<Scene>>);
    void setCycles(int);
};
