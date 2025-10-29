#include "Engine/Utility.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
#include <cassert>
#include <cmath>
#include <random>

void centerOrigin(sf::Text &text) {
    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin({std::floor(bounds.position.x + bounds.size.x / 2.f),
                    std::floor(bounds.position.y + bounds.size.y / 2.f)});
}

void centerOrigin(sf::Sprite &sprite) {
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin({std::floor(bounds.position.x + bounds.size.x / 2.f),
                      std::floor(bounds.position.y + bounds.size.y / 2.f)});
}

float length(sf::Vector2f vector) {
    return std::sqrt(vector.x * vector.x + vector.y * vector.y);
}

sf::Vector2f unitVector(sf::Vector2f vector) {
    sf::Vector2f zero(0.f, 0.f);
    assert(vector != zero);
    return vector / length(vector);
}

int randomInt(int inclusiveMax) {
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<> distr(0, inclusiveMax);
    return distr(gen);
}
