#include "PauseState.hpp"
#include "Game.hpp"
#include "PlayState.hpp"

void PauseState::handleInput(Game& game) {
    sf::Event event;
    while (game.getWindow().pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            game.getWindow().close();
        }
        else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::P) {
            game.changeState(std::make_unique<PlayState>());
        }
    }
}

void PauseState::update(Game& game) {
}

void PauseState::render(Game& game) {
    sf::RenderWindow& window = game.getWindow();
    window.clear();
    window.display();
}
