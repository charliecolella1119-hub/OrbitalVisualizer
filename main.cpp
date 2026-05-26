#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "Orbital.h"
#include "Renderer.h"

int main()
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    int orbitalMode = 1;
    int pointCount = 8000;
    float zoom = 90.0f;
    float rotationX = 0.0f;
    float rotationY = 0.0f;
    float offsetX = 700.0f;
    float offsetY = 500.0f;
    bool autoRotate = false;

    sf::RenderWindow window(sf::VideoMode({1400, 1000}), "Orbital Visualizer");
    window.setFramerateLimit(60);

    sf::Font font;

    if (!font.openFromFile("assets/Roboto-LightItalic.ttf"))
    {
        return 1;
    }

    std::vector<Point> points = generateOrbital(orbitalMode, pointCount);

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }

            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyPressed->code == sf::Keyboard::Key::Num1)
                {
                    orbitalMode = 1;
                    points = generateOrbital(orbitalMode, pointCount);
                }
                else if (keyPressed->code == sf::Keyboard::Key::Num2)
                {
                    orbitalMode = 2;
                    points = generateOrbital(orbitalMode, pointCount);
                }
                else if (keyPressed->code == sf::Keyboard::Key::Num3)
                {
                    orbitalMode = 3;
                    points = generateOrbital(orbitalMode, pointCount);
                }
                else if (keyPressed->code == sf::Keyboard::Key::Num4)
                {
                    orbitalMode = 4;
                    points = generateOrbital(orbitalMode, pointCount);
                }
                else if (keyPressed->code == sf::Keyboard::Key::Num5)
                {
                    orbitalMode = 5;
                    points = generateOrbital(orbitalMode, pointCount);
                }
                else if (keyPressed->code == sf::Keyboard::Key::Num6)
                {
                    orbitalMode = 6;
                    points = generateOrbital(orbitalMode, pointCount);
                }  
                else if (keyPressed->code == sf::Keyboard::Key::Num7)
                {
                    orbitalMode = 7;
                    points = generateOrbital(orbitalMode, pointCount);
                }
                else if (keyPressed->code == sf::Keyboard::Key::Num8)
                {
                    orbitalMode = 8;
                    points = generateOrbital(orbitalMode, pointCount);
                }     
                else if (keyPressed->code == sf::Keyboard::Key::Up)
                {
                    zoom += 10.0f;
                }
                else if (keyPressed->code == sf::Keyboard::Key::Down)
                {
                    zoom -= 10.0f;

                    if (zoom < 20.0f)
                    {
                        zoom = 20.0f;
                    }
                }
                else if (keyPressed->code == sf::Keyboard::Key::Equal)
                {
                    pointCount += 1000;
                    points = generateOrbital(orbitalMode, pointCount);
                }
                else if (keyPressed->code == sf::Keyboard::Key::Hyphen)
                {
                    pointCount -= 1000;

                    if (pointCount < 1000)
                    {
                        pointCount = 1000;
                    }

                    points = generateOrbital(orbitalMode, pointCount);
                }
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
                    else if (keyPressed->code == sf::Keyboard::Key::Escape)
                {
                    return 0;
                }
                else if (keyPressed->code == sf::Keyboard::Key::T)
                {
                    autoRotate = !autoRotate;
                }
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
            }
        }

        if (autoRotate)
        {
            rotationY += 0.01f;
        }
  

        window.clear(sf::Color::Black);

        drawPoints(window, points, zoom, offsetX, offsetY, rotationX, rotationY);
        drawUI(window, font, orbitalMode, pointCount, zoom);

        window.display();
    }

    return 0;
}


//--- g++ -std=c++17 main.cpp Orbital.cpp Renderer.cpp -o main \
-I/opt/homebrew/include \
-L/opt/homebrew/lib \
-lsfml-graphics \
-lsfml-window \
-lsfml-system ---//

