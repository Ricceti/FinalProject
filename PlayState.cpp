#include "PlayState.hpp"
#include "Game.hpp"
#include "PauseState.hpp"

void PlayState::handleInput(Game& game) {
    sf::Event event;
    while (game.getWindow().pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            game.getWindow().close();
        }
        else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::P) {
            game.changeState(std::make_unique<PauseState>());
        }
    }
}

void PlayState::update(Game& game) {
    // Actualiza la lógica del juego
}

void PlayState::render(Game& game) {
    sf::RenderWindow& window = game.getWindow();
    window.clear();
    window.display();
}
