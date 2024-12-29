#pragma once

#include <SDL2/SDL.h>
#include "vec2.h"

typedef std::array<unsigned char, 4> RGBA;

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
    	WIDTH = (float) w;
    	HEIGHT = (float) h;
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

    void fillPolygon(const std::vector<Vec2>& vertices)
    {
        // 1. Find minY and maxY
        float minY = vertices[0].y;
        float maxY = vertices[0].y;
        for (size_t i = 1; i < vertices.size(); i++)
        {
            if (vertices[i].y < minY) minY = vertices[i].y;
            if (vertices[i].y > maxY) maxY = vertices[i].y;
        }
    
        // We'll be scanning from minY to maxY in integer steps
        const int startY = static_cast<int>(std::floor(minY));
        const int endY   = static_cast<int>(std::ceil(maxY));
    
        // 2. For each scanline
        for (int y = startY; y <= endY; y++)
        {
            // Collect all x-coordinates where this scanline intersects an edge
            std::vector<float> intersectionXs;
    
            // Loop through edges of the polygon
            const size_t n = vertices.size();
            for (size_t i = 0; i < n; i++)
            {
                // Current edge is from vertices[i] to vertices[(i+1) % n]
                const Vec2& v1 = vertices[i];
                const Vec2& v2 = vertices[(i + 1) % n];
    
                // Make sure v1.y <= v2.y for intersection check (swap if needed)
                if (v1.y > v2.y)
                {
                    // Swap
                    std::swap(*const_cast<Vec2*>(&v1), *const_cast<Vec2*>(&v2));
                }
    
                // If the scanline y is strictly between v1.y and v2.y (inclusive of one side),
                // then compute intersection
                if (y >= v1.y && y < v2.y)
                {
                    // Avoid division by zero if the edge is horizontal
                    float dy = (v2.y - v1.y);
                    if (dy == 0) {
                        continue;
                    }
    
                    // Find intersection x using linear interpolation
                    float t = (y - v1.y) / dy;         // interpolation factor (0..1)
                    float intersectX = v1.x + t * (v2.x - v1.x);
    
                    intersectionXs.push_back(intersectX);
                }
            }
    
            // 3. Sort the intersection x-coordinates
            std::sort(intersectionXs.begin(), intersectionXs.end());
    
            // 4. Draw horizontal lines between pairs of intersection points
            //    intersectionXs should have an even number of entries if the polygon is simple
            for (size_t k = 0; k + 1 < intersectionXs.size(); k += 2)
            {
                // Start x and end x
                int xStart = static_cast<int>(std::floor(intersectionXs[k]));
                int xEnd   = static_cast<int>(std::ceil(intersectionXs[k + 1]));
    
                // Draw a horizontal line on the current scanline
                SDL_RenderDrawLine(renderer, xStart, y, xEnd, y);
            }
        }
    }
};
