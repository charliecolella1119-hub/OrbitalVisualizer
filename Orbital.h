#ifndef ORBITAL_H
#define ORBITAL_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

struct Point
{
    float x;
    float y;
    float z;
    sf::Color color;
};

double randomDouble(double min, double max);

std::string getOrbitalName(int orbitalMode);

std::vector<Point> generateOrbital(int orbitalMode, int targetPoints);

#endif

