#include "Renderer.h"
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <string>

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
                float rotationY,
                float cameraDistance)
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

        if (cameraDistance - rotatedZ < 0.1f)
            {
                continue;
            }

        float perspective =
            cameraDistance / (cameraDistance - rotatedZ);

        float screenX =
            rotatedX * zoom * perspective + offsetX;

        float screenY =
            rotatedY * zoom * perspective + offsetY;

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
                  return a.depth > b.depth;
              });

    for (const ProjectedPoint& projected : projectedPoints)
    {
        sf::CircleShape dot(projected.size);
        dot.setPosition({projected.screenX, projected.screenY});
        dot.setFillColor(projected.color);

        window.draw(dot);
    }
}

void drawNuclei(sf::RenderWindow& window,
                const sf::Font& font,
                float zoom,
                float offsetX,
                float offsetY,
                float rotationX,
                float rotationY,
                double bondLength)
{
    std::vector<sf::Vector3f> nuclei =
    {
        {static_cast<float>(-bondLength / 2.0), 0.0f, 0.0f},
        {static_cast<float>( bondLength / 2.0), 0.0f, 0.0f}
    };

    float xA = static_cast<float>(-bondLength / 2.0);
    float xB = static_cast<float>( bondLength / 2.0);

    float cosY = std::cos(rotationY);
    float sinY = std::sin(rotationY);

    float rotatedXA = xA * cosY;
    float rotatedZA = -xA * sinY;

    float rotatedXB = xB * cosY;
    float rotatedZB = -xB * sinY;

    float cosX = std::cos(rotationX);
    float sinX = std::sin(rotationX);

    float rotatedYA = -rotatedZA * sinX;
    rotatedZA = rotatedZA * cosX;

    float rotatedYB = -rotatedZB * sinX;
    rotatedZB = rotatedZB * cosX;

    float cameraDistance = 16.0f;

    float perspectiveA = cameraDistance / (cameraDistance - rotatedZA);
    float perspectiveB = cameraDistance / (cameraDistance - rotatedZB);

    sf::VertexArray bondAxis(sf::PrimitiveType::Lines, 2);

    bondAxis[0].position = {
        rotatedXA * zoom * perspectiveA + offsetX,
        rotatedYA * zoom * perspectiveA + offsetY
    };

    bondAxis[1].position = {
        rotatedXB * zoom * perspectiveB + offsetX,
        rotatedYB * zoom * perspectiveB + offsetY
    };

    bondAxis[0].color = sf::Color(180, 180, 180, 120);
    bondAxis[1].color = sf::Color(180, 180, 180, 120);

    window.draw(bondAxis);

    for (const sf::Vector3f& n : nuclei)
    {
        float x = n.x;
        float y = n.y;
        float z = n.z;

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

        float radius = 8.0f * perspective;

        sf::CircleShape nucleus(radius);
        nucleus.setOrigin({radius, radius});
        nucleus.setPosition({screenX, screenY});
        nucleus.setFillColor(sf::Color(180, 180, 180));
        nucleus.setOutlineColor(sf::Color(120, 120, 120));
        nucleus.setOutlineThickness(1.0f);

        window.draw(nucleus);

        float highlightRadius = radius * 0.35f;

        sf::CircleShape highlight(highlightRadius);
        highlight.setOrigin({highlightRadius, highlightRadius});
        highlight.setPosition(
        {
            screenX - radius * 0.35f,
            screenY - radius * 0.35f
        });
        highlight.setFillColor(sf::Color(230, 230, 230, 180));

        sf::Text label(font, "H", 14);
        label.setFillColor(sf::Color::White);
        label.setPosition({screenX - 5.0f, screenY - 28.0f});

        window.draw(label);

        window.draw(highlight);
    }
}

void drawUI(sf::RenderWindow& window,
            const sf::Font& font,
            int orbitalMode,
            int pointCount,
            float zoom,
            double bondLength,
            bool autoRotate,
            float cameraDistance)
{
    double overlap = std::exp(-bondLength);

    double energyEffect = 0.0;

    if (orbitalMode == 9)
    {
        energyEffect = -overlap;
    }
    else if (orbitalMode == 0)
    {
        energyEffect = overlap;
    }

    sf::Text title(font, getOrbitalName(orbitalMode), 24);
    title.setFillColor(sf::Color::White);
    title.setPosition({20.0f, 15.0f});
    window.draw(title);

    std::string infoString =
        "Points: " + std::to_string(pointCount) +
        " | Zoom: " + std::to_string(static_cast<int>(zoom)) +
        " | Camera: " + std::to_string(static_cast<int>(cameraDistance)) +
        " | Bond Length: " + std::to_string(bondLength) +
        " | Overlap: " + std::to_string(overlap) +
        " | Energy Effect: " + std::to_string(energyEffect) +
        " | Auto Rotate: " + std::string(autoRotate ? "ON" : "OFF");

    sf::Text info(font, infoString, 18);
    info.setFillColor(sf::Color::White);
    info.setPosition({20.0f, 55.0f});
    window.draw(info);

    std::string controlsString =
        "1-8: Atomic Orbitals | 9: H2 Bonding | 0: H2 Antibonding | "
        "WASD: Pan | Arrows/QE: Rotate | Mouse Wheel: Zoom | "
        "T: Auto | [ ]: Bond Length | , .: Camera | ESC: Quit | R: Reset View";

    sf::Text controls(font, controlsString, 15);
    controls.setFillColor(sf::Color::White);
    controls.setPosition({20.0f, 90.0f});
    window.draw(controls);

    sf::Text legend(
    font,
    "Legend: Cyan = positive phase | Magenta = negative phase | White/gray spheres = nuclei | Gray line = bond axis",
    15
    );

    legend.setFillColor(sf::Color::White);
    legend.setPosition({20.0f, 120.0f});
    window.draw(legend);    
}
