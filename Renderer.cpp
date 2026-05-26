#include "Renderer.h"
#include <string>
#include <cmath>
#include <algorithm>
#include <cmath>

struct ProjectedPoint
{
    float screenX;
    float screenY;
    float depth;
    float size;
    sf::Color color;
};

void drawPoints(sf::RenderWindow& window,
                const std::vector<Point>& points,
                float zoom,
                float offsetX,
                float offsetY,
                float rotationX,
                float rotationY)
{
    std::vector<ProjectedPoint> projectedPoints;

    for (const Point& p : points)
    {
        float x = p.x;
        float y = p.y;
        float z = p.z;

        float cosY = std::cos(rotationY);
        float sinY = std::sin(rotationY);

        float rotatedX = x * cosY + z * sinY;
        float rotatedZ = -x * sinY + z * cosY;

        float cosX = std::cos(rotationX);
        float sinX = std::sin(rotationX);

        float rotatedY = y * cosX - rotatedZ * sinX;
        rotatedZ = y * sinX + rotatedZ * cosX;

        float perspective = 1.0f / (1.0f + rotatedZ * 0.08f);

        float screenX = rotatedX * zoom * perspective + offsetX;
        float screenY = rotatedY * zoom * perspective + offsetY;

        float brightness = 1.0f - (rotatedZ + 4.0f) / 8.0f;

        if (brightness < 0.25f)
        {
            brightness = 0.25f;
        }

        if (brightness > 1.0f)
        {
            brightness = 1.0f;
        }

        sf::Color color =
        {
            static_cast<std::uint8_t>(p.color.r * brightness),
            static_cast<std::uint8_t>(p.color.g * brightness),
            static_cast<std::uint8_t>(p.color.b * brightness),
            255
        };

        projectedPoints.push_back(
        {
            screenX,
            screenY,
            rotatedZ,
            1.5f * perspective,
            color
        });
    }

    std::sort(projectedPoints.begin(),
              projectedPoints.end(),
              [](const ProjectedPoint& a, const ProjectedPoint& b)
              {
                  return a.depth < b.depth;
              });

    for (const ProjectedPoint& p : projectedPoints)
    {
        sf::CircleShape dot(p.size);
        dot.setPosition({p.screenX, p.screenY});
        dot.setFillColor(p.color);

        window.draw(dot);
    }
}

void drawUI(sf::RenderWindow& window,
            const sf::Font& font,
            int orbitalMode,
            int pointCount,
            float zoom)
{
    sf::Text label(font, getOrbitalName(orbitalMode), 24);
    label.setFillColor(sf::Color::White);
    label.setPosition({20.0f, 20.0f});
    window.draw(label);

    std::string controlsText =
        "1-8: Orbitals | Up/Down: Zoom | +/-: Density | WASD: Pan | Q/E: Rotate | T: Auto-Rotate";

    sf::Text controls(font, controlsText, 18);
    controls.setFillColor(sf::Color::White);
    controls.setPosition({20.0f, 55.0f});
    window.draw(controls);

    std::string info =
        "Points: " + std::to_string(pointCount) +
        " | Zoom: " + std::to_string(static_cast<int>(zoom));

    sf::Text infoText(font, info, 18);
    infoText.setFillColor(sf::Color::White);
    infoText.setPosition({20.0f, 85.0f});
    window.draw(infoText);
}

