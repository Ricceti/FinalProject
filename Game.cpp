#include "Game.hpp"
#include "GameState.hpp"
#include "MainMenuState.hpp"

Game::Game() : window(sf::VideoMode(800, 600), "State Pattern Game") {
    changeState(std::make_unique<MainMenuState>());
}

void Game::run() {
    while (window.isOpen()) {
        states.top()->handleInput(*this);
        states.top()->update(*this);
        states.top()->render(*this);
    }
}

void Game::changeState(std::unique_ptr<GameState> state) {
    while (!states.empty()) {
        states.pop();
    }
    states.push(std::move(state));
}

void Game::pushState(std::unique_ptr<GameState> state) {
    states.push(std::move(state));
}

void Game::popState() {
    if (!states.empty()) {
        states.pop();
    }
}

sf::RenderWindow& Game::getWindow() {
    return window;
}
