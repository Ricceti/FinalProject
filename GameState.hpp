#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include "Game.hpp"

class Game;

class GameState {
public:
    virtual ~GameState() = default;
    virtual void handleInput(Game& game) = 0;
    virtual void update(Game& game) = 0;
    virtual void render(Game& game) = 0;
};

#endif // GAMESTATE_HPP
