#ifndef RENDERER_H
#define RENDERER_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "Orbital.h"

void drawPoints(sf::RenderWindow& window,
                const std::vector<Point>& points,
                float zoom,
                float offsetX,
                float offsetY,
                float rotationX,
                float rotationY);

void drawNuclei(sf::RenderWindow& window,
                float zoom,
                float offsetX,
                float offsetY,
                float rotationX,
                float rotationY,
                double bondLength);

void drawUI(sf::RenderWindow& window,
            const sf::Font& font,
            int orbitalMode,
            int pointCount,
            float zoom,
            double bondLength,
            bool autoRotate);

#endif
