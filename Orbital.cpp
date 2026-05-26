#include "Orbital.h"
#include <cmath>
#include <cstdlib>

double randomDouble(double min, double max)
{
    return min + static_cast<double>(rand()) / RAND_MAX * (max - min);
}

std::string getOrbitalName(int orbitalMode)
{
    if (orbitalMode == 1) return "1s Orbital";
    if (orbitalMode == 2) return "2p_y Orbital";
    if (orbitalMode == 3) return "2p_x Orbital";
    if (orbitalMode == 4) return "2p_z Orbital";
    if (orbitalMode == 5) return "3d x^2-y^2 Orbital";
    if (orbitalMode == 6) return "3d_xy Orbital";
    if (orbitalMode == 7) return "2s Orbital";
    if (orbitalMode == 8) return "3d_z^2 Orbital";
    

    return "Unknown Orbital";
}

std::vector<Point> generateOrbital(int orbitalMode, int targetPoints)
{
    std::vector<Point> points;

    while (points.size() < targetPoints)
    {
        double x = randomDouble(-4.0, 4.0);
        double y = randomDouble(-4.0, 4.0);
        double z = randomDouble(-4.0, 4.0);

        double r = std::sqrt(x * x + y * y + z * z);

        double probability = 0.0;
        sf::Color pointColor = sf::Color::Cyan;

        if (orbitalMode == 1)
        {
            probability = std::exp(-2.0 * r);
        }
        else if (orbitalMode == 2)
        {
            probability = y * y * std::exp(-1.5 * r);
            pointColor = y > 0 ? sf::Color::Cyan : sf::Color::Magenta;
        }
        else if (orbitalMode == 3)
        {
            probability = x * x * std::exp(-1.5 * r);
            pointColor = x > 0 ? sf::Color::Cyan : sf::Color::Magenta;
        }
        else if (orbitalMode == 4)
        {
            probability = z * z * std::exp(-1.5 * r);

            if (z > 0)
            {
                pointColor = sf::Color::Cyan;
            }
            else
            {
                pointColor = sf::Color::Magenta;
            }
        }   
        else if (orbitalMode == 5)
        {
            double phase = x * y;

            probability = phase * phase * std::exp(-1.0 * r);

            if (phase > 0)
            {
                pointColor = sf::Color::Cyan;
            }
            else
            {
                pointColor = sf::Color::Magenta;
            }
        }
        else if (orbitalMode == 6)
        {
            double phase = x * x - y * y;
            probability = phase * phase * std::exp(-1.0 * r);
            pointColor = phase > 0 ? sf::Color::Cyan : sf::Color::Magenta;
        }
        else if (orbitalMode == 7)
        {
            double node = 2.0 - r;

            probability = node * node * std::exp(-1.0 * r);

            pointColor = sf::Color::Cyan;
        }
        else if (orbitalMode == 8)
        {
            double phase = 2.0 * z * z - x * x - y * y;
            probability = phase * phase * std::exp(-1.0 * r);

            if (phase > 0)
            {
                pointColor = sf::Color::Cyan;
            }
            else
            {
                pointColor = sf::Color::Magenta;
            }
        }
        
    



        double roll = randomDouble(0.0, 1.0);

        if (roll < probability)
        {
            points.push_back(
            {
                static_cast<float>(x),
                static_cast<float>(y),
                static_cast<float>(z),
                pointColor
            });
        }
    }

    return points;
}

