#include "HealthDisplay.hpp"
#include <iostream>

HealthDisplay::HealthDisplay() {
    if (!font.loadFromFile("C:\\Users\\LENOVO\\Documents\\Mortal Kombat\\resources\\images\\arenas\\win\\mk2.ttf")) {
        std::cout << "Error loading font" << std::endl;
    }
    player1Text.setFont(font);
    player1Text.setCharacterSize(44);
    player1Text.setFillColor(sf::Color::Blue);
    player1Text.setPosition(30, 10);

    player2Text.setFont(font);
    player2Text.setCharacterSize(44);
    player2Text.setFillColor(sf::Color::Red);
    player2Text.setPosition(1350, 10);
}

void HealthDisplay::update(int player1Health, int player2Health) {
    player1Text.setString("Sub-zero: " + std::to_string(player1Health));
    player2Text.setString("Kano: " + std::to_string(player2Health));
}

void HealthDisplay::draw(sf::RenderWindow& window) {
    window.draw(player1Text);
    window.draw(player2Text);
}
