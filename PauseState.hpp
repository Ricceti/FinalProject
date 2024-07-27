#ifndef PAUSESTATE_HPP
#define PAUSESTATE_HPP

#include "GameState.hpp"

class PauseState : public GameState {
public:
    void handleInput(Game& game) override;
    void update(Game& game) override;
    void render(Game& game) override;
};

#endif // PAUSESTATE_HPP
