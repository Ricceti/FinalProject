#pragma once

#include <SFML/Graphics.hpp>
#include "Observer.hpp"

class HealthDisplay : public Observer {
private:
    sf::Text player1Text;
    sf::Text player2Text;
    sf::Font font;
public:
    HealthDisplay();
    void update(int player1Health, int player2Health) override;
    void draw(sf::RenderWindow& window);
};
