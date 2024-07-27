#include "MainMenuState.hpp"
#include "Game.hpp"
#include "PlayState.hpp"

void MainMenuState::handleInput(Game& game) {
    sf::Event event;
    while (game.getWindow().pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            game.getWindow().close();
        }
        else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
            game.changeState(std::make_unique<PlayState>());
        }
    }
}

void MainMenuState::update(Game& game) {
    // Actualiza la lógica del menú principal
}

void MainMenuState::render(Game& game) {
    sf::RenderWindow& window = game.getWindow();
    window.clear();
    window.display();
}
