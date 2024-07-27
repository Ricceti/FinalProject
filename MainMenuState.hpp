#ifndef MAINMENUSTATE_HPP
#define MAINMENUSTATE_HPP

#include "GameState.hpp"

class MainMenuState : public GameState {
public:
    void handleInput(Game& game) override;
    void update(Game& game) override;
    void render(Game& game) override;
};

#endif 
