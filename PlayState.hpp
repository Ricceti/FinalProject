#ifndef PLAYSTATE_HPP
#define PLAYSTATE_HPP

#include "GameState.hpp"
#include <SFML/Graphics.hpp>

class PlayState : public GameState {
public:
    void handleInput(Game& game) override;
    void update(Game& game) override;
    void render(Game& game) override;
};

#endif // PLAYSTATE_HPP
