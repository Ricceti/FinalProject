#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include <memory>
#include <stack>

class GameState;

class Game {
public:
    Game();
    void run();
    void changeState(std::unique_ptr<GameState> state);
    void pushState(std::unique_ptr<GameState> state);
    void popState();
    sf::RenderWindow& getWindow();

private:
    sf::RenderWindow window;
    std::stack<std::unique_ptr<GameState>> states;
};

#endif // GAME_HPP
