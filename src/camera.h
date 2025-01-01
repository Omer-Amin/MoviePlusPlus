#pragma once

#include <SDL2/SDL.h>
#include "vec2.h"

typedef std::array<unsigned char, 4> RGBA;

static inline const RGBA M_CLEAR = {0, 0, 0, 255};
static inline const RGBA M_BLACK = {0, 0, 0, 0};
static inline const RGBA M_WHITE = {255, 255, 255, 0};
static inline const RGBA M_RED =   {255, 0, 0, 0};

struct Camera
{
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    float WIDTH;
    float HEIGHT;

    Vec2 offset = { 0.0f, 0.0f };
    Vec2 scale = { 1.0f, 1.0f };
    Vec2 origin = { 0.0f, 0.0f };

    Camera(int w, int h)
    {
    	WIDTH = static_cast<float>(w);
    	HEIGHT = static_cast<float>(h);
        SDL_Init(SDL_INIT_EVERYTHING);
        SDL_CreateWindowAndRenderer(w, h, 0, &window, &renderer);
    }

    inline void render()
    {
        SDL_RenderPresent(renderer);
    }

    inline void clear(const RGBA& rgba)
    {
        SDL_SetRenderDrawColor(renderer, rgba[0], rgba[1], rgba[2], rgba[3]);
        SDL_RenderClear(renderer);
    }

    inline void setColor(const RGBA& rgba)
    {
        SDL_SetRenderDrawColor(renderer, rgba[0], rgba[1], rgba[2], rgba[3]);
    }

    inline void poly(std::vector<Vec2>& vertices, bool close = true)
    {
        size_t sizeLess1 = vertices.size() - 1;
        for (int i = 0; i < sizeLess1; i++)
        {
            SDL_RenderDrawLineF(renderer, vertices[i].x, vertices[i].y, vertices[i + 1].x, vertices[i + 1].y);
        }

        if (close) SDL_RenderDrawLineF(renderer, vertices[sizeLess1].x, vertices[sizeLess1].y, vertices[0].x, vertices[0].y);
    }

    void polyFill(const std::vector<Vec2>& vertices) {
        if (vertices.size() < 3) return;
    
        float minY = vertices[0].y;
        float maxY = vertices[0].y;
        for (const auto& v : vertices) {
            if (v.y < minY) minY = v.y;
            if (v.y > maxY) maxY = v.y;
        }
    
        for (int y = static_cast<int>(minY); y <= static_cast<int>(maxY); ++y) {
            std::vector<float> intersections;
            for (size_t i = 0; i < vertices.size(); ++i) {
                const Vec2& v1 = vertices[i];
                const Vec2& v2 = vertices[(i + 1) % vertices.size()];
    
                if ((v1.y <= y && v2.y > y) || (v2.y <= y && v1.y > y)) {
                    float t = (y - v1.y) / (v2.y - v1.y);
                    float x = v1.x + t * (v2.x - v1.x);
                    intersections.push_back(x);
                }
            }
    
            std::sort(intersections.begin(), intersections.end());
    
            for (size_t i = 0; i < intersections.size(); i += 2) {
                if (i + 1 < intersections.size()) {
                    SDL_RenderDrawLine(renderer, static_cast<int>(intersections[i]), y,
                                       static_cast<int>(intersections[i + 1]), y);
                }
            }
        }
    }
};
