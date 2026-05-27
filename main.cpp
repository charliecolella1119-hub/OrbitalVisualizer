#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <optional>
#include <iostream>

#include "Orbital.h"
#include "Renderer.h"

float getDefaultZoom(int orbitalMode)
{
    if (orbitalMode == 1) return 110.0f; // 1s
    if (orbitalMode == 6) return 75.0f;  // 2s

    if (orbitalMode == 2 || orbitalMode == 3 || orbitalMode == 8)
    {
        return 75.0f; // 2p
    }

    if (orbitalMode == 4 || orbitalMode == 5 || orbitalMode == 7)
    {
        return 55.0f; // 3d
    }

    if (orbitalMode == 9 || orbitalMode == 0)
    {
        return 75.0f; // H2
    }

    return 90.0f;
}

int main()
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    int orbitalMode = 1;
    int pointCount = 8000;

    float zoom = 90.0f;
    float cameraDistance = 16.0f;
    float offsetX = 700.0f;
    float offsetY = 500.0f;

    float rotationX = 0.0f;
    float rotationY = 0.0f;

    bool autoRotate = false;

    double bondLength = 1.6;

    sf::RenderWindow window(
        sf::VideoMode({1400, 1000}),
        "Orbital Visualizer"
    );

    window.setFramerateLimit(60);

    sf::Font font;

    if (!font.openFromFile("assets/Roboto-LightItalic.ttf"))
    {
        std::cout << "Failed to load font." << std::endl;
        return 1;
    }

    std::cout << "Font loaded successfully." << std::endl;

    std::vector<Point> points =
        generateOrbital(orbitalMode,
                        pointCount,
                        bondLength);

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            // Close window button
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }

            // Mouse wheel zoom
            if (const auto* mouseWheel =
                event->getIf<sf::Event::MouseWheelScrolled>())
            {
                if (mouseWheel->delta > 0)
                {
                    zoom += 10.0f;
                }
                else if (mouseWheel->delta < 0)
                {
                    zoom -= 10.0f;

                    if (zoom < 20.0f)
                    {
                        zoom = 20.0f;
                    }
                }
            }

            // Keyboard controls
            if (const auto* keyPressed =
                event->getIf<sf::Event::KeyPressed>())
            {
                // Quit
                if (keyPressed->code == sf::Keyboard::Key::Escape)
                {
                    window.close();
                }

                // Orbital selection
                else if (keyPressed->code == sf::Keyboard::Key::Num1)
                {
                    orbitalMode = 1;
                }
                else if (keyPressed->code == sf::Keyboard::Key::Num2)
                {
                    orbitalMode = 2;
                }
                else if (keyPressed->code == sf::Keyboard::Key::Num3)
                {
                    orbitalMode = 3;
                }
                else if (keyPressed->code == sf::Keyboard::Key::Num4)
                {
                    orbitalMode = 4;
                    zoom = getDefaultZoom(orbitalMode);
                    points = generateOrbital(orbitalMode, pointCount, bondLength);
                }
                else if (keyPressed->code == sf::Keyboard::Key::Num5)
                {
                    orbitalMode = 5;
                    zoom = getDefaultZoom(orbitalMode);
                    points = generateOrbital(orbitalMode, pointCount, bondLength);
                }
                else if (keyPressed->code == sf::Keyboard::Key::Num6)
                {
                    orbitalMode = 6;
                }
                else if (keyPressed->code == sf::Keyboard::Key::Num7)
                {
                    orbitalMode = 7;
                    zoom = getDefaultZoom(orbitalMode);
                    points = generateOrbital(orbitalMode, pointCount, bondLength);

                }
                else if (keyPressed->code == sf::Keyboard::Key::Num8)
                {
                    orbitalMode = 8;
                }
                else if (keyPressed->code == sf::Keyboard::Key::Num9)
                {
                    orbitalMode = 9;
                }
                else if (keyPressed->code == sf::Keyboard::Key::Num0)
                {
                    orbitalMode = 0;
                }

                // Point count
                else if (keyPressed->code == sf::Keyboard::Key::Equal)
                {
                    pointCount += 1000;
                }
                else if (keyPressed->code == sf::Keyboard::Key::Hyphen)
                {
                    pointCount -= 1000;

                    if (pointCount < 1000)
                    {
                        pointCount = 1000;
                    }
                }

                // Pan
                else if (keyPressed->code == sf::Keyboard::Key::A)
                {
                    offsetX -= 25.0f;
                }
                else if (keyPressed->code == sf::Keyboard::Key::D)
                {
                    offsetX += 25.0f;
                }
                else if (keyPressed->code == sf::Keyboard::Key::W)
                {
                    offsetY -= 25.0f;
                }
                else if (keyPressed->code == sf::Keyboard::Key::S)
                {
                    offsetY += 25.0f;
                }

                // Rotation
                else if (keyPressed->code == sf::Keyboard::Key::Left)
                {
                    rotationY -= 0.1f;
                }
                else if (keyPressed->code == sf::Keyboard::Key::Right)
                {
                    rotationY += 0.1f;
                }
                else if (keyPressed->code == sf::Keyboard::Key::Q)
                {
                    rotationX -= 0.1f;
                }
                else if (keyPressed->code == sf::Keyboard::Key::E)
                {
                    rotationX += 0.1f;
                }

                // Auto rotate toggle
                else if (keyPressed->code == sf::Keyboard::Key::T)
                {
                    autoRotate = !autoRotate;
                }

                // Bond length controls
                else if (keyPressed->code == sf::Keyboard::Key::LBracket)
                {
                    bondLength -= 0.1;

                    if (bondLength < 0.5)
                    {
                        bondLength = 0.5;
                    }
                }
                else if (keyPressed->code == sf::Keyboard::Key::RBracket)
                {
                    bondLength += 0.1;
                }

                else if (keyPressed->code == sf::Keyboard::Key::Comma)
                {
                    cameraDistance -= 1.0f;

                    if (cameraDistance < 6.0f)
                    {
                        cameraDistance = 6.0f;
                    }
                }

                else if (keyPressed->code == sf::Keyboard::Key::Period)
                {
                    cameraDistance += 1.0f;

                    if (cameraDistance > 30.0f)
                    {
                        cameraDistance = 30.0f;
                    }
                }

                // Regenerate orbitals
                points = generateOrbital(
                    orbitalMode,
                    pointCount,
                    bondLength
                );
            }
        }

        // Auto rotate
        if (autoRotate)
        {
            rotationY += 0.01f;
        }

        // Clear screen
        window.clear(sf::Color::Black);

        // Draw orbitals
        drawPoints(window,
           points,
           zoom,
           offsetX,
           offsetY,
           rotationX,
           rotationY,
           cameraDistance);

        // Draw nuclei for molecular orbitals
        if (orbitalMode == 9 || orbitalMode == 0)
        {
            drawNuclei(window,
            font,
            zoom,
            offsetX,
            offsetY,
            rotationX,
            rotationY,
            bondLength);
            
        }

        // Draw UI
        drawUI(window,
        font,
        orbitalMode,
        pointCount,
        zoom,
        bondLength,
        autoRotate,
        cameraDistance);

        // Display frame
        window.display();
    }

    std::cout << "Program exiting normally." << std::endl;

    return 0;
}


//--- g++ -std=c++17 main.cpp Orbital.cpp Renderer.cpp -o main \
-I/opt/homebrew/include \
-L/opt/homebrew/lib \
-lsfml-graphics \
-lsfml-window \
-lsfml-system ---//

