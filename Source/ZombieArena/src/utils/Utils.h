//
// Created by Oleksandr Pogorelov on 2.9.2025.
//

#pragma once

#include "SFML/Graphics.hpp"
#include <vector>
#include <iostream>

namespace ZombieArena::Utils
{
    using namespace sf;

    class Utils
    {
    public:
        // Helper to normalize a 2D vector (safe for {0,0})
        static Vector2f Normalize2D(const Vector2f& v)
        {
            const float len2 = v.x * v.x + v.y * v.y;
            if (len2 <= 0.f) return {0.f, 0.f};
            const float inv = 1.f / std::sqrt(len2);
            return {v.x * inv, v.y * inv};
        }

        static float length(const Vector2f& v)
        {
            return std::sqrt(v.x*v.x + v.y*v.y);
        }

        static float clampf(float v, float lo, float hi)
        {
            return std::max(lo, std::min(v, hi));
        }

        static Vector2f rectCenter(const FloatRect& rect)
        {
            return { rect.position.x + rect.size.x * 0.5f, rect.position.y + rect.size.y * 0.5f};
        }

        static float rectRadiusApprox(const FloatRect& rect)
        {
            return 0.5f * std::min(rect.size.x, rect.size.y);
        }

        static bool circlesIntersect(const Vector2f& c1, const Vector2f& c2, float r1, float r2, float mod1 = 0.32f, float mod2 = 0.32f)
        {
            const float dx = c1.x - c2.x;
            const float dy = c1.y - c2.y;
            const float dist2 = dx * dx + dy * dy;
            const float rad1 = r1 * mod1;
            const float rad2 = r2 * mod2;
            const float rr = (rad1 + rad2) * (rad1 + rad2);

            return dist2 <= rr;
        }

    };
}