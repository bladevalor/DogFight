#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>

void centerOrigin(sf::Text &text);

void centerOrigin(sf::Sprite &sprite);

float length(sf::Vector2f vector);

sf::Vector2f unitVector(sf::Vector2f vector);

int randomInt(int inclusiveMax);
