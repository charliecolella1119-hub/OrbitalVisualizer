#include "Renderer.h"
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <string>
#include <iostream>

struct ProjectedPoint
{
    float screenX;
    float screenY;
    float depth;
    float size;
    sf::Color color;
};

sf::Texture createGaussianTexture()
{
    const int size = 64;

    sf::Image image(
    {
        static_cast<unsigned int>(size),
        static_cast<unsigned int>(size)
    },
    sf::Color::Transparent);

    float center = size / 2.0f;

    for (int y = 0; y < size; y++)
    {
        for (int x = 0; x < size; x++)
        {
            float dx = x - center;
            float dy = y - center;

            float dist2 = dx * dx + dy * dy;

            float sigma = 12.0f;

            float intensity =
                std::exp(-dist2 / (2.0f * sigma * sigma));

            std::uint8_t alpha =
                static_cast<std::uint8_t>(255.0f * intensity);

            image.setPixel(
            {
                static_cast<unsigned int>(x),
                static_cast<unsigned int>(y)
            },
            sf::Color(255, 255, 255, alpha));
        }
    }

    sf::Texture texture;

    if (!texture.loadFromImage(image))
    {
        std::cout << "Failed to load gaussian texture.\n";
    }

    texture.setSmooth(true);

    return texture;
}

void drawPoints(sf::RenderWindow& window,
                const std::vector<Point>& points,
                float zoom,
                float offsetX,
                float offsetY,
                float rotationX,
                float rotationY,
                float cameraDistance,
                int renderMode)
{
    bool glowMode = (renderMode == 1);
    bool volumetricMode = (renderMode == 2);

    std::vector<ProjectedPoint> projectedPoints;

    for (const Point& p : points)
    {
        float x = p.x;
        float y = p.y;
        float z = p.z;

        // Y rotation
        float cosY = std::cos(rotationY);
        float sinY = std::sin(rotationY);

        float rotatedX = x * cosY + z * sinY;
        float rotatedZ = -x * sinY + z * cosY;

        // X rotation
        float cosX = std::cos(rotationX);
        float sinX = std::sin(rotationX);

        float rotatedY = y * cosX - rotatedZ * sinX;
        rotatedZ = y * sinX + rotatedZ * cosX;

        // Prevent divide-by-zero / clipping
        if (cameraDistance - rotatedZ < 0.1f)
        {
            continue;
        }

        // Perspective projection
        float perspective =
            cameraDistance / (cameraDistance - rotatedZ);

        float screenX =
            rotatedX * zoom * perspective + offsetX;

        float screenY =
            rotatedY * zoom * perspective + offsetY;

        // Depth brightness
        float brightness =
            1.0f - (rotatedZ + 4.0f) / 8.0f;

        if (brightness < 0.25f)
        {
            brightness = 0.25f;
        }

        if (brightness > 1.0f)
        {
            brightness = 1.0f;
        }

        sf::Color color(
            static_cast<std::uint8_t>(p.color.r * brightness),
            static_cast<std::uint8_t>(p.color.g * brightness),
            static_cast<std::uint8_t>(p.color.b * brightness),
            255
        );

        projectedPoints.push_back(
        {
            screenX,
            screenY,
            rotatedZ,
            1.5f * perspective,
            color
        });
    }

    // Sort back-to-front
    std::sort(projectedPoints.begin(),
              projectedPoints.end(),
              [](const ProjectedPoint& a,
                 const ProjectedPoint& b)
              {
                  return a.depth > b.depth;
              });

    // =========================
    // GLOW / VOLUMETRIC MODES
    // =========================

    if (glowMode || volumetricMode)
    {
        sf::Texture gaussianTexture =
            createGaussianTexture();

        sf::RenderTexture densityTexture({1400, 1000});

        densityTexture.clear(sf::Color::Transparent);

        for (const ProjectedPoint& projected : projectedPoints)
        {
            sf::Sprite glow(gaussianTexture);

            float scale;

            if (glowMode)
            {
                scale = projected.size * 0.18f;
            }
            else
            {
                scale = projected.size * 0.28f;
            }

            glow.setOrigin({
                gaussianTexture.getSize().x / 2.0f,
                gaussianTexture.getSize().y / 2.0f
            });

            glow.setPosition({
                projected.screenX,
                projected.screenY
            });

            glow.setScale({scale, scale});

            sf::Color glowColor = projected.color;

            if (glowMode)
            {
                glowColor.a = 55;
            }
            else
            {
                glowColor.a = 28;
            }

            glow.setColor(glowColor);

            densityTexture.draw(glow, sf::BlendAdd);
        }

        densityTexture.display();

        sf::Sprite densitySprite(densityTexture.getTexture());

        if (volumetricMode)
        {
            densitySprite.setColor(
                sf::Color(220, 220, 220, 235)
            );
        }

        window.draw(densitySprite, sf::BlendAdd);
    }

    // =========================
    // NORMAL POINT MODE
    // =========================

    else
    {
        for (const ProjectedPoint& projected : projectedPoints)
        {
            sf::CircleShape dot(projected.size);

            dot.setPosition({
                projected.screenX,
                projected.screenY
            });

            dot.setFillColor(projected.color);

            window.draw(dot);
        }
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
        "T: Auto | [ ]: Bond Length | , .: Camera | ESC: Quit | R: Reset View | G: Render Mode";

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
