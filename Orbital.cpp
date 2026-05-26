#include "Orbital.h"
#include <cmath>
#include <cstdlib>

double randomDouble(double min, double max)
{
    return min + static_cast<double>(rand()) / RAND_MAX * (max - min);
}

std::string getOrbitalName(int orbitalMode)
{
    if (orbitalMode == 0) return "H2 Sigma Antibonding Orbital";
    if (orbitalMode == 1) return "1s Orbital";
    if (orbitalMode == 2) return "2p_y Orbital";
    if (orbitalMode == 3) return "2p_x Orbital";
    if (orbitalMode == 4) return "3d x^2-y^2 Orbital";
    if (orbitalMode == 5) return "3d_xy Orbital";
    if (orbitalMode == 6) return "2s Orbital";
    if (orbitalMode == 7) return "3d_z^2 Orbital";
    if (orbitalMode == 8) return "2p_z Orbital";
    if (orbitalMode == 9) return "H2 Sigma Bonding Orbital";

    return "Unknown Orbital";
}

std::vector<Point> generateOrbital(int orbitalMode,
                                   int targetPoints,
                                   double bondLength)
{
    std::vector<Point> points;

    while (points.size() < static_cast<std::size_t>(targetPoints))
    {
        double maxRadius = 6.0;

        if (orbitalMode == 1)
        {
            maxRadius = 4.0;
        }
        else if (orbitalMode == 6)
        {
            maxRadius = 7.0;
        }
        else if (orbitalMode == 2 ||
             orbitalMode == 3 ||
             orbitalMode == 8)
        {
            maxRadius = 7.0;
        }
        else
        {
            maxRadius = 8.0;
        }

    double r = randomDouble(0.0, maxRadius);

    double theta = std::acos(randomDouble(-1.0, 1.0));
    
    double phi = randomDouble(0.0, 2.0 * M_PI);
    

        double x = r * std::sin(theta) * std::cos(phi);
        double y = r * std::sin(theta) * std::sin(phi);
        double z = r * std::cos(theta);

        double probability = 0.0;
        sf::Color pointColor = sf::Color::Cyan;

        if (orbitalMode == 1)
        {
            probability = std::exp(-2.0 * r);
        }
        else if (orbitalMode == 2)
        {
            // 2p_y
            double angular = std::sin(theta) * std::sin(phi);
            probability = r * r * angular * angular * std::exp(-r);
            pointColor = angular > 0 ? sf::Color::Cyan : sf::Color::Magenta;
        }
        else if (orbitalMode == 3)
        {
            // 2p_x
            double angular = std::sin(theta) * std::cos(phi);
            probability = r * r * angular * angular * std::exp(-r);
            pointColor = angular > 0 ? sf::Color::Cyan : sf::Color::Magenta;        
        }
            // 3d x^2-y^2
        else if (orbitalMode == 4)
        {
            double angular =
            std::sin(theta) * std::sin(theta) * std::cos(2.0 * phi);

            probability =
            r * r * r * r *
            angular * angular *
            std::exp(-r);

            pointColor = angular > 0 ? sf::Color::Cyan : sf::Color::Magenta;
        }
            // 3d_xy
        else if (orbitalMode == 5)
        {
            double angular =
                std::sin(theta) * std::sin(theta) * std::sin(2.0 * phi);

            probability =
            r * r * r * r *
            angular * angular *
            std::exp(-r);

            pointColor = angular > 0 ? sf::Color::Cyan : sf::Color::Magenta;
        }
        else if (orbitalMode == 6)
        {
            double radial = 2.0 - r;
            probability = radial * radial * std::exp(-r);
            pointColor = sf::Color::Cyan;
        }
            // 3d_z^2
        else if (orbitalMode == 7)
        {
            double angular =
            3.0 * std::cos(theta) * std::cos(theta) - 1.0;

            probability =
                r * r * r * r *
                angular * angular *
                std::exp(-r);

            pointColor = angular > 0 ? sf::Color::Cyan : sf::Color::Magenta;
        }
        else if (orbitalMode == 8)
        {
            // 2p_z
            double angular = std::cos(theta);
            probability = r * r * angular * angular * std::exp(-r);
            pointColor = angular > 0 ? sf::Color::Cyan : sf::Color::Magenta;
        }
        else if (orbitalMode == 9 || orbitalMode == 0)
        {
            double rA = std::sqrt(
                (x + bondLength / 2.0) * (x + bondLength / 2.0) +
                y * y +
                z * z
            );

            double rB = std::sqrt(
                (x - bondLength / 2.0) * (x - bondLength / 2.0) +
                y * y +
                z * z
            );

            double psiA = std::exp(-rA);
            double psiB = std::exp(-rB);

            double psi = 0.0;

            if (orbitalMode == 9)
            {
                psi = psiA + psiB;
                pointColor = sf::Color(0, 220, 255);
            }
            else
            {
                psi = psiA - psiB;
                pointColor = psi > 0 ? sf::Color(0, 220, 255) : sf::Color::Magenta;
            }

            probability = psi * psi;
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
